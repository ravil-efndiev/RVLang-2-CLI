#include "Lexer.hpp"

namespace Rvlang
{
    Lexer::Lexer(const std::string& code)
    {
        m_Code = code;
    }

    Lexer::~Lexer() {}

    std::vector<Token> Lexer::GetTokens()
    {
        while (m_Position < m_Code.size())
        {
            NextToken();
        }

        return m_Tokens;
    }

    void Lexer::NextToken()
    {
        for (auto& type : typeList.Get())
        {
            auto regexp = std::regex(type.Regex);
            auto currentString = m_Code.substr(m_Position);

            std::smatch result; 

            if (std::regex_search(currentString, result, regexp)) 
            {
                Token token (type, result.str());
                m_Position += result.str().size();
                m_Tokens.push_back(token);
            
                return;
            }
        }
        throw Error("invalid token");
    }
}
