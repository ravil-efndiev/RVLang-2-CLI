#ifndef RVLANG_TOKEN_TYPE_HPP
#define RVLANG_TOKEN_TYPE_HPP

#include <System.hpp>

namespace rvlang
{
    struct TokenType
    {
        std::string Name;
        std::string Regex;

        TokenType() : Name(""), Regex("") {}
        TokenType(const std::string& name, const std::string& regex) : Name(name), Regex(regex) {}

        bool operator==(const TokenType& type)
        {
            return type.Name == Name && type.Regex == Regex;
        }
    };

    static std::unordered_map<std::string, TokenType> TypeList = 
    {
        {"number", TokenType("number", std::string(R"(^[0-9]+)"))},
        {"space", TokenType("space", std::string("^\\s+"))},
        {"semicolon", TokenType("semicolon", std::string("^\\;"))},
        {"assign_operator", TokenType("assign_operator", std::string("^\\="))},
        {"var_keyword", TokenType("var_keyword", std::string("^var"))},
        {"name", TokenType("name", std::string("^[_|A-Z|a-z]+[_|A-Z|a-z|0-9]+"))},
    };

    typedef std::unordered_map<std::string, TokenType> typeList_t;
}

#endif
