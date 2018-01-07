#include "Solver.h"
namespace cudacp {
void var_que::reserve(const int size) {
	m_data_.reserve(size);
	vid_set_.resize(size, 0);
}

void var_que::push(IntVar* v) {
	m_data_.push_back(v);
	vid_set_[v->id()] = 1;
}

IntVar* var_que::pop() {
	IntVar* v = m_data_.back();
	m_data_.pop_back();
	vid_set_[v->id()] = 0;
	return v;
}

void var_que::clear() {
	m_data_.clear();
	vid_set_.assign(vid_set_.size(), 0);
}

AC::AC(Network* m) :m_(m) {
	q_.reserve(m_->vars.size());
	stamp_var_.resize(m_->vars.size(), 0);
	stamp_tab_.resize(m_->tabs.size(), 0);
	tmp_tuple_.resize(m_->max_arity());
	Exclude(tmp_tuple_);
}

AC::AC(Network* m, const LookAhead look_ahead, const LookBack look_back) :
	m_(m),
	la_(look_ahead),
	lb_(look_back) {
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


}
