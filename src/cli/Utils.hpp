#pragma once

#include "System.hpp"

namespace Rvlang
{
    class Token;

    namespace Utils
    {
        extern std::string GetTextFromFile(const std::string& path);

        extern void PrintTokens(const std::vector<Token>& tokens);

        template<class Base, class T>
        std::shared_ptr<Base> As(const std::shared_ptr<T>& object)
        {
            return std::dynamic_pointer_cast<Base>(object);
        }
    }
}

