#include "Runtime.hpp"

namespace Rvlang
{

    const std::array<std::string, 1> Runtime::StandartFunctions = 
    {
        "print"
    };

    std::unordered_map<std::string, Variable> Runtime::s_Variables;

    std::any Runtime::Run(const Ptr<Node>& node)
    {
        if (node->GetType() == NodeType::Statements)
        {
            auto root = Utils::As<StatementsNode>(node);
            for (auto line : root->Lines)
            {
                Run(line);
            }
        }   

        if (node->GetType() == NodeType::Number)
        {
            auto num = Utils::As<ValueNode>(node);

            if (num->Value.Type.Name == INTEGER)
                return std::stoi(num->Value.Text); 

            if (num->Value.Type.Name == FLOAT)
                return std::stof(num->Value.Text);

            throw Error("impossible error: number isnt int or float");
        }

        if (node->GetType() == NodeType::String)
        {
            auto string = Utils::As<ValueNode>(node);
            auto str = string->Value.Text.substr(1, string->Value.Text.size() - 2);
            return str; 
        }

        if (node->GetType() == NodeType::Null)
        {
            return Null();
        }

        if (node->GetType() == NodeType::Variable)
        {
            auto name = Utils::As<VariableNode>(node)->Name.Text;
            auto it = s_Variables.find(name);
            if (it != s_Variables.end())
            {
                auto value = it->second.Value;
                if (std::holds_alternative<int>(value))
                    return std::get<int>(value);

                if (std::holds_alternative<float>(value))
                    return std::get<float>(value);

                if (std::holds_alternative<std::string>(value))
                    return std::get<std::string>(value);
            }
            throw Error("variable with name '" + name + "' is undefined");
        }

        if (node->GetType() == NodeType::BinaryOperation)
        {
            auto bin = Utils::As<BinaryOperationNode>(node);
            if (bin->Operator.Type == typeList[ASSIGN])
            {
                auto left  = Utils::As<VariableNode>(bin->LeftNode);
                auto right = Run(bin->RightNode);
                
                if (right.type() == typeid(int))
                    s_Variables[left->Name.Text] = {VariableType::Int, std::any_cast<int>(right)};

                if (right.type() == typeid(float))
                    s_Variables[left->Name.Text] = {VariableType::Float, std::any_cast<float>(right)};

                if (right.type() == typeid(std::string))
                    s_Variables[left->Name.Text] = {VariableType::String, std::any_cast<std::string>(right)};

                if (right.type() == typeid(Null))
                    s_Variables[left->Name.Text] = {VariableType::Null, std::any_cast<Null>(right)};
            }

            if (bin->Operator.Type.Category == TokenTypeCategory::MathOperator)
            {
                auto left  = Run(bin->LeftNode);
                auto right = Run(bin->RightNode);

                if (left.type() == typeid(int) && right.type() == typeid(int))
                {
                    if (bin->Operator.Type.Name == PLUS)
                        return std::any_cast<int>(left) + std::any_cast<int>(right);

                    else if (bin->Operator.Type.Name == MINUS)
                        return std::any_cast<int>(left) - std::any_cast<int>(right);

                    else if (bin->Operator.Type.Name == MULT)
                        return std::any_cast<int>(left) * std::any_cast<int>(right);

                    else if (bin->Operator.Type.Name == DIV)
                        return std::any_cast<int>(left) / std::any_cast<int>(right);
                }

                if (bin->Operator.Type.Name == PLUS)
                    return std::any_cast<float>(left) + std::any_cast<float>(right);

                else if (bin->Operator.Type.Name == MINUS)
                    return std::any_cast<float>(left) - std::any_cast<float>(right);

                else if (bin->Operator.Type.Name == MULT)
                    return std::any_cast<float>(left) * std::any_cast<float>(right);

                else if (bin->Operator.Type.Name == DIV)
                    return std::any_cast<float>(left) / std::any_cast<float>(right);

                throw Error("impossible error: math operator doesnt exist");
            }
        }

        if (node->GetType() == NodeType::FunctionCall)
        {
            auto func = Utils::As<FunctionCallNode>(node);
            auto it = std::find(StandartFunctions.begin(), StandartFunctions.end(), func->FunctionName.Text);
            if (it != StandartFunctions.end())
            {
                if (*it == "print")
                {
                    std::vector<std::string> strs;
                    for (auto arg : func->Arguments)
                    {
                        auto result = Run(arg);
                        if (result.type() == typeid(std::string))
                            strs.push_back(std::any_cast<std::string>(result));
                        else if (result.type() == typeid(int))
                            strs.push_back(std::to_string(std::any_cast<int>(result)));
                        else if (result.type() == typeid(float))
                            strs.push_back(std::to_string(std::any_cast<float>(result)));
                    }
                    StandartPrint(strs);
                    return 0;
                }
            }
        }

        return 0;
    }

    void StandartPrint(const std::vector<std::string>& args, char sep, char end)
    {
        if (args.size() == 1)
        {
            std::cout << args[0] << end;
            return;
        }

        for (auto str : args)
        {
            std::cout << str << sep;
        }
        std::cout << end;
    }
}
