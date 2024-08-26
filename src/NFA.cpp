#include "NFA.h"
#include<queue>
using namespace std;
int NFA::NOTCHAR = -2;
int NFA::END = -1;

NFA::NFA(string input) :
    startState(0), stateNumbers(-1), sstream(input + "#"), flag(false), m_next_token(input[0]), m_input(input), m_next_token_location(0) {
    operStack.push('#');
}

bool NFA::makeNFA() {
    char c;

    while ((c = getNextChar()) != END) {
        if (c == NOTCHAR) {
            process();
        } else {
            if (TRACENFA)
                cout << "this is a letter " << c << endl;

            State*p = new State(++stateNumbers);
            states.push_back(p);

            State*q = new State(++stateNumbers);
            states.push_back(q);

            if (c != '.') {
                alphabet.insert(c);
                p->addDestState(c, stateNumbers);
            } else {
                for (int a = 'A'; a <= 'Z'; ++a) {
                    alphabet.insert(a);
                    p->addDestState(a, stateNumbers);
                    alphabet.insert(a + 'a' - 'A');
                    p->addDestState(a + 'a' - 'A', stateNumbers);
                }
            }

            nodeStack.push(new Node(stateNumbers - 1, stateNumbers));
            endState = stateNumbers;
        }
    }

    return true;
}

void NFA::process() {
    if (TRACENFA)
        cout << "this is an oper" << letter << endl;

    char temp = operStack.top();

    while (comPri(letter) < inPri(temp)) {
        operStack.pop();

        switch (temp) {
        case '*': {
            Node*node = nodeStack.top();
            nodeStack.pop();
            State*p = new State(++stateNumbers);
            states.push_back(p);
            State*q = new State(++stateNumbers);
            states.push_back(q);
            p->addDestState(0, node->startState);
            p->addDestState(0, stateNumbers);

            if (TRACENFA)
                cout << "start" << node->startState << "end" << node->destState << endl;

            states[node->destState]->addDestState(0, stateNumbers);
            states[node->destState]->addDestState(0, node->startState);
            nodeStack.push(new Node(stateNumbers - 1, stateNumbers));

            /*addEqualState(p, states[node->startState]);
             addEqualState(p, states[stateNumbers]);
             addEqualState(states[node->destState], states[stateNumbers]);
             addEqualState(states[node->destState], states[node->startState]);*/

            startState = stateNumbers - 1;
            endState = stateNumbers;
            break;
        }

        case '.': {
            Node*right = nodeStack.top();
            nodeStack.pop();
            Node*left = nodeStack.top();
            nodeStack.pop();
            states[left->destState]->addDestState(0, right->startState);
            nodeStack.push(new Node(left->startState, right->destState));

            //addEqualState(states[left->destState],states[right->startState]);

            startState = left->startState;
            endState = right->destState;
            break;
        }

        case '|': {
            Node*right = nodeStack.top();
            nodeStack.pop();
            Node*left = nodeStack.top();
            nodeStack.pop();
            State*p = new State(++stateNumbers);
            states.push_back(p);
            State*q = new State(++stateNumbers);
            states.push_back(q);
            p->addDestState(0, left->startState);
            p->addDestState(0, right->startState);
            states[left->destState]->addDestState(0, stateNumbers);
            states[right->destState]->addDestState(0, stateNumbers);
            nodeStack.push(new Node(stateNumbers - 1, stateNumbers));
            startState = stateNumbers - 1;
            endState = stateNumbers;

            /*addEqualState(p, states[left->startState]);
             addEqualState(q, states[right->startState]);
             addEqualState(states[left->destState], states[stateNumbers]);
             addEqualState(states[right->destState], states[stateNumbers]);*/

            break;
        }
        }

        temp = operStack.top();
    }

    if (letter == ')') {
        if (operStack.top() != '(')
            /**/;

        operStack.pop();
    } else if (letter == '#') {
        operStack.pop();
    } else {
        operStack.push(letter);
    }
}

void NFA::display() const {
    cout << "NFA display" << endl << "**********************" << endl;
    cout << "Alphabet: ";

    for (set<char>::iterator it = alphabet.begin(); it != alphabet.end(); it++)
        cout << (*it);

    cout << endl;
    cout << "stateNumbers:" << stateNumbers + 1 << endl;
    cout << "startState: " << startState << " endState: " << endState << endl;

    for (size_t i = 0; i < states.size(); ++i) {
        cout << "State " << states[i]->state << endl;

        for (multimap<char, int>::iterator it = states[i]->stateChange.begin(); it
                != states[i]->stateChange.end(); it++) {
            cout << "char:" << it->first << endl;
            cout << " dest:" << it->second << endl;
        }

        cout << "Equal States";

        for (set<int>::iterator it = states[i]->equalStates.begin(); it
                != states[i]->equalStates.end(); it++) {
            cout << " " << *it;
        }

        cout << endl;
    }

    cout << "*********************" << endl;
}

bool NFA::checkExpression() {
    TreeNode* node = expr();
    int depth = TreeNode::findDepth(node);
    printTreeNew(node, depth);

    if (m_next_token_location != m_input.size()) {
        return false;
    }

    Node n = getNFANode(node);
    startState = n.startState;
    endState = n.destState;
    return true;
}

