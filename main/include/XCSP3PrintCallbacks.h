/*=============================================================================
* parser for CSP instances represented in XCSP3 Format
*
* Copyright (c) 2015 xcsp.org (contact <at> xcsp.org)
* Copyright (c) 2008 Olivier ROUSSEL (olivier.roussel <at> cril.univ-artois.fr)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*=============================================================================
*/
#ifndef COSOCO_XCSP3PRINTCALLBACKS_H
#define COSOCO_XCSP3PRINTCALLBACKS_H
#include "XCSP3CoreParser.h"
#include "XCSP3CoreCallbacks.h"
#include "XCSP3Variable.h"
#include "HModel.h"
using namespace cudacp;
/**
* This is an example that prints useful informations of a XCSP3 instance.
* You need to create your own class and to override functions of the callback.
* We suggest to make a map between XVariable and your own variables in order to
* facilitate the constructions of constraints.
*
* see main.cc to show declaration of the parser
*
*/

namespace XCSP3Core {

class API_DECLSPEC XCSP3PrintCallbacks : public XCSP3CoreCallbacks {
public:
	HModel* hm;

	XCSP3PrintCallbacks();

	virtual void beginInstance(InstanceType type) override;

	virtual void endInstance() override;

	virtual void beginVariables() override;

	virtual void endVariables() override;

	virtual void beginVariableArray(string id) override;

	virtual void endVariableArray() override;

	virtual void beginConstraints() override;

	virtual void endConstraints() override;

	virtual void beginGroup(string id) override;

	virtual void endGroup() override;

	virtual void beginBlock(string classes) override;

	virtual void endBlock() override;

	virtual void beginSlide(string id, bool circular) override;

	virtual void endSlide() override;

	virtual void beginObjectives() override;

	virtual void endObjectives() override;


	virtual void buildVariableInteger(string id, int minValue, int maxValue) override;

	virtual void buildVariableInteger(string id, vector<int> &values) override;

	virtual void buildConstraintExtension(string id, vector<XVariable *> list, vector<vector<int>> &tuples, bool support, bool hasStar) override;

	virtual void buildConstraintExtension(string id, XVariable *variable, vector<int> &tuples, bool support, bool hasStar) override;

	virtual void buildConstraintExtensionAs(string id, vector<XVariable *> list, bool support, bool hasStar) override;

	virtual void buildConstraintIntension(string id, string expr) override;

	virtual void buildConstraintPrimitive(string id, OrderType op, XVariable *x, int k, XVariable *y) override;


	virtual void buildConstraintRegular(string id, vector<XVariable *> &list, string st, vector<string> &final, vector<XTransition> &transitions) override;

	virtual void buildConstraintMDD(string id, vector<XVariable *> &list, vector<XTransition> &transitions) override;

	virtual void buildConstraintAlldifferent(string id, vector<XVariable *> &list) override;

	virtual void buildConstraintAlldifferentExcept(string id, vector<XVariable *> &list, vector<int> &except) override;

	virtual void buildConstraintAlldifferentList(string id, vector<vector<XVariable *>> &lists) override;

	virtual void buildConstraintAlldifferentMatrix(string id, vector<vector<XVariable *>> &matrix) override;

	virtual void buildConstraintAllEqual(string id, vector<XVariable *> &list) override;

	virtual void buildConstraintNotAllEqual(string id, vector<XVariable *> &list) override;

	virtual void buildConstraintOrdered(string id, vector<XVariable *> &list, OrderType order) override;

	virtual void buildConstraintLex(string id, vector<vector<XVariable *>> &lists, OrderType order) override;

	virtual void buildConstraintLexMatrix(string id, vector<vector<XVariable *>> &matrix, OrderType order) override;

	virtual void buildConstraintSum(string id, vector<XVariable *> &list, vector<int> &coeffs, XCondition &cond) override;

	virtual void buildConstraintSum(string id, vector<XVariable *> &list, XCondition &cond) override;

	virtual void buildConstraintSum(string id, vector<XVariable *> &list, vector<XVariable *> &coeffs, XCondition &cond) override;

	virtual void buildConstraintAtMost(string id, vector<XVariable *> &list, int value, int k) override;

	virtual void buildConstraintAtLeast(string id, vector<XVariable *> &list, int value, int k) override;

	virtual void buildConstraintExactlyK(string id, vector<XVariable *> &list, int value, int k) override;

	virtual void buildConstraintAmong(string id, vector<XVariable *> &list, vector<int> &values, int k) override;

	virtual void buildConstraintExactlyVariable(string id, vector<XVariable *> &list, int value, XVariable *x) override;

	virtual void buildConstraintCount(string id, vector<XVariable *> &list, vector<int> &values, XCondition &xc) override;

	virtual void buildConstraintCount(string id, vector<XVariable *> &list, vector<XVariable *> &values, XCondition &xc) override;

