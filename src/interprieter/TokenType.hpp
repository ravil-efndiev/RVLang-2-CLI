#pragma once

#include <System.hpp>

namespace Rvlang
{
    enum TokenTypeName
    {
        UNDEFINED = 0,
        NAME,      
        INTEGER, FLOAT, STRING,
        SPACE,
        COMMA,
        ASSIGN,
        VAR, FUNC,
        LPAR, RPAR,     
        LBRACE, RBRACE,
        PLUS, MINUS, MULT, DIV,  
        NULL_,
    };

    enum class TokenTypeCategory
    {
        Any, MathOperator, LogicalOperator
    };

    struct TokenType
    {
        TokenTypeName Name;
        std::string Regex;
        TokenTypeCategory Category;

        TokenType() : Name(UNDEFINED), Regex("") {}
        TokenType(TokenTypeName name, const std::string& regex, TokenTypeCategory category = TokenTypeCategory::Any) 
            : Name(name), Regex(regex), Category(category) {}

        bool operator==(const TokenType& type) const
        {
            return type.Name == Name;
        }
    };

    class TypeList
    {
    private:
        std::vector<TokenType> m_TypeList;

    public:
        TypeList()
        {
            m_TypeList = {
                TokenType(INTEGER, R"(^-?\b[0-9]+\b)"),
                TokenType(FLOAT, R"(^-?\b[0-9]+\.[0-9]+\b)"),
                TokenType(SPACE, ("^\\s+")),
                TokenType(ASSIGN, ("^\\=")),
                TokenType(VAR, ("^var")),
                TokenType(FUNC, "^func"),
                TokenType(NULL_, "^Null"),
                TokenType(NAME, ("^[_|A-Z|a-z]+[_|A-Z|a-z|0-9]+")),
                TokenType(LPAR, ("^\\(")),
                TokenType(RPAR, ("^\\)")),
                TokenType(LBRACE, ("^\\{")),
                TokenType(RBRACE, ("^\\}")),
                TokenType(PLUS, ("^\\+"), TokenTypeCategory::MathOperator),
                TokenType(MINUS, ("^\\-"), TokenTypeCategory::MathOperator),
                TokenType(MULT, ("^\\*"), TokenTypeCategory::MathOperator),
                TokenType(DIV, ("^\\/"), TokenTypeCategory::MathOperator),
                TokenType(COMMA, ("^,")),
                TokenType(STRING, (R"(^(["'])(?:(?=(\\?))\2.)*?\1)")),
            };
        }

        std::vector<TokenType> Get() const {return m_TypeList;}

        TokenType operator[](TokenTypeName type) const
        {
            auto it = std::find_if(m_TypeList.begin(), m_TypeList.end(), [&](auto p) {return p.Name == type;});
            return it != m_TypeList.end() ? *it : TokenType();
        }

    };

    const TypeList typeList;
}
