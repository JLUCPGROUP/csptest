#pragma once
#include "Network.h"

namespace cudacp {
///*extern*/ auto  nullexp = [](std::vector<int>& a) {return INT_MIN; };
IntVar::IntVar(HVar* v, const int num_vars) :
	id_(v->id),
	name_(v->name),
	init_size_(v->vals.size()),
	curr_size_(v->vals.size()),
	last_limit_(v->vals.size() % BITSIZE),
	num_bit_(ceil(static_cast<float>(v->vals.size()) / BITSIZE)),
	vals_(v->vals),
	hv_(v) {
	bit_tmp_.resize(num_bit_, 0);
	for (int i = 0; i < init_size_; ++i) {
		auto idx = get_bit_index(i);
		bit_tmp_[get<0>(idx)].set(get<1>(idx));
	}
	bit_doms_.resize(num_vars + 1, bit_tmp_);
	level_.resize(num_vars + 1, 0);
}

void IntVar::RemoveValue(const int a, const int p) {
	//if (p == -1) {

	//}
	//else {
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
	//}
}

void IntVar::ReduceTo(const int a, const int p) {
	const auto index = get_bit_index(a);
	while (level_[top_] > p)
		--top_;

	if (level_[top_] < p)
		++top_;

	for (auto& v : bit_doms_[top_])
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
	curr_size_ = 0;
	for (auto v : bit_doms_[top_])
		curr_size_ += v.count();
}

int IntVar::next(const int a) const {
	for (int i = (a + 1); i < init_size_; ++i) {
		const auto index = get_bit_index(i);
		if (bit_doms_[top_][get<0>(index)].test(get<1>(index)))
			return i;
	}
	return Limits::INDEX_OVERFLOW;
}

int IntVar::prev(const int a) const {
	for (int i = (a - 1); i >= 0; --i) {
		const auto index = get_bit_index(i);
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

void IntVar::show() {
	cout << name_ << " id = " << id_ << ": ";
	for (auto a : vals_)
		if (have(a))
			cout << a << " ";
	cout << endl;
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
const IntVal & IntVal::operator=(const IntVal & rhs) {
	v_ = rhs.v_;
	a_ = rhs.a_;
	aop_ = rhs.aop_;
	return *this;
}

void IntVal::flip() {
	aop_ = !aop_;
}

IntVal IntVal::next() const {
	return IntVal(v_, v_->next(a_), true);
}

bool IntVal::operator==(const IntVal & rhs) {
	return (this == &rhs) || (v_ == rhs.v_ && a_ == rhs.a_ && aop_ == rhs.aop_);
}

bool IntVal::operator!=(const IntVal & rhs) {
	return !((this == &rhs) || (v_ == rhs.v_ && a_ == rhs.a_ && aop_ == rhs.aop_));
}

ostream & operator<<(ostream & os, IntVal & v_val) {
	const string s = (v_val.aop_) ? " = " : " != ";
	os << "(" << v_val.vid() << s << v_val.a_ << ")";
	return os;
}
////////////////////////////////////////////////////////////////////////////

Tabular::Tabular(HTab* t, const vector<IntVar*> scp) :
	arity(scp.size()),
	scope(scp),
	id_(t->id),
	tuples_(t->tuples) {
	weight = 1;
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
	for (int i = arity - 1; i >= 0; --i) {
		if (scope[i] != v_a.v()) {
			if (scope[i]->next(t[i]) == Limits::INDEX_OVERFLOW) {
				t[i] = scope[i]->head();
			}

			else {
				t[i] = scope[i]->next(t[i]);
				return;
			}
		}
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

//ostream & operator<<(ostream & os, IntVal & v_val) {
//	os << "(" << v_val.v->id() << ", " << v_val.a << ")";
//	return os;
//}
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
Network::Network(HModel* h) :
	hm_(h),
	max_arity_(h->max_arity()),
	max_dom_size_(h->max_domain_size()),
	max_bitDom_size_(ceil(float(h->max_domain_size()) / BITSIZE)),
	num_vars_(h->vars.size()),
	num_tabs_(h->tabs.size()) {
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
			subscription[v].push_back(t);

}

void Network::GetFirstValidTuple(IntConVal& c_val, vector<int>& t) {
	IntVal v_a(c_val.v(), c_val.a());
	c_val.c()->GetFirstValidTuple(v_a, t);
}

void Network::GetNextValidTuple(IntConVal& c_val, vector<int>& t) {
	IntVal v_a(c_val.v(), c_val.a());
	c_val.c()->GetNextValidTuple(v_a, t);
}

int Network::GetIntConValIndex(IntConVal& c_val) const {
	return  c_val.c()->id() * max_arity_ * max_dom_size_ + c_val.c()->index(c_val.v()) * max_dom_size_ + c_val.a();
}

int Network::GetIntConValIndex(const int c_id, const int v_id, const int a) {
	IntConVal c_a(tabs[c_id], vars[v_id], a);
	return GetIntConValIndex(c_a);
}

IntConVal Network::GetIntConVal(const int index) {
	const int c_id = index / tabs.size();
	const int v_id = index % tabs.size() / max_dom_size_;
	const int a = index % tabs.size() % max_dom_size_;
	IntConVal c(tabs[c_id], tabs[c_id]->scope[v_id], a);
	return c;
}

void Network::RestoreUpto(const int level) {
	for (IntVar* v : vars)
		//if (!v->assigned())
		v->RestoreUpTo(level);
}

void Network::show() {
	for (auto v : vars)
		v->show();
}

Network::~Network() {
	for (auto v : vars)
		delete v;
	for (auto t : tabs)
		delete t;
	vars.clear();
	tabs.clear();
}

vector<IntVar*> Network::get_scope(HTab* t) {
	vector<IntVar*> tt(t->scope.size());
	for (int i = 0; i < t->scope.size(); ++i)
		tt[i] = vars[t->scope[i]->id];
	return tt;
}

void Network::get_scope(HTab* t, vector<IntVar*> scp) {
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
