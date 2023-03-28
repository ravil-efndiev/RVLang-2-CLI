#include "Lexer.hpp"

namespace rvlang
{
    Lexer::Lexer(const std::string& code)
    {
        m_Code = code;
    }

    Lexer::~Lexer()
    {
    }

    std::vector<Token> Lexer::GetTokens()
    {
        while (NextToken())
        {
            Log("found token");
        }

        return m_Tokens;
    }

    bool Lexer::NextToken()
    {
        if (m_Position >= m_Code.size())
        {
            return false;
        }

        for (auto& type : TypeList)
        {
            auto regexp = std::regex(type.second.Regex);
            auto currentString = m_Code.substr(m_Position);

            std::smatch result;

            if (std::regex_search(currentString, result, regexp)) 
            {
                Token token (type.second, result.str());
                m_Position += result.str().size();
                m_Tokens.push_back(token);
                return true;
            }
        }
        throw Error("invalid token");
    }
}
