#pragma once
#include "Defines.h"
#include "ConstantContainer.h"
#include "Board.h"
#include "Move.h"

class VM;

void InvokeNativeImpl(u8 nativeIndex, const char* name, VM& vm, const ConstantContainer* arguments, u8 argumentCount);