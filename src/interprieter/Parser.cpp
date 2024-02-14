#include "Parser.hpp"
#include "cli/Utils.hpp"

namespace Rvlang
{
    Parser::Parser(const std::vector<Token>& tokens)  : m_Tokens(tokens), m_Position(0)
    {
        m_Tokens.erase(std::remove_if(m_Tokens.begin(), m_Tokens.end(), [](const Token& token){return token.Type == typeList[SPACE];}), m_Tokens.end());
    }

    Parser::~Parser() {}

    template <class... Types>
    std::optional<Token> Parser::Find(const Types&... types)
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
        return std::nullopt;
    }

    template <class... Types>
    Token Parser::Require(const Types& ...types)
    {
        try
        {    
            Token token = *Find(types...);
            return token;
        }
        catch (const std::bad_variant_access& ex)
        {
            throw Error("Parser => required token not found");
        }
    }

    Ptr<StatementsNode> Parser::Parse()
    {
        Ptr<StatementsNode> mainNode = New<StatementsNode>();

        while (m_Position < m_Tokens.size())
        {
            Ptr<Node> lineNode = ParseLine();
            mainNode->AddNode(lineNode);
        }

        return mainNode;
    }

    Ptr<Node> Parser::ParseLine()
    {
        // variable declaration
        if (Find(typeList[VAR]))
        {
            auto varNode = New<VariableNode>(Require(typeList[NAME]));
            auto assignOperator = Find(typeList[ASSIGN]);

            if (assignOperator)
            {
                auto formulaNode = ParseExpression();

                if (!formulaNode) throw Error("No valid rvalue found during variable assignment");

                auto assign = New<BinaryOperationNode>(*assignOperator, varNode, *formulaNode);
                return assign;
            }

            auto null = New<ValueNode>(Token(), NodeType::Null);
            return New<BinaryOperationNode>(Token(typeList[ASSIGN], "="), varNode, null);
        }

        // line starts with usable name
        auto startName = Find(typeList[NAME]);
        if (startName)
        {
            Token name = *startName;
            
            // name is function call
            if ((Find(typeList[LPAR])))
            {
                auto args = ParseMultipleValues();
                auto functionCall = New<FunctionCallNode>(name, args);
                Require(typeList[RPAR]);
                return functionCall;
            }
        }
    }

    std::optional<Ptr<Node>> Parser::ParseUsable()
    {
        auto nameToken = Find(typeList[NAME]);
        if (nameToken) return New<VariableNode>(*nameToken);

        auto numberToken = Find(typeList[INTEGER], typeList[FLOAT]);
        if (numberToken) return New<ValueNode>(*numberToken, NodeType::Number);

        auto stringToken = Find(typeList[STRING]);
        if (stringToken) return New<ValueNode>(*stringToken, NodeType::String);

        return std::nullopt;
    }

    std::optional<Ptr<Node>> Parser::ParseExpression(std::optional<Ptr<Node>> start)
    {
        auto left = ParseParenthases();

        auto divMultOp = Find(typeList[MULT], typeList[DIV]);
        while (divMultOp)
        {
            auto right = ParseParenthases();
            if (!left || !right) throw Error("No valid value found during binary operation");
            left = New<BinaryOperationNode>(*divMultOp, *left, *right);
            divMultOp = Find(typeList[MULT], typeList[DIV]);
        }

        auto plusMinusOp = Find(typeList[PLUS], typeList[MINUS]);
        while (plusMinusOp)
        {
            auto right = ParseExpression();
            if (!left || !right) throw Error("No valid value found during binary operation");
            left = New<BinaryOperationNode>(*plusMinusOp, *left, *right);
            plusMinusOp = Find(typeList[PLUS], typeList[MINUS]);
        }
        return left;
    }
    
    std::optional<Ptr<Node>> Parser::ParseParenthases()
    {
        if (Find(typeList[LPAR]))
        {
            auto expression = ParseExpression();
            Require(typeList[RPAR]);
            return expression;
        }

        return ParseUsable();
    }
    
    std::vector<Ptr<Node>> Parser::ParseMultipleValues()
    {
        auto value = ParseExpression();
        if (!value) 
            return {};

        auto op = Find(typeList[COMMA]);
        if (!op)
            return {*value};
        
        std::vector<Ptr<Node>> values = {*value};

        while ((op))
        {
            auto next = ParseExpression();
            if (!next) throw Error("Valid value not found after ','");
            values.push_back(*next);
            op = Find(typeList[COMMA]);
        }

        return values;
    }
}