	virtual void buildConstraintNValues(string id, vector<XVariable *> &list, vector<int> &except, XCondition &xc) override;

	virtual void buildConstraintNValues(string id, vector<XVariable *> &list, XCondition &xc) override;

	virtual void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<int> values, vector<int> &occurs, bool closed) override;

	virtual void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<int> values, vector<XVariable *> &occurs,
		bool closed) override;

	virtual void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<int> values, vector<XInterval> &occurs,
		bool closed) override;

	virtual void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<XVariable *> values, vector<int> &occurs,
		bool closed) override;

	virtual void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<XVariable *> values, vector<XVariable *> &occurs,
		bool closed) override;

	virtual void buildConstraintCardinality(string id, vector<XVariable *> &list, vector<XVariable *> values, vector<XInterval> &occurs,
		bool closed) override;

	virtual void buildConstraintMinimum(string id, vector<XVariable *> &list, XCondition &xc) override;

	virtual void buildConstraintMinimum(string id, vector<XVariable *> &list, XVariable *index, int startIndex, RankType rank,
		XCondition &xc) override;

	virtual void buildConstraintMaximum(string id, vector<XVariable *> &list, XCondition &xc) override;

	virtual void buildConstraintMaximum(string id, vector<XVariable *> &list, XVariable *index, int startIndex, RankType rank,
		XCondition &xc) override;

	virtual void buildConstraintElement(string id, vector<XVariable *> &list, int value) override;

	virtual void buildConstraintElement(string id, vector<XVariable *> &list, XVariable *value) override;

	virtual void buildConstraintElement(string id, vector<XVariable *> &list, int startIndex, XVariable *index, RankType rank, int value) override;

	virtual void buildConstraintElement(string id, vector<XVariable *> &list, int startIndex, XVariable *index, RankType rank, XVariable *value) override;

	virtual void buildConstraintElement(string id, vector<int> &list, int startIndex, XVariable *index, RankType rank, XVariable *value) override;

	virtual void buildConstraintChannel(string id, vector<XVariable *> &list, int startIndex) override;

	virtual void buildConstraintChannel(string id, vector<XVariable *> &list1, int startIndex1, vector<XVariable *> &list2, int startIndex2) override;

	virtual void buildConstraintChannel(string id, vector<XVariable *> &list, int startIndex, XVariable *value) override;

	virtual void buildConstraintStretch(string id, vector<XVariable *> &list, vector<int> &values, vector<XInterval> &widths) override;

	virtual void
		buildConstraintStretch(string id, vector<XVariable *> &list, vector<int> &values, vector<XInterval> &widths, vector<vector<int>> &patterns) override;

	virtual void buildConstraintNoOverlap(string id, vector<XVariable *> &origins, vector<int> &lengths, bool zeroIgnored) override;

	virtual void buildConstraintNoOverlap(string id, vector<XVariable *> &origins, vector<XVariable *> &lengths, bool zeroIgnored) override;

	virtual void buildConstraintNoOverlap(string id, vector<vector<XVariable *>> &origins, vector<vector<int>> &lengths, bool zeroIgnored) override;

	virtual void buildConstraintNoOverlap(string id, vector<vector<XVariable *>> &origins, vector<vector<XVariable *>> &lengths, bool zeroIgnored) override;

	virtual void buildConstraintInstantiation(string id, vector<XVariable *> &list, vector<int> &values) override;


	virtual void buildConstraintCircuit(string id, vector<XVariable *> &list, int startIndex) override;


	virtual void buildConstraintCircuit(string id, vector<XVariable *> &list, int startIndex, int size) override;


	virtual void buildConstraintCircuit(string id, vector<XVariable *> &list, int startIndex, XVariable *size) override;


	virtual void buildObjectiveMinimizeExpression(string expr) override;

	virtual void buildObjectiveMaximizeExpression(string expr) override;


	virtual void buildObjectiveMinimizeVariable(XVariable *x) override;


	virtual void buildObjectiveMaximizeVariable(XVariable *x) override;


	virtual void buildObjectiveMinimize(ExpressionObjective type, vector<XVariable *> &list, vector<int> &coefs) override;


	virtual void buildObjectiveMaximize(ExpressionObjective type, vector<XVariable *> &list, vector<int> &coefs) override;


	virtual void buildObjectiveMinimize(ExpressionObjective type, vector<XVariable *> &list) override;


	virtual void buildObjectiveMaximize(ExpressionObjective type, vector<XVariable *> &list) override;

private:
	//int vars_map_ = 0;
	//int con_cnt = 0;
	unordered_map<string, int> vars_map_;
};

