#ifndef TABLE_H_
#define TABLE_H_
#include "MiniDFA.h"
#include "string.h"
class Table {
	MiniDFA& minidfa;
	int state;
	string s;
public:
	Table(MiniDFA& m) :
		minidfa(m) {
		state=m.startState;
	}
	void read(string s) {
		for (size_t i=0; i<s.size(); i++) {
			readIn(s[i]);
		}

		//if the end of the state is not in the endStates
		if (minidfa.endStates.find(state)==minidfa.endStates.end()) {
			cerr<<endl<<"ERROR"<<endl;
		} else {
			cout<<endl<<"RIGHT"<<endl;
		}
	}
	void readIn(char c) {
		cout<<c;
		map<char,int>* mystate = &(minidfa.states[state]->stateChange);
		map<char,int>::iterator it;
		it = mystate->find(c);
		if (it==mystate->end()) {
			cerr<<"ERROR"<<endl;
		} else {
			state=it->second;
		}
	}
};

#endif /*TABLE_H_*/
