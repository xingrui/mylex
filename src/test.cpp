#include<iostream>
#include "NFA.h"
#include "DFA.h"
#include "MiniDFA.h"
#include "Table.h"
using namespace std;
int match(string pattern, string content) {
	NFA nfa(pattern);
    bool result = nfa.checkExpression();
    cout << "checkExpression result:[" << result << "]" << endl;
    if(!result){
          return 1;
    }
	nfa.makeNFA();
	nfa.display();
	DFA dfa(nfa);
	//dfa.display();
	MiniDFA minidfa(dfa);
	//minidfa.display();
	Table table(minidfa);
	return table.match(content);
}

int main(int argc, char **argv){
    if (argc == 3) {
        cout << argv[1] << endl;
        cout << argv[2] << endl;
        cout << match(argv[1], argv[2]) << endl;
        return 0;
    }
    //cout << match("(a|b)*abb", "abbbbbbabb") << endl;
    cout << match("d(a|bb)*b", "dbbbbbbb") << endl;
    //cout << match(".*c", "aaaaaaaaaaaac") << endl;
    return 0;
}
