#include "IntVar.h"
namespace cudacp {
IntVar::IntVar(const int id, const string name, const vector<int> values) :
	id_(id), cur_size_(values.size()), init_size_(values.size()), name_(name), dom_(values) {

	for (int i = 0; i < cur_size_; ++i)
		map_[dom_[i]] = i;

}


bool IntVar::constains(const int value) {
	return map_[value] < cur_size_;
}

int IntVar::size() const {
	return cur_size_;
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
