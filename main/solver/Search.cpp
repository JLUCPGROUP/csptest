#pragma once
#include "Solver.h"
namespace cudacp {
Search::Search(Network *n, const LookAhead la, const LookBack lb, const Consistency c) :
	n_(n),
	c_type_(c),
	la_(la),
	lb_(lb) {
	switch (c_type_) {
	case C_AC3:
		ac_ = new AC3(n_);
		break;
	case C_AC3bit:
		ac_ = new AC3bit(n_);
		break;
		//case AC_3rm:
		//	ac_ = new AC3rm(nt_);
	default:
		break;
	}
	expl.initial(n_);
}

//SearchStatistics Search::enforce(const int time_limits) {}

Search::~Search() {
	delete ac_;
}

vector<IntVal> Search::HandleEmptyDomain(IntVar* v) {
	if (lb_ == LB_SBT)
		return I.vals();

	nogood.clear();
	for (auto a : v->values())
		nogood.insert(nogood.end(), expl[IntVal(v, a)].begin(), expl[IntVal(v, a)].end());

	return nogood;
}

void Search::UndoAssignment(IntVal v_a) {
	//for (size_t i = 0; i < length; i++) {

	//}
}

IntVal Search::select_v_value() const {
	IntVar* x = nullptr;
	int min_size = INT_MAX;
	for (auto v : n_->vars)
		if (!v->assigned())
			if (v->size() < min_size) {
				min_size = v->size();
				x = v;
			}

	return IntVal(x, x->head());
}
}

