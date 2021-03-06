#include "stdafx.h"
#include "Emulator.h"
#include "EmulatorFuncImpl.h"
#include "Disassembler.h"
#include "stdlib.h"
#include <iostream>
#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>
#include "inttypes.h"
using namespace std;


// this array of functions is responsible for 2 operand functions
void(*opOperationsDoubleOperandGroup[])(Operation *, Emulator* ) = {
	/* 00 */	default, // It is not 2 operand command
	/* 01 */	MOVW, // MOV(W) .1SSDD ZN-0
	/* 02 */	CMPW, // CMP(W) .2SSDD ZNCV
	/* 03 */	BITW, // BIT(W) .3SSDD ZN-0
	/* 04 */	BICW, // BIC(W) .4SSDD ZN-0
	/* 05 */	BISW, // BIS(W) .5SSDD ZN-0
	/* 06 */	ADD, // ADD( ) .6SSDD ZNCV
	/* 07 */	default, // 
	/* 10 */	default, // It is not 2 operand command
	/* 11 */	MOVB, // MOV(B) .1SSDD ZN-0
	/* 12 */	CMPB, // CMP(B) .2SSDD ZNCV
	/* 13 */	BITB, // BIT(B) .3SSDD ZN-0
	/* 14 */	BICB, // BIC(B) .4SSDD ZN-0
	/* 15 */	BISB, // BIS(B) .5SSDD ZN-0
	/* 16 */	SUB, // SUB( ) .6SSDD ZNCV
	/* 17 */	default  // 
};

// this array of functions is responsible for 2 operand functions
void(*opOperationsNoDoubleOperandGroup[])(Operation *, Emulator*) = {
	//                       OPCODE    ZNCV
	/* 0000 */	HALT_WAIT_RTI_IOT_RESET,
	// HALT or 
	// WAIT or 
	// RTI or
	// IOT or
	// RESET or
	/* 0001 */	JMP, // JMP 0001DD ----
	/* 0002 */	RTS, // RTS 00020R ----
	/* 0003 */	SWAP, // SWAP 0003DD ++00
	/* 0004 */	BR, // BR 0004XX ----
	/* 0005 */	BR,
	/* 0006 */	BR,
	/* 0007 */	BR ,
	/* 0010 */	BNE, // BNE 0010XX ----
	/* 0011 */	BNE,
	/* 0012 */	BNE,
	/* 0013 */	BNE,
	/* 0014 */	BEQ, // BEQ 0014XX -----
	/* 0015 */	BEQ,
	/* 0016 */	BEQ,
	/* 0017 */	BEQ,
	/* 0020 */	BGE, // BGE 0020XX ----
	/* 0021 */	BGE,
	/* 0022 */	BGE,
	/* 0023 */	BGE,
	/* 0024 */	BLT, // BLT 0024XX ----
	/* 0025 */	BLT,
	/* 0026 */	BLT,
	/* 0027 */	BLT,
	/* 0030 */	BGT, // BGT 0030XX -----
	/* 0031 */	BGT,
	/* 0032 */	BGT,
	/* 0033 */	BGT,
	/* 0034 */	default,
	/* 0035 */	default,
	/* 0036 */	default,
	/* 0037 */	default,
	/* 0040 */	JSR, // JSR 004RDD -----
	/* 0041 */	JSR, // JSR
	/* 0042 */	JSR, // JSR
	/* 0043 */	JSR, // JSR
	/* 0044 */	JSR, // JSR
	/* 0045 */	JSR, // JSR
	/* 0046 */	JSR, // JSR
	/* 0047 */	JSR, // JSR
	/* 0050 */	CLRW, // CLR(W) .050DD 1000
	/* 0051 */	COMW, // COM(W) .051DD ++00
	/* 0052 */	INCW, // INC(W)
	/* 0053 */	DECW, // DEC(W)
	/* 0054 */	NEGW, // NEG(W)
	/* 0055 */	ADCW, // ADC(W)
	/* 0056 */	SBCW, // SBC(W)
	/* 0057 */	TSTW, // TST(W)
	/* 0060 */	RORW, // ROR(W)
	/* 0061 */	ROLW, // ROL(W)
	/* 0062 */	ASRW, // ASR(W)
	/* 0063 */	ASLW, // ASL(W)
	/* 0064 */	default,
	/* 0065 */	default,
	/* 0066 */	default,
	/* 0067 */	default,
	/* 0070 */	default,
	/* 0071 */	default,
	/* 0072 */	default,
	/* 0073 */	default,
	/* 0074 */	default,
	/* 0075 */	default,
	/* 0076 */	default,
	/* 0077 */	default,
	/* 1000 */	BPL, // BPL 1000XX ----
	/* 1001 */	BPL,
	/* 1002 */	BPL,
	/* 1003 */	BPL,
	/* 1004 */	BMI, // BMI 1004XX ----
	/* 1005 */	BMI,
	/* 1006 */	BMI,
	/* 1007 */	BMI,
	/* 1010 */	BHI, // BHI 1010XX ---
	/* 1011 */	BHI,
	/* 1012 */	BHI,
	/* 1013 */	BHI,
	/* 1014 */	BLOS, // BLOS 1014XX ----
	/* 1015 */	BLOS,
	/* 1016 */	BLOS,
	/* 1017 */	BLOS,
	/* 1020 */	BVC, // BVC 1020XX ----
	/* 1021 */	BVC,
	/* 1022 */	BVC,
	/* 1023 */	BVC,
	/* 1024 */	BVS, // BVC 1024XX ---
	/* 1025 */	BVS,
	/* 1026 */	BVS,
	/* 1027 */	BVS,
	/* 1030 */	BCC, // BCC(BHIS) 1030XX ----
	/* 1031 */	BCC,
	/* 1032 */	BCC,
	/* 1033 */	BCC,
	/* 1034 */	BCS, // BCS(BLO) 1034XX ----
	/* 1035 */	BCS,
	/* 1036 */	BCS,
	/* 1037 */	BCS,
	/* 1040 */	EMT, // EMT
	/* 1041 */	EMT, // EMT
	/* 1042 */	EMT, // EMT
	/* 1043 */	EMT, // EMT
	/* 1044 */	TRAP, // TRAP
	/* 1045 */	TRAP, // TRAP
	/* 1046 */	TRAP, // TRAP
	/* 1047 */	TRAP, // TRAP
	/* 1050 */	CLRB, // CLR(W) .050DD 1000
	/* 1051 */	COMB, // COM(W) .051DD ++00
	/* 1052 */	INCB, // INC(W)
	/* 1053 */	DECB, // DEC(W)
	/* 1054 */	NEGB, // NEG(W)
	/* 1055 */	ADCB, // ADC(W)
	/* 1056 */	SBCB, // SBC(W)
	/* 1057 */	TSTB, // TST(W)
	/* 1060 */	RORB, // ROR(W)
	/* 1061 */	ROLB, // ROL(W)
	/* 1062 */	ASRB, // ASR(W)
	/* 1063 */	ASLB, // ASL(W)
	/* 1064 */	default,
	/* 1065 */	default,
	/* 1066 */	default,
	/* 1067 */	default,
	/* 1070 */	default,
	/* 1071 */	default,
	/* 1072 */	default,
	/* 1073 */	default,
	/* 1074 */	default,
	/* 1075 */	default,
	/* 1076 */	default,
	/* 1077 */	default
};

