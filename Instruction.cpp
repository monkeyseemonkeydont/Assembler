#include "stdafx.h"
#include "Instruction.h"
#include <string>

Instruction::Instruction() {
	MACHINE_OPCODE = { "add", "sub", "mult", "div", "load", "store", "read", "write", "b", "bm", "bz", "bp", "halt" };
	ASSEMBLY_OPCODE = { "org", "dc", "ds", "end" };
	m_IsNumericOperand = true;
	
}

Instruction::InstructionType Instruction::ParseInstruction(string &a_buff) {
	bool hasComment = false;
	clearValues();

	// first save the given string instruction
	m_instruction = a_buff;

	istringstream input(a_buff);
	string ibuff;
	int count = 0;

	input >> ibuff;

	// Handles the empty line
	if (ibuff.empty()) return ST_None;

	// IF there is a comment at the beginning, it means the instruction is just the comment
	if (ibuff.at(0) == ';') return ST_Comment;

	// Start the count and start the loop to fill in the values 
	while(ibuff != ""){

		int findComment = ibuff.find(';');
		// if the first of the string is comment, break the loop since we don't need it anymore
		if ( findComment >= 0) {
			hasComment = true;
			
			if (ibuff.at(0) == ';') break;

			ibuff = ibuff.substr(0, ibuff.find(';'));
		}

		switch(count) {
			case 0:
				m_Label = ibuff;
				break;
			case 1:
				m_OpCode = ibuff;
				break;
			case 2:
				m_Operand = ibuff;
				break;
		}
		
		// Read the next element into ibuff.
		ibuff = "";

		input >> ibuff;
		count++;

		if (hasComment) break;
	}

	// Now correct the places according to the count
	if (count == 2) {
		// only contains two words in the string.

		if (find(MACHINE_OPCODE.begin(), MACHINE_OPCODE.end(), m_OpCode) == MACHINE_OPCODE.end()) {
			if (find(ASSEMBLY_OPCODE.begin(), ASSEMBLY_OPCODE.end(), m_OpCode) == ASSEMBLY_OPCODE.end()) {
				m_Operand = m_OpCode;
				m_OpCode = m_Label;
				m_Label = "";
			}
		}
	}
	else if (count == 1) {
		m_OpCode = m_Label;
		m_Label = "";
	}

	// convert the OpCode to lower case
	for (int i = 0; i < m_OpCode.length(); ++i) {
		m_OpCode[i] = tolower(m_OpCode[i]);
	}

	// Compute the instruction type of the opcode
	if (find(MACHINE_OPCODE.begin(), MACHINE_OPCODE.end(), m_OpCode) != MACHINE_OPCODE.end()) {
		m_type = ST_MachineLanguage;
		m_NumOpCode = distance(MACHINE_OPCODE.begin(), find(MACHINE_OPCODE.begin(), MACHINE_OPCODE.end(), m_OpCode)) + 1;
	}
	else if (m_OpCode == "end") {
		m_type = ST_End;
	}
	else if (find(ASSEMBLY_OPCODE.begin(), ASSEMBLY_OPCODE.end(), m_OpCode) != ASSEMBLY_OPCODE.end()) {
		m_type =  ST_AssemblerInstr;
	}
	else {
		m_type =  ST_ILLEGAL;
	}

	// Try to check if the Operand is a numeric value and store it if it is true.
	try {
		m_OperandValue = stoi(m_Operand);
	}
	catch (invalid_argument&) {
		m_IsNumericOperand = false;
	}
		
	// Compare it with the OpCode and return what type of instruction it is
	return m_type;
}

int Instruction::LocationNextInstruction(int a_loc) {
	if (m_OpCode == "org" || m_OpCode == "ds") return a_loc + m_OperandValue;
	
	return ++a_loc;
}

void Instruction::clearValues() {
	m_Label = "";
	m_OpCode = "";
	m_Operand = "";
	m_instruction = "";
	m_OperandValue = 999999;
	m_NumOpCode = 0;
	m_IsNumericOperand = true;
}