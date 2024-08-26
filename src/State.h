#ifndef STATE_H_
#define STATE_H_
#include<map>
#include<set>
using namespace std;
class State {
    static int s_stateNumbers;
public:
    multimap<char, int>stateChange;
    set<int> equalStates;
    int state;
    State(int s) :
        state(s) {
        equalStates.insert(s);
    }

    State(): state(s_stateNumbers++) {
        equalStates.insert(state);
    }

    void addDestState(char c, int state) {
        stateChange.insert(make_pair(c, state));
    }

    virtual ~State() {

    }
};

#endif /*STATE_H_*/
