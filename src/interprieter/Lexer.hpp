#ifndef RVLANG_LEXER_HPP
#define RVLANG_LEXER_HPP

#include "Token.hpp"

namespace rvlang
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

        bool NextToken();

    };
}

#endif
