#pragma once
#include <sstream>
#include <string>
#include "Network.h"
#include "BMFileParser.h"
#include "XCSP3PrintCallbacks.h"
#include "Solver.h"
//#include "Timer.h"
//#include "BMFileParser.h"
//#include "XCSP3PrintCallbacks.h"

using namespace cudacp;
using namespace std;
//#define LOGFILE

const int TimeLimit = 600000;
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
	//VarEvt* ve = new VarEvt(n);

	AC3 ac(n);
	long t = clock_t();
	ac.EnforceGAC_var(n->vars, 0);
	long t2 = clock_t();
	cout << t2 - t << endl;
	//delete ve;
	delete n;
	delete hm;
	return 0;
}