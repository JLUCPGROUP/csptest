#include "Solver.h"
namespace cudacp {
Qsac::Qsac(Network* n, const VarHeu h) :
	n_(n),
	h_(h) {
	bitDoms_.resize(n_->vars.size(), bitSetDom(n_->max_bitDom_size()));
	for (int i = 0; i < n_->max_bitDom_size(); ++i)
		bitDoms_[i].assign(n_->vars[i]->bitDom().begin(), n_->vars[i]->bitDom().end());
}


void Qsac::push(IntVal* val) {
	auto a = val->get_bit_index();
	bitDoms_[val->vid()][get<0>(a)].set(get<1>(a));
}

IntVal Qsac::pop() {
	IntVal val = select_v_value();
	auto a = val.get_bit_index();
	bitDoms_[val.vid()][get<0>(a)].reset(get<1>(a));

	return val;
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
				if (size(v) < min_size) {
					min_size = size(v);
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

				if (size(x) == 1 || x_w == 0)
					x_dw = -1;
				else
					x_dw = size(x) / x_w;
				if (x_dw < min_size) {
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
}