Emulator::Emulator()
{
	resetRegisters();
	this->memory = new char[MEMORY_SIZE];
	memset(this->memory, 0, MEMORY_SIZE);
}

void Emulator::resetRegisters() {
	for (int i = 0; i < 8; i++){
		this->registers.R[i] = 0;
	}
	this->registers.flagC = 0;
	this->registers.flagI = 0;
	this->registers.flagN = 0;
	this->registers.flagT = 0;
	this->registers.flagV = 0;
	this->registers.flagZ = 0;
}

int Emulator::writeBufToMemory(char* buf, unsigned int position, unsigned int size){
	if (position + size < MEMORY_SIZE){
		memcpy(memory + position, buf, size);
		return 0;
	}
	cout << "Error, writing outside of memory " << hex << position;
	return 1;
}

int Emulator::writeByteToMemory(char byte, unsigned int position){
	if (position < MEMORY_SIZE){
		memory[position] = byte;
		return 0;
	}
	cout << "Error, writing outside of memory " << hex << position;
	return 1;
}

int Emulator::writeWordToMemory(uint16_t word, unsigned int position){
	if (position < MEMORY_SIZE -1 ){
		*((uint16_t*)(memory + position)) = word;
		return 0;
	}
	cout << "Error, writing outside of memory " << hex << position;
	return 1;
}


char Emulator::readByteFromMemory(unsigned int position){
	if (position < MEMORY_SIZE){
		return memory[position];
	}
	cout << "Error, reading outside of memory "<< hex << position;
	return 0;
}

uint16_t Emulator::readWordFromMemory(unsigned int position){
	if (position < MEMORY_SIZE-1){
		return *((uint16_t*)(memory + position)) ;
	}
	cout << "Error, reading outside of memory " << hex << position;
	return 0;
}


int Emulator::step(){
	Operation * operation = (Operation*)&this->memory[registers.R[R_PC]];
	if (Disassembler::checkIfItIsDoubleOperandCommand(operation ) ){
		opOperationsDoubleOperandGroup[Disassembler::getIndexForDobleOperandCommandInFunctionsArray(operation)](operation, this);
	}
	else{
		uint32_t index = Disassembler::getIndexForNoDobleOperandCommandInFunctionsArray(operation);
		opOperationsNoDoubleOperandGroup[index](operation, this);
	}
	return 0;
}

void Emulator::incPc(){
	this->registers.R[R_PC]+=2;
}

char* Emulator::getVideoMemory(){
	return &memory[VIDEO_MEMORY];
}