API_DECLSPEC void GetHModel(string file_path, HModel* m) {
	XCSP3PrintCallbacks cb; // my interface between the parser and the solver
	cb.hm = m;

	try {
		XCSP3CoreParser parser(&cb);
		parser.parse(file_path.c_str()); // fileName is a string
	}
	catch (exception &e) {
		cout.flush();
		cerr << "\n\tUnexpected exception :\n";
		cerr << "\t" << e.what() << endl;
		exit(1);
	}
}

}

using namespace XCSP3Core;


XCSP3PrintCallbacks::XCSP3PrintCallbacks() : XCSP3CoreCallbacks() {

}


template<class T>
void displayList(vector<T> &list, string separator = " ") {
	if (list.size() > 8) {
		for (int i = 0; i < 3; i++)
			cout << list[i] << separator;
		cout << " ... ";
		for (unsigned int i = list.size() - 4; i < list.size(); i++)
			cout << list[i] << separator;
		cout << endl;
		return;
	}
	for (unsigned int i = 0; i < list.size(); i++)
		cout << list[i] << separator;
	cout << endl;
}


void displayList(vector<XVariable *> &list, string separator = " ") {
	if (list.size() > 8) {
		for (int i = 0; i < 3; i++)
			cout << list[i]->id << separator;
		cout << " ... ";
		for (unsigned int i = list.size() - 4; i < list.size(); i++)
			cout << list[i]->id << separator;
		cout << endl;
		return;
	}
	for (unsigned int i = 0; i < list.size(); i++)
		cout << list[i]->id << separator;
	cout << endl;
}


void XCSP3PrintCallbacks::beginInstance(InstanceType type) {
	//cout << "Start Instance - type=" << type << endl;
}


void XCSP3PrintCallbacks::endInstance() {
	//cout << "End SAX parsing " << endl;
}


void XCSP3PrintCallbacks::beginVariables() {
	//cout << " start variables declaration" << endl;
}


void XCSP3PrintCallbacks::endVariables() {
	//cout << " end variables declaration" << endl << endl;
}


void XCSP3PrintCallbacks::beginVariableArray(string id) {
	//cout << "    array: " << id << endl;
}


void XCSP3PrintCallbacks::endVariableArray() {}


void XCSP3PrintCallbacks::beginConstraints() {
	//cout << " start constraints declaration" << endl;
}


void XCSP3PrintCallbacks::endConstraints() {
	//cout << "\n end constraints declaration" << endl << endl;
}


void XCSP3PrintCallbacks::beginGroup(string id) {
	//cout << "   start group of constraint " << id << endl;
}


void XCSP3PrintCallbacks::endGroup() {
	//cout << "   end group of constraint" << endl;
}


void XCSP3PrintCallbacks::beginBlock(string classes) {
	cout << "   start block of constraint classes = " << classes << endl;
}


void XCSP3PrintCallbacks::endBlock() {
	cout << "   end block of constraint" << endl;
}


// string id, bool circular
void XCSP3PrintCallbacks::beginSlide(string id, bool) {
	cout << "   start slide " << id << endl;
}


void XCSP3PrintCallbacks::endSlide() {
	cout << "   end slide" << endl;
}


void XCSP3PrintCallbacks::beginObjectives() {
	cout << "   start Objective " << endl;
}


void XCSP3PrintCallbacks::endObjectives() {
	cout << "   end Objective " << endl;
}


void XCSP3PrintCallbacks::buildVariableInteger(string id, int minValue, int maxValue) {
	const int vid = hm->AddVar(id, minValue, maxValue);
	vars_map_[id] = vid;
}


void XCSP3PrintCallbacks::buildVariableInteger(string id, vector<int> &values) {
	const int vid = hm->AddVar(id, values);
	vars_map_[id] = vid;
}


void XCSP3PrintCallbacks::buildConstraintExtension(string id, vector<XVariable *> list, vector<vector<int>> &tuples, bool support, bool hasStar) {
	vector<string> scope(list.size());
	for (size_t i = 0; i < scope.size(); ++i)
		scope[i] = list[i]->id;
	hm->AddTab(support, tuples, scope);
}


void XCSP3PrintCallbacks::buildConstraintExtension(string id, XVariable *variable, vector<int> &tuples, bool support, bool hasStar) {
	cout << "\n    extension constraint with one variable: " << id << endl;
	cout << "        " << (support ? "support" : "conflict") << " nb tuples: " << tuples.size() << " star: " << hasStar << endl;
	cout << (*variable) << endl;
}


// string id, vector<XVariable *> list, bool support, bool hasStar
void XCSP3PrintCallbacks::buildConstraintExtensionAs(string id, vector<XVariable *> list, bool, bool) {
	//cout << "\n    extension constraint similar as previous one: " << id << endl;
	vector<string> scope(list.size());
	for (size_t i = 0; i < scope.size(); ++i)
		scope[i] = list[i]->id;
	hm->AddTabAsPrevious(hm->tabs.back(), scope);
}


