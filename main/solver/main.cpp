#pragma once
#include <sstream>
#include <string>
#include "Network.h"
#include "BMFileParser.h"
#include "XCSP3PrintCallbacks.h"
#include "Solver.h"
#include "Timer.h"
//#include "Timer.h"
//#include "BMFileParser.h"
//#include "XCSP3PrintCallbacks.h"

using namespace cudacp;
using namespace std;
//#define LOGFILE

const int TimeLimit = 1800000000000;
const string x_path = "BMPath.xml";

int main() {
	string bm_path;
	if (FindBMPath(x_path))
		bm_path = _bm_path;
	cout << bm_path << endl;
	HModel *hm = new HModel();
	GetHModel(bm_path, hm);
	//hm->show();
	Network* n = new Network(hm);
	//AC3bit ac(n);
	//ac.enforce_arc(n->vars, 0);

	MAC mac(n, AC_3bit, DOM_WDEG);
	const SearchStatistics statistics = mac.enforce(TimeLimit);
	cout << "time = " << statistics.solve_time << endl;
	cout << "positive = " << statistics.num_positive << endl;
	cout << "negative = " << statistics.num_negative << endl;
	//n->show();
	//SAC1 sac(n, AC_3bit);
	//sac.enforce(n->vars, 0);
	//n->show();
	delete n;
	delete hm;
	return 0;
}