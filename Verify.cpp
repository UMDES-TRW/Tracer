#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include <bitset>



using namespace std;

//copied from Stephen Lanham's TRWSpecialUtilities.cpp
/*
 * @brief Determines whether or not an event is exogenous
 * @param event The event in question
 * @returns True if an event is exogenous
 */
bool IsEventExogenous(const string & event)
{
  for (unsigned int i = 0; i < event.size(); i++) 
  {
    //Allow 'n' in the 0th slot
    char c = event[i];
    if( i == 0 && c == 'n')
      continue;

    //Allow numbers and capital letters
    else if (isupper(c) || isdigit(c)) 
      continue;

    //Allow underscores
    else if (c == '_')
      continue;

    else
      return false;
  }
  return true;
}


bool isProper(vector<string> &dlState, vector<string> &unproper)
{
	for(unsigned int i = 0; i < unproper.size(); i++)
	{
		if(find(dlState.begin(), dlState.end(), unproper.at(i)) != dlState.end());
			return false;
	}
	return true;
}

void printVec(vector< vector<string> > &vec)
{
	for(unsigned int i = 0; i < vec.size(); i++)
	{
		cout<<"\tVector #"<<i<<"\n";
		for(unsigned int j = 0; j < (vec.at(i)).size(); j++)
		{
			cout<<"\t\t"<<vec.at(i).at(j)<<"\n";
		}
	}
	cout.flush();
	return;
}

void printVec(vector<string> &vec)
{
	for(unsigned int i = 0; i < vec.size(); i++)
	{
		cout<<"\t"<<vec.at(i)<<"\n";
	}
	cout.flush();
	return;
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		cout<<"Not enough inputs."<<endl;
		cout<<"You must also have deadlock_target.txt and unproper.txt in the same directory."<<endl;
		cout<<"Change deadlock_target.txt depending on which state you are checking."<<endl;
		cout<<"Unproper contains the list of unproper states."<<endl;
		cout<<"Usage: <executable> <raw FSM files>"<<endl;
		cout<<"Exiting..."<<endl;
		return 1;
	}

	ifstream deadlock("deadlock_target.txt");
	string input;
	vector<string> deadlock_state;	//vector representing deadlock state. with n individual automata, deadlock_state will have size n
	while(deadlock>>input)
	{
		deadlock_state.push_back(input);
	}

	//cout<<deadlock_state.size()<<endl;	//DEBUG


	//find out whether state is proper or not
	//open file of unproper states
	ifstream unproperFile("unproper.txt");
	vector<string> unproper;
	while(unproperFile>>input)
	{
		unproper.push_back(input);
	}

	bool proper = isProper(deadlock_state, unproper);
	if(proper)
	{
		cout<<"The deadlock state is proper."<<endl;
	}else{
		cout<<"The deadlock state is improper."<<endl;
	}

	//import data from .fsm files
	vector< vector<string> > possibleEvents;	//the events from dl state of each automaton
	vector< vector<string> > eventSet;			//the event set of eat automaton
	vector<string> exploreEvents;				//the events to explore; should be union of possibleEvents

	for(int i = 1; i < argc; i++)
	{
			assert(i <= (int)deadlock_state.size());
			ifstream automaton(argv[i]);
			vector<string> currentPossible;
			vector<string> currentSet;
			bool dlState = false;
			bool matchFound = false;
			getline(automaton, input);	//gets rid of number of states
			getline(automaton, input);	//gets rid of empty line
			while(getline(automaton, input))
			{
				stringstream is(input);
				is>>input;
				//cout<<input<<endl;	//DEBUG
				//cout<<deadlock_state.at(i - 1)<<endl;	//DEBUG
				if(input == deadlock_state.at(i - 1))
				{
					dlState = true;
					matchFound = true;
					//cout<<"Found deadlock state!"<<endl;	//DEBUG
				}else{
					dlState = false;
				}
				while(getline(automaton, input) && input != "")
				{
					stringstream ss(input);
					ss>>input;
					if(dlState)
					{
						if(find(exploreEvents.begin(), exploreEvents.end(), input) == exploreEvents.end())
						{
							exploreEvents.push_back(input);
						}
						currentPossible.push_back(input);
					}
					if(find(currentSet.begin(), currentSet.end(), input) == currentSet.end())
					{
						currentSet.push_back(input);
					}
				}
			}
			assert(matchFound);
			possibleEvents.push_back(currentPossible);
			eventSet.push_back(currentSet);
			//cout<<"completed"<<endl;	//DEBUG
	}
	cout<<"possibleEvents"<<endl;
	printVec(possibleEvents);
	cout<<"eventSet"<<endl;
	printVec(eventSet);
	cout<<"exploreEvents"<<endl;
	printVec(exploreEvents);


	//check that none of the events in exploreEvents can occur
	assert(possibleEvents.size() == eventSet.size());

	vector<bool> possibleVector(possibleEvents.size(), false);
	vector<bool> eventVector(possibleEvents.size(), false);

	for(unsigned int i = 0; i < exploreEvents.size(); i++)
	{
			for(unsigned int j = 0; j < possibleEvents.size(); j++)
			{
				vector<string> currentPossible = possibleEvents.at(j);
				vector<string> currentSet = eventSet.at(j);
				if(find(currentPossible.begin(), currentPossible.end(), exploreEvents.at(i)) != currentPossible.end() && (proper || !IsEventExogenous(exploreEvents.at(i))))
				{
					possibleVector.at(j) = true;
				}
				if(find(currentSet.begin(), currentSet.end(), exploreEvents.at(i)) != currentSet.end())
				{
					eventVector.at(j) = true;
				}
			}
			if(possibleVector == eventVector)
			{
				cout<<"The event "<<exploreEvents.at(i)<<" can occur. Exiting..."<<endl;
				return 2;
			}
			possibleVector.assign(possibleVector.size(), false);
			eventVector.assign(eventVector.size(), false);
	}
	cout<<"All events checked, state is a deadlock!"<<endl;
	cout<<"Exiting..."<<endl;
	return 0;
}