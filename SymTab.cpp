// Implementation of the symbol table class.

#include "stdafx.h"
#include "SymTab.h"

void
SymbolTable::AddSymbol(string &a_symbol, int a_loc)
{
	// If the symbol is already in the symbol table, record it as multiply defined.
	map<string, int>::iterator st;
	st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end()) {

		st->second = multiplyDefinedSymbol;
		return;
	}
	// Record a the  location in the symbol table.
	m_symbolTable[a_symbol] = a_loc;
}

void SymbolTable::DisplaySymbolTable() {
	int index = 0;
	cout << "Symbol#    Symbol    Location" << endl;
	for (map<string, int>::iterator it = m_symbolTable.begin(); it != m_symbolTable.end(); ++it) {
		cout << "   " <<index++ << "\t    " << it->first << "\t      " <<it->second << endl;
	}
}

bool SymbolTable::LookupSymbol(const string &a_symbol, int &a_loc) {
	if (m_symbolTable.find(a_symbol) == m_symbolTable.end()) {
		// The symbol does not exist
		// ERROR HANDLING HERE

		return false;
	}
	
	a_loc = m_symbolTable[a_symbol];
	return true;
}