/*	Tracer
*	-Outputs the paths to a set of deadlock states
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
using namespace std;


//REQUIRES:	vector of strings vec
//MODIFIES:	n/a
//EFFECTS:	prints the components of the vector, each on a newline and with a tab in front
void printVec(vector<string> &vec)
{
	for(unsigned i = 0; i < vec.size(); i++)
		cout<< "\t"<<vec.at(i)<<endl;
	return;
}

//REQUIRES: currentName: state to search from
//			visitedStates: used to keep track of which states have been visited
//			nextStates/ nextTransitions: empty vectors. used to store next states and next transitions, respectively
//			events: file containing blocking events
//MODIFIES: nextStates and nextTransitions
//EFFECTS: finds the next states and adds them to nextStates, along with the corresponding transitions from those states in nextTransitions
//		   if the next state has a transition state, there will only be 1 state and nextTransitions will represent all remaining transitions
//		   if the next state has a transition state, this function returns true. otherwise returns false
bool Find_Next_States(string &currentName, vector<string> &visitedStates, vector<string> &nextStates, vector<string> &nextTransitions, fstream &events)
{
	//cout<<"Function entered"<<endl;		//DEBUG
	string nextline;
	string stateName = "";	//stores current state name
	string s;	//used to get rid of lines
	bool isTransition = false;	//stores whether the current state being explored is a transition state
	bool transitionFound = false;	//stores whether a transition state has been found
	//cout<<"Current search target:"<<currentName<<endl;		//DEBUG
	while(!events.eof())
	{

		getline(events, nextline);
		if(events.eof() || nextline == "State found which transitions out of non-blocking space" || nextline == "Blocking state explored")
		{
			//check if there is a transition state
				if(transitionFound){
					
					events.clear();
					events.seekg(0, ios_base::beg);
					return true;
				}
			if(!events.eof())
			{
				//set isTransition based on the type of state
				if(nextline == "State found which transitions out of non-blocking space")
				{
					//cout<<endl<<"Is a transition!"<<endl;		//DEBUG
					isTransition = true;
				}else{
					//cout<<endl<<"Is not a transition!"<< endl;		//DEBUG
					isTransition = false;
				}
				
				getline(events, s, ':');	//Gets rid of text before state name
				getline(events, s, ' ');	//Gets rid of text before state name
				getline(events, stateName);	
				//cout<<stateName<<endl;		//DEBUG
				getline(events, s);
				
				if(isTransition && find(visitedStates.begin(), visitedStates.end(), stateName) != visitedStates.end())
				{
					//cout<<"Transition has been found!"<<endl;		//DEBUG
					transitionFound = true;
					nextTransitions.clear();
					nextStates.clear();
				}
				
			}
						
		}else{
			if(nextline == "") continue;	//gets rid of empty newlines
			string transition;
			string state;
			istringstream instring(nextline);
			instring >> transition;	//gets transition name
			instring >> state;	//gets state transitioned to
			//cout<<"Read state: "<<state<<endl;		//DEBUG
			
			if(transitionFound)
			{
				//cout<<"Adding transition "<<transition<<endl;		//DEBUG
				nextTransitions.push_back(transition);
				//cout<<"Adding state "<<state<<endl;		//DEBUG
				nextStates.push_back(state);
			}else{
				//this means that a transition has been found to the current state
				if(state == currentName && (find(visitedStates.begin(), visitedStates.end(), stateName) == visitedStates.end()))
				{
					//cout<<"Adding next state!"<<endl;		//DEBUG
					nextStates.push_back(stateName);
					nextTransitions.push_back(transition);
					//visitedStates.push_back(stateName);
				}
			}
		}
	}
	events.clear();
	events.seekg(0, ios_base::beg);
	return false;
}


//REQUIRES:	dlstate: deadlock state name to find path for
//			events: filestream of events, usually blocking-events-full.txt
//			pathTransitions: empty vector, where the discovered path's transitions will be stored
//			pathStates: empty vector, where the discovered path's states will be stored
//MODIFIES:	pathTransitions and pathStates
//EFFECTS: returns a vector repesenting the transitions to a deadlock state
void Find_Path(string dlstate, fstream &events, vector<string> &pathTransitions, vector<string> &pathStates)
{
	//a path is determined by a list of states and a list of transitions
	vector< vector<string> > exploreStates;	//keeps track of states in path currently being explored
	vector< vector<string> > exploreTransitions;	//keeps track of transitions in paths currently being explored
	
	//push back the deadlock state to explorePaths
	//STATES AND TRANSITIONS STORED IN REVERSE ORDER
	vector<string> first;	//just a placeholder to push back to explorePaths
	first.push_back(dlstate);	//push back the deadlock state name
	exploreStates.push_back(first);	//now have one vector in the vector array with deadlock state name
	//add the corresponding transitions list, which is empty
	first.clear();
	exploreTransitions.push_back(first);
	
	//keep searching until path is found, each iteration searches a path and adds one more state depth
	while(exploreStates.size() > 0 && exploreTransitions.size() > 0)
	{
			//check that sizes of exploreStates and exploreTransitions are equal
			assert(exploreStates.size() == exploreTransitions.size());
			
			//explore the path at the beginning of exploreStates/ exploreTransitions
			vector<string> currentStates = exploreStates.at(0);	//the current list of states
			vector<string> currentTransitions = exploreTransitions.at(0);	//the current list of transitions corresponding to those states
			string currentName = currentStates.at(currentStates.size() - 1);	//the last state, i.e. the state being explored
			
			//THESE ARE NOT PATHS
			vector<string> nextStates;	//states that transition to last state *NOT A PATH*
			vector<string> nextTransitions;	//corresponding transitions *NOT A PATH*
			
			//print out the path being explored
			//cout<<"Printing out current exploration path:"<<endl;
			//printVec(currentStates);
			
			if(Find_Next_States(currentName, currentStates, nextStates, nextTransitions, events))	//if a transition state is found then return the path
			{
				cout<<"Path has been found!"<<endl;
				//reverse the currentTransitions
				reverse(currentTransitions.begin(), currentTransitions.end());
				//printVec(currentTransitions);		//DEBUG
				//printVec(nextTransitions);		//DEBUG
				//add nextTransitions to the beginning
				currentTransitions.insert(currentTransitions.begin(), nextTransitions.begin(), nextTransitions.end());
				pathTransitions = currentTransitions;
				//do the same for states
				reverse(currentStates.begin(), currentStates.end());
				nextStates.pop_back();	//gets rid of duplicate transition state
				currentStates.insert(currentStates.begin(), nextStates.begin(), nextStates.end());
				pathStates = currentStates;
				//printVec(pathStates);		//DEBUG
				return;
			}else{
				//create new paths and add them to the next round of searches
				
				//check that the sizes of nextStates and nextTransitions are equal
				assert(nextStates.size() == nextTransitions.size());
				for(unsigned int j = 0; j < nextStates.size(); j++)
				{
					//add the new state to currentStates to create a new path, then add that path to exploreStates
					currentStates.push_back(nextStates.at(j));
					exploreStates.push_back(currentStates);
					//remove the last element so currentState returns to normal
					currentStates.pop_back();
					//do the same for transitions
					currentTransitions.push_back(nextTransitions.at(j));
					exploreTransitions.push_back(currentTransitions);
					currentTransitions.pop_back();
				}
			}
			//remove the path once it has been searched
			exploreStates.erase(exploreStates.begin());
			exploreTransitions.erase(exploreTransitions.begin());
			//check that sizes of exploreStates and exploreTransitions are equal
			assert(exploreStates.size() == exploreTransitions.size());
	}
	cout<<"Error! Path not found. Exiting..."<<endl;
	exit(9000);	//should never reach here. this means that no path has been found
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
	cout<<"Success!"<<endl<<endl;
	
	//find paths to deadlock states
	string deadlockName;
	vector<vector<string> > pathTransitions;	//paths to deadlock states
	vector<vector<string> > pathStates;			//corresponding states
	bool output = true;							//set to true to get deadlock_paths.txt, false otherwise
	ofstream outfile("deadlock_paths.txt");		//outfile to write paths
	while(getline(deadlocks, deadlockName))	//loop while there are still deadlock states to be explored
	{
		//perform breadth-first search
		//-find which states transition to this deadlock state
		//-if it is a transition state then add all remaining states, then terminate search
		//-add those as separate paths
		//-repeat with the last state
		istringstream instring(deadlockName);
		instring >> deadlockName;	//gets rid of "State"
		instring >> deadlockName;	//gets rid of "name:"
		instring >> deadlockName;	//gets deadlock name
		cout<<"-------------------FINDING PATH FOR DEADLOCK STATE "<<deadlockName<<"--------------------"<<endl<<endl;
		if(output) outfile<<"-------------------FINDING PATH FOR DEADLOCK STATE "<<deadlockName<<"--------------------"<<endl<<endl;
		vector<string> newTransitions;
		vector<string> newStates;
		Find_Path(deadlockName, events, newTransitions, newStates);
		pathTransitions.push_back(newTransitions);
		pathStates.push_back(newStates);
		//print path
		cout<<"Path is:"<<endl;
		outfile<<"Path is:"<<endl;
		for(unsigned int i = 0; i < newTransitions.size(); i++)
		{
			cout<<newTransitions.at(i)<<"\t"<<newStates.at(i)<<endl;
			if(output) outfile<<newTransitions.at(i)<<"\t"<<newStates.at(i)<<endl;
		}
		cout<<endl;
		if(output) outfile<<endl;
	}
	
	
	events.close();
	deadlocks.close();
	outfile.close();
	cout<<"Exiting..."<<endl;
	return 0;
}
