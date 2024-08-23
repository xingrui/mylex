#include "MiniDFA.h"
//minimize the nfa by the current char
void MiniDFA::minimize(char c) {
    const int currentSetNumbers = num_of_sets;
    typedef map<int, int> MAP;
    //contains the node created in the current process in order to put them to the queue later
    map<int, set<int> >temp;
    typedef map<int, set<int> >IntSetMap;

    if (TRACEMIN)
        cout << "the char is " << c << " currentSetNumbers:" << currentSetNumbers
             << " start minimize" << endl << "********************" << endl;

    for (int i = 0; i < currentSetNumbers; ++i) { //for every set in the sets

        //contains the relationship of the destSet and belongedSet
        MAP setChange;
        //holds the sets poped out
        set<int> cset = sets.front();
        sets.pop();
        num_of_sets--;

        if (TRACEMIN)
            cout << "i=" << i << " setNumbers " << setNumbers << endl;

        //system("sleep 1");
        for (set<int>::iterator st = cset.begin(); st != cset.end(); st++) { //for every element in this state
            DFAState* p = dfa.states[*st];
            map<char, int>::iterator sit = p->stateChange.find(c);
            int destSet;//the set which the destination of this  element belongs to

            if (sit == p->stateChange.end()) {
                destSet = -1;
            } else {
                destSet = belongSet[sit->second];
            }

            MAP::iterator iter = setChange.find(destSet);

            if (TRACEMIN)
                cout << "destSet of the element " << *st << " is " << destSet << endl;

            if (iter != setChange.end()) {
                int at = iter->second;
                temp[at].insert(*st);
            } else {
                num_of_sets++;
                setChange[destSet] = ++setNumbers;
                set<int> t;
                temp.insert(IntSetMap::value_type(setNumbers, t));
                temp[setNumbers].insert(*st);
            }
        }
    }

    for (map<int, set<int> >::iterator it = temp.begin(); it != temp.end(); it++) {
        sets.push(it->second);

        for (set<int>::iterator pt = it->second.begin(); pt != it->second.end(); pt++) {
            belongSet[*pt] = it->first;
        }

        if (TRACEMIN)
            cout << "an element was pushed" << endl;
    }

    if (TRACEMIN)
        cout << "setNumbers:" << setNumbers << endl << "********************" << endl;

    if (num_of_sets != currentSetNumbers) {
        minimize(c);
    }
}
