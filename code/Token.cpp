#include "Token.h"

void Token::Print()
{
    std::cout << line << ":" << column << " => ";
    switch (globalType)
    {
    case GlobalTokenType::_LITTERAL:
        std::cout << "_LITTERAL";
        break;
    case GlobalTokenType::_IDENTIFIER:
        std::cout << "_IDENTIFIER";
        break;
    case GlobalTokenType::_OPERAND:
        std::cout << "_OPERAND";
        break;
    case GlobalTokenType::_KEYWORD:
        std::cout << "_KEYWORD";
        break;
    case GlobalTokenType::_DELIMITOR:
        std::cout << "_DELIMITOR";
        break;
    }  
    std::cout << " / " << (int)type << ": " << view << std::endl;
}