#include "System.hpp"

namespace Rvlang
{
    void Log(const std::string& text, const std::string& prefix, const std::string& prefixColor)
    {
        auto prefixReal = prefix != "" ? const_cast<std::string&>(prefix).append(": ") : "";
        std::cout << prefixColor << prefixReal << COLOR_NONE << text << std::endl;
    }

    void LogError(const std::string& text)
    {
        std::cerr << COLOR_RED << "Error: " << COLOR_NONE << text << std::endl;
    }
}
