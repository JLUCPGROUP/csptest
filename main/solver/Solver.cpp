#include "Solver.h"
namespace cudacp {

VarEvt::VarEvt(Network* m) :
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


AssignedStack::AssignedStack(Network* m) :gm_(m) {
	max_size_ = m->vars.size();
	vals_.resize(m->vars.size());
	asnd_.resize(m->vars.size(), false);
}

void AssignedStack::initial(Network* m) {
	gm_ = m;
	max_size_ = m->vars.size();
	vals_.resize(m->vars.size());
	asnd_.resize(m->vars.size(), false);
};

void AssignedStack::push(IntVal& v_a) {
	vals_[top_] = v_a;
	asnd_[v_a.vid()] = v_a.op();
	v_a.v()->assign(v_a.op());
	++top_;
};

IntVal AssignedStack::pop() {
	--top_;
	asnd_[vals_[top_].vid()] = false;
	vals_[top_].v()->assign(false);
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

///////////////////////////////////////////////////////////////////////
arc_que::arc_que(const int cons_size, const int max_arity) :
	arity_(max_arity),
	m_size_(max_arity*cons_size + 1),
	m_front_(0),
	m_rear_(0) {
	m_data_.resize(m_size_);
	vid_set_.resize(m_size_);
}

//arc_que::~arc_que() {
//	DeleteQue();
//}

void arc_que::MakeQue(const size_t cons_size, const size_t max_arity) {
	arity_ = max_arity;
	m_size_ = max_arity*cons_size + 1;
	m_front_ = 0;
	m_rear_ = 0;

	m_data_.resize(m_size_);
	vid_set_.resize(m_size_);
}

//void arc_que::DeleteQue() {
//	delete[] m_data_;
//	delete[] vid_set_;
//	m_data_ = NULL;
//	vid_set_ = NULL;
//	arity_ = 0;
//}

bool arc_que::empty() const {
	return m_front_ == m_rear_;
}

bool arc_que::full() const {
	return m_front_ == (m_rear_ + 1) % m_size_;
}

bool arc_que::push(arc& ele) throw(std::bad_exception) {
	if (full())
		throw std::bad_exception();

	if (have(ele))
		return false;

	m_data_[m_rear_] = ele;
	m_rear_ = (m_rear_ + 1) % m_size_;
	have(ele) = 1;

	return true;
}

arc arc_que::pop() throw(std::bad_exception) {
	if (empty())
		throw std::bad_exception();

	arc tmp = m_data_[m_front_];
	m_front_ = (m_front_ + 1) % m_size_;
	have(tmp) = 0;

	return tmp;
}
/////////////////////////////////////////////////////////////////////////

}
