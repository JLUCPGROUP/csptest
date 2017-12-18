/*
* GModel.h
*
*  Created on: 2017年9月27日
*      Author: leezear
*/

#pragma once
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

namespace cudacp {

using namespace Gecode;
using namespace std;
class GModel : public Space {
public:
	IntVarArray vs;
	int mds;
	GModel();
	GModel(bool share, GModel& s);
	Space* copy(bool share) override;
	void print(void) const;
	virtual ~GModel();
};

} /* namespace cudacp */