void XCSP3PrintCallbacks::buildConstraintIntension(string id, string expr) {
	//cout << "\n    intension constraint : " << id << " : " << expr << endl;
	hm->AddTab(expr);
}


// string id, OrderType op, XVariable *x, int k, XVariable *y
void XCSP3PrintCallbacks::buildConstraintPrimitive(string id, OrderType ot, XVariable *x, int k, XVariable *y) {
	cout << "\n   intension constraint " << id << ": " << x->id << (k >= 0 ? "+" : "") << k << " op " << y->id << endl;
}


// string id, vector<XVariable *> &list, string start, vector<string> &final, vector<XTransition> &transitions
void XCSP3PrintCallbacks::buildConstraintRegular(string, vector<XVariable *> &list, string start, vector<string> &final, vector<XTransition> &transitions) {
	cout << "\n    regular constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        start: " << start << endl;
	cout << "        final: ";
	displayList(final, ",");
	cout << endl;
	cout << "        transitions: ";
	for (unsigned int i = 0; i < (transitions.size() > 4 ? 4 : transitions.size()); i++) {
		cout << "(" << transitions[i].from << "," << transitions[i].val << "," << transitions[i].to << ") ";
	}
	if (transitions.size() > 4) cout << "...";
	cout << endl;
}


// string id, vector<XVariable *> &list, vector<XTransition> &transitions
void XCSP3PrintCallbacks::buildConstraintMDD(string, vector<XVariable *> &list, vector<XTransition> &transitions) {
	cout << "\n    mdd constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        transitions: ";
	for (unsigned int i = 0; i < (transitions.size() > 4 ? 4 : transitions.size()); i++) {
		cout << "(" << transitions[i].from << "," << transitions[i].val << "," << transitions[i].to << ") ";
	}
	if (transitions.size() > 4) cout << "...";
	cout << endl;
}


void XCSP3PrintCallbacks::buildConstraintAlldifferent(string id, vector<XVariable *> &list) {
	cout << "\n    allDiff constraint" << id << endl;
	cout << "        ";
	displayList(list);
}


void XCSP3PrintCallbacks::buildConstraintAlldifferentExcept(string id, vector<XVariable *> &list, vector<int> &except) {
	cout << "\n    allDiff constraint with exceptions" << id << endl;
	cout << "        ";
	displayList(list);
	cout << "        Exceptions:";
	displayList(except);
}


void XCSP3PrintCallbacks::buildConstraintAlldifferentList(string id, vector<vector<XVariable *>> &lists) {
	cout << "\n    allDiff list constraint" << id << endl;
	for (unsigned int i = 0; i < (lists.size() < 4 ? lists.size() : 3); i++) {
		cout << "        ";
		displayList(lists[i]);

	}
}


void XCSP3PrintCallbacks::buildConstraintAlldifferentMatrix(string id, vector<vector<XVariable *>> &matrix) {
	cout << "\n    allDiff matrix constraint" << id << endl;
	for (unsigned int i = 0; i < matrix.size(); i++) {
		cout << "        ";
		displayList(matrix[i]);
	}
}


void XCSP3PrintCallbacks::buildConstraintAllEqual(string id, vector<XVariable *> &list) {
	cout << "\n    allEqual constraint" << id << endl;
	cout << "        ";
	displayList(list);
}


void XCSP3PrintCallbacks::buildConstraintNotAllEqual(string id, vector<XVariable *> &list) {
	cout << "\n    not allEqual constraint" << id << endl;
	cout << "        ";
	displayList(list);
}


// string id, vector<XVariable *> &list, OrderType order
void XCSP3PrintCallbacks::buildConstraintOrdered(string, vector<XVariable *> &list, OrderType order) {
	cout << "\n    ordered constraint" << endl;
	string sep;
	if (order == LT) sep = " < ";
	if (order == LE) sep = " <= ";
	if (order == GT) sep = " > ";
	if (order == GE) sep = " >= ";
	cout << "        ";
	displayList(list, sep);
}


// string id, vector<vector<XVariable *>> &lists, OrderType order
void XCSP3PrintCallbacks::buildConstraintLex(string, vector<vector<XVariable *>> &lists, OrderType order) {
	cout << "\n    lex constraint   nb lists: " << lists.size() << endl;
	string sep;
	if (order == LT) sep = " < ";
	if (order == LE) sep = " <= ";
	if (order == GT) sep = " > ";
	if (order == GE) sep = " >= ";
	cout << "        operator: " << sep << endl;
	for (unsigned int i = 0; i < lists.size(); i++) {
		cout << "        list " << i << ": ";
		cout << "        ";
		displayList(lists[i], " ");
	}
}


