// Class to parse and provide information about instructions

#pragma once

// The elements of an instruction.
class Instruction {

public:

	Instruction();
	~Instruction() { };

	// Codes to indicate the type of instruction we are processing.  Why is this inside the
	// class?
	enum InstructionType {
		ST_MachineLanguage, // A machine language instruction.
		ST_AssemblerInstr,  // Assembler Language instruction.
		ST_Comment,          // Comment or blank line
		ST_End,                    // end instruction.
		ST_ILLEGAL,			// Illegal instruction type
		ST_None
	};
	// Parse the Instruction.
	InstructionType ParseInstruction(string &a_buff);

	// Compute the location of the next instruction.
	int LocationNextInstruction(int a_loc);

	// To access the label
	inline string &GetLabel() {

		return m_Label;
	};
	// To determine if a label is blank.
	inline bool isLabel() {

		return !m_Label.empty();
	};

	inline string &GetInstruction() {
		return m_instruction;
	}

	inline int getOpCodeValue() {
		return m_NumOpCode;
	}

	inline int getOperandValue() {
		return m_OperandValue;
	}

	inline string getOperand() {
		return m_Operand;
	}

	inline bool isNumericOperand() {
		return m_IsNumericOperand;
	}

	inline string getOpCode() {
		return m_OpCode;
	}

	inline void setOpCode(string str) {
		m_OpCode = str;
	}

	inline void setLabel(string str) {
		m_Label = str;
	}

	inline void setOperand(string str) {
		m_Operand = str;
	}
private:


	// The elemements of a instruction
	string m_Label;            // The label.
	string m_OpCode;       // The symbolic op code.
	string m_Operand;      // The operand.


	string m_instruction;    // The original instruction.

	// Derived values.
	int m_NumOpCode;     // The numerical value of the op code.
	InstructionType m_type; // The type of instruction.

	bool m_IsNumericOperand;// == true if the operand is numeric.
	int m_OperandValue;   // The value of the operand if it is numeric.

	vector<string> MACHINE_OPCODE;
	vector<string> ASSEMBLY_OPCODE;

	void clearValues();

};

