#pragma once
#include <sstream>
#include <fstream>
#include <string>
#include "XCSP3PrintCallbacks.h"
#include "BMFileParser.h"
#include "Timer.h"
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
	hm->Show();

	delete hm;
	return 0;
}