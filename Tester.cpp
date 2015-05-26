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
	cout<<"Function entered"<<endl;
	string nextline;
	string stateName = "";	//stores current state name
	string s;	//used to get rid of lines
	bool isTransition = false;	//stores whether the current state being explored is a transition state
	bool transitionFound = false;	//stores whether a transition state has been found
	cout<<"Current search target:"<<currentName<<endl;
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
					cout<<endl<<"Is a transition!"<<endl;
					isTransition = true;
				}else{
					cout<<endl<<"Is not a transition!"<< endl;
					isTransition = false;
				}
				
				getline(events, s, ':');	//Gets rid of text before state name
				getline(events, s, ' ');	//Gets rid of text before state name
				getline(events, stateName);	
				cout<<stateName<<endl;
				getline(events, s);
				
				if(isTransition && find(visitedStates.begin(), visitedStates.end(), stateName) != visitedStates.end())
				{
					cout<<"Transition has been found!"<<endl;
					transitionFound = true;
					nextTransitions.clear();
					nextStates.clear();
				}
				
			}
						
		}else{
			if(nextline.empty()) continue;	//gets rid of empty newlines
			string transition;
			string state;
			istringstream instring(nextline);
			instring >> transition;	//gets transition name
			instring >> state;	//gets state transitioned to
			cout<<"Read state: "<<state<<endl;
			
			if(transitionFound)
			{
				cout<<"Adding transition!"<<endl;
				nextTransitions.push_back(transition);
				cout<<"Adding state!"<<endl;
				nextStates.push_back(state);
			}else{
				//this means that a transition has been found to the current state
				if(state == currentName && (find(visitedStates.begin(), visitedStates.end(), stateName) == visitedStates.end()))
				{
					cout<<"Adding next state!"<<endl;
					nextStates.push_back(stateName);
					nextTransitions.push_back(transition);
					visitedStates.push_back(stateName);
				}
			}
		}
	}
	events.clear();
	events.seekg(0, ios_base::beg);
	if(nextStates.size() == 0 || nextTransitions.size() == 0)	//this means that no transitions for this state were found
	{
		cout<<"Error! Deadlock state not found. Exiting..."<<endl;
		exit(9001);
	}
	cout<<endl<<"Exiting false"<<endl<<endl;
	return false;
}

int main()
{
	fstream infile("test2.txt");
	cout<<"Opening test..."<<endl;
	if(!infile.is_open())
	{
		cout<<"Error: Can't open file."<<" Exiting..."<<endl;
		return 3;
	}
	string currentName = "(((K,K)))";
	vector<string> visitedStates;
	vector<string> nextStates;
	vector<string> nextTransitions;
	//add the current name to the list of explored states
	visitedStates.push_back(currentName);
	Find_Next_States(currentName, visitedStates, nextStates, nextTransitions, infile);
	for(unsigned int i = 0; i < nextStates.size(); i++)
	{
		cout<<"Adjacent State: "<<nextStates.at(i)<<endl;
	}
	for(unsigned int i = 0; i < nextTransitions.size(); i++)
	{
		cout<<"Transition Name: "<<nextTransitions.at(i)<<endl;
	}
	for(unsigned int i = 0; i < visitedStates.size(); i++)
	{
		cout<<"Visited State: "<<visitedStates.at(i)<<endl;
	}
	
	cout<<"Exiting..."<<endl;
	return 0;
}
