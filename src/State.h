#ifndef STATE_H_
#define STATE_H_
#include<map>
#include<set>
using namespace std;
class State {
public:
    multimap<char, int>stateChange;
    set<int> equalStates;
    int state;
    State(int s) :
        state(s) {
        equalStates.insert(s);
    }

    void addDestState(char c, int state) {
        stateChange.insert(make_pair(c, state));
    }

    virtual ~State() {

    }
};

#endif /*STATE_H_*/
