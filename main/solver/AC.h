//#pragma once
//#include "Network.h"
//namespace cudacp {
//
//class VarEvt {
//public:
//	VarEvt(Network* m);
//	virtual ~VarEvt(){};
//
//	IntVar* operator[](const int i) const;
//	void push_back(IntVar* v);
//	void clear();
//	int size() const;
//	IntVar* at(const int i) const;
//
//private:
//	//IntVar** vars_;
//	vector<IntVar*> vars;
//	int size_;
//	int cur_size_;
//};
//
//class AC {
//public:
//	AC(Network *m);
//	~AC();
//	virtual bool enforce(VarEvt* x_evt, const int level = 0) = 0;
//
//};
//}
//
