#include "Solver.h"
namespace cudacp {
AC3bit::AC3bit(Network * m) :
	AC3(m),
	max_bitDom_size_(m->max_bitDom_size()) {
	if (m_->max_arity() != 2) {
		std::cout << "error!" << std::endl;
	}
	const vector<bitset<BITSIZE>> a(max_bitDom_size_, 0);
	bitSup_.resize(m_->tabs.size()*m_->vars.size()*m_->max_domain_size(), a);
	for (Tabular* c : m_->tabs) {
		for (auto t : c->tuples()) {
			const int index[] = { m_->GetIntConValIndex(IntConVal(c, c->scope[0], t[0])),
				m_->GetIntConValIndex(IntConVal(c, c->scope[1], t[1])) };
			bitSup_[index[0]][t[0] / BITSIZE].set(t[1] % BITSIZE);
			bitSup_[index[1]][t[1] / BITSIZE].set(t[0] % BITSIZE);
		}
	}
}

bool AC3bit::seek_support(IntConVal& c_val) {
	for (IntVar *y : c_val.c()->scope)
		if (y != c_val.v())
			for (int i = 0; i < max_bitDom_size_; ++i)
				if ((bitSup_[m_->GetIntConValIndex(c_val)][i] & y->bitDom()[i]) != 0)
					return true;

	return false;
}
}