// string id, vector<vector<XVariable *>> &matrix, OrderType order
void XCSP3PrintCallbacks::buildConstraintLexMatrix(string, vector<vector<XVariable *>> &matrix, OrderType order) {
	cout << "\n    lex matrix constraint   matrix  " << endl;
	string sep;
	if (order == LT) sep = " < ";
	if (order == LE) sep = " <= ";
	if (order == GT) sep = " > ";
	if (order == GE) sep = " >= ";

	for (unsigned int i = 0; i < (matrix.size() < 4 ? matrix.size() : 3); i++) {
		cout << "        ";
		displayList(matrix[i]);
	}
	cout << "        Order " << sep << endl;
}


// string id, vector<XVariable *> &list, vector<int> &coeffs, XCondition &cond
void XCSP3PrintCallbacks::buildConstraintSum(string, vector<XVariable *> &list, vector<int> &coeffs, XCondition &cond) {
	cout << "\n        sum constraint:";
	if (list.size() > 8) {
		for (int i = 0; i < 3; i++)
			cout << (coeffs.size() == 0 ? 1 : coeffs[i]) << "*" << *(list[i]) << " ";
		cout << " ... ";
		for (unsigned int i = list.size() - 4; i < list.size(); i++)
			cout << (coeffs.size() == 0 ? 1 : coeffs[i]) << "*" << *(list[i]) << " ";
	}
	else {
		for (unsigned int i = 0; i < list.size(); i++)
			cout << (coeffs.size() == 0 ? 1 : coeffs[i]) << "*" << *(list[i]) << " ";
	}
	cout << cond << endl;
}


// string id, vector<XVariable *> &list, XCondition &cond
void XCSP3PrintCallbacks::buildConstraintSum(string, vector<XVariable *> &list, XCondition &cond) {
	cout << "\n        unweighted sum constraint:";
	cout << "        ";
	displayList(list, "+");
	cout << cond << endl;

}


// string id, vector<XVariable *> &list, vector<XVariable *> &coeffs, XCondition &cond
void XCSP3PrintCallbacks::buildConstraintSum(string, vector<XVariable *> &list, vector<XVariable *> &coeffs, XCondition &cond) {
	cout << "\n        scalar sum constraint:";
	if (list.size() > 8) {
		for (int i = 0; i < 3; i++)
			cout << coeffs[i]->id << "*" << *(list[i]) << " ";
		cout << " ... ";
		for (unsigned int i = list.size() - 4; i < list.size(); i++)
			cout << coeffs[i]->id << "*" << *(list[i]) << " ";
	}
	else {
		for (unsigned int i = 0; i < list.size(); i++)
			cout << coeffs[i]->id << "*" << *(list[i]) << " ";
	}
	cout << cond << endl;
}


// string id, vector<XVariable *> &list, int value, int k
void XCSP3PrintCallbacks::buildConstraintAtMost(string, vector<XVariable *> &list, int value, int k) {
	cout << "\n    AtMost constraint: val=" << value << " k=" << k << endl;
	cout << "        ";
	displayList(list);
}


// string id, vector<XVariable *> &list, int value, int k
void XCSP3PrintCallbacks::buildConstraintAtLeast(string, vector<XVariable *> &list, int value, int k) {
	cout << "\n    Atleast constraint: val=" << value << " k=" << k << endl;
	cout << "        ";
	displayList(list);
}


// string id, vector<XVariable *> &list, int value, int k
void XCSP3PrintCallbacks::buildConstraintExactlyK(string, vector<XVariable *> &list, int value, int k) {
	cout << "\n    Exactly constraint: val=" << value << " k=" << k << endl;
	cout << "        ";
	displayList(list);
}


// string id, vector<XVariable *> &list, vector<int> &values, int k
void XCSP3PrintCallbacks::buildConstraintAmong(string, vector<XVariable *> &list, vector<int> &values, int k) {
	cout << "\n    Among constraint: k=" << k << endl;
	cout << "        ";
	displayList(list);
	cout << "        values:";
	displayList(values);

}


// string id, vector<XVariable *> &list, int value, XVariable *x
void XCSP3PrintCallbacks::buildConstraintExactlyVariable(string, vector<XVariable *> &list, int value, XVariable *x) {
	cout << "\n    Exactly Variable constraint: val=" << value << " variable=" << *x << endl;
	cout << "        ";
	displayList(list);
}


// string id, vector<XVariable *> &list, vector<int> &values, XCondition &xc
void XCSP3PrintCallbacks::buildConstraintCount(string, vector<XVariable *> &list, vector<int> &values, XCondition &xc) {
	cout << "\n    count constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        values: ";
	cout << "        ";
	displayList(values);
	cout << "        condition: " << xc << endl;
}


// string id, vector<XVariable *> &list, vector<XVariable *> &values, XCondition &xc
void XCSP3PrintCallbacks::buildConstraintCount(string, vector<XVariable *> &list, vector<XVariable *> &values, XCondition &xc) {
	cout << "\n    count constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        values: ";
	displayList(values);
	cout << "        condition: " << xc << endl;
}


