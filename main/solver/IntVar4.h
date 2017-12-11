//#pragma once
//#include <vector>
//#include <bitset>
//using namespace std;
//namespace cudacp {
//typedef unsigned __int64 u64;
//const int BITSIZE = 64;
//class bitPre {
//public:
//	bitPre() {}
//	bitPre(const int size);
//	virtual ~bitPre();
//	void RemoveValue(const int a);
//	//void ReduceTo(const int a);
//	void AddValue(const int a);
//	//void RestoreUpTo();
//
//	int num_bit_dom;
//	vector<bitset<BITSIZE>> bit_dom;
//};
//class IntVar {
//public:
//	IntVar() {};
//	//IntVar(const int id){}
//	IntVar(const int id, const int *values, const int size);
//	~IntVar();
//
//	void RemoveValue(const int a, const int p = 0);
//	void ReduceTo(const int a, const int p = 0);
//	void AddValue(const int a);
//	void RestoreUpTo(const int p);
//	int* begin();
//	int* next();
//	int* end();
//	int value(const int idx) const;
//	int size() const;
//	int capacity() const;
//	int assigned() const;
//	void assigned(const bool a);
//	int next(const int a) const;
//	int prev(const int a) const;
//	bool have(const int a) const;
//	int head() const;
//	int tail() const;
//	bool faild() const;
//
//	int stamp() const { return stamp_; }
//	void stamp(int val) { stamp_ = val; }
//
//protected:
//	vector<int> vals_;
//
//	vector<int>  absent_;
//	vector<int>  next_;
//	vector<int>  prev_;
//	vector<int>  prev_absent_;
//
//	int asin_idx_ = -1;
//	int size_;
//	int cur_size_;
//
//	int head_ = 0;
//	int tail_;
//	int tail_absent_ = -1;
//	bool assigned_ = false;
//	int* ptr_;
//	int lmt_;
//	int stamp_;
//
//	bitPre *b_;
//
//};
//}