#pragma once
#include "IntVar.h"
namespace cudacp {
class IntVal {
public:
	IntVal() {}
	IntVal(IntVar *v, const int a) :v_(v), a_(a) {}
	~IntVal() {}

	IntVar* v() const;
	void v(IntVar* val);
	const IntVal& operator=(const IntVal& rhs);
	//const int id() { return v_->id(); }
	int a() const { return a_; }
	void a(const int val) { a_ = val; }
	friend std::ostream& operator<< (std::ostream &os, IntVal &v_val);
private:
	IntVar* v_;
	int a_;
};
}