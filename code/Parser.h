#pragma once
#include "Expression.h"
#include "ExpressionTrait.h"
#include "BindingPower.h"

class Parser
{
public:
	Parser();
	~Parser();
	void Parse(std::vector<Token>& tokens);
	ProgramExpression* GetProgram();
private:
	Token& Consume(TokenType type, bool jumpLineToken = true);
	Token& Peek(int indexOffset = 0, bool jumpLineToken = true);

	ProgramExpression* ParseProgram();

	Expression* ParseDeclaration();
	BlockExpression* ParseBlock();

	FunctionDeclarationExpression* ParseFunctionDeclaration();
	FunctionCallExpression* ParseFunctionCall();
	ParameterExpression* ParseParameter();

	VariableDeclarationExpression* ParseVariableDeclaration();
	VariableAssignmentExpression* ParseVariableAssignment();

	TypeExpression* ParseType();
	ReturnExpression* ParseReturn();
	DoWhileExpression* ParseDoWhile();
	IfExpression* ParseIf();
	CommentExpression* ParseComment();

	Expression* ParseExpression(int minLeftBindingPower = 0);

	Expression* ParsePrefix();
	BindingPower GetBindingPower(Token& token);
private:
	std::vector<Token> m_tokens;
	size_t m_tokenIndex = 0;
	ProgramExpression* m_program = nullptr;

	static un_set<TokenType> s_typeTokens;
	static un_set<TokenType> s_lineTokens;
};