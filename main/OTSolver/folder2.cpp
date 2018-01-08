//#pragma once
//#include <string>
//#include <iostream>
//#include "ortools/base/join.h"
//#include "XCSP3PrintCallbacks.h"
//#include "BMFileParser.h"
//#include "ortools/constraint_solver/constraint_solveri.h"
//#include "ortools/base/logging.h"
//#include "ortools/util/tuple_set.h"
//#include <windows.h>
//#include <io.h> 
//using namespace cudacp;
//using namespace std;
//using namespace operations_research;
//
//
//const int64 time_limit = 1800000;
//const string XPath = "BMPath.xml";
//const string bmp_root = "E:\\Projects\\benchmarks\\xcsp\\";
//const string bmp_ext = ".xml";
//void getFilesAll(string path, vector<string>& files);
//
//class MySearchMonitor : public SearchMonitor {
//public:
//	explicit MySearchMonitor(Solver* s)
//		: SearchMonitor(s) {}
//
//	//void EnterSearch() override {
//	//	std::cout << "123" << endl;
//	//}
//	void ApplyDecision(Decision * const d) override {
//		++n_;
//	}
//	int64 nodes() const {
//		return n_;
//	}
//private:
//
//	int64 n_ = 0;
//};
//
//int main(const int argc, char ** argv) {
//
//	if (argc <= 1) {
//		std::cout << "no argument" << endl;
//		return 0;
//	}
//
//	vector<string> files;
//	getFilesAll(bmp_root + argv[1], files);
//
//	vector<int64> search_times;
//	vector<int64> nodes;
//	for (const auto f : files) {
//		int64_t solve_time = time_limit;
//
//		HModel *hm = new HModel();
//		GetHModel(f, hm);
//		Solver s("CPSimple");
//		vector<IntVar*> vars(hm->vars.size());
//
//		for (auto v : hm->vars)
//			vars[v->id] = s.MakeIntVar(v->vals, v->name);
//
//		for (auto t : hm->tabs) {
//			vector<IntVar*> vs;
//			for (auto v : t->scope)
//				vs.push_back(vars[v->id]);
//			IntTupleSet ts(t->scope.size());
//			ts.InsertAll(t->tuples);
//			s.AddConstraint(s.MakeAllowedAssignments(vs, ts));
//			ts.Clear();
//		}
//
//		DecisionBuilder* const db = s.MakePhase(vars, Solver::CHOOSE_MIN_SIZE, Solver::ASSIGN_MIN_VALUE);
//		SearchLimit* limit = s.MakeTimeLimit(time_limit);
//		MySearchMonitor * const sm = new MySearchMonitor(&s);
//		s.NewSearch(db, limit, sm);
//		s.NextSolution();
//		search_times.push_back(s.wall_time());
//		nodes.push_back(sm->nodes());
//
//		s.EndSearch();
//
//		delete sm;
//		delete hm;
//	}
//
//	cout << "---------------search_times---------------" << endl;
//	for (int64 i : search_times) {
//		cout << i << endl;
//	}
//	cout << "---------------nodes---------------" << endl;
//	for (int64 i : nodes) {
//		cout << i << endl;
//	}
//
//	return 0;
//};
//
//void getFilesAll(const string path, vector<string>& files) {
//	//文件句柄 
//	intptr_t  hFile = 0;
//	//文件信息 
//	struct _finddata_t fileinfo;
//	string p;
//	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
//		do {
//			if ((fileinfo.attrib & _A_SUBDIR)) {
//				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
//					//files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
//					getFilesAll(p.assign(path).append("\\").append(fileinfo.name), files);
//				}
//			}
//			else {
//				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
//			}
//		} while (_findnext(hFile, &fileinfo) == 0);
//		_findclose(hFile);
//	}
//}