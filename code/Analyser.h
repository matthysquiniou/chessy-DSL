#pragma once
#include "Defines.h"
#include "Expression.h"
#include "AnalyserError.h"

class Scope;

class Analyser
{
public:
    Analyser(ProgramExpression* program);
    void Analyse();
    std::vector<AnalyserError> GetErrors();
    bool HasErrors();
    void PrintErrors();

private:
    void GlobalAnalyse();
    void CollectGlobals();
    void CheckMain();
    void AnalyseBodies();
    void AddBuiltin();

    TokenType AnalyseExpression(Expression* expr);

    void BlockAnalyse(BlockExpression* block);
    void ReturnAnalyse(ReturnExpression* ret);
    void DoWhileAnalyse(DoWhileExpression* doWhile);
    void IfAnalyse(IfExpression* ifStmt);

    void FunctionAnalyse(FunctionDeclarationExpression* func);
    TokenType ParameterAnalyse(ParameterExpression* param);
    TokenType FunctionCallAnalyse(FunctionCallExpression* call);

    void VarDeclarationAnalyse(VariableDeclarationExpression* varDec);
    void VarAssignmentAnalyse(VariableAssignmentExpression* varAssign);

    TokenType IdentifierAnalyse(IdentifierExpression* id);
    void TypeAnalyse(TypeExpression* type);
    TokenType LiteralAnalyse(LiteralExpression* literal);

    TokenType UnaryAnalyse(UnaryExpression* unary);
    TokenType BinaryAnalyse(BinaryExpression* binary);

    std::pair<bool,TokenType> TypeInference(TokenType typeA, TokenType typeB);

    void CommentAnalyse(CommentExpression* comment);

    void AddScope(FunctionDeclarationExpression* func = nullptr);
    void PopScope(bool popFunc = false);

    bool IsAssignable(TokenType fromType, TokenType toType);

private:
    std::vector<Scope*> m_scopes;
    std::vector<AnalyserError> m_errors;
    ProgramExpression* m_program = nullptr;
    FunctionDeclarationExpression* m_currentFunc = nullptr;
    int m_nextSlot = 0;
    std::vector<int> m_previousNextSlot;
    std::vector<FunctionDeclarationExpression*> m_previousFunc;
};