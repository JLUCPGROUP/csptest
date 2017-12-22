﻿#include "Model.h"

namespace cudacp {

bool Existed(vector<int>& tuple) {
	return tuple[0] != INT_MAX;
}

void Exclude(vector<int>& tuple) {
	tuple[0] = INT_MAX;
}

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
	id_(t->id),
	arity(scp.size()),
	scope(scp),
	tuples_(t->tuples) {

}

bool Tabular::sat(vector<int>& t) const {
	return binary_search(tuples_.begin(), tuples_.end(), t);
}

void Tabular::GetFirstValidTuple(IntVal& v_a, vector<int>& t) {
	for (int i = 0; i < arity; ++i)
		if (scope[i] != v_a.v())
			t[i] = scope[i]->head();
		else
			t[i] = v_a.a();
}

void Tabular::GetNextValidTuple(IntVal& v_a, vector<int>& t) {
	for (int i = arity - 1; i >= 0; --i)
		if (scope[i] != v_a.v())
			if (scope[i]->next(t[i]) == Limits::INDEX_OVERFLOW)
				t[i] = scope[i]->head();
			else {
				t[i] = scope[i]->next(t[i]);
				return;
			}

			Exclude(t);
}

int Tabular::index(IntVar* v) const {
	for (int i = scope.size() - 1; i >= 0; --i)
		if (scope[i] == v)
			return i;
	return -1;
}

bool Tabular::IsValidTuple(vector<int>& t) {
	if (!Existed(t))
		return false;

	for (IntVar* v : scope)
		if (!v->have(t[index(v)]))
			return false;
	return true;
}

ostream & operator<<(ostream & os, IntVal & v_val) {
	os << "(" << v_val.v_->id() << ", " << v_val.a_ << ")";
	return os;
}
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
Solver::Solver(HModel* h) :
	hm_(h),
	num_vars_(h->vars.size()),
	num_tabs_(h->tabs.size()),
	max_arity_(h->max_arity()),
	max_dom_size_(h->max_domain_size()) {
	vars.reserve(num_vars_);
	tabs.reserve(num_tabs_);

	for (auto hv : hm_->vars) {
		IntVar *v = new IntVar(hv, num_vars_);
		vars.push_back(v);
	}

	for (auto ht : hm_->tabs) {
		Tabular *t = new Tabular(ht, get_scope(ht));
		tabs.push_back(t);
	}

	for (auto t : tabs)
		for (auto v : t->scope)
			subscriptions[v].push_back(t);
}

void Solver::GetFirstValidTuple(IntConVal& c_val, vector<int>& t) {
	IntVal v_a(c_val.v(), c_val.a());
	c_val.c()->GetFirstValidTuple(v_a, t);
}

void Solver::GetNextValidTuple(IntConVal& c_val, vector<int>& t) {
	IntVal v_a(c_val.v(), c_val.a());
	c_val.c()->GetNextValidTuple(v_a, t);
}

int Solver::GetIntConValIndex(IntConVal& c_val) const {
	return  c_val.c()->id() * max_arity_ * max_dom_size_ + c_val.c()->index(c_val.v()) * max_dom_size_ + c_val.a();
}

int Solver::GetIntConValIndex(const int c_id, const int v_id, const int a) {
	IntConVal c_a(tabs[c_id], vars[v_id], a);
	return GetIntConValIndex(c_a);
}

IntConVal Solver::GetIntConVal(const int index) {
	const int c_id = index / tabs.size();
	const int v_id = index % tabs.size() / max_dom_size_;
	const int a = index % tabs.size() % max_dom_size_;
	IntConVal c(tabs[c_id], tabs[c_id]->scope[v_id], a);
	return c;
}

vector<IntVar*>& Solver::get_scope(HTab* t) {
	vector<IntVar*> tt(t->scope.size());
	for (int i = 0; i < t->scope.size(); ++i)
		tt[i] = vars[t->scope[i]->id];
	return tt;
}

void Solver::get_scope(HTab* t, vector<IntVar*> scp) {
	for (int i = 0; i < t->scope.size(); ++i)
		scp[i] = vars[t->scope[i]->id];
}

const IntConVal& IntConVal::operator=(const IntConVal& rhs) {
	c_ = rhs.c_;
	v_ = rhs.v_;
	a_ = rhs.a_;

	return *this;
}
}
