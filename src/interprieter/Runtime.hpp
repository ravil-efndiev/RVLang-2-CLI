#pragma once
#include "AST.hpp"
#include "TokenType.hpp"
#include "cli/Utils.hpp"

namespace Rvlang
{
    void StandartPrint(const std::vector<std::string>& args, char sep = ' ', char end = '\n');

    struct Null {};

    typedef std::variant<int, float, std::string, Null> VariableValue;

    enum class VariableType
    {
        Int, Float, String, Null
    };

    struct Variable
    {
        VariableType Type;
        VariableValue Value;
    };

    class Runtime
    {
    public:
        static std::any Run(const Ptr<Node>& node);

        static const std::array<std::string, 1> StandartFunctions;

    private:
        static std::unordered_map<std::string, Variable> s_Variables;
    };
}

