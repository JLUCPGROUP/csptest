#pragma once
#include <sstream>
#include <string>
#include "XCSP3PrintCallbacks.h"
#include "BMFileParser.h"
#include "Timer.h"
#include "GModel.h"
#include "BuildGModel.h"
#include "SAC1.h"

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
	GModel* gm = new GModel();
	BuildGModel(hm, gm);
	//gm->status();
	//gm->print();

	int s[2] = { 0, 0 };

	for (auto v : gm->vs)
		s[0] += v.size();

	gm->status();

	for (auto v : gm->vs)
		s[1] += v.size();

	cout << s[0] - s[1] << endl;

	//branch(*gm, gm->vs, INT_VAR_SIZE_MIN(), INT_VALUES_MIN());
	//DFS<GModel> ee(gm);

	//if (GModel* ss = ee.next()) {
	//	ss->print();
	//	cout << "nodes = " << ee.statistics().node << endl;
	//	delete ss;
	//}

	//GModel* dgm = static_cast<GModel*>(gm->clone());
	//Search::TimeStop ts(200000);
	//Search::Options options;
	//options.stop = &ts;
	//branch(*dgm, dgm->vs, INT_VAR_SIZE_MIN(), INT_VALUES_MIN());
	//DFS<GModel> ee(dgm, options);
	//delete dgm;

	//int find = -1;
	//Timer t0;
	//if (GModel* ss = ee.next()) {
	//	ss->print();
	//	find = 1;
	//	cout << "nodes = " << ee.statistics().node << endl;
	//	delete ss;
	//}
	//else
	//	find = 0;

	//const int64_t gecode_solve_time = t0.elapsed();
	//cout << "---------------gecode solving---------------" << endl;
	//cout << "Gecode solve time = " << gecode_solve_time << endl;

	//SAC1 sac1(gm);
	//Timer t;
	//const bool result = sac1.enforce();
	//const int64_t sac_time = t.elapsed();

	//gm->print();
	//if (!result) {
	//	cout << "UNSAC || SAC time = " << sac_time << endl;
	//	cout << "--------------------end---------------------" << endl;
	//	delete hm;
	//	delete gm;
	//	return 0;
	//}
	//s[1] = 0;
	//for (auto v : gm->vs)
	//	s[1] += v.size();
	//cout << s[0] - s[1] << endl;

	delete gm;
	delete hm;
	return 0;
}