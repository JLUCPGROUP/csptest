#include "IntVal.h"
namespace cudacp {
IntVar* IntVal::v() const {
	return v_;
}

void IntVal::v(IntVar* val) {
	v_ = val;
}

const IntVal & IntVal::operator=(const IntVal & rhs) {
	v_ = rhs.v_;
	a_ = rhs.a_;

	return *this;
}

ostream & operator<<(ostream & os, IntVal & v_val) {
	os << "(" << v_val.v_->id() << ", " << v_val.a_ << ")";
	return os;
}

}