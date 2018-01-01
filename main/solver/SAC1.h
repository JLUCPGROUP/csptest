#pragma once
#include "Network.h"
#include "Solver.h"

namespace cudacp {
class SAC1 {
public:
	SAC1(Network* n, ACAlgorithm a);
	bool enforce() const;
	virtual ~SAC1();
protected:
	Network* n_;
	AC* ac_;
	ACAlgorithm ac_algzm_;
};
}

