#pragma once
#include "IntVar.h"

using namespace std;

namespace cudacp {
class Tabular {
public:
	Tabular(const int id, const vector<IntVar*>& scope, vector<vector<int>>& tuples) :
		id_(id), scope_(scope), tuples_(tuples) {};
	~Tabular() {};

	vector<int>& GetFirstValidTuple(IntVal& v_a);
protected:
	int id_;
	vector<IntVar*> scope_;
	vector<vector<int>> tuples_;

};
}