//#include "IntVar.h"
//namespace cudacp {
//IntVar::IntVar(const int id, const string name, const vector<int> values) :
//	id_(id), curr_size_(values.size()), init_size_(values.size()), name_(name), dom_(values) {
//
//	for (int i = 0; i < curr_size_; ++i)
//		map_[dom_[i]] = i;
//
//	//absent_.resize(init_size_);
//}
//
//
//bool IntVar::constains(const int value) {
//	return map_[value] < curr_size_;
//}
//
//int IntVar::size() const {
//	return curr_size_;
//}
//
//void IntVar::swap(const int i, const int j) {
//	//swap dom_[i] and dom[j]
//	dom_[i] = dom_[i] ^ dom_[j];
//	dom_[j] = dom_[i] ^ dom_[j];
//	dom_[i] = dom_[i] ^ dom_[j];
//
//	//update map
//	map_[dom_[i]] = j;
//	map_[dom_[j]] = i;
//}
//
//void IntVar::bind(const int v) {
//	if (map_[v] < curr_size_)
//		curr_size_ = 0;
//	else {
//		swap(map_[v], 0);
//		curr_size_ = 1;
//	}
//}
//
//void IntVar::remove(const int v) {
//	if (map_[v] < curr_size_) {
//		swap(map_[v], curr_size_ - 1);
//		--curr_size_;
//	}
//}
//
//void IntVar::clear_marks() {
//	mark_ = 0;
//}
//
//void IntVar::mark(const int v) {
//	if (map_[v] < curr_size_&&map_[v] >= mark_) {
//		swap(map_[v], mark_);
//		++mark_;
//	}
//}
//
//void IntVar::restrict(){
//	curr_size_ = mark_;
//}
//
//
//}
