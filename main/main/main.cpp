// main.cpp: 定义控制台应用程序的入口点。
//
#include <windows.h>
#include <io.h>
#include "XCSP3PrintCallbacks.h"
#include "BMFileParser.h"
using namespace std;
using namespace cudacp;

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


