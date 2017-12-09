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

}
