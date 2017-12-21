#pragma once
#include "IntVar.h"
#include "Tabular.h"
#include "HModel.h"
namespace cudacp
{
class Solver {
public:
	vector<IntVar*> vars;
	vector<Tabular*> tabs;
	Solver(HModel* h);
	~Solver();
private:
	HModel *hm_;
	const int num_vars_;
	const int num_tabs_;
};
}