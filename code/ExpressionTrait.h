#pragma once
#include "ExpressionType.h"
#include "Expression.h"

template <ExpressionType K> struct ExpressionTraits;
template <> struct ExpressionTraits<ExpressionType::_PROGRAM>             { using type = ProgramExpression; };
template <> struct ExpressionTraits<ExpressionType::_UNARY>               { using type = UnaryExpression; };
template <> struct ExpressionTraits<ExpressionType::_BINARY>              { using type = BinaryExpression; };
template <> struct ExpressionTraits<ExpressionType::_BLOCK>               { using type = BlockExpression; };
template <> struct ExpressionTraits<ExpressionType::_TYPE>                { using type = TypeExpression; };
template <> struct ExpressionTraits<ExpressionType::_LITERAL>             { using type = LiteralExpression; };
template <> struct ExpressionTraits<ExpressionType::_IDENTIFIER>          { using type = IdentifierExpression; };
template <> struct ExpressionTraits<ExpressionType::_FUNCTION_DECLARATION>{ using type = FunctionDeclarationExpression; };
template <> struct ExpressionTraits<ExpressionType::_FUNCTION_CALL>       { using type = FunctionCallExpression; };
template <> struct ExpressionTraits<ExpressionType::_PARAMETER>           { using type = ParameterExpression; };
template <> struct ExpressionTraits<ExpressionType::_VARIABLE_DECLARATION>{ using type = VariableDeclarationExpression; };
template <> struct ExpressionTraits<ExpressionType::_RETURN>              { using type = ReturnExpression; };
template <> struct ExpressionTraits<ExpressionType::_DO_WHILE>            { using type = DoWhileExpression; };
template <> struct ExpressionTraits<ExpressionType::_VARIABLE_ASSIGNMENT> { using type = VariableAssignmentExpression; };
template <> struct ExpressionTraits<ExpressionType::_IF>                  { using type = IfExpression; };
template <> struct ExpressionTraits<ExpressionType::_COMMENT>             { using type = CommentExpression; };

template <ExpressionType K>
typename ExpressionTraits<K>::type* ExpressionCast(Expression* e) {
    if (!e || e->type != K) return nullptr;
    return static_cast<typename ExpressionTraits<K>::type*>(e);
}