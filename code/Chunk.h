#pragma once
#include "Defines.h"
#include "Instruction.h"
#include "ConstantContainer.h"
#include "FunctionInfo.h"

struct Chunk
{
private:
	vector<Instruction> instructions;
	vector<ConstantContainer> constants;
	vector<FunctionInfo> functions;
    size globalSlotCountValue = 0;

public:
	size push_instruction(const Instruction instructionCode);
	size push_constant(const ConstantContainer constantValue);
	size push_function(const FunctionInfo& f);
	
	Instruction& read_instruction(const size index) { return instructions[index]; }
	ConstantContainer& read_constants(const size index) { return constants[index]; }
	FunctionInfo& read_functions(const size index) { return functions[index]; }

	size instruction_size() { return instructions.size(); }
	size constant_size() const { return constants.size(); }

	void set_global_slot_count(const size count) { globalSlotCountValue = count; }
    size global_slot_count() const { return globalSlotCountValue; }
};