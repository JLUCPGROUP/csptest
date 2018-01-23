#pragma once
#include "Network.h"
#include "Timer.h"

namespace cudacp {

enum ACAlgorithm {
	AC_1, AC_2, AC_3, AC_4, AC_6, AC_7, AC_2001, AC_3bit, AC_3rm, STR_1, STR_2, STR_3, A_FC, A_FC_bit, A_RNSQ
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

enum VarHeu {
	DOM, DOM_WDEG
};

namespace Heuristic {
enum Var {
	VRH_LEX,
	VRH_DOM_MIN,
	VRH_VWDEG,
	VRH_DOM_DEG_MIN,
	VRH_DOM_WDEG_MIN
};

enum Val {
	VLH_MIN,
	VLH_MIN_DOM,
	VLH_MIN_INC,
	VLH_MAX_INC,
	VLH_VWDEG
};

enum DecisionScheme {
	DS_BI,
	DS_NB
};
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
	int& have(const arc a) { return vid_set_[a.c_id() * arity_ + a.c()->index(a.v())]; };
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
	AssignedStack() {}
	void update_model_assigned();;
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
	void del(const IntVal val);
	bool assiged(const int v) const;
	bool assiged(const IntVar* v) const;
	vector<IntVal> vals() const;
	friend ostream& operator<< (ostream &os, AssignedStack &I);
	friend ostream& operator<< (ostream &os, AssignedStack* I);

protected:
	Network* gm_;
	vector<IntVal> vals_;
	vector<bool> asnd_;
	//int top_ = 0;
	int max_size_;
};

class DeleteExplanation {
public:
	DeleteExplanation() {};
	DeleteExplanation(Network *m);
	~DeleteExplanation() {}
	void initial(Network* m);
	vector<IntVal>& operator[](const IntVal val);
protected:
	vector<vector<vector<IntVal>>> val_array_;
	Network *m_;
};

class var_que {
public:
	int& have(const IntVar* v) { return vid_set_[v->id()]; };
	var_que() {}
	//var_que(Network* n);
	virtual ~var_que() {};

