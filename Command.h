#pragma once
#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <list>

using std::string;
using std::vector;
using std::list;
class Command
{
	public:
		string name_;// command name
		string input_;
		string output_;
		vector<string> coeff_;
		int i_type_;
		int o_type_;
		bool is_background;
		Command():i_type_(0),o_type_(0),is_background(false){};
};
/*typedef enum JobStatus
{
	Running =  0,
	Stopped =  1,
	Finished =  2
};
*/
class Job
{
public:
	vector<int> pid_list_;
	int status_;// 0:running; 1:stopped; 2:finished
	string content_;
	Job():status_(0){};
	
};


