#pragma once
#include "Defines.h"

enum class OpCode : u8
{
    _MOV,      
    _LOADC,    
    _LOAD,  
    _LOADG,    
    _STORE,   
    _STOREG,   
    _PUSH,      
    _POP,        

    _ADD, _SUB, _MUL, _DIV, _MOD, _POW,
    _NEG,

    _AND, _OR, _XOR, _NOT,
    _SHL, _SHR,

    _CMP_EQ, _CMP_NE, _CMP_LT, _CMP_LE, _CMP_GT, _CMP_GE,

    _JMP,       
    _JZ,         
    _JNZ,       


    _CALL,  
    _CALL_NATIVE,     
    _RET,      

    _HALT
};