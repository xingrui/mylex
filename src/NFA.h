#ifndef NFA_H_
#define NFA_H_
#include<iostream>
#include<vector>
#include<stack>
#include<sstream>
#include<ctype.h>
#include "State.h"
#include "globals.h"
class DFA;
using namespace std;
enum NodeType {
    OpNode,
    ValueNode
};
enum OpType {
    OpOR,
    OpConcat,
    OpStar
};

class TreeNode {
public:
    TreeNode *left;
    TreeNode *right;
    NodeType nodeType;
    union Data {
        OpType opType;
        char value;
    };

    Data data;
    static int findDepth(TreeNode *node) {
        if (node == NULL) return 0;

        int left = findDepth(node->left);
        int right = findDepth(node->right);
        return left > right ? left + 1 : right + 1;
    }

    static char staticToChar(TreeNode *node) {
        if (node == NULL) return '_';

        return node->toChar();
    }

    char toChar() const {
        switch (nodeType) {
        case OpNode:
            switch (data.opType) {
            case OpType::OpOR:
                return '|';

            case OpType::OpConcat:
                return '+';

            case OpType::OpStar:
                return '*';

            default:
                return ' ';
            }

            break;

        case ValueNode:
            return data.value;
        }
    }
};

class NFA {
    class Node {
    public:
        int startState, destState;
        Node(int start, int dest): startState(start), destState(dest) {

        }
    };

public:
    NFA(string input);
    bool makeNFA();
    Node getNFANode(TreeNode *node);
    bool checkExpression();
    void display() const;
    virtual ~NFA();
    const vector<State *> &getStates() const {
        return states;
    }

    int getEndState() const {
        return endState;
    }

    int getStartState() const {
        return startState;
    }

    const set<char> &getAlphabet() const {
        return alphabet;
    }

private:
    vector<State *>states;
    int startState;
    static int NOTCHAR;
    static int END;
    int stateNumbers;
    int endState;
    set<char>alphabet;
    stack<char>operStack;
    stack<Node *>nodeStack;
    stringstream sstream;
    string m_input;
    int m_next_token_location;
    char m_next_token;
    char letter;
    bool flag;
    void addEqualState(State *p, State *b) {
        for (set<int>::iterator it = b->equalStates.begin(); it
                != b->equalStates.end(); it++) {
            p->equalStates.insert((*it));
        }
    }

    //cases that need to add . between character
    //before : ) * letter
    //after : ( letter
    //
    char getNextChar() {
        char c;

        if (!(sstream >> c) || letter == '#')
            return END;

        if (flag && (c == '(' || isalpha(c))) {
            letter = '.';
            sstream.putback(c);
            flag = false;
            return NOTCHAR;
        }

        flag = false;

        if (c == '#' || c == '(' || c == ')' || c == '*' || c == '|' || c == '.') {
            letter = c;

            if (c == ')' || c == '*')
                flag = true;

            return NOTCHAR;
        }

        flag = true;
        return c;
    }

    void process();
    static int comPri(char c) {
        switch (c) {
        case '#':
            return 0;

        case '(':
            return 8;

        case ')':
            return 1;

        case '|':
            return 2;

        case '.':
            return 4;

        case '*':
            return 6;

        default:
            return 0;
        }
    }

    static int inPri(char c) {
        switch (c) {
        case '#':
            return 0;

        case '(':
            return 1;

        case ')':
            return 8;

        case '|':
            return 3;

        case '.':
            return 5;

        case '*':
            return 7;

        default:
            return 0;
        }
    }

    /*
    <expr> -> <term> { | <term> }
    <term> -> <factor> { <factor> }
    <factor> -> <element> {*}
    <element> -> <expr> | letter(contain digit)
    */
    TreeNode *expr(void);
    TreeNode *term(void);
    TreeNode *factor(void);
    TreeNode *element(void);
    void error(void) {
        cerr << "Error[m_next_token:" << m_next_token << "]" << endl;
        exit(1);
    }

    void match(char expectedToken) {
        if (m_next_token == expectedToken) {
            // cout << m_next_token << " matched" << endl;
            m_next_token = get_next_token();
        } else {
            error();
        }
    }

    char get_next_token() {
        ++m_next_token_location;

        if (m_next_token_location < m_input.size()) {
            m_next_token = m_input[m_next_token_location];
            return m_next_token;
        } else {
            m_next_token = ' ';
        }

        return ' ';
    }

    void printTreeNew(TreeNode *treeNode, int depth) const;
    void printTree(TreeNode *treeNode, int level) const;
};

#endif /*NFA_H_*/
