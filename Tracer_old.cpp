/*	Tracer
*	-Outputs the path to a deadlock state
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
//EFFECTS: returns a boolean value whether or not a cstring is a valid integer
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
	if(argc > 3)
	{
		cout<<"Too many input arguments. Enter no flags or -h for help. Exiting..."<<endl;
		return 1;
	}

	
	if(argc <= 1 || (argc == 2 && (strcmp(argv[1], "-h") == 0)))
	{
		cout<<"Tracer: Outputs the path to a blocking state"<<endl;
		cout<<"Intended for use with:"<<endl<<
		"\t-'blocking-events-full.txt' from Stephen Lanham's CountBlockingStates"<<endl<<
		"\t-'deadlock_states.txt from BlockFind.cpp."<<endl;
		cout<<"Usage: <executable> [-h] <blocking-events-full.txt> <deadlock_states.txt>"<<endl;
		cout<<"Exiting..."<<endl;
		return 2;
	}
	

	//open files
	fstream events(argv[1]);
	fstream deadlocks(argv[2]);

	//Check if files have been opened
	cout<<"Opening blocking events..."<<endl;
	if(!events.is_open())
	{
		cout<<"Error: Can't open file " << argv[1] <<". Exiting..."<<endl;
		return 3;
	}
	cout<<"Opening deadlock states..."<<endl;
	if(!deadlocks.is_open())
	{
		cout<<"Error: Can't open file " << argv[2] <<". Exiting..."<<endl;
		return 3;
	}
	cout<<"Success!"<<endl;
	


	//find state with shortest string
	cout<<"Finding paths for all deadlock states"<<endl;

	String name = "";	//name for the deadlock state being explored
	String s = "";		//holds current line in the events file
	vector<vector<String>> transitions;	//holds the transitions for each deadlock state
	vector<String> deadlock_names;	//holds the names of deadlock states
	while(getline(deadlocks,name))
	{
		deadlock_names.push_back(name);
		String currentstate = "";	//name of the current state being explored in events
		vector<String> currentTransitions;	//vector of transitions for the current event
		vector<String> deadlockTransitions;	//vector of transitions for the deadlock states
		bool transition;	//stores whether the event transitions out of non-blocking space
		bool finalTransitionReached = false;	//stores whether the final transition has been reached for this deadlock state
		while(getline(events,s))
		{
			if(s != "State found which transitions out of non-blocking space" && s != "Blocking state explored")
			{
				if(s.empty()) continue;	//gets rid of empty newlines
				istringstream instring(s);
				instring >> s;	//gets transition name
				currentTransitions.push_back(s);
				instring >> s;	//gets the name of state being transitioned to
				if(s == name)	//
				{
					if(transition)
					{
						//transition state
						finalTransitionReached = true;	//set to true so that the set of final transitions will be added to the transitions list
					}else{
						//blocking state
						//add the transition last found (last currentTransition) to the front of deadlockTransitions (since we are tracing backwards)
						deadlockTransitions.insert(deadlockTransitions.begin(), currentTransitions.at(currentTransitions.size() - 1))
					}
				}
				
	
			}else{
				
				if(finalTransitionReached)	//transition has been reached, append all transitions from this state onto the full list
				{
					deadlockTransitions.insert(deadlockTransitions.begin(), currentTransitions.begin(), currentTransitions.end());
					break;
				}
	
				if(s == "State found which transitions out of non-blocking space")
				{
					transition = true;
				}else{
					transition = false;
				}
				
				getline(infile, currentstate);	//reads name of state
				getline(infile, s);	//gets rid of 'Transition History:' or 'Marked? YES/NO'
				events.clear();
	
			}
	
			
		}
	}

	//compare last state to shortest

	//cout<<"Current smallest: "<<events_short.size()<<endl;			//<--DEBUG MESSAGE
	//cout<<"Current (last):"<<events.size()<<endl;					//<--DEBUG MESSAGE
	if(events.size() < events_short.size() && events.size() >= MINIMUM_STRING_LENGTH)	//exclude strings less than minimum string length
		{
			name_short = name;
			events_short = events;
			//cout<<"CHANGED (last)"<<endl;					//<--DEBUG MESSAGE
		}
	

	//check if the string found has the required minimum length
	if(events_short.size() < MINIMUM_STRING_LENGTH)
	{
		cout<<"Error: Unable to find a string with minimum length "<<MINIMUM_STRING_LENGTH<<"."<<endl;
		cout<<"Exiting..."<<endl;
		return 5;
	}

	cout<<"Success!"<<endl<<endl;

	//report state with shortest string
	cout<<"State with shortest string: "<<endl<<endl<<name_short<<endl<<endl;
	cout<<"List of events (length "<<events_short.size()<<") :"<<endl<<endl;
	for(unsigned int i = 0; i < events_short.size(); i++)
	{
		cout<<events_short.at(i)<<"  ";
	}
	cout<<endl<<endl;

	cout<<"Exiting..."<<endl;
	return 0;
}
