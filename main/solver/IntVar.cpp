#include "IntVar.h"
#include <tuple>

namespace cudacp {
IntVar::IntVar(const int id, vector<int>& values, const string name) :
	id_(id), name_(name), num_bit_(ceil(values.size() / BITSIZE)), init_size_(values.size()), curr_size_(values.size()), last_limit_(values.size() % BITSIZE), vals_(values) {
	//initial bit_doms_
	bit_doms_.resize(num_bit_);
	for (size_t i = 0; i < num_bit_; i++) {
		bitset<BITSIZE> a;
		a.set();
		const tuple<int, bitset<BITSIZE>> t = make_tuple(0, a);
		bit_doms_[i].reserve(BITSIZE);
		bit_doms_[i].push_back(t);
	}

	get<1>(bit_doms_[num_bit_ - 1].back()) <<= BITSIZE - last_limit_;
}

void IntVar::remove_value(const int a, const int p) {
	tuple<int, bitset<BITSIZE>> t = bit_doms_[a / BITSIZE].back();

}

tuple<int, int> IntVar::get_bit_index(const int idx) const {
	tuple<int, int> a;
	get<0>(a) = idx / BITSIZE;
	get<1>(a) = idx % BITSIZE;
	return a;
}

void IntVar::bind(const int a) {
	if (map_[a] >= cur_size_) {
		cur_size_ = 0;
	}
	else {
		swap(map_[a], 0);
	}
}

void IntVar::remove(const int a) {
	if (map_[a] < cur_size_) {
		swap(map_[a], cur_size_ - 1);
		--cur_size_;
	}
}

void IntVar::mark(const int a) {
	if (map_[a] < cur_size_ && map_[a] >= mark_) {
		swap(map_[a], mark_);
		++mark_;
	}
}

void IntVar::restrict(){
	cur_size_ = mark_;
}

void IntVar::clear_mark() {
	mark_ = 0;
}

void IntVar::swap(const int i, const int j) {
	//swap dom_[i] and dom_[j].
	dom_[i] = dom_[i] ^ dom_[j];
	dom_[j] = dom_[i] ^ dom_[j];
	dom_[i] = dom_[i] ^ dom_[j];

	//update map_
	map_[dom_[i]] = i;
	map_[dom_[j]] = j;
}
}
