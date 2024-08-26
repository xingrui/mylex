#ifndef DFA_H_
#define DFA_H_
#include"NFA.h"
#include"DFAState.h"
typedef multimap <char, int>::iterator CIT;
typedef pair<CIT, CIT> Range;
class MiniDFA;
class DFA {
    friend class MiniDFA;
    NFA &nfa;
    int stateNumbers;
    vector<set<int> >myStates;
    vector<DFAState *>states;
    set<int>endStates;
    size_t current;
    const int startState;
    const set<char> &alphabet;
    int getCurrent() const {
        return current;
    }

    bool hasState(const set<int> &state) {
        for (size_t i = 0; i < myStates.size(); ++i) {
            if (isEqual(state, myStates[i])) {
                current = i;
                return true;
            }
        }

        return false;
    }

    void getEqual(State *p, set<int> &temp);
    void getEqualStates();
    bool isEqual(const set<int> &a, const set<int> &b) {
        if (a.size() != b.size())
            return false;

        set<int>::iterator ita = a.begin(), itb = b.begin();

        for (; ita != a.end(); ita++, itb++) {
            if ((*ita) != (*itb))
                return false;
        }

        return true;
    }

    void process(int, set<int> &s, char c);
public:
    DFA(NFA &a) :
        nfa(a), stateNumbers(-1), startState(0), alphabet(a.getAlphabet()) {
        makeDFA();
    }

    void makeDFA();
    void display() const {
        cout << "DFA display:" << endl << "********************" << endl;
        cout << "stateNumbers:" << stateNumbers + 1 << endl;

        for (size_t i = 0; i < myStates.size(); i++) {
            cout << "State: " << i << " Equal States: ";

            for (set<int>::iterator it = myStates[i].begin(); it
                    != myStates[i].end(); it++) {
                cout << (*it) << " ";
            }

            cout << endl;
            map<char, int> &stateChange = states[i]->stateChange;

            for (map<char, int>::iterator it = stateChange.begin(); it
                    != stateChange.end(); it++) {
                cout << "char: " << it->first << " dest: " << it->second << endl;
            }
        }

        cout << "start state " << startState << endl;
        cout << "end states: ";

        for (set<int>::iterator st = endStates.begin(); st != endStates.end(); st++) {
            cout << *st << " ";
        }

        cout << endl << "********************" << endl;
    }

    virtual ~DFA();
};

#endif /*DFA_H_*/
