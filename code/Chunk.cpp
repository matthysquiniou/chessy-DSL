#include "Chunk.h"

size_t Chunk::push_instruction(const Instruction instructionCode)
{
    instructions.push_back(instructionCode);
    return instructions.size() - 1;
}

size_t Chunk::push_constant(const ConstantContainer constantValue)
{
    constants.push_back(constantValue);
    return constants.size() - 1;
}

size_t Chunk::push_function(const FunctionInfo &f)
{
    functions.push_back(f);
    return functions.size() - 1;
}
