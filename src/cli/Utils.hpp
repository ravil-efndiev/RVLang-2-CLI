#ifndef RVLANG_UTILS_HPP
#define RVLANG_UTILS_HPP

#include "System.hpp"

namespace rvlang
{
    class Token;

    namespace utils
    {
        extern std::string GetTextFromFile(const std::string& path);

        extern void PrintTokens(const std::vector<Token>& tokens);
    }
}

#endif
