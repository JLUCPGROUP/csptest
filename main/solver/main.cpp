//#pragma once
//#include <sstream>
//#include <string>
//#include "Network.h"
//#include "BMFileParser.h"
//#include "XCSP3PrintCallbacks.h"
//#include "Solver.h"
//#include "Timer.h"
////#include "Timer.h"
////#include "BMFileParser.h"
////#include "XCSP3PrintCallbacks.h"
//
//using namespace cudacp;
//using namespace std;
////#define LOGFILE
//
//const int TimeLimit = 1800000;
//const string x_path = "BMPath.xml";
//
//int main() {
//	string bm_path;
//	if (FindBMPath(x_path))
//		bm_path = _bm_path;
//	cout << bm_path << endl;
//	HModel *hm = new HModel();
//	GetHModel(bm_path, hm);
//	//hm->show();
//	Network* n = new Network(hm);
//
//	MAC* mac = new MAC(n, AC_3bit);
//	const SearchStatistics statistics = mac->enforce(TimeLimit);
//	cout << " time = " << statistics.solve_time << endl;
//	cout << " positive = " << statistics.num_positive << endl;
//	cout << " negative = " << statistics.num_negative << endl;
//	delete mac;
//	delete n;
//	delete hm;
//	return 0;
//}