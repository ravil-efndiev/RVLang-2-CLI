#pragma once
#include "Token.hpp"
#include "DataTypes.hpp"

namespace Rvlang
{
    class Node 
    {
    public:
        virtual NodeType GetType() = 0;
    };

    struct StatementsNode final : public Node
    {
        std::vector<std::shared_ptr<Node>> Lines;

        NodeType GetType() override {return NodeType::Statements;}

        void AddNode(const std::shared_ptr<Node>& node)
        {
            Lines.push_back(node);
        }
    };
    
    struct VariableNode final : public Node
    {
        std::string Name;

        VariableNode(const std::string& name) : Name(name) {}
        NodeType GetType() override {return NodeType::Variable;}
    };

    struct VariableDeclNode final : public Node
    {
        std::string Name;
        std::shared_ptr<Node> InitialValue;
        VariableType Type;

        VariableDeclNode(const std::string& name, const std::shared_ptr<Node>& initialValue, VariableType type = VariableType::DeductLater) 
            : Name(name), InitialValue(initialValue), Type(type) {}
        NodeType GetType() override {return NodeType::Variable;}
    };

    struct NumberNode final : public Node
    {
        Token Number;
        NumberType ValueType;

        NumberNode(const Token& number, NumberType type) : Number(number), ValueType(type) {}

        NodeType GetType() override {return NodeType::Number;}
    };

    struct StringLiteralNode final : public Node
    {
        std::string String;

        StringLiteralNode(const std::string& str) : String(str) {}

        NodeType GetType() override {return NodeType::String;}
    };

    struct NullNode final : public Node
    {
        const std::string NullStringValue = "null";
        NodeType GetType() override {return NodeType::Null;}
    };
    
    struct BinaryOperationNode final : public Node
    {
        Token Operator;
        std::shared_ptr<Node> LeftNode;   
        std::shared_ptr<Node> RightNode;

        BinaryOperationNode(const Token& op, const std::shared_ptr<Node>& left, const std::shared_ptr<Node>& right)
            : Operator(op), LeftNode(left), RightNode(right) {}  

        NodeType GetType() override {return NodeType::BinaryOperation;}
    };

    struct FunctionCallNode final : public Node
    {
        std::string FunctionName;
        std::vector<std::shared_ptr<Node>> Arguments;

        FunctionCallNode(const std::string& functionName, const std::vector<std::shared_ptr<Node>>& args) 
            : FunctionName(functionName), Arguments(args) {}

        NodeType GetType() override {return NodeType::FunctionCall;}
    };

    struct FunctionPrototypeNode final : public Node
    {
        std::string FunctionName;
        VariableType ReturnType;
        std::vector<std::shared_ptr<VariableDeclNode>> Arguments;

        FunctionPrototypeNode(const std::string& name, std::vector<std::shared_ptr<VariableDeclNode>> args, VariableType type)
            : FunctionName(name), Arguments(std::move(args)), ReturnType(type) {}

        NodeType GetType() override {return NodeType::FunctionProto;}
    };

    struct FunctionDeclNode final : public Node
    {
        std::shared_ptr<StatementsNode> Code;
        std::shared_ptr<FunctionPrototypeNode> Proto;

        FunctionDeclNode(const std::shared_ptr<FunctionPrototypeNode>& proto, const std::shared_ptr<StatementsNode>& code)
            : Proto(proto), Code(code) {}

        NodeType GetType() override {return NodeType::FunctionDecl;}
    };
}

