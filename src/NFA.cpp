#include "NFA.h"
int NFA::NOTCHAR=-2;
int NFA::END=-1;
NFA::NFA(string input) :
	startState(0), stateNumbers(-1), sstream(input+"#"), flag(false), m_next_token(input[0]),m_input(input),m_next_token_location(0) {
	operStack.push('#');
}
bool NFA::makeNFA() {
	char c;
	while ((c=getNextChar())!=END) {
		if (c==NOTCHAR) {
			process();
		} else {
			if (TRACENFA)
				cout<<"this is a letter "<<c<<endl;

			State*p=new State(++stateNumbers);
			states.push_back(p);

			State*q=new State(++stateNumbers);
			states.push_back(q);

            if (c != '.') {
                    alphabet.insert(c);
                    p->addDestState(c, stateNumbers);
            } else {
                for (int a = 'A'; a <= 'Z'; ++a) {
                    alphabet.insert(a);
                    p->addDestState(a, stateNumbers);
                    alphabet.insert(a + 'a' - 'A');
                    p->addDestState(a + 'a' - 'A', stateNumbers); }
            }
			nodeStack.push(new Node(stateNumbers-1,stateNumbers));
			endState=stateNumbers;
		}
	}
	return true;
}
void NFA::process() {
	if (TRACENFA)
		cout<<"this is an oper"<<letter<<endl;
	char temp=operStack.top();
	while (comPri(letter)<inPri(temp)) {
		operStack.pop();
		switch (temp) {
		case '*': {
			Node*node=nodeStack.top();
			nodeStack.pop();
			State*p=new State(++stateNumbers);
			states.push_back(p);
			State*q=new State(++stateNumbers);
			states.push_back(q);
			p->addDestState(0, node->startState);
			p->addDestState(0, stateNumbers);
			if (TRACENFA)
				cout<<"start"<<node->startState<<"end"<<node->destState<<endl;
			states[node->destState]->addDestState(0, stateNumbers);
			states[node->destState]->addDestState(0, node->startState);
			nodeStack.push(new Node(stateNumbers-1,stateNumbers));

			/*addEqualState(p, states[node->startState]);
			 addEqualState(p, states[stateNumbers]);
			 addEqualState(states[node->destState], states[stateNumbers]);
			 addEqualState(states[node->destState], states[node->startState]);*/

			startState=stateNumbers-1;
			endState=stateNumbers;
			break;
		}
		case '.': {
			Node*right=nodeStack.top();
			nodeStack.pop();
			Node*left=nodeStack.top();
			nodeStack.pop();
			states[left->destState]->addDestState(0, right->startState);
			nodeStack.push(new Node(left->startState,right->destState));

			//addEqualState(states[left->destState],states[right->startState]);

			startState=left->startState;
			endState=right->destState;
			break;
		}
		case '|': {
			Node*right=nodeStack.top();
			nodeStack.pop();
			Node*left=nodeStack.top();
			nodeStack.pop();
			State*p = new State(++stateNumbers);
			states.push_back(p);
			State*q=new State(++stateNumbers);
			states.push_back(q);
			p->addDestState(0, left->startState);
			p->addDestState(0, right->startState);
			states[left->destState]->addDestState(0, stateNumbers);
			states[right->destState]->addDestState(0, stateNumbers);
			nodeStack.push(new Node(stateNumbers-1,stateNumbers));
			startState=stateNumbers-1;
			endState=stateNumbers;

			/*addEqualState(p, states[left->startState]);
			 addEqualState(q, states[right->startState]);
			 addEqualState(states[left->destState], states[stateNumbers]);
			 addEqualState(states[right->destState], states[stateNumbers]);*/

			break;
		}
		}
		temp=operStack.top();
	}
	if (letter==')') {
		if (operStack.top()!='(')
			/**/;
		operStack.pop();
	} else if (letter=='#') {
		operStack.pop();
	} else {
		operStack.push(letter);
	}
}
void NFA::display() const {
	cout<<"NFA display"<<endl<<"**********************"<<endl;
	cout<<"Alphabet: ";
	for (set<char>::iterator it=alphabet.begin(); it!=alphabet.end(); it++)
		cout<<(*it);
	cout<<endl;
	cout<<"stateNumbers:"<<stateNumbers+1<<endl;
	cout<<"startState: "<<startState<<" endState: "<<endState<<endl;
	for (size_t i=0; i<states.size(); ++i) {
		cout<<"State "<<states[i]->state<<endl;
		for (multimap<char,int>::iterator it=states[i]->stateChange.begin(); it
				!=states[i]->stateChange.end(); it++) {
			cout<<"char:"<<it->first<<endl;
			cout<<" dest:"<<it->second<<endl;
		}
		cout<<"Equal States";
		for (set<int>::iterator it=states[i]->equalStates.begin(); it
				!=states[i]->equalStates.end(); it++) {
			cout<<" "<<*it;
		}
		cout<<endl;
	}
	cout<<"*********************"<<endl;
}
bool NFA::checkExpression() {
    TreeNode* node = expr();
    cout << "checkExpression:" << node->content << endl;
    return m_next_token_location == m_input.size();
}
/*
<expr> -> <term> { | <term> }
<term> -> <factor> { <factor> }
<factor> -> <element> [*]
<element> -> (<expr>) | letter(contain digit)
*/
TreeNode* NFA::expr(void) {
    TreeNode* node = new TreeNode();
    TreeNode* t1 = term();
    node->content += t1->content;
    while(m_next_token == '|') {
        match('|');
        node->content += '|';
        TreeNode* t2 = term();
        node->content += t2->content;
    }
    cout << "expr:" << node->content << endl;
    return node;
}
TreeNode* NFA::term(void) {
    TreeNode* node = new TreeNode();
    TreeNode* f1 = factor();
    node->content += f1->content;
    while(m_next_token == '(' || isalnum(m_next_token) || m_next_token == '.') {
        TreeNode* f2 = factor();
        node->content += f2->content;
    }
    cout << "term:" << node->content << endl;
    return node;
}
TreeNode* NFA::factor(void) {
    TreeNode* node = new TreeNode();
    TreeNode* e1 = element();
    node->content += e1->content;
    while (m_next_token == '*') {
        match('*');
        node->content += '*';
    }
    cout << "factor:" << node->content << endl;
    return node;
}
TreeNode* NFA::element(void) {
    TreeNode* node = new TreeNode();
    if(m_next_token == '(') {
        match('(');
        node->content += '(';
        TreeNode* e1 = expr();
        node->content += e1->content;
        match(')');
        node->content += ')';
    } else if (isalnum(m_next_token) || m_next_token == '.' ) {
        node->content += m_next_token;
        match(m_next_token);
    } else {
        error();
        return node;
    }
    cout << "element:" << node->content <<endl;
    return node;
}
NFA::~NFA() {
}
