#include "Utils.hpp"

#include <interprieter/Token.hpp>

namespace Rvlang
{
    namespace Utils
    {
        std::string GetTextFromFile(const std::string &path)
        {
            std::ifstream file (path, std::ios::in);

            if (!file.is_open())
            {
                std::string text = "file \"";
                text.append(path).append("\" not found");
                file.close();
                throw Error(text);
            }

            std::string line, source;

            while (std::getline(file, line))
            {
                line.append("\n");
                source.append(line);
            }

            file.close();
            return source;
        }

        void PrintTokens(const std::vector<Token> &tokens)
        {
            for (auto& token : tokens)
            {
                std::cout << "token \"" << token.Text << "\", type: " << token.Type.Name << std::endl;
            }
        }
    }
}
