
//Implements the Assembler Class

#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"


Assembler::Assembler(int argc, char *argv[])
	: m_facc(argc, argv)
{
	
}

void Assembler::PassI()
{
	Errors::InitErrorReporting();

	int loc = 0;        // Tracks the location of the instructions to be generated.

	// Successively process each line of source code.
	for (;;) {
		contents = 0;

		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) {

			// If there are no more lines, we are missing an end statement.
			// We will let this error be reported by Pass II.
			return;
		}

		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);

		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) return;

		// Labels can only be on machine language and assembler language
		// instructions.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) continue;

		// If the instruction has a label, record it and its location in the
		// symbol table.
		if (m_inst.isLabel()) {
			// Checks if the label is multiply defined
			int tmp;
			if (m_symtab.LookupSymbol(m_inst.GetLabel(), tmp)) {
				cout << "The symbol exists" << endl;
				Errors::RecordError("Multiply defined label \'" + m_inst.GetLabel() + "\'. This has created a huge problem");
			}
			m_symtab.AddSymbol(m_inst.GetLabel(), loc);
		}
		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}
}

void Assembler::PassII() {
	// Rewinds to the beginning of the file
	m_facc.rewind();

	int loc = 0;        // Tracks the location of the instructions to be generated.

	// Prints the beginning of the output.
	cout << setw(8) << "Location" << setw(10) << "Contents" << setw(30) << "Original Statement" << endl;

	// Successively process each line of source code.
	for (;;) {

		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) {

			// If there are no more lines, we are missing an end statement.
			Errors::RecordError("No End Statement provided");
			break;
		}

		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);

		// Validates the label, opcode and operand if they follow the format
		if (m_inst.GetLabel().length() > 10) {
			Errors::RecordError("Label size too big for \'" + m_inst.GetLabel() + "\'. It should be between 1-10 characters");
			m_inst.setLabel("?");
		}
		else {
			char firstChar = m_inst.GetLabel()[0];
			if (firstChar >= '0' && firstChar <= '9')
				Errors::RecordError("The first character for label cannot be a number.");
		}

		// Validates the OpCode
		if (st == Instruction::ST_ILLEGAL) {
			Errors::RecordError("The OpCode '" + m_inst.getOpCode() + "' is illegal. It is not standard syntax");
		}

		// If this is an end statement, there is nothing left to do in pass I.
		if (st == Instruction::ST_End) {
			cout << "\t \t" << m_inst.GetInstruction() << endl;
			break;
		}

		// Validates the Operand
		if (m_inst.isNumericOperand()) {
			if (m_inst.getOperandValue() > 999999) {
				Errors::RecordError("The value for '" +m_inst.getOpCode() + "' is very high.");
			}
		}


		// Start the output of the file
		// If the instruction is not the machine or assembler, then just print out the instruction
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) {
			cout << "\t \t \t" << m_inst.GetInstruction() << endl;
			continue;
		}
		
		// Computes the content/the machine level instruction
		contents = m_inst.getOpCodeValue() * 10000;
		
		// Checks if the instruction is assembly language and computes the 4 digits of machine language
		if (st == Instruction::ST_AssemblerInstr) {
			if (m_inst.getOpCode() != "end") {
				if (m_inst.getOperand().empty()) {
					Errors::RecordError("The Operand for the OpCode cannot be empty for '" + m_inst.getOpCode() + "'");
				}
				else if (!m_inst.isNumericOperand()) {
					Errors::RecordError("The Operand for '" + m_inst.getOpCode() + "' should be a numeric value");
				}
				if (m_inst.getOpCode() != "org" && m_inst.GetLabel().empty()) {
					Errors::RecordError("You did not provide the label for '" + m_inst.getOpCode() + "'");
				}
			}
			if (m_inst.getOpCode() == "dc") {
				contents += m_inst.getOperandValue();
			}
		}
		else {
			// Check if the Operand is numeric. If not, get the location in symbol table and add it to contents
			if (m_inst.getOperand().empty() && m_inst.getOpCodeValue() != 13) {
				Errors::RecordError("The Operand is missing in Opcode '" + m_inst.getOpCode() + "'");
			}
			else if (!m_inst.isNumericOperand() && !m_inst.getOperand().empty()) {
				int symbolLocation = 0;
				if (!m_symtab.LookupSymbol(m_inst.getOperand(), symbolLocation)) {
					Errors::RecordError("Symbol \'" + m_inst.getOperand() + "\' not found");
					symbolLocation = 9999;
				}

				contents += symbolLocation;
			}
		}

		// Insert it into the accumulator
		m_emul.insertMemory(loc, contents);


		// Prints out the location, contents and original statement
		cout << setfill (' ') << setw(6) << loc << "\t";
		if (contents != 0) cout << "   " << setfill('0') << setw(6) << contents;
		else cout << "\t";
		cout << "\t" << m_inst.GetInstruction() << endl;

		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}

	Errors::DisplayErrors();
}

void Assembler::RunEmulator(){
	if (!Errors::isEmpty()) {
		cout << "Cannot run the emulator due to existing errors" << endl;
	}
	else {
		m_emul.runProgram();
	}
}