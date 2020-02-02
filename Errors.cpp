#include "stdafx.h"
#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;

void Errors::InitErrorReporting() {
	m_ErrorMsgs.erase(m_ErrorMsgs.begin(), m_ErrorMsgs.end());
}

void Errors::RecordError(const string &a_emsg) {
	m_ErrorMsgs.push_back(a_emsg);
}

void Errors::DisplayErrors() {
	for (int i = 0; i < m_ErrorMsgs.size(); ++i) {
		cout << m_ErrorMsgs[i] << endl;
	}
}

bool Errors::isEmpty() {
	return m_ErrorMsgs.empty();
}