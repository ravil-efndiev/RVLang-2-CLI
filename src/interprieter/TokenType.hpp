#pragma once

#include <System.hpp>

namespace Rvlang
{
    static const std::string NUMBER    = "number";
    static const std::string SPACE     = "space";
    static const std::string SEMICOLON = "semicolon";
    static const std::string ASSIGN    = "assign_operator";
    static const std::string VAR       = "var_keyword";
    static const std::string NAME      = "name";

    struct TokenType
    {
        std::string Name;
        std::string Regex;

        TokenType() : Name(""), Regex("") {}
        TokenType(const std::string& name, const std::string& regex) : Name(name), Regex(regex) {}

        bool operator==(const TokenType& type)
        {
            return type.Name == Name;
        }
    };

    static std::unordered_map<std::string, TokenType> TypeList = 
    {
        {NUMBER, TokenType(NUMBER, std::string(R"(^[0-9]+)"))},
        {SPACE, TokenType(SPACE, std::string("^\\s+"))},
        {SEMICOLON, TokenType(SEMICOLON, std::string("^\\;"))},
        {ASSIGN, TokenType(ASSIGN, std::string("^\\="))},
        {VAR, TokenType(VAR, std::string("^var"))},
        {NAME, TokenType(NAME, std::string("^[_|A-Z|a-z]+[_|A-Z|a-z|0-9]+"))},
    };

    typedef std::unordered_map<std::string, TokenType> typeList_t;
}