// string id, vector<XVariable *> &list, vector<int> &except, XCondition &xc
void XCSP3PrintCallbacks::buildConstraintNValues(string, vector<XVariable *> &list, vector<int> &except, XCondition &xc) {
	cout << "\n    NValues with exceptions constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        exceptions: ";
	displayList(except);
	cout << "        condition:" << xc << endl;
}


// string id, vector<XVariable *> &list, XCondition &xc
void XCSP3PrintCallbacks::buildConstraintNValues(string, vector<XVariable *> &list, XCondition &xc) {
	cout << "\n    NValues  constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        condition:" << xc << endl;
}


// string id, vector<XVariable *> &list, vector<int> values, vector<int> &occurs, bool closed
void XCSP3PrintCallbacks::buildConstraintCardinality(string, vector<XVariable *> &list, vector<int> values, vector<int> &occurs, bool closed) {
	cout << "\n    Cardinality constraint (int values, int occurs)  constraint closed: " << closed << endl;
	cout << "        ";
	displayList(list);
	cout << "        values:";
	displayList(values);
	cout << "        occurs:";
	displayList(occurs);
}


// string id, vector<XVariable *> &list, vector<int> values, vector<XVariable *> &occurs, bool closed
void XCSP3PrintCallbacks::buildConstraintCardinality(string, vector<XVariable *> &list, vector<int> values, vector<XVariable *> &occurs, bool closed) {
	cout << "\n    Cardinality constraint (int values, var occurs)  constraint closed: " << closed << endl;
	cout << "        ";
	displayList(list);
	cout << "        values:";
	displayList(values);
	cout << "        occurs:";
	displayList(occurs);
}


// string id, vector<XVariable *> &list, vector<int> values, vector<XInterval> &occurs, bool closed
void XCSP3PrintCallbacks::buildConstraintCardinality(string, vector<XVariable *> &list, vector<int> values, vector<XInterval> &occurs, bool closed) {
	cout << "\n    Cardinality constraint (int values, interval occurs)  constraint closed: " << closed << endl;
	cout << "        ";
	displayList(list);
	cout << "        values:";
	displayList(values);
	cout << "        occurs:";
	displayList(occurs);
}


// string id, vector<XVariable *> &list, vector<XVariable *> values, vector<int> &occurs, bool closed
void XCSP3PrintCallbacks::buildConstraintCardinality(string, vector<XVariable *> &list, vector<XVariable *> values, vector<int> &occurs, bool closed) {
	cout << "\n    Cardinality constraint (var values, int occurs)  constraint closed: " << closed << endl;
	cout << "        ";
	displayList(list);
	cout << "        values:";
	displayList(values);
	cout << "        occurs:";
	displayList(occurs);
}


// string id, vector<XVariable *> &list, vector<XVariable *> values, vector<XVariable *> &occurs, bool closed
void XCSP3PrintCallbacks::buildConstraintCardinality(string, vector<XVariable *> &list, vector<XVariable *> values, vector<XVariable *> &occurs, bool closed) {
	cout << "\n    Cardinality constraint (var values, var occurs)  constraint closed: " << closed << endl;
	cout << "        ";
	displayList(list);
	cout << "        values:";
	displayList(values);
	cout << "        occurs:";
	displayList(occurs);
}


// string id, vector<XVariable *> &list, vector<XVariable *> values, vector<XInterval> &occurs, bool closed
void XCSP3PrintCallbacks::buildConstraintCardinality(string, vector<XVariable *> &list, vector<XVariable *> values, vector<XInterval> &occurs, bool closed) {
	cout << "\n    Cardinality constraint (var values, interval occurs)  constraint closed: " << closed << endl;
	cout << "        ";
	displayList(list);
	cout << "        values:";
	displayList(values);
	cout << "        occurs:";
	displayList(occurs);
}


// string id, vector<XVariable *> &list, XCondition &xc
void XCSP3PrintCallbacks::buildConstraintMinimum(string, vector<XVariable *> &list, XCondition &xc) {
	cout << "\n    minimum  constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        condition: " << xc << endl;
}


// string id, vector<XVariable *> &list, XVariable *index, int startIndex, RankType rank, XCondition &xc
void XCSP3PrintCallbacks::buildConstraintMinimum(string, vector<XVariable *> &list, XVariable *index, int startIndex, RankType, XCondition &xc) {
	cout << "\n    arg_minimum  constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        index:" << *index << endl;
	cout << "        Start index : " << startIndex << endl;
	cout << "        condition: " << xc << endl;

}


// string id, vector<XVariable *> &list, XCondition &xc
void XCSP3PrintCallbacks::buildConstraintMaximum(string, vector<XVariable *> &list, XCondition &xc) {
	cout << "\n    maximum  constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        condition: " << xc << endl;
}


