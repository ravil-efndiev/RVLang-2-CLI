#pragma once

#include "Token.hpp"

namespace Rvlang
{
    class Lexer
    {
    public:
        Lexer(const std::string& code);
        ~Lexer();

        std::vector<Token> GetTokens();

    private:
        std::string m_Code;
        std::vector<Token> m_Tokens;
        int m_Position = 0;

        void NextToken();

    };
}

