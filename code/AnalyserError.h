#pragma once
#include "Defines.h"
#include "Expression.h"

struct AnalyserError
{
public:
    str m_message;
    Expression* m_expr = nullptr;
    void Print()
    {
        if (m_expr == nullptr)
            return;
        std::cout << "line : " << std::to_string(m_expr->line) << ", column : " << std::to_string(m_expr->line) << ", error : " << m_message << std::endl;
    }
};