// string id, vector<XVariable *> &list, XVariable *index, int startIndex, RankType rank, XCondition &xc
void XCSP3PrintCallbacks::buildConstraintMaximum(string, vector<XVariable *> &list, XVariable *index, int startIndex, RankType, XCondition &xc) {
	cout << "\n    arg_maximum  constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        index:" << *index << endl;
	cout << "        Start index : " << startIndex << endl;
	cout << "        condition: " << xc << endl;
}


// string id, vector<XVariable *> &list, int value
void XCSP3PrintCallbacks::buildConstraintElement(string, vector<XVariable *> &list, int value) {
	cout << "\n    element constant constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        value: " << value << endl;
}


// string id, vector<XVariable *> &list, XVariable *value
void XCSP3PrintCallbacks::buildConstraintElement(string, vector<XVariable *> &list, XVariable *value) {
	cout << "\n    element variable constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        value: " << *value << endl;
}


// string id, vector<XVariable *> &list, int startIndex, XVariable *index, RankType rank, int value
void XCSP3PrintCallbacks::buildConstraintElement(string, vector<XVariable *> &list, int startIndex, XVariable *index, RankType, int value) {
	cout << "\n    element constant (with index) constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        value: " << value << endl;
	cout << "        Start index : " << startIndex << endl;
	cout << "        index : " << *index << endl;
}


// string id, vector<XVariable *> &list, int startIndex, XVariable *index, RankType rank, XVariable *value
void XCSP3PrintCallbacks::buildConstraintElement(string, vector<XVariable *> &list, int startIndex, XVariable *index, RankType, XVariable *value) {
	cout << "\n    element variable (with index) constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        value: " << *value << endl;
	cout << "        Start index : " << startIndex << endl;
	cout << "        index : " << *index << endl;
}


// string, vector<int> &list, int startIndex, XVariable *index, RankType rank, XVariable *value
void XCSP3PrintCallbacks::buildConstraintElement(string, vector<int> &list, int startIndex, XVariable *index, RankType, XVariable *value) {
	cout << "\n    element variable with list of integers (with index) constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        value: " << *value << endl;
	cout << "        Start index : " << startIndex << endl;
	cout << "        index : " << *index << endl;
}


// string id, vector<XVariable *> &list, int startIndex
void XCSP3PrintCallbacks::buildConstraintChannel(string, vector<XVariable *> &list, int startIndex) {
	cout << "\n    channel constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        Start index : " << startIndex << endl;
}


// string id, vector<XVariable *> &list1, int startIndex1, vector<XVariable *> &list2, int startIndex2
void XCSP3PrintCallbacks::buildConstraintChannel(string, vector<XVariable *> &list1, int, vector<XVariable *> &list2, int) {
	cout << "\n    channel constraint" << endl;
	cout << "        list1 ";
	displayList(list1);
	cout << "        list2 ";
	displayList(list2);
}


// string id, vector<XVariable *> &list, int startIndex, XVariable *value
void XCSP3PrintCallbacks::buildConstraintChannel(string, vector<XVariable *> &list, int, XVariable *value) {
	cout << "\n    channel constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        value: " << *value << endl;
}


// string id, vector<XVariable *> &list, vector<int> &values, vector<XInterval> &widths
void XCSP3PrintCallbacks::buildConstraintStretch(string, vector<XVariable *> &list, vector<int> &values, vector<XInterval> &widths) {
	cout << "\n    stretch constraint" << endl;
	cout << "        ";
	displayList(list);
	cout << "        values :";
	displayList(values);
	cout << "        widths:";
	displayList(widths);
}


// string id, vector<XVariable *> &list, vector<int> &values, vector<XInterval> &widths, vector<vector<int>> &patterns
void XCSP3PrintCallbacks::buildConstraintStretch(string, vector<XVariable *> &list, vector<int> &values, vector<XInterval> &widths, vector<vector<int>> &patterns) {
	cout << "\n    stretch constraint (with patterns)" << endl;
	cout << "        ";
	displayList(list);
	cout << "        values :";
	displayList(values);
	cout << "        widths:";
	displayList(widths);
	cout << "        patterns";
	for (unsigned int i = 0; i < patterns.size(); i++)
		cout << "(" << patterns[i][0] << "," << patterns[i][1] << ") ";
	cout << endl;
}


// string id, vector<XVariable *> &origins, vector<int> &lengths, bool zeroIgnored
void XCSP3PrintCallbacks::buildConstraintNoOverlap(string, vector<XVariable *> &origins, vector<int> &lengths, bool) {
	cout << "\n    nooverlap constraint" << endl;
	cout << "        origins";
	displayList(origins);
	cout << "        lengths";
	displayList(lengths);
}


