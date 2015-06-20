#include <cstdio>
class Command
{
	public:
		string name_;// command name
		string input_;
		string output_;
		vector<string> coeff_;
		int type;
		Command():type(0){};
};