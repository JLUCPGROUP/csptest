#pragma once
#include <string>
#include <vector>
#include <bitset>
#include <deque>
using namespace std;
namespace  cudacp {
const static int BITSIZE = 32;
class IntVar {
public:
	//explicit IntVar(const int id) :id_(id) {}
	IntVar(const int id, vector<int>& values, const string name);
	~IntVar();

	void remove_value(const int a, const int p = 0);
	void ReduceTo(const int a, const int p = 0);
	void AddValue(const int a);
	void RestoreUpTo(const int p);
	int value(const int idx) const;
	int size() const;
	int capacity() const;
	int assigned() const;
	void assigned(const bool a);
	int next(const int a) const;
	int prev(const int a) const;
	bool have(const int a) const;
	int head() const;
	int tail() const;
	bool faild() const;

	//bool propagated(const int level) const { return (level == propagated_); }
	int stamp() const { return stamp_; }
	void stamp(int val) { stamp_ = val; }
	//void bpresent(cp::bitPre * val) { b_ = val; }
	//int propagated() const { return propagated_; }
	//void propagated(int val) { propagated_ = val; }
protected:
	int id_;
	string name_;
	int num_bit_;
	int init_size_;
	int curr_size_;
	int last_limit_;
	int stamp_ = 0;
	int curr_level_ = 0;
	vector<int> vals_;
	vector<vector<tuple<int, bitset<BITSIZE>>>> bit_doms_;
	inline tuple<int, int> get_bit_index(const int idx) const;

	/*
	int* absent_;
	int* next_;
	int* prev_;
	int* prev_absent_;

	int asin_idx_ = -1;
	int size_;
	int cur_size_;

	int head_ = 0;
	int tail_;
	int tail_absent_ = -1;
	bool assigned_ = false;
	int* ptr_;
	int lmt_;
	int stamp_;
	std::vector<Constraint* > cs_;

	bitPre *b_;
	*/


};
}