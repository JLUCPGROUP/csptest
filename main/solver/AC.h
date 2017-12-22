#pragma once
#include "Model.h"
namespace cudacp {

class VarEvt {
public:
	VarEvt(Model* m);
	virtual ~VarEvt(){};

	IntVar* operator[](const int i) const;
	void push_back(IntVar* v);
	void clear();
	int size() const;
	IntVar* at(const int i) const;

private:
	//IntVar** vars_;
	vector<IntVar*> vars;
	int size_;
	int cur_size_;
};

class AC {
public:
	AC(Model *m);
	~AC();
	virtual bool EnforceGAC_var(VarEvt* x_evt, const int level = 0) = 0;

};
}

