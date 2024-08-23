#ifndef MINIDFA_H_
#define MINIDFA_H_
#include "DFA.h"
#include <queue>
class Table;
class MiniDFA {
    DFA &dfa;
    set<char>alphabet;
    int startState;
    set<int>endStates;
    //store the states after minimize
    vector<DFAState *>states;
    //show which the current state belongs to after mapping.
    vector<int>belongSet;
    //show the sets which to be processed now.
    queue<set<int> >sets;
    //all the sets created now
    int setNumbers;
    //current set numbers.
    int num_of_sets;
    void minimize(char c);
    friend class Table;
public:
    MiniDFA(DFA &d) :
        dfa(d), alphabet(d.alphabet), belongSet(d.stateNumbers + 1, 0) {
        num_of_sets = 2;
        setNumbers = 2;
        set<int>a, b;

        for (int index = 0; index <= dfa.stateNumbers; ++index) {
            if (d.endStates.find(index) == d.endStates.end()) {
                a.insert(index);
            } else {
                belongSet[index] = 1;
                b.insert(index);
            }
        }

        sets.push(a);
        sets.push(b);

        for (set<char>::iterator ct = alphabet.begin(); ct != alphabet.end(); ct++) {
            minimize(*ct);
        }

        vector<bool> flag;

        for (int i = 0; i <= dfa.stateNumbers; ++i) {
            flag.push_back(false);
            int temp = belongSet[i] + num_of_sets - setNumbers - 1;
            belongSet[i] = temp;
        }

        for (int i = 0; i < num_of_sets; ++i) {
            states.push_back(new DFAState(i));
        }

        for (int i = 0; i <= dfa.stateNumbers; ++i) {
            int temp = belongSet[i];

            if (!flag[temp]) {
                typedef map<char, int>::iterator IT;
                map<char, int> *m = &(dfa.states[i]->stateChange);

                for (IT it = m->begin(); it != m->end(); it++) {
                    states[temp]->addDestState(it->first, belongSet[it->second]);
                }
            }

            flag[temp] = true;
        }

        for (set<int>::iterator st = dfa.endStates.begin(); st
                != dfa.endStates.end(); st++) {
            endStates.insert(belongSet[*st]);
        }

        startState = belongSet[dfa.startState];
    }

    void display() const {
        cout << "MiniDFA display" << endl << "******************" << endl;
        cout << "alphabet is ";

        for (set<char>::iterator ait = alphabet.begin(); ait != alphabet.end(); ait++) {
            cout << *ait;
        }

        cout << endl;

        for (int i = 0; i <= dfa.stateNumbers; ++i) {
            cout << "prestate " << i << "currentstate " << belongSet[i] << endl;
        }

        cout << endl;

        for (size_t i = 0; i < states.size(); i++) {
            cout << "State: " << i << endl;
            map<char, int> &stateChange = states[i]->stateChange;

            for (map<char, int>::iterator it = stateChange.begin(); it
                    != stateChange.end(); it++) {
                cout << "char: " << it->first << " dest: " << it->second << endl;
            }
        }

        cout << "start state: " << startState << endl;
        cout << "end states: ";

        for (set<int>::iterator st = endStates.begin(); st != endStates.end(); st++) {
            cout << *st << " ";
        }

        cout << endl;

        cout << "num_of_sets " << num_of_sets << endl;
        cout << "setNumbers " << setNumbers << endl;
        cout << "******************" << endl;
    }

    virtual ~MiniDFA() {

    }
};

#endif /*MINIDFA_H_*/
