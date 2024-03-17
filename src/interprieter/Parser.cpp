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

    Token Parser::Require(const TokenType& type)
    {
        try
        {    
            Token token = *Find(type);
            return token;
        }
        catch (const std::bad_variant_access& ex)
        {
            LogError("Parser => required token not found");
            exit(RVLANG_ERROR);
        }
    }

    std::shared_ptr<StatementsNode> Parser::Parse()
    {
        std::shared_ptr<StatementsNode> mainNode = std::make_shared<StatementsNode>();

        while (m_Position < m_Tokens.size())
        {
            std::shared_ptr<Node> lineNode = ParseLine();
            mainNode->AddNode(lineNode);
        }

        return mainNode;
    }

    std::shared_ptr<Node> Parser::ParseLine()
    {
        // variable declaration with type deduction
        auto varUntyped = ParseVariableDeclUntyped();
        if (varUntyped)
            return varUntyped;

        // variable declaration
        auto varTyped = ParseVariableDeclTyped();
        if (varTyped)
            return varTyped;

        // line starts with usable name
        auto startName = Find(typeList[NAME]);
        if (startName)
        {
            Token name = *startName;
            
            // name is function call
            if ((Find(typeList[LPAR])))
            {
                auto args = ParseMultipleValues();
                auto functionCall = std::make_shared<FunctionCallNode>(name.Text, args);
                Require(typeList[RPAR]);
                return functionCall;
            }
        }

        // line starts with `func` keyword
        if (Find(typeList[FUNC]))
        {
            auto name = Require(typeList[NAME]);

            std::vector<std::shared_ptr<VariableDeclNode>> args;
            if (Find(typeList[LPAR])) 
            {
                args = ParseFunctionArgs();
                Require(typeList[RPAR]);
            }

            VariableType returnType = VariableType::Void;
            if (Find(typeList[ARROW])) {
                returnType = varTypes.find(Require(typeList[TYPE]).Text)->second;
            }

            auto proto = std::make_shared<FunctionPrototypeNode>(name.Text, args, returnType);

            if (Find(typeList[LBRACE])) {
                std::shared_ptr<StatementsNode> code = ParseCodeBlock();

                auto declaration = std::make_shared<FunctionDeclNode>(proto, code);
                return declaration;
            }
            return proto;
        }
    }

    std::shared_ptr<VariableDeclNode> Parser::ParseVariableDeclTyped()
    {
        auto typeTok = Find(typeList[TYPE]);
        if (typeTok)
        {
            std::string name = Require(typeList[NAME]).Text;
            auto assignOperator = Find(typeList[ASSIGN]);
            VariableType type = varTypes.find(typeTok->Text)->second;

            if (assignOperator)
            {
                auto formulaNode = ParseExpression();

                if (!formulaNode) throw Error("No valid rvalue found during variable assignment");

                auto decl = std::make_shared<VariableDeclNode>(name, formulaNode, type);
                return decl;
            }

            auto null = std::make_shared<NullNode>();
            return std::make_shared<VariableDeclNode>(name, null, type);
        }
        return nullptr;
    }

    std::shared_ptr<VariableDeclNode> Parser::ParseVariableDeclUntyped()
    {
        if (Find(typeList[VAR]))
        {
            std::string name = Require(typeList[NAME]).Text;
            Token assignOperator = Require(typeList[ASSIGN]);

            auto formulaNode = ParseExpression();

            if (!formulaNode) throw Error("No valid rvalue found during variable assignment");

            auto decl = std::make_shared<VariableDeclNode>(name, formulaNode);
            return decl;
        }
        return nullptr;
    }
    
    std::shared_ptr<Node> Parser::ParseUsable()
    {
        auto nameToken = Find(typeList[NAME]);
        if (nameToken) return std::make_shared<VariableNode>(nameToken->Text);

        auto intToken = Find(typeList[INTEGER]);
        if (intToken) return std::make_shared<NumberNode>(*intToken, NumberType::i32);

        auto floatToken = Find(typeList[FLOAT]);
        if (floatToken) return std::make_shared<NumberNode>(*floatToken, NumberType::f32);

        auto stringToken = Find(typeList[STRING]);
        if (stringToken) return std::make_shared<StringLiteralNode>(stringToken->Text);

        auto nullToken = Find(typeList[NULL_]);
        if (nullToken) return std::make_shared<NullNode>();

        return nullptr;
    }

    std::shared_ptr<Node> Parser::ParseExpression()
    {
        auto left = ParseParenthases();

        auto divMultOp = Find(typeList[MULT], typeList[DIV]);
        while (divMultOp)
        {
            auto right = ParseParenthases();
            if (!left || !right) throw Error("No valid value found during binary operation");
            left = std::make_shared<BinaryOperationNode>(*divMultOp, left, right);
            divMultOp = Find(typeList[MULT], typeList[DIV]);
        }

        auto plusMinusOp = Find(typeList[PLUS], typeList[MINUS]);
        while (plusMinusOp)
        {
            auto right = ParseExpression();
            if (!left || !right) throw Error("No valid value found during binary operation");
            left = std::make_shared<BinaryOperationNode>(*plusMinusOp, left, right);
            plusMinusOp = Find(typeList[PLUS], typeList[MINUS]);
        }
        return left;
    }
    
    std::shared_ptr<Node> Parser::ParseParenthases()
    {
        if (Find(typeList[LPAR]))
        {
            auto expression = ParseExpression();
            Require(typeList[RPAR]);
            return expression;
        }

        return ParseUsable();
    }
    
    std::vector<std::shared_ptr<Node>> Parser::ParseMultipleValues()
    {
        auto value = ParseExpression();
        if (!value) 
            return {};

        auto op = Find(typeList[COMMA]);
        if (!op)
            return {value};
        
        std::vector<std::shared_ptr<Node>> values = {value};

        while (op)
        {
            auto next = ParseExpression();
            if (!next) throw Error("Valid value not found after ','");
            values.push_back(next);
            op = Find(typeList[COMMA]);
        }

        return values;
    }

    std::vector<std::shared_ptr<VariableDeclNode>> Parser::ParseFunctionArgs()
    {
        auto var = ParseVariableDeclTyped();
        if (!var) 
            return {};

        auto op = Find(typeList[COMMA]);
        if (!op)
            return {var};
        
        std::vector<std::shared_ptr<VariableDeclNode>> vars = {var};

        while (op)
        {
            auto next = ParseVariableDeclTyped();
            if (!next) throw Error("Function argument not found after ','");
            vars.push_back(next);
            op = Find(typeList[COMMA]);
        }

        return vars;
    }

    std::shared_ptr<StatementsNode> Parser::ParseCodeBlock()
    {
        auto block = std::make_shared<StatementsNode>();

        auto rbrace = Find(typeList[RBRACE]);
        while (!rbrace)
        {
            std::shared_ptr<Node> lineNode = ParseLine();
            block->AddNode(lineNode);
        }

        return block;
    }

}
