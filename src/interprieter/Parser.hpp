#pragma once

#include "AST.hpp"
#include "TokenType.hpp"

namespace Rvlang
{
    class Parser
    {
    public:
        Parser(const std::vector<Token>& tokens);
        ~Parser();

        StatementsNode Parse();


    private:
        std::vector<Token> m_Tokens;
        int m_Position;

        template <class ... Types>
        std::variant<Token, std::nullptr_t> Find(const Types&... types);

        template <class ... Types>
        Token Require(const Types&... types);

        std::shared_ptr<Node> ParseLine();
        std::shared_ptr<Node> ParseVariable();
        std::shared_ptr<Node> ParseFormula();
    };
}

