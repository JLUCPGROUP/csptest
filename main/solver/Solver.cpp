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


void AssignedStack::initial(Network* m) {
	gm_ = m;
	max_size_ = m->vars.size();
	vals_.resize(m->vars.size());
	asnd_.resize(m->vars.size(), false);
};

void AssignedStack::push(IntVal& v_a) {
	vals_[top_] = v_a;
	asnd_[v_a.vid()] = v_a.op();
	++top_;
};

IntVal AssignedStack::pop() {
	--top_;
	asnd_[vals_[top_].vid()] = false;
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

AC::AC(Network* m) :m_(m) {
	q_.reserve(m_->vars.size());
	stamp_var_.resize(m_->vars.size(), 0);
	stamp_tab_.resize(m_->tabs.size(), 0);
	tmp_tuple_.resize(m_->max_arity());
	Exclude(tmp_tuple_);
}

void AC::insert(IntVar* v) {
	q_.push_back(v);
	++t_;
	stamp_var_[v->id()] = t_;
}

AC3::AC3(Network* m) :
	AC(m) {
	inital_q_arc();
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

bool AC3::EnforceGAC_var(vector<IntVar*>& x_evt, const int level) {
	level_ = level;
	q_.clear();

	//for (int i = 0; i < x_evt.size(); ++i)
		//insert(x_evt->at(i));
	for (auto v : x_evt)
		insert(v);
	while (!q_.empty()) {
		IntVar* x = q_.back();
		q_.pop_back();
		for (Tabular* c : m_->subscription[x]) {
			if (stamp_var_[x->id()] > stamp_tab_[c->id()]) {
				for (auto y : c->scope) {
					if (!y->assigned()) {
						bool aa = false;
						for (auto z : c->scope) {
							if ((z != x) && stamp_var_[z->id()] > stamp_tab_[c->id()])
								aa = true;
						}


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

bool AC3::EnforceGAC_arc(vector<IntVar*>& x_evt, const int level) {
	level_ = level;
	arc c_x;

	for (int i = 0; i < x_evt.size(); ++i)
		for (Tabular* c : m_->subscription[x_evt[i]])
			for (IntVar* x : c->scope)
				if (x != x_evt[i])
					Q.push(arc(c, x));

	while (!Q.empty()) {
		c_x = Q.pop();
		//cout << "--" << c_x << endl;

		if (revise(c_x)) {
			if (c_x.v()->faild())
				return false;

			for (Tabular* c : m_->subscription[c_x.v()])
				if (c != c_x.c())
					for (IntVar* v : c->scope)
						if ((v != c_x.v()) && (!v->assigned()))
							Q.push(arc(c, v));
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
			std::cout << "(" << c_x.v_id() << ", " << a << ")" << std::endl;
			++se.num_delete;
		}
		a = c_x.v()->next(a);
	}

	return num_elements != c_x.v()->size();
}

bool AC3::seek_support(IntConVal & c_val) {
	m_->GetFirstValidTuple(c_val, tmp_tuple_);
	//cout << "c-value" << c_val << endl;
	while (Existed(tmp_tuple_)) {
		//cout << "tuple: " << tmp_tuple_[0] << "," << tmp_tuple_[1] << endl;
		if (c_val.c()->sat(tmp_tuple_))
			return true;
		else
			m_->GetNextValidTuple(c_val, tmp_tuple_);
	}
	return false;
}

void AC3::inital_q_arc() {
	Q.MakeQue(m_->tabs.size(), m_->max_arity());
}
}
