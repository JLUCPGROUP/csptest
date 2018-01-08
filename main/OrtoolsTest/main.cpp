//#pragma once
//#include <string>
//#include <iostream>
//#include "ortools/base/join.h"
//#include "XCSP3PrintCallbacks.h"
//#include "BMFileParser.h"
//#include "ortools/constraint_solver/constraint_solveri.h"
//#include "ortools/base/logging.h"
//#include "ortools/util/tuple_set.h"
//
//using namespace cudacp;
//using namespace std;
//using namespace operations_research;
//
////#define LOGFILE
//const int64 time_limit = 400000;
////const int64 time_limit = 50000;
//const string XPath = "BMPath.xml";
//const string bmp_root = "E:/Projects/benchmarks/xcsp/";
//const string bmp_ext = ".xml";
////const string bmp_ext = "_X2.xml";
////const int num_bm = 10;
//
//int main(const int argc, char ** argv) {
//
//	if (argc <= 1) {
//		std::cout << "no argument" << endl;
//		return 0;
//	}
//	//string num_bm_str = argv[2];
//	const int num_bm = atoi(argv[2]);
//
//	for (size_t i = 0; i < argc - 2; i++) {
//		int64 solve_time = 0;
//		int64 num_solve = 0;
//		int64 num_nosolution = 0;
//		int64 num_node = 0;
//
//		for (size_t j = 0; j < num_bm; j++) {
//			char num[3];
//			sprintf_s(num, "%02d", j);
//			const string bm_path = bmp_root + argv[i + 1] + num + bmp_ext;
//			std::cout << bm_path << endl;
//			HModel *hm = new HModel();
//			GetHModel(bm_path, hm);
//
//
//			//////////////////////////////////////////////////////////////////////////
//			Solver s("CPSimple");
//			vector<IntVar*> vars(hm->vars.size());
//
//			for (auto v : hm->vars)
//				vars[v->id] = s.MakeIntVar(v->vals, v->name);
//
//			for (auto t : hm->tabs) {
//				vector<IntVar*> vs;
//				for (auto v : t->scope)
//					vs.push_back(vars[v->id]);
//				IntTupleSet ts(t->scope.size());
//				ts.InsertAll(t->tuples);
//				s.AddConstraint(s.MakeAllowedAssignments(vs, ts));
//				ts.Clear();
//			}
//			//std::cout << "-------------------------solve-------------------------" << endl;
//			DecisionBuilder* const db = s.MakePhase(vars, Solver::CHOOSE_MIN_SIZE, Solver::ASSIGN_MIN_VALUE);
//			SearchLimit* limit = s.MakeTimeLimit(time_limit);
//			s.NewSearch(db, limit);
//
//			if (s.NextSolution()) {
//				solve_time += s.wall_time();
//				num_node += s.branches();
//				++num_solve;
//
//				//for (size_t k = 0; k < vars.size(); ++k) 
//				//	cout << "(" << k << " = " << vars[k]->Value() << ") ";
//				//cout << endl;
//			}
//			else {
//				if (s.wall_time() < s.GetTime(limit)) {
//					//no solution
//					solve_time += s.wall_time();
//					num_node += s.branches();
//					++num_solve;
//					++num_nosolution;
//				}
//			}
//
//			s.EndSearch();
//			delete hm;
//			//std::cout << "--------------------------end--------------------------" << endl;
//		}
//		std::cout << "---------------------------------------------------------------------" << endl;
//		std::cout << argv[i + 1] << endl;
//		std::cout << "num_solved = " << num_solve << " || no solutions = " << num_nosolution << " || nodes = " << num_node << " || sum time = " << solve_time << endl;
//	}
//	return 0;
//};