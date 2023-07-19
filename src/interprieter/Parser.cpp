#include "Parser.hpp"

namespace Rvlang
{
    Parser::Parser(const std::vector<Token>& tokens)  : m_Tokens(tokens), m_Position(0)
    {
        m_Tokens.erase(std::remove_if(m_Tokens.begin(), m_Tokens.end(), [](const Token& token){return token.Type == TypeList[SPACE];}));
    }

    Parser::~Parser() {}

    template <class... Types>
    std::variant<Token, std::nullptr_t> Parser::Find(const Types&... types)
    {
        std::vector<TokenType> params = { { types... } };
        if (m_Position < m_Tokens.size())
        {
            Token currentToken = m_Tokens[m_Position];
            if (std::find_if(params.begin(), params.end(), [currentToken](const TokenType& type){return type.Name == currentToken.Type.Name;}) != params.end())
            {
                m_Position++;
                return currentToken;
            }
        }
        return nullptr;
    }

    template <class... Types>
    Token Parser::Require(const Types& ...types)
    {
        try
        {    
            Token token = std::get<Token>(Find(types...));
            return token;
        }
        catch (const std::bad_variant_access& ex)
        {
            throw Error("Parser => required token not found");
        }
    }

    StatementsNode Parser::Parse()
    {
        StatementsNode mainNode;

        while (m_Position < m_Tokens.size())
        {
            std::shared_ptr<Node> lineNode = ParseLine();
            Require(TypeList[SEMICOLON]);
            mainNode.AddNode(lineNode.get());
        }

        return mainNode;
    }

    std::shared_ptr<Node> Parser::ParseLine()
    {
        if (std::holds_alternative<Token>(Find(TypeList[VAR])))
        {
            auto varNode = ParseVariable();
            auto assignOperator = Require(TypeList[ASSIGN]);
            auto formulaNode = ParseFormula();

            auto assign = std::make_shared<BinaryOperationNode>(assignOperator, varNode.get(), formulaNode.get());
            return assign;
        }
    }

    std::shared_ptr<Node> Parser::ParseVariable()
    {
        Token nameToken = Require(TypeList[NAME]);
        return std::make_shared<VariableNode>(nameToken);
    }

    std::shared_ptr<Node> Parser::ParseFormula()
    {
    }
}
