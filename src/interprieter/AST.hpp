#pragma once
#include <interprieter/Token.hpp>

namespace Rvlang
{
    enum class NodeType 
    {
        Statements,
        Variable,
        Number, 
        BinaryOperation, 
        FunctionCall, 
        FunctionProto, 
        FunctionDecl, 
        String, 
        Null
    };

    enum class NumberType
    {
        u8, u16, u32, u64,
        i8, i16, i32, i64,
        f32, f64
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
        std::string Name;

        VariableNode(const std::string& name) : Name(name) {}
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
        Ptr<Node> LeftNode;   
        Ptr<Node> RightNode;

        BinaryOperationNode(const Token& op, const Ptr<Node>& left, const Ptr<Node>& right)
            : Operator(op), LeftNode(left), RightNode(right) {}  

        NodeType GetType() override {return NodeType::BinaryOperation;}
        
    };

    struct FunctionCallNode final : public Node
    {
        std::string FunctionName;
        std::vector<Ptr<Node>> Arguments;

        FunctionCallNode(const std::string& functionName, const std::vector<Ptr<Node>>& args) 
            : FunctionName(functionName), Arguments(args) {}

        NodeType GetType() override {return NodeType::FunctionCall;}
    };

    struct FunctionPrototypeNode final : public Node
    {
        std::string FunctionName;
        std::vector<Ptr<Node>> Arguments;

        FunctionPrototypeNode(const std::string& name, std::vector<Ptr<Node>> args)
            : FunctionName(name), Arguments(std::move(args)) {}

        NodeType GetType() override {return NodeType::FunctionProto;}
    };

    struct FunctionDeclNode final : public Node
    {
        Ptr<StatementsNode> Code;
        Ptr<FunctionPrototypeNode> Proto;

        FunctionDeclNode(const Ptr<FunctionPrototypeNode>& proto, const Ptr<StatementsNode>& code)
            : Proto(proto), Code(code) {}

        NodeType GetType() override {return NodeType::FunctionDecl;}
    };
}

