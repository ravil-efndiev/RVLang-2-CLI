#pragma once

#include <System.hpp>

namespace Rvlang
{
    static const std::string INTEGER   = "integer";
    static const std::string FLOAT     = "float";
    static const std::string SPACE     = "space";
    static const std::string SEMICOLON = "semicolon";
    static const std::string ASSIGN    = "assign_operator";
    static const std::string VAR       = "var_keyword";
    static const std::string NAME      = "name";
    static const std::string LPAR      = "left_par";
    static const std::string RPAR      = "right_par";
    static const std::string PLUS      = "plus";
    static const std::string MINUS     = "minus";
    static const std::string MULT      = "multiply";
    static const std::string DIV       = "divide";
    static const std::string COMMA     = "comma";
    static const std::string STRING    = "string";

    enum class TokenTypeCategory
    {
        Any, MathOperator, LogicalOperator
    };

    struct TokenType
    {
        std::string Name;
        std::string Regex;
        TokenTypeCategory Category;

        TokenType() : Name(""), Regex("") {}
        TokenType(const std::string& name, const std::string& regex, TokenTypeCategory category = TokenTypeCategory::Any) 
            : Name(name), Regex(regex), Category(category) {}

        bool operator==(const TokenType& type) const
        {
            return type.Name == Name;
        }
    };

    typedef std::vector<std::pair<std::string, TokenType>> typeList_t;

    class TypeList
    {
    private:
        typeList_t m_TypeList;

    public:
        TypeList()
        {
            m_TypeList = {
                {INTEGER, TokenType(INTEGER, R"(^-?\b[0-9]+\b)")},
                {FLOAT, TokenType(FLOAT, R"(^-?\b[0-9]+\.[0-9]+\b)")},
                {SPACE, TokenType(SPACE, ("^\\s+"))},
                {SEMICOLON, TokenType(SEMICOLON, ("^;"))},
                {ASSIGN, TokenType(ASSIGN, ("^\\="))},
                {VAR, TokenType(VAR, ("^var"))},
                {NAME, TokenType(NAME, ("^[_|A-Z|a-z]+[_|A-Z|a-z|0-9]+"))},
                {LPAR, TokenType(LPAR, ("^\\("))},
                {RPAR, TokenType(RPAR, ("^\\)"))},
                {PLUS, TokenType(PLUS, ("^\\+"), TokenTypeCategory::MathOperator)},
                {MINUS, TokenType(MINUS, ("^\\-"), TokenTypeCategory::MathOperator)},
                {MULT, TokenType(MULT, ("^\\*"), TokenTypeCategory::MathOperator)},
                {DIV, TokenType(DIV, ("^\\/"), TokenTypeCategory::MathOperator)},
                {COMMA, TokenType(COMMA, ("^,"))},
                {STRING, TokenType(STRING, (R"(^(["'])(?:(?=(\\?))\2.)*?\1)"))},
            };
        }

        typeList_t Get() const {return m_TypeList;}

        TokenType operator[](const std::string& type) const
        {
            auto it = std::find_if(m_TypeList.begin(), m_TypeList.end(), [&](auto p) {return p.first == type;});
            return it != m_TypeList.end() ? it->second : TokenType();
        }

    };

    const TypeList typeList;
}
