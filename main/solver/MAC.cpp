#include "Solver.h"
using namespace std;
namespace cudacp {

MAC::MAC(Network * n, const ACAlgorithm ac_algzm, const Heuristic::Var varh, const Heuristic::Val valh) :
	n_(n),
	ac_algzm_(ac_algzm),
	varh_(varh),
	valh_(valh) {
	x_evt_.reserve(n_->vars.size());
	I.initial(n_);

	switch (ac_algzm) {
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
		//case A_RNSQ:
		//	ac_ = new RNSQ(n_);
		break;
	default:
		break;
	}
}

//SearchStatistics MAC::enforce(const int time_limits) {
//	Timer t;
//	consistent_ = ac_->enforce_arc(n_->vars, 0);
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
//		consistent_ = ac_->enforce_arc(x_evt_, I.size());
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
//			consistent_ = v_a.v()->size() && ac_->enforce_arc(x_evt_, I.size());
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
	consistent_ = ac_->enforce(n_->vars, 0).state;
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
		//cout << v_a << endl;
		I.push(v_a);
		++statistics_.num_positive;
		v_a.v()->ReduceTo(v_a.a(), I.size());
		x_evt_.push_back(v_a.v());
		consistent_ = ac_->enforce(x_evt_, I.size()).state;
		x_evt_.clear();
		I.update_model_assigned();
		if (consistent_&&I.full()) {
			cout << I << endl;
			finished_ = true;
			//++sol_count_;
			//consistent_ = false;
		}

		while (!consistent_ && !I.empty()) {
			v_a = I.pop();
			//cout << " != " << v_a << endl;
			for (IntVar* v : n_->vars)
				v->RestoreUpTo(I.size());

			v_a.v()->RemoveValue(v_a.a(), I.size());
			++statistics_.num_negative;
			x_evt_.push_back(v_a.v());
			consistent_ = v_a.v()->size() && ac_->enforce(x_evt_, I.size()).state;
			x_evt_.clear();
			I.update_model_assigned();
		}

		if (!consistent_)
			finished_ = true;
	}

	statistics_.solve_time = t.elapsed();
	return statistics_;
}

SearchStatistics MAC::enforce_fc(const int time_limits) {
	Timer t;
	//consistent_ = ac_->enforce(n_->vars, 0).state;
	x_evt_.clear();
	//if (!consistent_) {
	//	statistics_.solve_time = t.elapsed();
	//	return statistics_;
	//}

	while (!finished_) {
		if (t.elapsed() > time_limits) {
			statistics_.solve_time = t.elapsed();
			statistics_.time_out = true;
			return statistics_;
		}

		IntVal v_a = select_v_value();
		I.push(v_a);
		//cout << v_a << " I.size() = " << I.size() << endl;
		++statistics_.num_positive;
		v_a.v()->ReduceTo(v_a.a(), I.size());
		x_evt_.push_back(v_a.v());
		consistent_ = ac_->enforce(x_evt_, I.size()).state;
		x_evt_.clear();

		if (consistent_&&I.full()) {
			//cout << I << endl;
			finished_ = true;
			//++sol_count_;
			//consistent_ = false;
		}

		while (!consistent_ && !I.empty()) {
			v_a = I.pop();
			//cout << "!=" << v_a << " I.size() = " << I.size() << endl;
			for (IntVar* v : n_->vars) {
				if (!v->assigned()) {
					v->RestoreUpTo(I.size() + 1);
				}
			}

			v_a.v()->RemoveValue(v_a.a(), I.size());
			++statistics_.num_negative;
			consistent_ = v_a.v()->size();
			//x_evt_.push_back(v_a.v());
			//consistent_ = v_a.v()->size() && ac_->enforce(x_evt_, I.size()).state;
			//x_evt_.clear();
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
	IntVar* v = select_var();
	const int a = select_val(v);
	IntVal val(v, a);
	return val;
}

int MAC::select_val(const IntVar* v) const {
	int val = -1;
	switch (valh_) {
	case Heuristic::VLH_MIN:
		val = v->head();
		break;
	case Heuristic::VLH_MIN_DOM: break;
	case Heuristic::VLH_MIN_INC: break;
	case Heuristic::VLH_MAX_INC: break;
	case Heuristic::VLH_VWDEG: val = -1; break;
	default:;
	}
	return val;
}

IntVar* MAC::select_var() const {
	IntVar* var = nullptr;
	double min_size = DBL_MAX;
	switch (varh_) {
	case Heuristic::VRH_DOM_MIN: {
		for (auto v : n_->vars)
			if (!v->assigned())
				if (v->size() < min_size) {
					min_size = v->size();
					var = v;
				}
	}
								 break;
	case Heuristic::VRH_LEX:
		var = n_->vars[I.size() + 1];
		break;
	case Heuristic::VRH_VWDEG: break;
	case Heuristic::VRH_DOM_DEG_MIN: {
		for (auto v : n_->vars)
			if (!v->assigned()) {
				const int dom_deg = v->size() / n_->neighborhood[v].size();
				if (dom_deg < min_size) {
					min_size = dom_deg;
					var = v;
				}
			}
	}
									 break;
	case Heuristic::VRH_DOM_WDEG_MIN:

		for (auto x : n_->vars) {
			if (!x->assigned()) {
				double x_w = 0.0;
				double x_dw = 0.0;
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
				if (x_dw < min_size) {
					min_size = x_dw;
					var = x;
				}
			}
		}
		break;
	default:
		var = nullptr;
		break;
	}
	return var;
}
}
