#include "VM.h"
#include "ChessNative.h"
#include "Builtins.h"

VM::VM()
{
    m_Stack.resize(stackAllocationSize);
    m_Frames.resize(framesAllocationSize);
    m_OldInstructionIndex.resize(1024);
    m_BoardStates.reserve(1024 * 1024);
}

void VM::Run()
{
    size globalSlotCount = m_pChunk->global_slot_count();
    size instructionSize = m_pChunk->instruction_size();

    m_InstructionIndex = 0;
    m_StackIndex = globalSlotCount;
    m_FramesIndex = 0;
    m_Frames[0] = { 0, globalSlotCount, {} };

    while(m_InstructionIndex < instructionSize)
    {
        Instruction& rInstruction = Read_Instruction();
        switch (rInstruction.operation)
        {
        case OpCode::_MOV:
        {
            m_Registers[rInstruction.regA] = m_Registers[rInstruction.regB];
            break;
        }
        case OpCode::_LOADC:
        {
            m_Registers[rInstruction.regA] = m_pChunk->read_constants(rInstruction.regB);
            break;
        }
        case OpCode::_LOAD:
        {
            StackFrame& rFrame = m_Frames[m_FramesIndex];
            if(rInstruction.regB >= rFrame.frameSize)
            {
                std::cout << "Index in frame out of range" << std::endl;
                exit(400);
            }
            size element = rFrame.beginIndex + rInstruction.regB;
            m_Registers[rInstruction.regA] = m_Stack[element];
            break;
        }
        case OpCode::_STORE:
        {
            StackFrame& rFrame = m_Frames[m_FramesIndex];
            if(rInstruction.regA >= rFrame.frameSize)
            {
                std::cout << "Index in frame out of range" << std::endl;
                exit(400);
            }
            size element = rFrame.beginIndex + rInstruction.regA;
            m_Stack[element] = m_Registers[rInstruction.regB];
            break;
        }
        case OpCode::_LOADG:
        {
            StackFrame& rFrame = m_Frames[0];
            size element = rFrame.beginIndex + rInstruction.regB;
            m_Registers[rInstruction.regA] = m_Stack[element];
            break;
        }
        case OpCode::_STOREG:
        {
            StackFrame& rFrame = m_Frames[0];
            size element = rFrame.beginIndex + rInstruction.regA;
            m_Stack[element] = m_Registers[rInstruction.regB];
            break;
        }
        case OpCode::_PUSH:
        {
            if (m_Stack.size() <= m_StackIndex)
            {
                m_Stack.resize(m_Stack.size() + stackAllocationSize);
            }
            m_Stack[m_StackIndex] = m_Registers[rInstruction.regA];
            m_StackIndex++;
            break;
        }
        case OpCode::_POP:
        {
            m_StackIndex--;
            m_Registers[rInstruction.regA] = m_Stack[m_StackIndex];
            break;
        }

        case OpCode::_ADD:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = ADD_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_SUB:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = SUB_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_MUL:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = MUL_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_DIV:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = DIV_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_MOD:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = MOD_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_POW:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = POW_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_NEG:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            m_Registers[rInstruction.regA] = NEG_TABLE[(size)typeB](m_Registers[rInstruction.regB]);
            break;
        }

        case OpCode::_AND:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = AND_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_OR:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = OR_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_XOR:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = XOR_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_NOT:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            m_Registers[rInstruction.regA] = NOT_TABLE[(size)typeB](m_Registers[rInstruction.regB]);
            break;
        }
        case OpCode::_SHL:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = SHL_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_SHR:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = SHR_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_CMP_EQ:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = CMPEQ_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_CMP_NE:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = CMPNE_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_CMP_LT:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = CMPLT_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_CMP_LE:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = CMPLE_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_CMP_GT:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = CMPGT_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_CMP_GE:
        {
            ConstantType typeB = m_Registers[rInstruction.regB].type;
            ConstantType typeC = m_Registers[rInstruction.regC].type;
            m_Registers[rInstruction.regA] = CMPGE_TABLE[(size)typeB][(size)typeC](m_Registers[rInstruction.regB], m_Registers[rInstruction.regC]);
            break;
        }
        case OpCode::_JMP:
        {
            m_InstructionIndex = rInstruction.regB;
            break;
        }
        case OpCode::_JZ:
        {
            const ConstantContainer& value = m_Registers[rInstruction.regA];
            bool isZero = (value.type == ConstantType::_BOOL) ? (value.data.as_bool == false) : true;
            if (value.type != ConstantType::_BOOL)
            {
                switch (value.type)
                {
                case ConstantType::_I8:  isZero = (value.data.as_i8  == 0); break;
                case ConstantType::_I16: isZero = (value.data.as_i16 == 0); break;
                case ConstantType::_I32: isZero = (value.data.as_i32 == 0); break;
                case ConstantType::_I64: isZero = (value.data.as_i64 == 0); break;
                case ConstantType::_U8:  isZero = (value.data.as_u8  == 0); break;
                case ConstantType::_U16: isZero = (value.data.as_u16 == 0); break;
                case ConstantType::_U32: isZero = (value.data.as_u32 == 0); break;
                case ConstantType::_U64: isZero = (value.data.as_u64 == 0); break;
                default: isZero = true; break;
                }
            }
            if (isZero)
            {
                m_InstructionIndex = rInstruction.regB;
            }
            break;
        }
        case OpCode::_JNZ:
        {
            const ConstantContainer& value = m_Registers[rInstruction.regA];
            bool isNonZero = (value.type == ConstantType::_BOOL) ? (value.data.as_bool == true) : false;
            if (value.type != ConstantType::_BOOL)
            {
                switch (value.type)
                {
                case ConstantType::_I8:  isNonZero = (value.data.as_i8  != 0); break;
                case ConstantType::_I16: isNonZero = (value.data.as_i16 != 0); break;
                case ConstantType::_I32: isNonZero = (value.data.as_i32 != 0); break;
                case ConstantType::_I64: isNonZero = (value.data.as_i64 != 0); break;
                case ConstantType::_U8:  isNonZero = (value.data.as_u8  != 0); break;
                case ConstantType::_U16: isNonZero = (value.data.as_u16 != 0); break;
                case ConstantType::_U32: isNonZero = (value.data.as_u32 != 0); break;
                case ConstantType::_U64: isNonZero = (value.data.as_u64 != 0); break;
                default: isNonZero = false; break;
                }
            }
            if (isNonZero)
            {
                m_InstructionIndex = rInstruction.regB;
            }
            break;
        }
        case OpCode::_CALL:
        {
            FunctionInfo& functionInfo = m_pChunk->read_functions(rInstruction.regA);
            size argumentCount = rInstruction.regB;
            size totalPushed = argumentCount + 1;
            size returnValueSlot = m_StackIndex - totalPushed;
            if (m_Stack.size() < returnValueSlot + functionInfo.frame_size)
            {
                m_Stack.resize(m_Stack.size() + stackAllocationSize);
            }
            for (size slotOffset = m_StackIndex; slotOffset < returnValueSlot + functionInfo.frame_size; slotOffset++)
            {
                m_Stack[slotOffset] = ConstantContainer{};
            }
            m_OldInstructionIndex.push_back(m_InstructionIndex);
            if (m_Frames.size() <= m_FramesIndex + 1)
            {
                m_Frames.resize(m_Frames.size() + framesAllocationSize);
            }
            m_FramesIndex++;
            m_Frames[m_FramesIndex] = { returnValueSlot, functionInfo.frame_size };
            m_StackIndex = returnValueSlot + functionInfo.frame_size;
            m_InstructionIndex = functionInfo.entry;
            break;
        }
        case OpCode::_CALL_NATIVE:
        {
            InvokeNative(static_cast<u8>(rInstruction.regA), static_cast<u8>(rInstruction.regB));
            break;
        }
        case OpCode::_RET:
        {
            m_InstructionIndex = m_OldInstructionIndex.back();
            m_OldInstructionIndex.pop_back();
            StackFrame& currentFrame = m_Frames[m_FramesIndex];
            ConstantContainer returnValue = m_Stack[currentFrame.beginIndex];
            m_StackIndex = currentFrame.beginIndex;
            m_FramesIndex--;
            m_Registers[0] = returnValue;
            break;
        }
        case OpCode::_HALT:
            return;
        
        default:
            break;
        }
    }
}

void VM::InvokeNative(u8 nativeIndex, u8 argumentCount)
{
    ConstantContainer arguments[16];
    for (int argumentIndex = argumentCount - 1; argumentIndex >= 0; argumentIndex--)
    {
        m_StackIndex--;
        arguments[argumentIndex] = m_Stack[m_StackIndex];
    }
    const std::vector<BuiltinDesc>& builtins = GetBuiltins();
    InvokeNativeImpl(nativeIndex, builtins[nativeIndex].name, *this, arguments, argumentCount);
}

Instruction &VM::Read_Instruction()
{
    size_t current = m_InstructionIndex;
    m_InstructionIndex++;
    return m_pChunk->read_instruction(current);
}