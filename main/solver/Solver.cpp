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
	//vals_.resize(m->vars.size());
	vals_.reserve(m->vars.size());
	asnd_.resize(m->vars.size(), false);
}

void AssignedStack::initial(Network* m) {
	gm_ = m;
	max_size_ = m->vars.size();
	//vals_.resize(m->vars.size());
	vals_.reserve(m->vars.size());
	asnd_.resize(m->vars.size(), false);
};

void AssignedStack::push(IntVal& v_a) {
	//vals_[top_] = v_a;
	//asnd_[v_a.vid()] = v_a.op();
	//v_a.v()->assign(v_a.op());
	//++top_;
	vals_.push_back(v_a);
	asnd_[v_a.vid()] = v_a.op();
	v_a.v()->assign(v_a.op());
};

IntVal AssignedStack::pop() {
	//--top_;
	//asnd_[vals_[top_].vid()] = false;
	//vals_[top_].v()->assign(false);
	//return vals_[top_];
	auto val = vals_.back();
	vals_.pop_back();
	asnd_[val.vid()] = false;
	val.v()->assign(false);
	return val;
}

//IntVal AssignedStack::top() const { return vals_[top_]; };
//int AssignedStack::size() const { return top_; }
//int AssignedStack::capacity() const { return max_size_; }
//bool AssignedStack::full() const { return top_ == max_size_; }
//bool AssignedStack::empty() const { return top_ == 0; }
//IntVal AssignedStack::operator[](const int i) const { return vals_[i]; };
//IntVal AssignedStack::at(const int i) const { return vals_[i]; };
//void AssignedStack::clear() { top_ = 0; };
//bool AssignedStack::assiged(const int v) const { return asnd_[v]; }
//bool AssignedStack::assiged(const IntVar* v) const { return assiged(v->id()); };
IntVal AssignedStack::top() const { return vals_.back(); }
int AssignedStack::size() const { return vals_.size(); }
int AssignedStack::capacity() const { return vals_.capacity(); }
bool AssignedStack::full() const { return size() == max_size_; }
bool AssignedStack::empty() const { return vals_.empty(); }
IntVal AssignedStack::operator[](const int i) const { return vals_[i]; }
IntVal AssignedStack::at(const int i) const { return vals_[i]; }
void AssignedStack::clear() { vals_.clear(); }

void AssignedStack::del(const IntVal val) {
	remove(vals_.begin(), vals_.end(), val);
	asnd_[val.vid()] = false;
}

bool AssignedStack::assiged(const int v) const { return asnd_[v]; }
bool AssignedStack::assiged(const IntVar* v) const { return assiged(v->id()); }
vector<IntVal> AssignedStack::vals() const { return vals_; }

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

void AssignedStack::update_model_assigned() {
	for (auto val : vals_) 
		val.v()->assign(true);
}

///////////////////////////////////////////////////////////////////////
DeleteExplanation::DeleteExplanation(Network* m) :
	m_(m) {
	val_array_.resize(m_->vars.size(), vector<vector<IntVal>>(m_->max_domain_size()));
}

void DeleteExplanation::initial(Network* m) {
	m_ = m;
	val_array_.resize(m_->vars.size(), vector<vector<IntVal>>(m_->max_domain_size()));
}

vector<IntVal>& DeleteExplanation::operator[](const IntVal val) {
	return val_array_[val.vid()][val.a()];
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

void arc_que::MakeQue(const size_t cons_size, const size_t max_arity) {
	arity_ = max_arity;
	m_size_ = max_arity*cons_size + 1;
	m_front_ = 0;
	m_rear_ = 0;

	m_data_.resize(m_size_);
	vid_set_.resize(m_size_);
}

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
