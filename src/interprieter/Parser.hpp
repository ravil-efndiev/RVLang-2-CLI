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

        std::shared_ptr<StatementsNode> Parse();

    private:
        std::vector<Token> m_Tokens;
        int m_Position;

        template <class... Types>
        std::optional<Token> Find(const Types&... types);

        Token Require(const TokenType& type);

        std::shared_ptr<VariableDeclNode> ParseVariableDeclTyped();
        std::shared_ptr<VariableDeclNode> ParseVariableDeclUntyped();

        std::shared_ptr<Node> ParseLine();
        std::shared_ptr<Node> ParseUsable();
        std::shared_ptr<Node> ParseExpression();
        std::shared_ptr<Node> ParseParenthases();
        
        std::vector<std::shared_ptr<Node>> ParseMultipleValues();
        std::shared_ptr<StatementsNode> ParseCodeBlock();
        std::vector<std::shared_ptr<VariableDeclNode>> ParseFunctionArgs();
    };
}

