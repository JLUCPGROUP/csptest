#include "Model.h"

namespace cudacp {

IntVar::IntVar(HVar* v, const int num_vars) :
	id_(v->id),
	name_(v->name),
	init_size_(v->vals.size()),
	curr_size_(v->vals.size()),
	last_limit_(v->vals.size() % BITSIZE),
	num_bit_(ceil(v->vals.size() / BITSIZE)),
	vals_(v->vals),
	hv_(v) {

	vector<bitset<BITSIZE>> a(num_bit_, ULLONG_MAX);
	a.back() << BITSIZE - last_limit_;
	bit_doms_.resize(init_size_, a);
	level_.resize(init_size_, 0);
}

void IntVar::RemoveValue(const int a, const int p) {
	const auto index = get_bit_index(a);
	while (level_[top_] > p)
		--top_;

	if (level_[top_] < p) {
		++top_;
		bit_doms_[top_].assign(bit_doms_[(top_ - 1)].begin(), bit_doms_[(top_ - 1)].end());
	}
	bit_doms_[top_][get<0>(index)].reset(get<1>(index));
	level_[top_] = p;
	--curr_size_;
}

void IntVar::ReduceTo(const int a, const int p) {
	const auto index = get_bit_index(a);
	while (level_[top_] > p)
		--top_;

	if (level_[top_] < p)
		++top_;
	for (auto v : bit_doms_[top_])
		v.reset();
	bit_doms_[top_][get<0>(index)].set(get<1>(index));
	level_[top_] = p;

	curr_size_ = 1;
}

void IntVar::AddValue(const int a) {
	const auto index = get_bit_index(a);
	bit_doms_[top_][get<0>(index)].set(get<1>(index));
	++curr_size_;
}

void IntVar::RestoreUpTo(const int p) {
	while (level_[top_] > p)
		--top_;
}

int IntVar::next(const int a) const {
	for (int i = (a + 1); i < init_size_; ++i) {
		const auto index = get_bit_index(a);
		if (bit_doms_[top_][get<0>(index)].test(get<1>(index)))
			return i;
	}
	return Limits::INDEX_OVERFLOW;
}

int IntVar::prev(const int a) const {
	for (int i = (a - 1); i >= 0; --i) {
		const auto index = get_bit_index(a);
		if (bit_doms_[top_][get<0>(index)].test(get<1>(index)))
			return i;
	}
	return Limits::INDEX_OVERFLOW;
}

bool IntVar::have(const int a) const {
	const auto index = get_bit_index(a);
	return bit_doms_[top_][get<0>(index)].test(get<1>(index));
}

int IntVar::head() const {
	for (int i = 0; i < num_bit_; ++i)
		if (bit_doms_[top_][i].any())
			for (int j = 0; j < BITSIZE; ++j)
				if (bit_doms_[top_][i].test(j))
					return get_value(i, j);
	return Limits::INDEX_OVERFLOW;
}

int IntVar::tail() const {
	for (int i = (num_bit_ - 1); i >= 0; --i)
		if (bit_doms_[top_][i].any())
			for (int j = (BITSIZE - 1); j >= 0; --j)
				if (bit_doms_[top_][i].test(j))
					return get_value(i, j);
	return Limits::INDEX_OVERFLOW;
}


tuple<int, int> IntVar::get_bit_index(const int idx) const {
	tuple<int, int> a;
	get<0>(a) = idx / BITSIZE;
	get<1>(a) = idx % BITSIZE;
	return a;
}

int IntVar::get_value(const int i, const int j) {
	return i*BITSIZE + j;
}

////////////////////////////////////////////////////////////////////////////
IntVar* IntVal::v() const {
	return v_;
}

void IntVal::v(IntVar* val) {
	v_ = val;
}

const IntVal & IntVal::operator=(const IntVal & rhs) {
	v_ = rhs.v_;
	a_ = rhs.a_;

	return *this;
}

int IntVal::a() const {
	return a_;
}

void IntVal::a(const int val) {
	a_ = val;
}

Tabular::Tabular(HTab* t, const vector<IntVar*> scp) :
	id(t->id),
	arity(scp.size()),
	scope(scp),
	tuples_(t->tuples) {

}

void Tabular::GetFirstValidTuple(IntVal& v_a, vector<int>& t) {}

ostream & operator<<(ostream & os, IntVal & v_val) {
	os << "(" << v_val.v_->id() << ", " << v_val.a_ << ")";
	return os;
}
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
Solver::Solver(HModel* h) :
	hm_(h),
	num_vars_(hm_->vars.size()),
	num_tabs_(hm_->tabs.size()) {
	vars.reserve(num_vars_);
	tabs.reserve(num_tabs_);

	for (auto hv : hm_->vars) {
		IntVar *v = new IntVar(hv, num_vars_);
		vars.push_back(v);
	}

	for (auto ht : hm_->tabs)
		Tabular *t = new Tabular(ht, get_scope(ht));
}

}
