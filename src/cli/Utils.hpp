#pragma once

#include "System.hpp"

namespace Rvlang
{
    class Token;

    namespace Utils
    {
        extern std::string GetTextFromFile(const std::string& path);

        extern void PrintTokens(const std::vector<Token>& tokens);
    }
}

