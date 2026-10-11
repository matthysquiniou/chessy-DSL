#pragma once
#include "Expression.h"
#include "Chunk.h"
#include "PendingJump.h"

class Compiler
{
public:
Compiler();
void Compile(ProgramExpression* program); 
Chunk& GetChuck();

private:
    void CompileProgram();
    void CompileDeclaration(Expression* expr);

    void CompileExpression(Expression* expr);

    void CompileBlock(BlockExpression* block);
    void CompileReturn(ReturnExpression* ret);
    void CompileDoWhile(DoWhileExpression* doWhile);
    void CompileIf(IfExpression* ifStmt);

    void CompileFunctionDeclaration(FunctionDeclarationExpression* functionDeclaration);
    void CompileFunctionCall(FunctionCallExpression* functionCall);

    void CompileVariableDeclaration(VariableDeclarationExpression* variableDeclaration);
    void CompileVariableAssignment(VariableAssignmentExpression* variableAssignment);

    void CompileIdentifier(IdentifierExpression* identifier);
    void CompileLiteral(LiteralExpression* literal);

    void CompileBinary(BinaryExpression* binary);
    void CompileUnary(UnaryExpression* unary);

    size_t Emit(OpCode op, u8 a=0, u8 b=0, u8 c=0);
    size_t EmitJump(OpCode op, u8 reg, size_t label);

    size_t NewLabel();
    void BindLabel(size_t label, size_t pc);
    void ResolveLabels();
    int GetConstIndex(ConstantContainer& c);

    OpCode GetBinaryOp(TokenType token);

    int FindMainFunctionIndex();
private:
    vector<PendingJump> m_pendingJumps;
    vector<size_t> m_labelPc;  

    ProgramExpression* m_program = nullptr;
    un_map<str, int> m_consts;
    Chunk m_chunk;
};