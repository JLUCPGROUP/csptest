#include "Solver.h"
using namespace std;
namespace cudacp {
MAC::MAC(Network * n, const ACAlgorithm ac_algzm) :
	n_(n),
	ac_algzm_(ac_algzm) {
	x_evt_.reserve(n_->vars.size());
	I.initial(n_);
	//= new AssignedStack(n_);
	switch (ac_algzm) {
	case AC_3:
		ac_ = new AC3(n_);
		break;
	case AC_3bit:
		ac_ = new AC3bit(n_);
		break;
		//case AC_3rm:
		//	ac_ = new AC3rm(nt_);
	default:
		break;
	}
}

//SearchStatistics MAC::enforce(const int time_limits) {
//	Timer t;
//	consistent_ = ac_->EnforceGAC_arc(n_->vars, 0);
//	x_evt_.clear();
//	if (!consistent_) {
//		statistics_.solve_time = t.elapsed();
//		return statistics_;
//	}
//
//	while (!finished_) {
//		if (t.elapsed() > time_limits) {
//			statistics_.solve_time = t.elapsed();
//			statistics_.time_out = true;
//			return statistics_;
//		}
//
//		IntVal v_a = select_v_value();
//		I.push(v_a);
//		cout << v_a << endl;
//		++statistics_.num_positive;
//		v_a.v()->ReduceTo(v_a.a(), I.size());
//		x_evt_.push_back(v_a.v());
//		consistent_ = ac_->EnforceGAC_arc(x_evt_, I.size());
//		cout << ac_->del() << endl;
//		x_evt_.clear();
//
//		if (consistent_&&I.full()) {
//			cout << I << endl;
//			finished_ = true;
//			//++sol_count_;
//			//consistent_ = false;
//		}
//
//		while (!consistent_ && !I.empty()) {
//			v_a = I.pop();
//
//			for (IntVar* v : n_->vars)
//				if (!v->assigned())
//					v->RestoreUpTo(I.size() + 1);
//
//			v_a.v()->RemoveValue(v_a.a(), I.size());
//			cout << "!=" << v_a << endl;
//			++statistics_.num_negative;
//			x_evt_.push_back(v_a.v());
//			consistent_ = v_a.v()->size() && ac_->EnforceGAC_arc(x_evt_, I.size());
//			cout << ac_->del() << endl;
//			x_evt_.clear();
//		}
//
//		if (!consistent_)
//			finished_ = true;
//	}
//
//	statistics_.solve_time = t.elapsed();
//	return statistics_;
//}

SearchStatistics MAC::enforce(const int time_limits) {
	Timer t;
	consistent_ = ac_->EnforceGAC_arc(n_->vars, 0);
	x_evt_.clear();
	if (!consistent_) {
		statistics_.solve_time = t.elapsed();
		return statistics_;
	}

	while (!finished_) {
		if (t.elapsed() > time_limits) {
			statistics_.solve_time = t.elapsed();
			statistics_.time_out = true;
			return statistics_;
		}

		IntVal v_a = select_v_value();
		I.push(v_a);
		++statistics_.num_positive;
		v_a.v()->ReduceTo(v_a.a(), I.size());
		x_evt_.push_back(v_a.v());
		consistent_ = ac_->EnforceGAC_arc(x_evt_, I.size());
		x_evt_.clear();

		if (consistent_&&I.full()) {
			//cout << I << endl;
			finished_ = true;
			//++sol_count_;
			//consistent_ = false;
		}

		while (!consistent_ && !I.empty()) {
			v_a = I.pop();

			for (IntVar* v : n_->vars)
				if (!v->assigned())
					v->RestoreUpTo(I.size() + 1);

			v_a.v()->RemoveValue(v_a.a(), I.size());
			++statistics_.num_negative;
			x_evt_.push_back(v_a.v());
			consistent_ = v_a.v()->size() && ac_->EnforceGAC_arc(x_evt_, I.size());
			x_evt_.clear();
		}

		if (!consistent_)
			finished_ = true;
	}

	statistics_.solve_time = t.elapsed();
	return statistics_;
}

MAC::~MAC() {
	delete ac_;
	//delete I;
}

IntVal MAC::select_v_value() const {
	//IntVar* v = n_->vars[I->size()];
	//return IntVal(v, v->head());
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
