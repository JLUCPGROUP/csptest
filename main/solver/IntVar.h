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
	inline int size() const;
	void bind(const int a);
	void remove(const int a);
	void mark(const int a);
	void restrict();
	void clear_mark();
protected:
	int id_;
	int cur_size_;
	int init_size_;
	string name_;
	vector<int> dom_;
	unordered_map<int, int> map_;
	int mark_;

	void swap(const int i, const int j);
};
}