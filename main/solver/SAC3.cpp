#include "Solver.h"
namespace cudacp {
Qsac::Qsac(Network* n, const VarHeu h) :
	n_(n),
	h_(h) {
	bitDoms_.resize(n_->vars.size(), bitSetDom(n_->max_bitDom_size()));

	for (int i = 0; i < n_->max_bitDom_size(); ++i) {
		bitDoms_[i].assign(n_->vars[i]->bitDom().begin(), n_->vars[i]->bitDom().end());
	}
}

int Qsac::size(IntVar* v) {
	int size = 0;
	for (auto& a : bitDoms_[v->id()])
		size += a.count();
	return size;
}

IntVal Qsac::select_v_value() const {
	IntVal val(nullptr, -1);
	switch (h_) {
	case DOM: {
		float min_size = FLT_MAX;
		for (auto v : n_->vars)
			if (!v->assigned())
				if (size(v) < min_size) {
					min_size = size(v);
					val.v(v);
				}
		val.a(val.v()->head());
	}
			  break;
	case DOM_WDEG: {
		float min_size = FLT_MAX;
		for (auto x : n_->vars) {
			if (!x->assigned()) {
				float x_w = 0.0;
				float x_dw = 0.0;
				for (auto c : n_->subscription[x]) {
					int cnt = 0;
					for (auto y : c->scope)
						if (!y->assigned())
							++cnt;
					if (cnt > 1)
						x_w += c->weight;
				}

				if (x->size() == 1 || x_w == 0)
					x_dw = -1;
				else
					x_dw = x->size() / x_w;
				if (x_dw < min_size) {
					min_size = x_dw;
					val.v(x);
				}
			}
		}
		val.a(val.v()->head());
	}
				   break;
	default:;
	}
	return val;
}
}
