#ifndef NODE_H_
#define NODE_H_

class Node {
public:
    int startState, destState;
    Node(int start, int dest): startState(start), destState(dest) {

    }
};

#endif /*NODE_H_*/
