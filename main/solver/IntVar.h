#pragma once
#include <string>
#include <vector>
#include <map>
using namespace std;
namespace cudacp {
class IntVar {
public:

	IntVar() {};
	IntVar(const int id, const string name, const vector<int> values);
	bool constains(const int value);
	int size() const ;
protected:
	int id_;
	int cur_size_;
	int init_size_;
	string name_;
	vector<int> dom_;
	map<int, int> map_;
};
}