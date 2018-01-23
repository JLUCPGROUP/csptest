////#pragma once
//#include "GModel.h"
//#include "Timer.h"
//#include "SAC1.h"
//#include "CPUSolver.h"
////using namespace Gecode;
////using namespace std;
//////#define LOGFILE
//using namespace cudacp;
//#include <gecode/driver.hh>
//#include <gecode/int.hh>
//#include <gecode/minimodel.hh>
//
//const int time_limit = 1800000;
//
//using namespace Gecode;
//using namespace std;
////class Queens : public GModel {
////public:
////	Queens(const int n) {
////		vs = IntVarArray(*this, n, 0, n - 1);
////		mds = n;
////		for (int i = 0; i < n; i++)
////			for (int j = i + 1; j < n; j++) {
////				rel(*this, vs[i] != vs[j]);
////				rel(*this, vs[i] + i != vs[j] + j);
////				rel(*this, vs[i] - i != vs[j] - j);
////			}
////	};
////	Queens(bool share, Queens& s) : GModel(share, s) {
////		//vs.update(*this, share, s.vs);
////	}
////	Space* copy(bool share) override {
////		return new Queens(share, *this);
////	}
////	void Queens::print(void) const {
////		for (size_t i = 0; i < vs.size(); i++) {
////			cout << "(" << i << " = " << vs[i] << ") ";
////		}
////		cout << endl;
////	}
////	~Queens() {};
////};
//
//int main(const int argc, char ** argv) {
//	//if (argc <= 1) {
//	//	std::cout << "no argument" << endl;
//	//	return 0;
//	//}
//
//	//const int n = atoi(argv[1]);
//	int n = 4;
//	GModel* gm = new GModel();
//	gm->vs = IntVarArray(*gm, n, 0, n - 1);
//
//	for (int i = 0; i < n; i++)
//		for (int j = i + 1; j < n; j++) {
//			rel(*gm, gm->vs[i] != gm->vs[j]);
//			rel(*gm, gm->vs[i] + i != gm->vs[j] + j);
//			rel(*gm, gm->vs[i] - i != gm->vs[j] - j);
//		}
//
//	//Queens* qs = new Queens(8);
//	//branch(*qs, qs->vs, INT_VAR_SIZE_MIN(), INT_VALUES_MIN());
//	//qs->print();
//	//DFS<Queens> ee(qs);
//	//if (Queens* ss = ee.next()) {
//	//	ss->print();
//	//	cout << "nodes = " << ee.statistics().node << endl;
//	//	delete ss;
//	//}
//
//
//	//gm->print();
//	////delete gm;
//
//	GModel* dgm = static_cast<GModel*>(gm->clone());
//	Search::TimeStop ts(time_limit);
//	Search::Options options;
//	options.stop = &ts;
//	//branch(*dgm, dgm->vs, INT_VAR_SIZE_MIN(), INT_VALUES_MIN());
//	branch(*dgm, dgm->vs, INT_VAR_NONE(), INT_VALUES_MIN());
//	DFS<GModel> ee(dgm, options);
//	delete dgm;
//	int find = -1;
//	Timer t0;
//	if (GModel* ss = ee.next()) {
//		ss->print();
//		find = 1;
//		cout << "nodes = " << ee.statistics().node << endl;
//		cout << "time = " << t0.elapsed() << endl;
//		delete ss;
//	}
//	else
//		find = 0;
//	const int64_t gecode_solve_time = t0.elapsed();
//	cout << "---------------gecode solving---------------" << endl;
//	cout << "Gecode solve time = " << gecode_solve_time << endl;
//
//
//	//SAC1 sac1(qs);
//	//Timer t;
//	//const bool result = sac1.enforce();
//	//const int64_t sac_time = t.elapsed();
//	//if (!result) {
//	//	cout << "UNSAC || SAC time = " << sac_time << endl;
//	//	cout << "--------------------end---------------------" << endl;
//	//	delete qs;
//	//	return 0;
//	//}
//
//	//cout << "------------------modeling------------------" << endl;
//	//const SearchStatistics statistics = StartSearch(gm, Heuristic::VRH_DOM_MIN, Heuristic::VLH_MIN, time_limit);
//	//const string  slv_str = (statistics.num_sol > 0) ? "SAT!!" : "UNSAT";
//	//delete gm;
//	//cout << "SAC time = " << sac_time << "|| Build time = " << statistics.build_time << endl;
//	//cout << "------------------solving-------------------" << endl;
//	//cout << slv_str << "|| Solve time = " << statistics.solve_time << "|| nodes = " << statistics.nodes << endl;
//	//cout << "------------------sleeping------------------" << endl;
//	gm->print();
//	delete gm;
//	return 0;
//}
//
