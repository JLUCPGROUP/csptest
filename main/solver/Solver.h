#pragma once
#include "Model.h"
namespace cudacp {
inline bool Existed(vector<int>& tuple) {
	return tuple[0] != INT_MAX;
}

struct SearchStatistics {
	u64 num_sol = 0;
	u64 num_positive = 0;
	u64 num_negative = 0;
	u64 nodes = 0;
	u64 build_time = 0;
	u64 solve_time = 0;
	bool time_out = false;
	//int n_deep = 0;
};

struct SearchError {
	bool seek_support_fail = true;
	bool revise_fail = true;
	int num_delete = 0;
	Tabular * tab = nullptr;
	IntVal v_a_fail;
	int level = 0;
};

class VarEvt {
public:
	VarEvt(Model* m);
	virtual ~VarEvt() {};

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

class AssignedStack {
public:
	AssignedStack() {};

	void initial(Model* m);
	~AssignedStack() {};
	void push(IntVal& v_a);
	IntVal pop();
	IntVal top() const;
	int size() const;
	int capacity() const;
	bool full() const;
	bool empty() const;
	IntVal operator[](const int i) const;
	IntVal at(const int i) const;
	void clear();
	bool assiged(const int v) const;
	friend ostream& operator<< (ostream &os, AssignedStack &I);
	friend ostream& operator<< (ostream &os, AssignedStack* I);

protected:
	Model* gm_;
	vector<IntVal> vals_;
	vector<bool> asnd_;
	int top_ = 0;
	int max_size_;
};

//class var_que {
//public:
//#define  have(a) vid_set_[a.c_id() * arity_ + a.c()->index(a.v())]
//	var_que() {}
//	var_que(const int cons_size, const int max_arity);
//	virtual ~var_que();
//
//	void MakeQue(const size_t cons_size, const size_t max_arity);
//	void DeleteQue();
//	bool empty() const;
//	bool full() const;
//	bool push(arc& ele) throw(std::bad_exception);
//	arc pop() throw(std::bad_exception);
//
//private:
//	vector<IntVar *> vars_;
//	arc* m_data_;
//	int* vid_set_;
//	size_t arity_;
//	size_t m_size_;
//	int m_front_;
//	int m_rear_;
//};

class AC {
public:
	AC(Model *m);
	~AC() {};
	virtual bool EnforceGAC_var(VarEvt* x_evt, const int level = 0) = 0;
	void insert(IntVar* v);
protected:
	vector<IntVar*> q_;
	Model *m_;
	vector<unsigned> stamp_var_;
	vector<unsigned> stamp_tab_;
	unsigned t_ = 0;
	vector<int> tmp_tuple_;
};
class AC3 :public AC {
public:
	AC3(Model *m);
	~AC3() {};
	bool EnforceGAC_var(VarEvt* x_evt, const int level = 0) override;
	SearchError se;
protected:
	//pro_que<T> q;
	int level_;
	virtual bool revise(arc& c_x);
	virtual bool seek_support(IntConVal& c_val);
	//private:
	//	void inital_Q_arc();
};

}