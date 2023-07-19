#ifndef RVLANG_AST
#define RVLANG_AST

#include <interprieter/Token.hpp>

namespace rvlang
{
    struct Node {};

    struct StatementsNode : public Node
    {
        std::vector<Node*> Lines;

        void AddNode(Node* node)
        {
            Lines.push_back(node);
        }
    };

    struct VariableNode : public Node
    {
        Token Name;

        VariableNode(const Token& name) : Name(name) {}
    };

    struct NumberNode : public Node
    {
        Token Number;

        NumberNode(const Token& number) : Number(number) {}
    };

    struct BinaryOperationNode : public Node
    {
        Token Operator;
        Node* LeftNode;   
        Node* RightNode;

        BinaryOperationNode(const Token& op, Node* left, Node* right)
            : Operator(op), LeftNode(left), RightNode(right) {}  
    };
}

#endif
