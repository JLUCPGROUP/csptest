#pragma once
#include "Network.h"
#include "Timer.h"

namespace cudacp {

enum ACAlgorithm {
	AC_1, AC_2, AC_3, AC_4, AC_6, AC_7, AC_2001, AC_3bit, AC_3rm, STR_1, STR_2, STR_3
};
enum Consistency {
	C_AC3, C_AC4, C_AC2001, C_AC3bit, C_AC3rm, C_STR1, C_STRC2, C_STR3, C_FC
};

enum LookBack {
	LB_SBT, LB_IBT, LB_DBT

};

enum LookAhead {
	LA_BC, LA_FC, LA_MAC
};

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

struct ConsistencyState {
	//是否执行失败
	bool state;
	bool seek_support_fail = true;
	bool revise_fail = true;
	//删除变量值数
	int num_delete = 0;
	//造成失败的约束
	Tabular* tab = nullptr;
	//造成失败的变量
	IntVar* var = nullptr;
	IntVal v_a_fail;
	int level = 0;
};

struct SearchError {
	bool seek_support_fail = true;
	bool revise_fail = true;
	int num_delete = 0;
	Tabular * tab = nullptr;
	IntVar* v = nullptr;
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
	virtual ~arc_que() {};

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
	AssignedStack() {};
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
	bool assiged(const IntVar* v) const;
	vector<IntVal> vals() const;
	friend ostream& operator<< (ostream &os, AssignedStack &I);
	friend ostream& operator<< (ostream &os, AssignedStack* I);

protected:
	Network* gm_;
	vector<IntVal> vals_;
	vector<bool> asnd_;
	int top_ = 0;
	int max_size_;
};

class DeleteExplanation {
public:
	DeleteExplanation(Network *m);
	~DeleteExplanation() {}
	void initial(Network* m);
	vector<IntVal>& operator[](const IntVal val);
protected:
	vector<vector<vector<IntVal>>> val_array_;
	Network *m_;
};

class AC {
public:
	AC(Network *m);
	AC(Network *m, const LookAhead look_ahead, const LookBack look_back);
	virtual ~AC() {};
	virtual bool EnforceGAC_var(VarEvt* x_evt, const int level = 0) = 0;
	virtual ConsistencyState EnforceGAC_var(vector<IntVar*>& x_evt, const int level = 0) = 0;
	virtual ConsistencyState EnforceGAC_arc(vector<IntVar*>& x_evt, const int level) = 0;
	void insert(IntVar* v);

	int del() const { return delete_; }
protected:
	vector<IntVar*> q_;
	Network *m_;
	vector<unsigned> stamp_var_;
	vector<unsigned> stamp_tab_;
	unsigned t_ = 0;
	vector<int> tmp_tuple_;
	int delete_ = 0;
	LookAhead la_;
	LookBack lb_;
};
class AC3 :public AC {
public:
	AC3(Network *m);
	virtual ~AC3() {};
	bool EnforceGAC_var(VarEvt* x_evt, const int level = 0) override;
	ConsistencyState EnforceGAC_var(vector<IntVar*>& x_evt, const int level = 0) override;
	ConsistencyState EnforceGAC_arc(vector<IntVar*>& x_evt, const int level = 0) override;
	//SearchError se;
	ConsistencyState cs;
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

class AC3bit :
	public AC3 {
public:
	AC3bit(Network *m);
	virtual ~AC3bit() {};
protected:
	virtual bool seek_support(IntConVal& c_val) override;
	int max_bitDom_size_;
	vector<vector<bitset<BITSIZE>>> bitSup_;
};

class MAC {
public:
	MAC(Network *n, ACAlgorithm ac_algzm);
	SearchStatistics enforce(const int time_limits);
	virtual ~MAC();
	int sol_count() const { return sol_count_; }
	void sol_count(const int val) { sol_count_ = val; }

private:
	int sol_count_ = 0;
	Network *n_;
	AC* ac_;
	vector<IntVar*> x_evt_;
	//VarEvt* x_evt_;
	ACAlgorithm ac_algzm_;
	AssignedStack I;
	IntVal select_v_value() const;
	bool consistent_;
	bool finished_ = false;
	SearchStatistics statistics_;
};

class Search {
public:
	Search(Network *n, const LookAhead look_ahead, const LookBack look_back, const Consistency consistency);
	SearchStatistics enforce(const int time_limits);
	virtual ~Search();
	int sol_count() const { return sol_count_; }
	void sol_count(const int val) { sol_count_ = val; }
	virtual vector<IntVal> HandleEmptyDomain(IntVar* v);
	virtual vector<IntVal> CheckConsistencyAfterAssignment(IntVar *v);
	virtual vector<IntVal> CheckConsistencyAfterRefutati(IntVar *v);
	virtual void UndoAssignment(IntVal v_a);

private:
	int sol_count_ = 0;
	Network *n_;
	AC* ac_;
	vector<IntVar*> x_evt_;
	//VarEvt* x_evt_;
	Consistency c_type_;
	vector<IntVal> nogood;
	AssignedStack I;
	IntVal select_v_value() const;
	bool consistent_;
	bool finished_ = false;
	SearchStatistics statistics_;
	LookAhead la_;
	LookBack lb_;
	DeleteExplanation expl;
};

}
