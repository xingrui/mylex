#ifndef DFASTATE_H_
#define DFASTATE_H_

class DFAState {
public:
    map<char, int>stateChange;
    int state;
    DFAState(int s) :
        state(s) {

    }

    void addDestState(char c, int state) {
        stateChange[c] = state;
    }
};

#endif /*DFASTATE_H_*/
