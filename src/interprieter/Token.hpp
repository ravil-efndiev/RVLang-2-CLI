#ifndef RVLANG_TOKEN_HPP
#define RVLANG_TOKEN_HPP

#include "TokenType.hpp"

namespace rvlang
{
    struct Token
    {
        TokenType Type;
        std::string Text;

        Token(TokenType type, const std::string& text) : Type(type), Text(text) {}
    };
}

#endif
