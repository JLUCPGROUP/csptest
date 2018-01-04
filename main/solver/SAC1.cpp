#include "Solver.h"

namespace cudacp {
SAC1::SAC1(Network* n, const ACAlgorithm a) :
	n_(n),
	ac_algzm_(a) {
	switch (a) {
	case AC_3:
		ac_ = new AC3(n_);
		break;
	case AC_3bit:
		ac_ = new AC3bit(n_);
		break;
	case A_FC:
		ac_ = new FC(n_);
		break;
	case A_FC_bit:
		ac_ = new FCbit(n_);
		break;
		//case AC_3rm:
		//	ac_ = new AC3rm(nt_);
	default:
		break;
	}

	x_evt_ = n_->vars;
}
SAC1::~SAC1() {
	// TODO Auto-generated destructor stub
	delete ac_;
}

bool SAC1::enforce(vector<IntVar*> x_evt, const int level) {
	ConsistencyState cs = ac_->enforce(n_->vars, level);
	bool result = cs.state;
	cout << cs.num_delete << endl;
	del_ += cs.num_delete;
	x_evt_.clear();
	auto modified = false;

	if (!result)
		return false;

	do {
		modified = false;
		for (auto x : n_->vars) {
			for (auto a : x->values()) {
				if (x->have(a)) {
					//cout << "(" << x->id() << "," << a << ")" << endl;
					x->ReduceTo(a, level + 1);
					x->assign(true);
					x_evt_.push_back(x);
					result = ac_->enforce(x_evt_, level + 1).state;
					x_evt_.clear();

					if (!result) {
						//cout << "delete: (" << x->id() << "," << a << ")" << endl;
						++del_;
						n_->RestoreUpto(level);
						x->assign(false);
						x->RemoveValue(a, level);
						x_evt_.push_back(x);
						cs = ac_->enforce(x_evt_, level);
						result = cs.state;
						del_ += cs.num_delete;
						x_evt_.clear();

						if (!result)
							return false;

						modified = true;
						break;
					}
					else {
						x->assign(false);
						n_->RestoreUpto(level);
					}
				}
			}
		}
	} while (modified);

	cout << "delete:" << del_ << endl;
	return true;
}

}
