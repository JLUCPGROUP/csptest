#pragma once
#include "Network.h"
namespace cudacp {

//inline bool Existed(vector<int>& tuple) {
//	return tuple[0] != INT_MAX;
//}

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
	VarEvt(Network* m);
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

class arc_que {
public:
#define  have(a) vid_set_[a.c_id() * arity_ + a.c()->index(a.v())]
	arc_que() {}
	arc_que(const int cons_size, const int max_arity);
	virtual ~arc_que(){};

	void MakeQue(const size_t cons_size, const size_t max_arity);
	//void DeleteQue();
	bool empty() const;
	bool full() const;
	bool push(arc& ele) throw(std::bad_exception);
	arc pop() throw(std::bad_exception);

private:
	vector<arc> m_data_;
	vector<int> vid_set_;
	size_t arity_;
	size_t m_size_;
	int m_front_;
	int m_rear_;
};

class AssignedStack {
public:
	AssignedStack(Network* m);

	void initial(Network* m);
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
	Network* gm_;
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
	AC(Network *m);
	~AC() {};
	virtual bool EnforceGAC_var(VarEvt* x_evt, const int level = 0) = 0;
	void insert(IntVar* v);
protected:
	vector<IntVar*> q_;
	Network *m_;
	vector<unsigned> stamp_var_;
	vector<unsigned> stamp_tab_;
	unsigned t_ = 0;
	vector<int> tmp_tuple_;
};
class AC3 :public AC {
public:
	AC3(Network *m);
	~AC3() {};
	bool EnforceGAC_var(VarEvt* x_evt, const int level = 0) override;
	bool EnforceGAC_var(vector<IntVar*>& x_evt, const int level = 0);
	bool AC3::EnforceGAC_arc(vector<IntVar*>& x_evt, const int level);
	SearchError se;
protected:
	//pro_que<T> q;
	arc_que Q;
	int level_;
	virtual bool revise(arc& c_x);
	virtual bool seek_support(IntConVal& c_val);
	void inital_q_arc();
	//private:
	//	void inital_Q_arc();
};

class MAC {
public:
	MAC(Network *n);
	void enforce();
	virtual ~MAC();
	int sol_count() const { return sol_count_; }
	void sol_count(int val) { sol_count_ = val; }

private:
	int sol_count_ = 0;
	Network *n_;
	AC3* ac_;
	vector<IntVar*> x_evt_;
	//VarEvt* x_evt_;
	//ACAlgorithm ac_algzm_;
	AssignedStack* I;
	IntVal select_v_value() const;
	bool consistent_;
	bool finished_ = false;
};

}