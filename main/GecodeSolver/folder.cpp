//#pragma once
//#include <gecode/search.hh>
//#include <string>
//#include "XCSP3PrintCallbacks.h"
//#include "BuildGModel.h"
//#include "BMFileParser.h"
//#include "SAC1.h"
//#include <windows.h>
//#include <io.h>  
//#include "Timer.h"
//using namespace cudacp;
//using namespace Gecode;
//using namespace std;
//
//
//const string XPath = "BMPath.xml";
//const int64_t TimeLimit = 1800000;
//const string bmp_root = "E:\\Projects\\benchmarks\\xcsp\\";
//const string bmp_ext = ".xml";
//void getFilesAll(string path, vector<string>& files);
//
//
//int main(const int argc, char ** argv) {
//
//	if (argc <= 1) {
//		cout << "no argument" << endl;
//		return 0;
//	}
//
//	vector<string> files;
//	getFilesAll(bmp_root + argv[1], files);
//	vector<int64_t> nodes;
//	vector<int64_t> solve_time;
//	nodes.reserve(files.size());
//	solve_time.reserve(files.size());
//
//	for (const auto f : files) {
//		cout << f << endl;
//
//		HModel *hm = new HModel();
//		GetHModel(f, hm);
//		GModel* gm = new GModel();
//		BuildGModel(hm, gm);
//
//		Search::TimeStop ts(TimeLimit);
//		Search::Options options;
//		options.stop = &ts;
//		branch(*gm, gm->vs, INT_VAR_SIZE_MIN(), INT_VALUES_MIN());
//		//branch(*gm, gm->vs, INT_VAR_AFC_SIZE_MAX(), INT_VALUES_MIN());
//		DFS<GModel> ee(gm, options);
//
//		Timer t0;
//		if (GModel* ss = ee.next()) {
//			//ss->print();
//			//cout << "nodes = " << ee.statistics().node << endl;
//			delete ss;
//		}
//		nodes.push_back(ee.statistics().node);
//		solve_time.push_back(t0.elapsed());
//
//		delete hm;
//		delete gm;
//	}
//
//	cout << "---------------Gecode_time---------------" << endl;
//	for (auto i : solve_time)
//		cout << i << endl;
//	cout << "---------------nodes---------------" << endl;
//	for (auto i : nodes)
//		cout << i << endl;
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