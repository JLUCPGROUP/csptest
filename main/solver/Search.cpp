#pragma once
#include "Solver.h"
namespace cudacp {
Search::Search(Network *n, const LookAhead la, const LookBack lb, const Consistency c) :
	n_(n),
	consistency_(c),
	la_(la),
	lb_(lb) {

}
}

