#include<iostream>
#include "NFA.h"
#include "DFA.h"
#include "MiniDFA.h"
#include "Table.h"
using namespace std;
int main() {
	NFA nfa("(a|b)*abb");
	nfa.makeNFA();
	//nfa.display();
	DFA dfa(nfa);
	//dfa.display();
	MiniDFA minidfa(dfa);
	//minidfa.display();
	Table table(minidfa);
	table.read("abbbbbbabb");
}
