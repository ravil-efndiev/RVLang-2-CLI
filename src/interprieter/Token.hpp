#pragma once

#include "TokenType.hpp"

namespace Rvlang
{
    struct Token
    {
        TokenType Type;
        std::string Text;

        Token() : Type(), Text("") {}
        Token(TokenType type, const std::string& text) : Type(type), Text(text) {}
    };
}

