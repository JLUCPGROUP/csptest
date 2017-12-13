/*
* HModel.h
*
*  Created on: 2017年9月26日
*      Author: leezear
*/

#ifndef HMODEL_H_
#define HMODEL_H_
#define _EXPORTING
#ifdef _EXPORTING
#define API_DECLSPEC __declspec(dllexport)
#else
#define API_DECLSPEC __declspec(dllimport)
#endif

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <cmath>
#include <map>
#include <numeric>
#include <valarray>



namespace cudacp {

const int MAX_VALUE = INT_MAX - 4096;
const int MAX_OPT = INT_MIN + 4096;

enum PredicateOperator {
	PO_NON = INT_MIN, 		///<空		-2147483648
	PO_L_PAR = INT_MIN + 1,	///<左括号	-2147483647
	PO_R_PAR = INT_MIN + 2,	///<右括号	-2147483646
	PO_COMMA = INT_MIN + 3,	///<逗号		-2147483645
	PO_ADD = INT_MIN + 10,	///<加法		-2147483638
	PO_SUB = INT_MIN + 11,	///<减法		-2147483637
	PO_MUL = INT_MIN + 12,	///<乘法		-2147483636
	PO_DIV = INT_MIN + 13,	///<除法		-2147483635
	PO_MOD = INT_MIN + 14,	///<取余		-2147483634
	PO_EQ = INT_MIN + 30,	///<"="		-2147483618
	PO_NE = INT_MIN + 31,	///<"!="	-2147483617
	PO_LT = INT_MIN + 32,	///<"<"		-2147483616
	PO_LE = INT_MIN + 33,	///<"<="	-2147483615
	PO_GT = INT_MIN + 34,	///<">"		-2147483614
	PO_GE = INT_MIN + 35,	///<">="	-2147483613
	PO_ABS = INT_MIN + 60,	///<取模		-2147483588
	PO_AND = INT_MIN + 101,	///<逻辑与	-2147483547
	PO_OR = INT_MIN + 102,	///<逻辑或	-2147483546
	PO_DIST = INT_MIN + 103	///<abs(x-y)
};

enum ExpType {
	ET_OP, ET_CONST, ET_VAR
};

enum ConType {
	CT_EXT, CT_INT
};
typedef std::unordered_map<std::string, std::function<int(std::vector<int>&)>> func_map;

namespace Funcs {
//auto Opposite = 
//unordered_map<string, function<int(int, int)>> predictions = {
//	{ '*', [](int i, int j) {return i * j; } },
//};
//auto mod = [](int i, int j) {return i%j; };
namespace ops {
auto neg = [](std::vector<int>& a) {return 0 - a[0]; };
auto abs = [](std::vector<int>& a) {return std::abs(a[0]); };
auto add = [](std::vector<int>& a) {return accumulate(a.begin(), a.end(), 0); };
auto sub = [](std::vector<int>& a) {return a[0] - a[1]; };
auto mul = [](std::vector<int>& a) {return accumulate(a.begin(), a.end(), 1, std::multiplies<int>()); };
auto div = [](std::vector<int>& a) {return a[0] / a[1]; };
auto mod = [](std::vector<int>& a) {return a[0] % a[1]; };
auto sqr = [](std::vector<int>& a) {return static_cast<int>(sqrt(a[0])); };
auto pow = [](std::vector<int>& a) {return std::pow(a[0], a[1]); };
auto min = [](std::vector<int>& a) {return *min_element(a.begin(), a.end()); };
auto max = [](std::vector<int>& a) {return *max_element(a.begin(), a.end()); };
auto dist = [](std::vector<int>& a) {return abs(std::vector<int>(sub(a))); };

auto le = [](std::vector<int>& a) {return a[0] <= a[1]; };
auto lt = [](std::vector<int>& a) {return a[0] < a[1]; };
auto ge = [](std::vector<int>& a) {return a[0] >= a[1]; };
auto gt = [](std::vector<int>& a) {return a[0] > a[1]; };
auto ne = [](std::vector<int>& a) {return a[0] != a[1]; };
auto eq = [](std::vector<int>& a) {return std::all_of(a.begin(), a.end(), [&a](int n) {return n == a[0]; }); };

auto not = [](std::vector<int>& a) {return !a[0]; };
auto and = [](std::vector<int>& a) {return std::all_of(a.begin(), a.end(), [&a](int n) {return n && a[0]; }); };
auto or = [](std::vector<int>& a) {return std::any_of(a.begin(), a.end(), [&a](int n) {return n || a[0]; }); };
//auto xor =[](std::vector<int>& a){return std::for_each(a.begin(), a.end(),[](int b))}
}

func_map int_pres_map =
{
	{ "neg", ops::neg },
	{ "abs", ops::abs },
	{ "add", ops::add },
	{ "sub", ops::sub },
	{ "mul", ops::mul },
	{ "div", ops::div },
	{ "mod", ops::mod },
	{ "sqr", ops::sqr },
	{ "pow", ops::pow },
	{ "min", ops::min },
	{ "max", ops::max },
	{"dist",ops::dist},
	{ "le", ops::le },
	{ "lt", ops::lt },
	{ "ge", ops::ge },
	{ "gt", ops::gt },
	{ "ne", ops::ne },
	{ "eq", ops::eq },
	{ "not", ops::not},
	{ "and", ops::and },
	{ "or", ops:: or },
};

}

using namespace std;
class API_DECLSPEC PostfixExpr {
public:
	vector<int> stack;
	vector<string> data;
	explicit PostfixExpr(const string exprs);
	//int get_operator(string s);
	//void push(int expr);
	//void pop();
	//int top();
};

class API_DECLSPEC HVar {
public:
	int id;
	string name;
	vector<int> vals;
	unordered_map<int, int> val_map;
	vector<int> anti_map;
	const int std_min = 0;
	const int std_max;
	HVar(const int id, const string name, const int min_val, const int max_val);
	HVar(const int id, const string name, vector<int>& v);
	~HVar();
	void Show();
private:
};

//class API_DECLSPEC HCon {
//public:
//	int id;
//	//string name;
//	vector<HVar*> scope;
//	bool isSTD = false;
//	ConType type;
//	HCon(const int id, const ConType ct) :
//		id(id), type(ct) {};
//	HCon(const int id, const ConType ct, vector<HVar*>& scp) :
//		id(id), scope(scp), type(ct) {};
//};

class API_DECLSPEC HTab {
public:
	int id;
	vector<HVar*> scope;
	bool isSTD = false;
	bool semantics;
	vector<vector<int>> tuples;
	HTab(const int id, const bool sem, vector<vector<int>>& ts, vector<HVar*>& scp);
	HTab(HTab* t, vector<HVar*>& scp);
	int GetAllSize() const;
	void GetSTDTuple(vector<int>& src_tuple, vector<int>& std_tuple);
	void GetORITuple(vector<int>& std_tuple, vector<int>& ori_tuple);
	bool SAT(vector<int>& t);
	bool SAT_STD(vector<int>& t);
	void Show();
	void GetTuple(int idx, vector<int>& t, vector<int>& t_idx);
private:
	//临时变量
	vector<int> tmp_t_;
	API_DECLSPEC friend ostream& operator<<(ostream &os, const vector<HVar*>& a);
};

//class API_DECLSPEC HPre :public HCon {
//public:
//	bool isSTD = false;
//	HPre(const int id, string expr);
//	vector<string> data;
//	//void get_postfix(string expr);
//
//	//int GetAllSize() const;
//	//void GetSTDTuple(vector<int>& src_tuple, vector<int>& std_tuple);
//	//void GetORITuple(vector<int>& std_tuple, vector<int>& ori_tuple);
//	//bool SAT(vector<int>& t);
//	//bool SAT_STD(vector<int>& t);
//	//void Show();
//	//void GetTuple(int idx, vector<int>& t, vector<int>& t_idx);
//private:
//	//临时变量
//	//vector<int> tmp_t_;
//	//API_DECLSPEC friend ostream& operator<<(ostream &os, const vector<HVar*>& a);
//};

class API_DECLSPEC HModel {
public:
	vector<HVar*> vars;
	vector<HTab*> tabs;
	unordered_map<HVar*, vector<HTab*>> subscriptions;

	HModel();
	virtual ~HModel();
	int AddVar(const string name, const int min_val, const int max_val);
	int AddVar(const string name, vector<int>& v);
	//int AddCon(const ConType type, const string expr);
	//int AddCon(const ConType type, const bool sem, vector<vector<int>>& ts, vector<HVar*>& scp);
	//void AddCon(HCon* c, vector<string>& scp);
	int AddTab(const bool sem, vector<vector<int>>& ts, vector<HVar*>& scp);
	int AddTab(const bool sem, vector<vector<int>>& ts, vector<string>& scp);
	int AddTab(const string expr);
	int AddTabAsPrevious(HTab* t, vector<string>& scp);
	int max_domain_size() const { return mds_; }
	int max_arity() const { return mas_; };
	void show();
private:
	void get_postfix(const string expr, vector<string>& stack, vector<string>& scp);
	ExpType get_type(std::string expr);
	void subscript(HTab *t);
	void get_scope(vector<string>& scp_str, vector<HVar*>& scp);
	unordered_map<string, HVar*> var_n_;
	size_t mds_ = 0;
	size_t mas_ = 0;
};

} /* namespace cudacp */

#endif /* HMODEL_H_ */