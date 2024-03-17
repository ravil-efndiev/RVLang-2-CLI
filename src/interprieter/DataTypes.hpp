#include "System.hpp"

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

    enum class VariableType 
    {
        i32, f32, String, Void, DeductLater,
    };

    static const std::unordered_map<std::string, VariableType> varTypes 
    {
        {"i32", VariableType::i32},
        {"f32", VariableType::f32},
        {"string", VariableType::String},
        {"void", VariableType::Void},
    };
}