	//void initial(Network* n);
	//void DeleteQue();
	bool have(IntVar* v);
	bool empty() const;
	void initial(const int size);
	bool full() const;
	void push(IntVar* v);
	IntVar* pop();
	void clear();
	int max_size() const;
	int size() const;

private:
	vector<IntVar*> m_data_;
	vector<int> vid_set_;
	size_t max_size_;
	int m_front_;
	int m_rear_;
	int size_;
};

class AC {
public:
	AC(Network *m);
	AC(Network *m, const LookAhead look_ahead, const LookBack look_back);
	virtual ~AC() {};
	//virtual bool enforce(VarEvt* x_evt, const int level = 0) = 0;
	virtual ConsistencyState enforce(vector<IntVar*>& x_evt, const int level = 0) = 0;
	//virtual ConsistencyState enforce_arc(vector<IntVar*>& x_evt, const int level) = 0;
	ConsistencyState cs;
	//void q_insert(IntVar* v);
	void insert(IntVar* v);
	int del() const { return delete_; }
protected:
	//vector<IntVar*> q_;
	var_que q_;
	Network *m_;
	vector<u64> stamp_var_;
	vector<u64> stamp_tab_;
	u64 t_ = 0;
	vector<int> tmp_tuple_;
	int delete_ = 0;
	LookAhead la_;
	LookBack lb_;
};

class AC3 :public AC {
public:
	AC3(Network *m);
	virtual ~AC3() {};
	//bool enforce(VarEvt* x_evt, const int level = 0) override;
	ConsistencyState enforce(vector<IntVar*>& x_evt, const int level = 0) override;
	//ConsistencyState enforce_arc(vector<IntVar*>& x_evt, const int level = 0) override;
	//SearchError se;

protected:
	//pro_que<T> q;
	//arc_que Q;
	int level_ = 0;
	virtual bool revise(arc& c_x);
	virtual bool seek_support(IntConVal& c_val);
	//void inital_q_arc();
	//private:
	//	void inital_Q_arc();
};

class FC :public AC3 {
public:
	FC(Network* n);
	ConsistencyState enforce(vector<IntVar*>& x_evt, const int level = 0) override;
private:
	//int max_bitDom_size_;
	//vector<vector<bitset<BITSIZE>>> bitSup_;
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

class FCbit :public AC3bit {
public:
	FCbit(Network* n);
	ConsistencyState enforce(vector<IntVar*>& x_evt, const int level = 0) override;
private:
	//int max_bitDom_size_;
	//vector<vector<bitset<BITSIZE>>> bitSup_;
};

class SAC1 {
public:
	SAC1(Network* n, ACAlgorithm a);
	virtual bool enforce(vector<IntVar*> x_evt, const int level);
	virtual ~SAC1();
protected:
	int del_ = 0;
	int level_;
	Network* n_;
	AC* ac_;
	ACAlgorithm ac_algzm_;
	vector<IntVar*> x_evt_;
};

class Qsac {
public:
	Qsac() {};
	Qsac(Network* n, const VarHeu h);
	~Qsac() {};
	void initial(Network* n, const VarHeu h);
	void push(IntVal val);
	IntVal pop();
	bool empty() const;
	int size(IntVar* v) const;
	void update();
	bool vars_assigned();
	void reset();
	bool vars_assigned(AssignedStack& I) const;
	bool in(IntVal v);
	//void delete_vals();
protected:
	int head(IntVar* v) const;
	IntVal select_v_value() const;
	vector<bitSetVector> bitDoms_;
	Network* n_;
	VarHeu h_;
	int num_bit_vars_;
	vector<bitset<BITSIZE>> vars_assigned_;
	vector<bitset<BITSIZE>> vars_assigned_old_;
	vector<bitset<BITSIZE>> tmp_empty_;
};

class SAC3 :public SAC1 {
public:
	SAC3(Network* n, ACAlgorithm a, const VarHeu h);
	bool enforce(vector<IntVar*> x_evt, const int level) override;
protected:
	IntVal BuildBranch();
	Qsac q_;
	VarHeu h_;
	AssignedStack I_;
};

class RNSQ :public AC3bit {
public:
	RNSQ(Network *m);
	ConsistencyState conditionFC(IntVar* v, const int level = 0);
	ConsistencyState neiborAC(vector<IntVar*>& x_evt, IntVar* x, const int level = 0);
	ConsistencyState enforce(vector<IntVar*>& x_evt, const int level = 0) override;
protected:
	unordered_map<IntVar*, bitSetVector> neibor_;
	bool is_neibor(IntVar* x, IntVar* v);
	var_que q_nei_;
	var_que q_var_;
	void insert_(var_que& q, IntVar* v);
	bool in_neibor_exp(Tabular* t, IntVar* x);
	bool in_neibor(Tabular* t, IntVar* x);
	bool has_sigleton_domain_neibor(IntVar* x) const;
};

class MAC {
public:
	MAC(Network *n, ACAlgorithm ac_algzm, const Heuristic::Var varh, const Heuristic::Val valh);
	SearchStatistics enforce(const int time_limits);
	SearchStatistics enforce_fc(const int time_limits);
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
	int select_val(const IntVar* v) const;
	IntVar* select_var() const;
	bool consistent_;
	bool finished_ = false;
	SearchStatistics statistics_;
	Heuristic::Var varh_;
	Heuristic::Val valh_;
};

//class Search {
//public:
//	Search(Network *n, const LookAhead look_ahead, const LookBack look_back, const Consistency consistency);
//	//SearchStatistics enforce(const int time_limits);
//	virtual ~Search();
//	int sol_count() const { return sol_count_; }
//	void sol_count(const int val) { sol_count_ = val; }
//	virtual vector<IntVal> HandleEmptyDomain(IntVar* v);
//	//virtual vector<IntVal> CheckConsistencyAfterAssignment(IntVar *v);
//	//virtual vector<IntVal> CheckConsistencyAfterRefutati(IntVar *v);
//	virtual void UndoAssignment(IntVal v_a);
//
//private:
//	int sol_count_ = 0;
//	Network *n_;
//	AC* ac_;
//	vector<IntVar*> x_evt_;
//	//VarEvt* x_evt_;
//	Consistency c_type_;
//	vector<IntVal> nogood;
//	AssignedStack I;
//	IntVal select_v_value() const;
//	bool consistent_;
//	bool finished_ = false;
//	SearchStatistics statistics_;
//	LookAhead la_;
//	LookBack lb_;
//	DeleteExplanation expl;
//};

}
