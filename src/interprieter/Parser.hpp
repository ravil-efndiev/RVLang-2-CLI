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

        Ptr<StatementsNode> Parse();


    private:
        std::vector<Token> m_Tokens;
        int m_Position;

        template <class ... Types>
        std::optional<Token> Find(const Types&... types);

        template <class ... Types>
        Token Require(const Types&... types);

        Ptr<Node>                ParseLine();
        std::optional<Ptr<Node>> ParseUsable();
        std::optional<Ptr<Node>> ParseExpression();
        std::optional<Ptr<Node>> ParseParenthases();
        std::vector<Ptr<Node>>   ParseMultipleValues();
        Ptr<StatementsNode>      ParseCodeBlock();
    };
}

