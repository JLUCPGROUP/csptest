//#include "IntVar.h"
//#include <tuple>
//
//namespace cudacp {
//IntVar::IntVar(const int id, vector<int>& values, const string name) :
//	id_(id), name_(name), num_bit_(ceil(values.size() / BITSIZE)), init_size_(values.size()), curr_size_(values.size()),
//	last_limit_(values.size() % BITSIZE), vals_(values) {
//	//initial bit_doms_
//	bit_doms_.reserve(init_size_);
//	vector<bitset<BITSIZE>> a(num_bit_);
//
//	for (auto i : a)
//		i.set();
//	a.back() <<= BITSIZE - last_limit_;
//	bit_doms_.push_back(make_tuple(0, a));
//}
//
//void IntVar::remove_value(const int a, const int p) {
//	auto t = bit_doms_.back();
//
//	if (get<0>(t) > p) {
//		bit_doms_.pop_back();
//		t = bit_doms_.back();
//	}
//
//	auto index = get_bit_index(a);
//	vector<bitset<BITSIZE>> b(get<1>(t));
//	get<1>(t)[get<0>(index)][get<0>(index)] = false;
//}
//
//tuple<int, int> IntVar::get_bit_index(const int idx) const {
//	tuple<int, int> a;
//	get<0>(a) = idx / BITSIZE;
//	get<1>(a) = idx % BITSIZE;
//	return a;
//}
//
//}