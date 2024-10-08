#include "DFA.h"

void DFA::makeDFA() {
    getEqualStates();
    State*p = nfa.getStates()[nfa.getStartState()];
    myStates.push_back(p->equalStates);
    states.push_back(new DFAState(++stateNumbers));

    if (TRACEDFA)
        cout << endl;

    size_t currentLocation = 0;

    while (currentLocation < myStates.size()) {
        if (TRACEDFA)
            cout << "this is on the big state of " << currentLocation << endl;

        for (set<char>::iterator it = alphabet.begin(); it
                != alphabet.end(); it++) {
            process(currentLocation, myStates[currentLocation], *it);
        }

        ++currentLocation;

        if (TRACEDFA)
            cout << "mystate " << myStates.size() << " size" << endl;
    }
}

void DFA::getEqual(State *p, set<int> &temp) {
    multimap<char, int> &stateChange = p->stateChange;
    Range range = stateChange.equal_range(0);

    for (CIT i = range.first; i != range.second; ++i) {
        int t = i->second;

        if (temp.find(t) == temp.end()) {
            temp.insert(t);
            getEqual(nfa.getStates()[t], temp);
        }
    }
}

void DFA::getEqualStates() {
    for (size_t i = 0; i < nfa.getStates().size(); i++) {
        State* sp = nfa.getStates()[i];
        getEqual(sp, sp->equalStates);
    }
}

void DFA::process(int current, set<int> &s, char c) {
    if (TRACEDFA)
        cout << "this is on the letter " << c << endl;

    set<int> temp;

    if (TRACEDFA)
        cout << "size " << s.size() << endl;

    for (set<int>::iterator its = s.begin(); its != s.end(); its++) {
        multimap<char, int> &stateChange = nfa.getStates()[*its]->stateChange;
        Range range = stateChange.equal_range(c);

        if (TRACEDFA)
            cout << (*its) << " start ";

        for (CIT i = range.first; i != range.second; ++i) {
            if (TRACEDFA)
                cout << i->second << " ";

            set<int> &vt = nfa.getStates()[i->second]->equalStates;

            for (set<int>::iterator it = vt.begin(); it != vt.end(); it++)
                temp.insert(*it);
        }

        if (TRACEDFA)
            cout << "end " << endl;
    }

    if (!temp.empty()) {
        if (!hasState(temp)) {
            if (TRACEDFA)
                cout << "ADDING STATES" << endl;

            myStates.push_back(temp);
            states.push_back(new DFAState(++stateNumbers));

            if (temp.find(nfa.getEndState()) != temp.end()) {
                endStates.insert(stateNumbers);
            }

            states[current]->addDestState(c, stateNumbers);
        } else {
            states[current]->addDestState(c, getCurrent());
        }
    }
}

DFA::~DFA() {
}