// string id, vector<XVariable *> &origins, vector<XVariable *> &lengths, bool zeroIgnored
void XCSP3PrintCallbacks::buildConstraintNoOverlap(string, vector<XVariable *> &origins, vector<XVariable *> &lengths, bool) {
	cout << "\n    nooverlap constraint" << endl;
	cout << "        origins:";
	displayList(origins);
	cout << "        lengths";
	displayList(lengths);
}


// string id, vector<vector<XVariable *>> &origins, vector<vector<int>> &lengths, bool zeroIgnored
void XCSP3PrintCallbacks::buildConstraintNoOverlap(string, vector<vector<XVariable *>> &origins, vector<vector<int>> &lengths, bool) {
	cout << "\n    kdim (int lengths) nooverlap constraint" << endl;
	cout << "origins: " << endl;
	for (unsigned int i = 0; i < origins.size(); i++) {
		cout << "        ";
		displayList(origins[i]);
	}
	cout << "lengths: " << endl;
	for (unsigned int i = 0; i < origins.size(); i++) {
		cout << "        ";
		displayList(lengths[i]);
	}

}


// string id, vector<vector<XVariable *>> &origins, vector<vector<XVariable *>> &lengths, bool zeroIgnored
void XCSP3PrintCallbacks::buildConstraintNoOverlap(string, vector<vector<XVariable *>> &origins, vector<vector<XVariable *>> &lengths, bool) {
	cout << "\n    kdim (lenghts vars nooverlap constraint" << endl;
	cout << "origins: " << endl;
	for (unsigned int i = 0; i < origins.size(); i++) {
		cout << "        ";
		displayList(origins[i]);
	}
	cout << "lengths: " << endl;
	for (unsigned int i = 0; i < origins.size(); i++) {
		cout << "        ";
		displayList(lengths[i]);
	}
}


// string id, vector<XVariable *> &list, vector<int> &values
void XCSP3PrintCallbacks::buildConstraintInstantiation(string, vector<XVariable *> &list, vector<int> &values) {
	cout << "\n    instantiation constraint" << endl;
	cout << "        list:";
	displayList(list);
	cout << "        values:";
	displayList(values);

}


// string id, vector<XVariable *> &list, int startIndex
void XCSP3PrintCallbacks::buildConstraintCircuit(string, vector<XVariable *> &list, int startIndex) {
	cout << "\n    circuit constraint" << endl;
	cout << "        list:";
	displayList(list);
	cout << "        startIndex:" << startIndex << endl;
}


// string id, vector<XVariable *> &list, int startIndex, int size
void XCSP3PrintCallbacks::buildConstraintCircuit(string, vector<XVariable *> &list, int startIndex, int size) {
	cout << "\n    circuit constraint" << endl;
	cout << "        list:";
	displayList(list);
	cout << "        startIndex:" << startIndex << endl;
	cout << "        size: " << size << endl;
}


// string id, vector<XVariable *> &list, int startIndex, XVariable *size
void XCSP3PrintCallbacks::buildConstraintCircuit(string, vector<XVariable *> &list, int startIndex, XVariable *size) {
	cout << "\n    circuit constraint" << endl;
	cout << "        list:";
	displayList(list);
	cout << "        startIndex:" << startIndex << endl;
	cout << "        size: " << size->id << endl;
}


void XCSP3PrintCallbacks::buildObjectiveMinimizeExpression(string expr) {

	cout << "\n    objective: minimize" << expr << endl;
}


void XCSP3PrintCallbacks::buildObjectiveMaximizeExpression(string expr) {
	cout << "\n    objective: maximize" << expr << endl;
}


void XCSP3PrintCallbacks::buildObjectiveMinimizeVariable(XVariable *x) {
	cout << "\n    objective: minimize variable " << x << endl;
}


void XCSP3PrintCallbacks::buildObjectiveMaximizeVariable(XVariable *x) {
	cout << "\n    objective: maximize variable " << x << endl;
}


void XCSP3PrintCallbacks::buildObjectiveMinimize(ExpressionObjective type, vector<XVariable *> &list, vector<int> &coefs) {
	XCSP3CoreCallbacks::buildObjectiveMinimize(type, list, coefs);
}


void XCSP3PrintCallbacks::buildObjectiveMaximize(ExpressionObjective type, vector<XVariable *> &list, vector<int> &coefs) {
	XCSP3CoreCallbacks::buildObjectiveMaximize(type, list, coefs);
}


void XCSP3PrintCallbacks::buildObjectiveMinimize(ExpressionObjective type, vector<XVariable *> &list) {
	XCSP3CoreCallbacks::buildObjectiveMinimize(type, list);
}


void XCSP3PrintCallbacks::buildObjectiveMaximize(ExpressionObjective type, vector<XVariable *> &list) {
	XCSP3CoreCallbacks::buildObjectiveMaximize(type, list);
}


#endif //COSOCO_XCSP3PRINTCALLBACKS_H
