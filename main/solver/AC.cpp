#include "AC.h"

namespace cudacp {
VarEvt::VarEvt(Model* m) :
	size_(m->vars.size()),
	cur_size_(0) {
	vars = m->vars;
}

IntVar* VarEvt::operator[](const int i) const {
	return vars[i];
}

int VarEvt::size() const {
	return cur_size_;
}

IntVar* VarEvt::at(const int i) const {
	return vars[i];
}

void VarEvt::push_back(IntVar* v) {
	vars[cur_size_] = v;
	++cur_size_;
}

void VarEvt::clear() {
	cur_size_ = 0;
}
AC::AC(Model* m) {}
AC::~AC() {}
}



