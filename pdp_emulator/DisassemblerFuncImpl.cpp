#include "Disassembler.h"
#include "stdafx.h"
#include "Emulator.h"
#include <iostream>
#include <sstream>
using namespace std;

#define operationWithOffset( operation, offset) ( (Operation*) ((uint16_t*)operation + offset))

void parseJustOperand(Operation* operation, unsigned int &size, stringstream& ss, unsigned int &next_command_read , Operand operand){
	if (operand.split.reg == 07){
		switch (operand.split.mode)
		{
		case 2:
			next_command_read++;
			ss << "#" << hex << operationWithOffset(operation, next_command_read)->raw << " ";
			break;
		case 3:
			next_command_read++;
			ss << "@#" << hex << operationWithOffset(operation, next_command_read)->raw << " ";
			break;
		case 6:
			next_command_read++;
			ss << hex << operationWithOffset(operation, next_command_read)->raw << " ";
			break;
		case 7:
			next_command_read++;
			ss << hex << operationWithOffset(operation, next_command_read)->raw << " ";
			break;
		//default:
			//break;
		}
	}
	else{
		switch (operand.split.mode)
		{
		case 0:
			ss << "R" << operand.split.reg<< " ";
			break;
		case 1:
			ss << "(R" << operand.split.reg << ") ";
			break;
		case 2:
			ss << "(R" << operand.split.reg << ")+ ";
			break;
		case 3:
			ss << "@(R" << operand.split.reg << ")+ ";
			break;
		case 4:
			ss << "-(R" << operand.split.reg << ") ";
			break;
		case 5:
			ss << "@-(R" << operand.split.reg << ") ";
			break;
		case 6:
			next_command_read++;
			ss << hex << operationWithOffset(operation, next_command_read)->raw << dec << "(R" << operand.split.reg << ") ";
			break;
		case 7:
			next_command_read++;
			ss << "@" << hex << operationWithOffset(operation, next_command_read)->raw << dec << "(R" << operand.split.reg << ") ";
			break;
		default:
			break;
		}
	}
}

string parseDoubleOperands(Operation* operation, unsigned int &size){
	unsigned int next_command_read = 0;
	stringstream ss;
	Operand operand;
	operand.raw = 0;
	operand.split.mode = operation->DWORD.ss; // ((operation->DWORD.ss) << 3) + operation->DWORD.SS;
	operand.split.reg = operation->DWORD.SS;
	parseJustOperand( operation, size, ss, next_command_read, operand);
	operand.raw = 0;
	operand.split.mode = operation->DWORD.dd; //operand.raw = ((operation->DWORD.dd) << 3) + operation->DWORD.DD;
	operand.split.reg = operation->DWORD.DD;
	parseJustOperand(operation, size, ss, next_command_read, operand);
	size = next_command_read;
	return ss.str();
}

string parseOperand(Operation* operation, unsigned int &size){
	unsigned int next_command_read = 0;
	stringstream ss;
	Operand operand;
	//operand.raw = ((operation->DWORD.dd) << 3) + operation->DWORD.DD;
	operand.split.mode = operation->DWORD.dd;
	operand.split.mode = operation->DWORD.DD;
	parseJustOperand(operation, size, ss, next_command_read, operand);
	size = next_command_read;
	return ss.str();
}

string default(Operation* operation, unsigned int &size){ stringstream ss; ss << "Undefined opOperation is called " << oct << operation->raw; string s = ss.str(); cout << s << endl; return s; };
string MOVW(Operation* operation, unsigned int &size){ return "MOVW " + parseDoubleOperands(operation, size); };
string MOVB(Operation* operation, unsigned int &size){ return "MOVB " + parseDoubleOperands(operation, size); };
string CMPW(Operation* operation, unsigned int &size){ return "CMPW " + parseDoubleOperands(operation, size); };
string CMPB(Operation* operation, unsigned int &size){ return "CMPB " + parseDoubleOperands(operation, size); };
string BITW(Operation* operation, unsigned int &size){ return "BITW " + parseDoubleOperands(operation, size); };
string BITB(Operation* operation, unsigned int &size){ return "BITB " + parseDoubleOperands(operation, size); };
string BICW(Operation* operation, unsigned int &size){ return "BICW " + parseDoubleOperands(operation, size); };
string BICB(Operation* operation, unsigned int &size){ return "BICB " + parseDoubleOperands(operation, size); };
string BISW(Operation* operation, unsigned int &size){ return "BISW " + parseDoubleOperands(operation, size); };
string BISB(Operation* operation, unsigned int &size){ return "BISB " + parseDoubleOperands(operation, size); };
string ADD(Operation* operation, unsigned int &size){ return "ADD " + parseDoubleOperands(operation, size); };
string SUB(Operation* operation, unsigned int &size){ return "SUB " + parseDoubleOperands(operation, size); };

string HALT_WAIT_RTI_IOT_RESET(Operation* operation, unsigned int &size){ 
	switch (operation->raw)
	{
	case 0:
		return "HALT ";
	case 1:
		return "WAIT ";
	case 3:
		return "RTI ";
	case 4:
		return "IOT ";
	case 5:
		return "RESET ";
	default:
		break;
	}
	stringstream ss; ss << "Undefined opOperation is called " << oct << operation->raw; string s = ss.str(); cout << s << endl; return s; };
