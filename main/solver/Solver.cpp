#include "Solver.h"
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


void AssignedStack::initial(Model* m) {
	gm_ = m;
	max_size_ = m->vars.size();
	vals_.resize(m->vars.size());
	asnd_.resize(m->vars.size(), false);
};

void AssignedStack::push(IntVal& v_a) {
	vals_[top_] = v_a;
	asnd_[v_a.v] = v_a.aop ? true : false;
	++top_;
};

IntVal AssignedStack::pop() {
	--top_;
	asnd_[vals_[top_].v] = false;
	return vals_[top_];
}

IntVal AssignedStack::top() const { return vals_[top_]; };
int AssignedStack::size() const { return top_; }
int AssignedStack::capacity() const { return max_size_; }
bool AssignedStack::full() const { return top_ == max_size_; }
bool AssignedStack::empty() const { return top_ == 0; }
IntVal AssignedStack::operator[](const int i) const { return vals_[i]; };
IntVal AssignedStack::at(const int i) const { return vals_[i]; };
void AssignedStack::clear() { top_ = 0; };
bool AssignedStack::assiged(const int v) const { return asnd_[v]; };

ostream & operator<<(ostream & os, AssignedStack & I) {
	for (int i = 0; i < I.size(); ++i)
		os << I[i] << " ";
	return os;
}

ostream & operator<<(ostream & os, AssignedStack * I) {
	for (int i = 0; i < I->size(); ++i)
		os << I->at(i) << " ";
	return os;
}


AC::AC(Model* m) :m_(m) {
	q_.reserve(m_->vars.size());
	stamp_var_.resize(m_->vars.size(), 0);
	stamp_tab_.resize(m_->tabs.size(), 0);
	tmp_tuple_.reserve(m_->max_arity());
}

void AC::insert(IntVar* v) {
	q_.push_back(v);
	++t_;
	stamp_var_[v->id()] = t_;
}

AC3::AC3(Model* m) :
	AC(m) {


}

bool AC3::EnforceGAC_var(VarEvt* x_evt, const int level) {
	level_ = level;
	q_.clear();

	for (int i = 0; i < x_evt->size(); ++i)
		insert(x_evt->at(i));

	while (!q_.empty()) {
		IntVar* x = q_.back();
		for (Tabular* c : m_->subscription[x]) {
			if (stamp_var_[x->id()] > stamp_tab_[c->id()]) {
				for (auto y : c->scope) {
					if (!y->assigned()) {
						bool aa = false;
						for (auto z : c->scope)
							if ((z != x) && stamp_var_[z->id()] > stamp_tab_[c->id()])
								aa = true;

						if ((y != x) || aa) {
							if (revise(arc(c, y))) {
								if (y->size() == 0) {
									return false;
								}
								insert(y);
							}
						}

					}
				}
				++t_;
				stamp_tab_[c->id()] = t_;
			}
		}
	}
	return true;
}

bool AC3::revise(arc& c_x) {
	const int num_elements = c_x.v()->size();
	int a = c_x.v()->head();

	while (a != Limits::INDEX_OVERFLOW) {
		if (!seek_support(IntConVal(c_x, a))) {
			c_x.v()->RemoveValue(a, level_);
			//std::cout << "(" << c_x.v_id() << ", " << a << ")" << std::endl;
			++se.num_delete;
		}
		a = c_x.v()->next(a);
	}

	return num_elements != c_x.v()->size();
}

bool AC3::seek_support(IntConVal & c_val) {
	m_->GetFirstValidTuple(c_val, tmp_tuple_);

	while (Existed(tmp_tuple_))
		if (c_val.c()->sat(tmp_tuple_))
			return true;
		else
			m_->GetNextValidTuple(c_val, tmp_tuple_);

	return false;
}
}
