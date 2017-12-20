#pragma once
#include <string>
#include <vector>
#include <bitset>
#include <deque>
#include <tuple>
#include <string>
using namespace std;

namespace  cudacp {
//const string MARK(256, '1');
template<int N>
class IntVar {
public:
	//explicit IntVar(const int id) :id_(id) {}
	IntVar(const int id, vector<int>& values, const string name) :
		id_(id),
		name_(name),
		init_size_(values.size()),
		curr_size_(values.size()),
		vals_(values) {
		bit_doms_.reserve(init_size_);
		bitset<N> a;
		a.set();
		a << N - init_size_;
		bit_doms_.push_back(make_tuple(0, a));
	};

	~IntVar();

	void RemoveValue(const int a, const int p = 0) {
		auto t = bit_doms_.back();

		if (get<0>(t) >= p) {
			bit_doms_.pop_back();
			t = bit_doms_.back();
		}

		get<0>(t) = p;
		get<1>(t)[a] = 0;
		bit_doms_.push_back(t);
		--curr_size_;
	};

	void ReduceTo(const int a, const int p = 0) {
		auto t = bit_doms_.back();

		if (get<0>(t) >= p) {
			bit_doms_.pop_back();
			t = bit_doms_.back();
		}

		get<0>(t) = p;
		get<1>(t).reset();
		get<1>(t)[a] = 1;
		bit_doms_.push_back(t);
		curr_size_ = 1;
	};

	void AddValue(const int a) {
		auto t = bit_doms_.back();
		get<1>(t)[a] = 1;
		bit_doms_.push_back(t);
		++curr_size_;
	}

	void RestoreUpTo(const int p) {
		auto t = bit_doms_.back();

		if (get<0>(t) > p) {
			bit_doms_.pop_back();
			t = bit_doms_.back();
		}
	}

	int value(const int idx) const {
		vals_[idx];
	}
	int size() const { return curr_size_; };
	int capacity() const { return init_size_; };
	bool assigned() const { return assigned_; };
	void assign(const bool a) { assigned_ = a; };
	int next(const int a) const;
	int prev(const int a) const;
	bool have(const int a) const;
	int head() const;
	int tail() const;
	bool faild() const { return cur_size_ == 0; };

	//bool propagated(const int level) const { return (level == propagated_); }
	int stamp() const { return stamp_; }
	void stamp(int val) { stamp_ = val; }
	//void bpresent(cp::bitPre * val) { b_ = val; }
	//int propagated() const { return propagated_; }
	//void propagated(int val) { propagated_ = val; }
protected:
	int id_;
	string name_;
	int init_size_;
	int curr_size_;
	int value_;
	int stamp_ = 0;
	bool assigned_ = false;
	//int last_limit_;
	//int curr_level_ = 0;
	vector<int> vals_;
	vector<tuple<int, bitset<N>>> bit_doms_;
	//inline tuple<int, int> get_bit_index(const int idx) const;

};
}