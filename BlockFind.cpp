/*	BlockFind
*	-Finds all names of deadlock states
*	-Use this program to only output deadlock states
*	-If names and paths to deadlock states are required, use Tracer.cpp instead
*
*	Written for the UMDES TRW Project (2015)
*	Austin Chen
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <cstdlib>
using namespace std;


//REQUIRES: cstring str
//MODIFIES: n/a
//EFFECTS:	returns a boolean value whether or not a cstring is a valid integer
bool is_num(char *str) {
	while (*str)
	{
		if (!isdigit(*str++))
		{
			return false;
		}
	}
	return true;
}

int main(int argc, char* argv[])
{
	//check inputs
	if(argc > 2)
	{
		cout<<"Too many input arguments. Enter no flags or -h for help. Exiting..."<<endl;
		return 1;
	}

	
	if(argc <= 1 || (argc == 2 && (strcmp(argv[1], "-h") == 0)))
	{
		cout<<"BlockFind: Finds all deadlock state names."<<endl;
		cout<<"Intended for use with 'blocking-events-full.txt' from Stephen Lanham's CountBlockingStates."<<endl;
		cout<<"Usage: <executable> [-h] <blocking-events-full.txt>"<<endl;
		cout<<"Exiting..."<<endl;
		return 2;
	}
	
	//open file
	fstream infile(argv[1]);

	//Check if file has been opened
	cout<<"Opening file..."<<endl;
	if(!infile.is_open())
	{
		cout<<"Error: Can't open file " << argv[1] <<". Exiting..."<<endl;
		return 3;
	}
	cout<<"Success!"<<endl;
	


	//find deadlock states (states with no transitions)
	cout<<"Finding deadlock states..."<<endl;
	string name = "";	//stores name of state
	string name_short = "";	//stores name of state with shortest string
	string s;	//s is used to store strings that are being "worked with"
	vector<string> events;	//stores the events that lead to this state from the initial state
	vector<string> deadlock_states;	//stores names of deadlock states

	bool valid = false;	//true if s is "Blocking state explored," false otherwise
	while(getline(infile,s))
	{
		if(s != "State found which transitions out of non-blocking space" && s != "Blocking state explored")
		{
			if(s.empty()) continue;	//gets rid of empty newlines
			istringstream instring(s);
			instring >> s;	//gets transition name
			events.push_back(s);
			instring >> s;	//gets rid of list of states after transition name

		}else{

			//cout<<"Current smallest: "<<events_short.size()<<endl; 	//<--DEBUG MESSAGE
			//cout<<"Current: "<<events.size()<<endl;			//<--DEBUG MESSAGE

			//check to see if current state has a shorter string than the one stored
		
			//if short name is empty, that means that no states have been read yet - make first state the short state
			if(name_short == "")
			{
				name_short = name;
			}else if(events.size() == 0 && valid)	//exclude strings less than minimum string length
			{
				deadlock_states.push_back(name);
				//cout<<"CHANGED"<<endl;				//<--DEBUG MESSAGE
			}

			if(s == "Blocking state explored") valid = true;
			else valid = false;

			getline(infile, name);	//reads name of state
			getline(infile, s);	//gets rid of 'Transition History:' or 'Marked? YES/NO'
			events.clear();
			
			

		}

		
	}
	
	if(events.size() == 0 && valid)
		{
			deadlock_states.push_back(name);
			//cout<<"CHANGED (last)"<<endl;					//<--DEBUG MESSAGE
		}

	cout<<"Outputting deadlock states to deadlock_states.txt..."<<endl;
	ofstream outfile("deadlock_states.txt");
	for(unsigned int i = 0; i < deadlock_states.size(); i++)
	{
		outfile << deadlock_states.at(i) << endl;
	}
	cout<<"Success!"<<endl;

	cout<<"Exiting..."<<endl;
	return 0;
}
