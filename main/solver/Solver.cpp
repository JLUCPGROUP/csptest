//#include "Model.h"
//#include "HModel.h"
//
//namespace cudacp {
////Model::Model(HModel* h) :
////	hm_(h) {
////	//vars.reserve(hm_->vars.size());
////	//for (int i = 0; i < num_vars_; ++i)
////	//{
////	//	
////	//}
////	//IntVar *v = new IntVar()
////	//vars.push_back()
////}
//Model::Model(HModel* h) :
//	hm_(h),
//	num_vars_(hm_->vars.size()),
//	num_tabs_(hm_->tabs.size()) {
//	vars.reserve(num_vars_);
//	tabs.reserve(num_tabs_);
//
//	for (auto hv : hm_->vars) {
//		IntVar *v = new IntVar(hv, num_vars_);
//		vars.push_back(v);
//	}
//}
//}
