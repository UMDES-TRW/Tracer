/*	BuildFSM
*	-Outputs an automata (FSM format) corresponding to a given path
*	-Uses Stephen Lanham's FSM struct
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
#include <algorithm>
#include <cassert>

#include "FSM.h"

using namespace std;

/*
bool isProper(string name, vector<string> &unproper)
{
	for(unsigned int i = 0; i < unproper.size(); i++)
	{
		if(name.find(unproper.at(i)) != string::npos)
			return false;
	}
	return true;
}
*/

int main(int argc, char* argv[])
{
	//check input, read file
	if(argc < 4)
	{
		cout<<"BuildFSM - builds a FSM corresponding to a path to a deadlock state."<<endl;
		cout<<"Changes the state names to 1, 2, 3,... in the order they appear. Table can be found in deadlockFSM_names.txt"<<endl;
		cout<<"Usage: <executable> <deadlock_paths.txt> <abstracted_event_union.txt> <raw_event_union.txt>"<<endl;
		cout<<"Note: BuildFSM only builds a FSM for the first path in the input file."<<endl;
		return 1;
	}
	
	ifstream infile(argv[1]);
	
	if(!infile.good())
	{
		cout<<"Error in reading file. Exiting..."<<endl;
		return 2;
	}

	string s;
	vector<string> abstracted_event_union;
	vector<string> raw_event_union;

	ifstream abstracted(argv[2]);
	assert(abstracted.good());
	while(abstracted>>s)
	{
		abstracted_event_union.push_back(s);
		abstracted>>s;
		abstracted>>s;
	}
	ifstream raw(argv[3]);
	assert(raw.good());
	while(raw>>s)
	{
		raw_event_union.push_back(s);
		raw>>s;
		raw>>s;
	}
	
	//build vector of unproper states
	/*
	ifstream unproper_file(argv[2]);
	string input;
	vector<string> unproper;
	while(getline(unproper_file, input))
	{
		unproper.push_back(input);
	}
	*/
	//bool test = isProper("((lololol), q1002,q3863,q2387,q3774)", unproper);
	//cout<<test<<endl;
	//return 20;
	
	//get event union
	/*
	vector<FSM> automata;
	int nextArg = 2;
  	//~~~~~~~~~~~~~~~~~~ Read in automata ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
  	cout << "Importing " << (argc - nextArg) << " .fsm files..."; cout.flush();
  	for(; nextArg<argc; nextArg++)
  	{
    	string filepath(argv[nextArg]);
    	automata.push_back(FSM(filepath));
  	}
  	cout << "Success" << endl;
  	vector<string> Union;
  	for(unsigned int i = 0; i < automata.size(); i++)
  	{
  		vector<string> next = automata.at(i).GetAllEvents();
  		Union.insert(Union.end(), next.begin(), next.end());
  	}*/
	
	ofstream outfile("deadlockFSM_names.txt");
	int count = 1;
	

	FSM target;
	//add all events
	target.AddEvents(abstracted_event_union);
	target.GetState("InitialState");
	string prevState = "InitialState";
	target.SetInitialState("InitialState");
	target.MarkState("InitialState");
	

	getline(infile, s);	//gets rid of headers
	getline(infile, s);	//gets rid of headers
	getline(infile, s);	//gets rid of headers
	string transition, nextState;
	while(infile>>transition && transition != "-------------------FINDING")
	{
		infile>>nextState;
		//check if state is proper
		//bool proper = isProper(nextState, unproper);
		outfile<<'Z'<<count<<"\t"<<nextState<<endl;
		stringstream ss;
		ss<<count;
		nextState = 'Z' + ss.str();
		count++;
		//cout<<nextState<<"\t"<<count<<endl;	//DEBUG
		target.GetState(nextState);
		if(true){	//marks all states
			target.MarkState(nextState);
		}
		target.AddTransition(prevState, nextState, transition);
		prevState = nextState;
	}

	//add self loops at the last state
/*
	for(unsigned int i = 0; i < raw_event_union.size(); i++)
	{
		if(find(abstracted_event_union.begin(), abstracted_event_union.end(), raw_event_union.at(i)) == abstracted_event_union.end())
		target.AddTransition(nextState, nextState, raw_event_union.at(i));
	}
	*/
	target.Export("deadlockFSM.fsm");
	
	cout<<"Finished. Exiting..."<<endl;
	
	return 0;
}
