#pragma once

#include <interprieter/Token.hpp>

namespace Rvlang
{
    enum class NodeType 
    {
        Statements, Variable, Number, BinaryOperation, FunctionCall, String, Null
    };

    class Node 
    {
    public:
        virtual NodeType GetType() = 0;
    };

    struct StatementsNode final : public Node
    {
        std::vector<Ptr<Node>> Lines;

        StatementsNode() {}
        
        NodeType GetType() override {return NodeType::Statements;}

        void AddNode(const Ptr<Node>& node)
        {
            Lines.push_back(node);
        }
    };

    struct VariableNode final : public Node
    {
        Token Name;

        VariableNode(const Token& name) : Name(name) {}
        NodeType GetType() override {return NodeType::Variable;}
    };

    struct ValueNode final : public Node
    {
        Token Value;
        NodeType ValueType; // only Number, String, Null are valid

        ValueNode(const Token& value, NodeType type) : Value(value) 
        {
            if (type != NodeType::Number && type != NodeType::String && type != NodeType::Null)
                throw Error("Value node can only have value types");
            ValueType = type;    
        }

        NodeType GetType() override {return ValueType;}
    };
    
    struct BinaryOperationNode final : public Node
    {
        Token Operator;
        Ptr<Node> LeftNode;   
        Ptr<Node> RightNode;

        BinaryOperationNode(const Token& op, const Ptr<Node>& left, const Ptr<Node>& right)
            : Operator(op), LeftNode(left), RightNode(right) {}  

        NodeType GetType() override {return NodeType::BinaryOperation;}
        
    };

    struct FunctionCallNode final : public Node
    {
        Token FunctionName;
        std::vector<Ptr<Node>> Arguments;

        FunctionCallNode(const Token& functionName, const std::vector<Ptr<Node>>& args) 
            : FunctionName(functionName), Arguments(args) {}

        NodeType GetType() override {return NodeType::FunctionCall;}
    };
}