/*
<expr> -> <term> { | <term> }
<term> -> <factor> { <factor> }
<factor> -> <element> [*]
<element> -> (<expr>) | letter(contain digit)
*/
TreeNode *NFA::expr(void) {
    TreeNode* left = term();

    while (m_next_token == '|') {
        TreeNode* node = new TreeNode();
        node->nodeType = NodeType::OpNode;
        node->data.opType = OpType::OpOR;
        node->left = left;
        match('|');
        node->right = term();
        left = node;
    }

    return left;
}

TreeNode *NFA::term(void) {
    TreeNode* left = factor();

    while (m_next_token == '(' || isalnum(m_next_token) || m_next_token == '.') {
        TreeNode* node = new TreeNode();
        node->nodeType = NodeType::OpNode;
        node->data.opType = OpType::OpConcat;
        node->left = left;
        node->right = factor();
        left = node;
    }

    return left;
}

TreeNode *NFA::factor(void) {
    TreeNode* left = element();

    while (m_next_token == '*') {
        match('*');
        TreeNode* node = new TreeNode();
        node->left = left;
        node->nodeType = NodeType::OpNode;
        node->data.opType = OpType::OpStar;
        left = node;
    }

    return left;
}

TreeNode *NFA::element(void) {
    TreeNode* node = NULL;

    if (m_next_token == '(') {
        match('(');
        node = expr();
        match(')');
    } else if (isalnum(m_next_token) || m_next_token == '.' ) {
        node = new TreeNode();
        node->data.value = m_next_token;
        node->nodeType = ValueNode;
        match(m_next_token);
    } else {
        error();
        return node;
    }

    return node;
}

void NFA::printTreeNew(TreeNode *current, int depth) const {
    std::queue<TreeNode *> q;
    q.push(current);
    ostringstream ss;
    ss << string((1 << depth) - 1, '=') << endl;

    for (int i = 0; i < depth; ++i) {
        string str((1 << depth ) - 1, ' ');
        std::queue<TreeNode *> temp_q;
        int current = (1 << (depth - i - 1)) - 1;
        int step = 1 << (depth - i);

        while (!q.empty()) {
            TreeNode* c = q.front();
            q.pop();
            str[current] = TreeNode::staticToChar(c);
            current += step;

            if (c == NULL) {
                temp_q.push(NULL);
                temp_q.push(NULL);
            } else {
                temp_q.push(c->left);
                temp_q.push(c->right);
            }
        }

        q.swap(temp_q);
        ss << str << endl;
    }

    ss << string((1 << depth) - 1, '=') << endl;
    cout << ss.str();
}

NFA::Node NFA::getNFANode(TreeNode *treeNode) {
    switch (treeNode->nodeType) {
    case OpNode:
        switch (treeNode->data.opType) {
        case OpType::OpOR: {
            Node leftNode = getNFANode(treeNode->left);
            Node rightNode = getNFANode(treeNode->right);
            State*p = new State(++stateNumbers);
            states.push_back(p);
            State*q = new State(++stateNumbers);
            states.push_back(q);
            p->addDestState(0, leftNode.startState);
            p->addDestState(0, rightNode.startState);
            states[leftNode.destState]->addDestState(0, q->state);
            states[rightNode.destState]->addDestState(0, q->state);
            return Node(leftNode.startState, rightNode.destState);
        }

        case OpType::OpConcat: {
            Node leftNode = getNFANode(treeNode->left);
            Node rightNode = getNFANode(treeNode->right);
            states[leftNode.destState]->addDestState(0, rightNode.startState);
            return Node(leftNode.startState, rightNode.destState);
        }

        case OpType::OpStar: {
            Node leftNode = getNFANode(treeNode->left);
            State*p = new State(++stateNumbers);
            states.push_back(p);
            State*q = new State(++stateNumbers);
            states.push_back(q);
            p->addDestState(0, leftNode.startState);
            p->addDestState(0, q->state);
            states[leftNode.destState]->addDestState(0, q->state);
            states[leftNode.destState]->addDestState(0, leftNode.startState);
            return Node(p->state, q->state);
        }

        default:
            break;
        }

        break;

    case ValueNode: {
        State*p = new State(++stateNumbers);
        states.push_back(p);

        State*q = new State(++stateNumbers);
        states.push_back(q);
        char c = treeNode->data.value;

        if (c != '.') {
            alphabet.insert(c);
            p->addDestState(c, q->state);
        } else {
            for (int a = 'A'; a <= 'Z'; ++a) {
                alphabet.insert(a);
                p->addDestState(a, q->state);
                alphabet.insert(a + 'a' - 'A');
                p->addDestState(a + 'a' - 'A', q->state);
            }
        }

        return Node(p->state, q->state);
    }

    default:
        break;
    }

    return Node(0, 0);
}

void NFA::printTree(TreeNode *current, int level) const {
    if (current == NULL) return;

    cout << string(level, ' ') << current->toChar() << endl;
    printTree(current->left, level + 1);
    printTree(current->right, level + 1);
}

NFA::~NFA() {
}
