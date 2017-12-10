#pragma once
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
namespace cudacp {
class IntVar {
public:

	IntVar() {};
	IntVar(const int id, const string name, const vector<int> values);
	bool constains(const int value);
	int size() const;
	void swap(const int i, const int j);
	void bind(const int v);
	void remove(const int v);
	void clear_marks();
	void mark(const int v);
	void restrict();
protected:
	int id_;
	int curr_size_;
	int init_size_;
	int mark_;
	string name_;
	vector<int> dom_;
	unordered_map<int, int> map_;
};
}