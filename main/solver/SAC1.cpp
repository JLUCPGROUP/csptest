#include "SAC1.h"

namespace cudacp {
SAC1::SAC1(Network* n, const ACAlgorithm a) :
	n_(n), {
	switch (a) {
	case AC_3:
		ac_ = new AC3(n_);
		break;
	case AC_3bit:
		ac_ = new AC3bit(n_);
		break;
	case A_FC:
		ac_ = new FC(n_);
		break;
	case A_FC_bit:
		ac_ = new FCbit(n_);
		break;
		//case AC_3rm:
		//	ac_ = new AC3rm(nt_);
	default:
		break;
	}
}
SAC1::~SAC1() {
	// TODO Auto-generated destructor stub

}

bool SAC1::enforce() const {

}

}
