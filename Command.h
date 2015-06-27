#include <cstdio>
class Command
{
	public:
		string name_;// command name
		string input_;
		string output_;
		vector<string> coeff_;
		int type;
		bool is_background;
		Command():type(0),is_background(false){};
};