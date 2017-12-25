#include "Solver.h"
using namespace std;
namespace cudacp {
MAC::MAC(Network * n) :
	n_(n) {
	x_evt_.reserve(n_->vars.size());
	I = new AssignedStack(n_);
	ac_ = new AC3(n_);
}

void MAC::enforce() {
	consistent_ = ac_->EnforceGAC_var(n_->vars, 0);
	x_evt_.clear();
	if (!consistent_)
		return;

	while (!finished_) {
		IntVal v_a = select_v_value();
		I->push(v_a);
		v_a.v()->ReduceTo(v_a.a(), I->size());
		x_evt_.push_back(v_a.v());
		consistent_ = ac_->EnforceGAC_var(x_evt_, I->size());
		x_evt_.clear();

		if (consistent_&&I->full()) {
			cout << I << endl;
			finished_ = true;
			//++sol_count_;
			//consistent_ = false;
		}

		while (!consistent_ && !I->empty()) {
			v_a = I->pop();

			for (IntVar* v : n_->vars)
				if (!v->assigned())
					v->RestoreUpTo(I->size() + 1);

			v_a.v()->RemoveValue(v_a.a(), I->size());
			x_evt_.push_back(v_a.v());
			consistent_ = v_a.v()->size() && ac_->EnforceGAC_var(x_evt_, I->size());
			x_evt_.clear();
		}

		if (!consistent_)
			finished_ = true;
	}

}

MAC::~MAC() {
	delete ac_;
	delete I;
}

IntVal MAC::select_v_value() const {
	//IntVar* v = n_->vars[I->size()];
	//return IntVal(v, v->head());
	IntVar* x = nullptr;
	int min_size = INT_MAX;
	for (auto v : n_->vars) {
		if (!v->assigned()) {
			if (v->size() < min_size) {
				min_size = v->size();
				x = v;
			}
		}
	}

	return IntVal(x, x->head());

}
}
