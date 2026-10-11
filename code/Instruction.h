#pragma once 
#include "Defines.h"
#include "OpCode.h"

struct Instruction
{
	OpCode operation;
	size regA;
	size regB;
	size regC;
};