#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

int main(int argc, char* argv[])
{
	ifstream deadlock(argv[1]);
	string input;
	while(deadlock>>input && input != "events"){}
	vector<string> events;
	while(deadlock>>input)
	{
		events.push_back(input);
		deadlock>>input;
		deadlock>>input;
	}

	cout<<"Finding FSMs with common events..."<<endl;
	for(int i = 2; i < argc; i++)
	{
		bool flag = false;
		ifstream current(argv[i]);
		getline(current, input);
		getline(current, input);
		while(getline(current, input))
		{
			while(getline(current, input) && input != "")
			{
				istringstream inputString;
				inputString.str(input);
				inputString>>input;
				if(find(events.begin(), events.end(), input)!=events.end())
					flag = true;
			}
		}
		if(flag)
		{
			cout<<argv[i]<<endl;;
		}
	}
	
	return 0;
}
