#ifndef RVLANG_TOKEN_TYPE_HPP
#define RVLANG_TOKEN_TYPE_HPP

#include <System.hpp>

namespace rvlang
{
    struct TokenType
    {
        std::string Name;
        std::string Regex;

        TokenType(const std::string& name, const std::string& regex) : Name(name), Regex(regex) {}
    };

    static std::unordered_map<std::string, TokenType> TypeList = 
    {
        {"number", TokenType("number", std::string(R"(^[0-9]+)"))},
        {"space", TokenType("space", std::string("^\\s+"))},
    };

    typedef std::unordered_map<std::string, TokenType> typeList_t;
}

#endif
