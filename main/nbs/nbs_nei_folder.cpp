//#pragma once
//#include <gecode/search.hh>
//#include <string>
//#include "BuildGModel.h"
//#include "SAC1.h"
//#include "CPUSolver.h"
//#include <windows.h>
//#include <io.h>  
//#include "XCSP3PrintCallbacks.h"
//using namespace cudacp;
//using namespace std;
//
//
//const string XPath = "BMPath.xml";
//const int64_t TimeLimit = 1800000;
//const string X_PATH = "E:\\Projects\\benchmarks\\xcsp\\";
//const string bmp_ext = ".xml";
//void getFilesAll(string path, vector<string>& files);
//
//
//int main(const int argc, char ** argv) {
//
//	if (argc <= 1) {
//		std::cout << "no argument" << endl;
//		return 0;
//	}
//
//	vector<string> files;
//	getFilesAll(X_PATH + argv[1], files);
//
//	vector<u64> build_times;
//	vector<u64> search_times;
//	vector<u64> nodes;
//
//	for (const auto f : files) {
//		cout << f << endl;
//		HModel *hm = new HModel();
//		GetHModel(f, hm);
//
//		//const SearchStatistics statistics = StartSearch(hm, Heuristic::VRH_DOM_MIN, Heuristic::VLH_MIN, TimeLimit, true, 0);
//		//const SearchStatistics statistics = StartSearch(hm, Heuristic::VRH_DOM_DEG_MIN, Heuristic::VLH_MIN, TimeLimit, true, 0);
//		const SearchStatistics statistics = StartSearch(hm, Heuristic::VRH_DOM_WDEG_MIN, Heuristic::VLH_MIN, TimeLimit, true, 0);
//		build_times.push_back(statistics.build_time);
//		search_times.push_back(statistics.solve_time);
//		nodes.push_back(statistics.nodes);
//
//		delete hm;
//	}
//	cout << "---------------build_times---------------" << endl;
//	for (u64 i : build_times) {
//		cout << i << endl;
//	}
//	cout << "---------------search_times---------------" << endl;
//	for (u64 i : search_times) {
//		cout << i << endl;
//	}
//	cout << "---------------nodes---------------" << endl;
//	for (u64 i : nodes) {
//		cout << i << endl;
//	}
//
//	return 0;
//}
//
//void getFilesAll(const string path, vector<string>& files) {
//	//文件句柄 
//	intptr_t  h_file = 0;
//	//文件信息 
//	struct _finddata_t fileinfo;
//	string p;
//	if ((h_file = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
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
//		} while (_findnext(h_file, &fileinfo) == 0);
//		_findclose(h_file);
//	}
//}