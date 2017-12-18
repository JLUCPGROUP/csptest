/*
* SAC1.cpp
*
*  Created on: 2016年11月17日
*      Author: leezear
*/

#include "SAC1.h"
using namespace Gecode;
namespace cudacp {
SAC1::SAC1(GModel* model) :
	model(model) {
	// TODO Auto-generated constructor stub

}

SAC1::~SAC1() {
	// TODO Auto-generated destructor stub
}

bool SAC1::enforce() const {
	bool modified = false;
	SpaceStatus status = model->status();

	if (status == SS_FAILED)
		return false;

	do {
		modified = false;
		for (int i = 0; i < model->vs.size(); i++) {
			IntVar v = model->vs[i];
			Int::IntView view(v);
			//for (IntVarValues k(view); k();++k) {
			for (int j = view.min(); j <= view.max(); ++j) {
				if (view.in(j)) {
					GModel *s = static_cast<GModel*>(model->clone());
					Int::IntView sv(s->vs[i]);
					sv.eq(*s, j);
					status = s->status();
					if (status == SS_FAILED) {
						view.nq(*model, j);
						status = model->status();
						if (status == SS_FAILED)
							return false;

						modified = true;
					}
					delete s;
				}
			}
		}
	} while (modified);

	return true;
}
} /* namespace cudacp */
