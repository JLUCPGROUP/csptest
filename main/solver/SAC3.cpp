#include "Solver.h"
namespace cudacp {
Qsac::Qsac(Network* n, const VarHeu h) :
	n_(n),
	h_(h) {
	bitDoms_.resize(n_->vars.size(), bitSetVector(n_->max_bitDom_size()));
	for (int i = 0; i < n_->max_bitDom_size(); ++i)
		bitDoms_[i].assign(n_->vars[i]->bitDom().begin(), n_->vars[i]->bitDom().end());
}

void Qsac::initial(Network* n, const VarHeu h) {
	n_ = n;
	h_ = h;
	num_bit_vars_ = ceil(static_cast<float>(n_->vars.size()) / BITSIZE);
	vars_assigned_.resize(num_bit_vars_);

	for (int i = 0; i < n_->vars.size(); ++i) {
		auto idx = GetBitIdx(i);
		vars_assigned_[get<0>(idx)].set(get<1>(idx));
	}
	vars_assigned_old_ = vars_assigned_;
	tmp_empty_ = vars_assigned_;
	bitDoms_.resize(n_->vars.size(), bitSetVector(n_->max_bitDom_size()));
	for (int i = 0; i < n_->vars.size(); ++i)
		bitDoms_[i].assign(n_->vars[i]->bitDom().begin(), n_->vars[i]->bitDom().end());
}

void Qsac::push(IntVal val) {
	auto a = val.get_bit_index();
	bitDoms_[val.vid()][get<0>(a)].set(get<1>(a));
	auto b = GetBitIdx(val.vid());
	vars_assigned_[get<0>(b)].reset(get<1>(b));
}

IntVal Qsac::pop() {
	IntVal val = select_v_value();

	auto a = val.get_bit_index();
	bitDoms_[val.vid()][get<0>(a)].reset(get<1>(a));
	auto b = GetBitIdx(val.vid());
	vars_assigned_[get<0>(b)].set(get<1>(b));
	return val;
}

bool Qsac::empty() const {
	int s = 0;
	for (auto & v : n_->vars)
		s += size(v);
	return s;
}

int Qsac::size(IntVar* v) const {
	int size = 0;
	for (auto& a : bitDoms_[v->id()])
		size += a.count();
	return size;
}

void Qsac::update() {
	for (int i = 0; i < n_->vars.size(); ++i)
		for (int j = 0; j < n_->max_bitDom_size(); ++j)
			bitDoms_[i][j] &= n_->vars[i]->bitDom()[j];
}

bool Qsac::vars_assigned() {
	for (int i = 0; i < n_->vars.size(); ++i) {
		IntVar* v = n_->vars[i];
		auto a = GetBitIdx(v->id());
		if (vars_assigned_[get<0>(a)].test(get<1>(a)) && size(v) != 0)
			return false;
	}
	return true;
}

void Qsac::reset() {
	for (auto&a : vars_assigned_)
		a.reset();
}

bool Qsac::vars_assigned(AssignedStack& I) const {
	for (int i = 0; i <= n_->vars.size(); ++i) {
		IntVar* v = n_->vars[i];
		if (size(v) != 0 && I.assiged(v) == false) {
			return false;
		}
	}

	return true;
}

bool Qsac::in(IntVal val) {
	auto a = val.get_bit_index();
	return  bitDoms_[val.vid()][get<0>(a)].test(get<1>(a));
}

int Qsac::head(IntVar* v) const {
	for (int i = 0; i < n_->max_bitDom_size(); ++i)
		if (bitDoms_[v->id()][i].any())
			for (int j = 0; j < BITSIZE; ++j)
				if (bitDoms_[v->id()][i].test(j))
					return GetValue(i, j);
	return Limits::INDEX_OVERFLOW;
}

IntVal Qsac::select_v_value() const {
	IntVal val(nullptr, -1);
	switch (h_) {
	case DOM: {
		float min_size = FLT_MAX;
		for (auto v : n_->vars)
			if (!v->assigned())
				if (v->size() < min_size&&size(v) > 0) {
					min_size = v->size();
					val.v(v);
				}
		val.a(head(val.v()));
	}
			  break;
	case DOM_WDEG: {
		float min_size = FLT_MAX;
		for (auto x : n_->vars) {
			if (!x->assigned()) {
				float x_w = 0.0;
				float x_dw;
				for (auto c : n_->subscription[x]) {
					int cnt = 0;
					for (auto y : c->scope)
						if (!y->assigned())
							++cnt;
					if (cnt > 1)
						x_w += c->weight;
				}

				if (x->size() == 1 || x_w == 0)
					x_dw = -1;
				else
					x_dw = x->size() / x_w;
				if (x_dw < min_size&&size(x) > 0) {
					min_size = x_dw;
					val.v(x);
				}
			}
		}
		val.a(head(val.v()));
	}
				   break;
	default:;
	}
	return val;
}

SAC3::SAC3(Network* n, ACAlgorithm a, const VarHeu h) :
	SAC1(n, a),
	h_(h) {
	q_.initial(n_, h_);
	I_.initial(n_);
}

bool SAC3::enforce(vector<IntVar*> x_evt, const int level) {
	level_ = level;
	ConsistencyState cs = ac_->enforce(n_->vars, level);
	bool result = cs.state;
	auto modified = false;
	x_evt_.clear();
	if (!result)
		return false;

	do {
		modified = false;
		q_.update();
		while (q_.empty()) {
			IntVal val = BuildBranch();
			n_->RestoreUpto(level_);
			//此处assigned 要处下理下
			if (val != Nil_Val) {
				val.v()->RemoveValue(val.a(), level_);
				x_evt_.push_back(val.v());
				cs = ac_->enforce(x_evt_, level_);
				result = cs.state;
				del_ += cs.num_delete;
				x_evt_.clear();

				if (!result)
					return false;

				q_.update();
				modified = true;
			}
		}
	} while (modified);

	return true;
}

IntVal SAC3::BuildBranch() {
	int length = 0;
	IntVal val = Nil_Val;
	bool result;
	q_.reset();
	do {
		val = q_.pop();
		cout << val << endl;
		val.v()->ReduceTo(val.a(), level_ + 1);
		I_.push(val);
		x_evt_.push_back(val.v());
		result = ac_->enforce(x_evt_, level_ + 1).state;
		x_evt_.clear();

		if (result) {
			++length;
		}
		else if (length > 0) {
			q_.push(val);
		}

	} while (result || q_.vars_assigned(I_));
	I_.clear();
	if (length == 0) {
		return val;
	}
	else {
		return Nil_Val;
	}
}
}
