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


namespace Functions {
//auto Opposite = 
//unordered_map<string, function<int(int, int)>> predictions = {
//	{ '*', [](int i, int j) {return i * j; } },
//};
//auto mod = [](int i, int j) {return i%j; };
auto neg = [](std::vector<int>& a) {return 0 - a[0]; };
auto abs = [](std::vector<int>& a) {return std::abs(a[0]); };
auto add = [](std::vector<int>& a) {return accumulate(a.begin(), a.end(), 0); };
auto sub = [](std::vector<int>& a) {return a[0] - a[1]; };
auto mul = [](std::vector<int>& a) {return accumulate(a.begin(), a.end(), 1, std::multiplies<int>()); };
auto div = [](std::vector<int>& a) {return a[0] / a[1]; };
auto mod = [](std::vector<int>& a) {return a[0] % a[1]; };
auto sqr = [](std::vector<int>& a) {return static_cast<int>(sqrt(a[0])); };
auto pow = [](std::vector<int>& a) {return std::pow(a[0], a[1]); };
auto min = [](std::vector<int>& a) {return std::min_element(a.begin(), a.end()); };
auto max = [](std::vector<int>& a) {return std::max_element(a.begin(), a.end()); };
auto dis = [](std::vector<int>& a) {return abs(std::vector<int>(sub(a))); };

auto le = [](std::vector<int>& a) {return a[0] <= a[1]; };
auto lt = [](std::vector<int>& a) {return a[0] < a[1]; };

std::unordered_map<std::string, std::function<int(std::vector<int>&)>> int_pres_map =
{
	{"neg", neg},
	{"abs", abs},
	{"add", add},
};
//function<int<vector<int>&>>

}

using namespace std;
class API_DECLSPEC PostfixExpr {
	vector<int> data_;
	void push(int expr);
	void pop();
	int top();
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

class API_DECLSPEC HCon {
public:
	int id;
	//string name;
	vector<HVar*> scope;
	bool isSTD = false;
	HCon(const int id) :
		id(id) {};
	HCon(const int id, vector<HVar*>& scp) :
		id(id), scope(scp) {};
};

class API_DECLSPEC HTab :public HCon {
public:
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

class API_DECLSPEC HPre :public HCon {
public:
	bool isSTD = false;
	HPre(const int id, string expr);
	//int GetAllSize() const;
	//void GetSTDTuple(vector<int>& src_tuple, vector<int>& std_tuple);
	//void GetORITuple(vector<int>& std_tuple, vector<int>& ori_tuple);
	//bool SAT(vector<int>& t);
	//bool SAT_STD(vector<int>& t);
	//void Show();
	//void GetTuple(int idx, vector<int>& t, vector<int>& t_idx);
private:
	//临时变量
	//vector<int> tmp_t_;
	//API_DECLSPEC friend ostream& operator<<(ostream &os, const vector<HVar*>& a);
};

class API_DECLSPEC HModel {
public:
	vector<HVar*> vars;
	vector<HTab*> tabs;
	unordered_map<string, HVar*> var_n_;
	//	unordered_map<int, HVar*> var_i_;
	HModel();
	virtual ~HModel();
	void AddVar(const int id, const string name, const int min_val,
		const int max_val);
	void AddVar(const int id, const string name, vector<int>& v);
	void AddTab(const int id, const bool sem, vector<vector<int>>& ts, vector<HVar*>& scp);
	void AddTab(const int id, const bool sem, vector<vector<int>>& ts, vector<string>& scp);
	void AddTabAsPrevious(HTab* t, vector<string>& scp);
	int max_domain_size() const { return mds_; }
	void show();
private:
	size_t mds_ = 0;
	size_t mas_ = 0;
};

} /* namespace cudacp */

#endif /* HMODEL_H_ */
