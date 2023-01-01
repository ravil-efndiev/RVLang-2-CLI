#include "Utils.hpp"

namespace rvlang
{
    namespace utils
    {
        std::string GetTextFromFile(const std::string &path)
        {
            std::ifstream file;
            file.open(path);

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
                line.push_back('\n');
                source.append(line);
            }

            file.close();
            return source;
        }
    }
}