string HALT(Operation* operation, unsigned int &size){ stringstream ss; ss << "Undefined opOperation is called " << oct << operation->raw; string s = ss.str(); cout << s << endl; return s; };
string WAIT(Operation* operation, unsigned int &size){ stringstream ss; ss << "Undefined opOperation is called " << oct << operation->raw; string s = ss.str(); cout << s << endl; return s; };
string RTI(Operation* operation, unsigned int &size){ stringstream ss; ss << "Undefined opOperation is called " << oct << operation->raw; string s = ss.str(); cout << s << endl; return s; };
string IOT(Operation* operation, unsigned int &size){ stringstream ss; ss << "Undefined opOperation is called " << oct << operation->raw; string s = ss.str(); cout << s << endl; return s; };
string RESET(Operation* operation, unsigned int &size){ stringstream ss; ss << "Undefined opOperation is called " << oct << operation->raw; string s = ss.str(); cout << s << endl; return s; };
string JMP(Operation* operation, unsigned int &size){ return "JMP " + parseOperand(operation, size); };
string RTS(Operation* operation, unsigned int &size){ return "RTS " + parseOperand(operation, size); };
string SWAP(Operation* operation, unsigned int &size){ return "SWAP " + parseOperand(operation, size); };
string BR(Operation* operation, unsigned int &size){
	stringstream ss;
	ss << "BR ";
	ss << "PC";
	if (operation->BRANCH.XX >= 0) ss << "+";
	ss << operation->BRANCH.XX;
	return ss.str();
};
string BNE(Operation* operation, unsigned int &size){ 
	stringstream ss; 
	ss << "BNE ";
	ss << "PC";
	if (operation->BRANCH.XX >= 0) ss << "+";
	ss << operation->BRANCH.XX;
	return ss.str();
};
string BEQ(Operation* operation, unsigned int &size){ return "BEQ " + parseOperand(operation, size); };
string BGE(Operation* operation, unsigned int &size){ return "BGE " + parseOperand(operation, size); };
string BLT(Operation* operation, unsigned int &size){ return "BLT " + parseOperand(operation, size); };
string BGT(Operation* operation, unsigned int &size){ return "BGT " + parseOperand(operation, size); };
string JSR(Operation* operation, unsigned int &size){ return "SUB " + parseDoubleOperands(operation, size); };

string CLRW(Operation* operation, unsigned int &size){ return "CLRW " + parseOperand(operation, size); };
string COMW(Operation* operation, unsigned int &size){ return "COMW " + parseOperand(operation, size); };
string INCW(Operation* operation, unsigned int &size){ return "INCW " + parseOperand(operation, size); };
string DECW(Operation* operation, unsigned int &size){ return "DECW " + parseOperand(operation, size); };
string NEGW(Operation* operation, unsigned int &size){ return "NEGW " + parseOperand(operation, size); };
string ADCW(Operation* operation, unsigned int &size){ return "ADCW " + parseOperand(operation, size); };
string SBCW(Operation* operation, unsigned int &size){ return "SBCW " + parseOperand(operation, size); };
string TSTW(Operation* operation, unsigned int &size){ return "TSTW " + parseOperand(operation, size); };
string RORW(Operation* operation, unsigned int &size){ return "RORW " + parseOperand(operation, size); };
string ROLW(Operation* operation, unsigned int &size){ return "ROLW " + parseOperand(operation, size); };
string ASRW(Operation* operation, unsigned int &size){ return "ASRW " + parseOperand(operation, size); };
string ASLW(Operation* operation, unsigned int &size){ return "ASLW " + parseOperand(operation, size); };

string CLRB(Operation* operation, unsigned int &size){ return "CLRB " + parseOperand(operation, size); };
string COMB(Operation* operation, unsigned int &size){ return "COMB " + parseOperand(operation, size); };
string INCB(Operation* operation, unsigned int &size){ return "INCB " + parseOperand(operation, size); };
string DECB(Operation* operation, unsigned int &size){ return "DECB " + parseOperand(operation, size); };
string NEGB(Operation* operation, unsigned int &size){ return "NEGB " + parseOperand(operation, size); };
string ADCB(Operation* operation, unsigned int &size){ return "ADCB " + parseOperand(operation, size); };
string SBCB(Operation* operation, unsigned int &size){ return "SBCB " + parseOperand(operation, size); };
string TSTB(Operation* operation, unsigned int &size){ return "TSTB " + parseOperand(operation, size); };
string RORB(Operation* operation, unsigned int &size){ return "RORB " + parseOperand(operation, size); };
string ROLB(Operation* operation, unsigned int &size){ return "ROLB " + parseOperand(operation, size); };
string ASRB(Operation* operation, unsigned int &size){ return "ASRB " + parseOperand(operation, size); };
string ASLB(Operation* operation, unsigned int &size){ return "ASLB " + parseOperand(operation, size); };

string BPL(Operation* operation, unsigned int &size){ return "BPL " + parseOperand(operation, size); };
string BMI(Operation* operation, unsigned int &size){ return "BMI " + parseOperand(operation, size); };
string BHI(Operation* operation, unsigned int &size){ return "BHI " + parseOperand(operation, size); };
string BLOS(Operation* operation, unsigned int &size){ return "BLOS " + parseOperand(operation, size); };
string BVC(Operation* operation, unsigned int &size){ return "BVC " + parseOperand(operation, size); };
string BVS(Operation* operation, unsigned int &size){ return "BVS " + parseOperand(operation, size); };
string BCC(Operation* operation, unsigned int &size){ return "BCC " + parseOperand(operation, size); };
string BCS(Operation* operation, unsigned int &size){ return "BCS " + parseOperand(operation, size); };
string EMT(Operation* operation, unsigned int &size){ return "EMT "; };
string TRAP(Operation* operation, unsigned int &size){ return "TRAP "; };

