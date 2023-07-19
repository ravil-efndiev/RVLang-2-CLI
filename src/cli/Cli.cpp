#include "Cli.hpp"

#include "interprieter/Lexer.hpp"

namespace Rvlang
{
    void PrintHelp()
    {
        Log("Rvlang comand line toolchain: ");
        Log("rvlang [flags] <filename>", "Usage", COLOR_BLUE);
        Log("rvlang <single flag>", "\tOr");
        Log("", "Flags", COLOR_BLUE);
        Log("\t --help (aka -h)     open this list");
        Log("\t --version (aka -v)  see current rvlang version installed");
    }

    void PrintVersion()
    {
        Log("Current rvlang version installed: ");
        Log(RVLANG_VERSION);
    }

    void ParseArgs(const std::vector<std::string>& args) 
    {
        auto argc = args.size();
        for (int i = 1; i < argc; i++)
        {
            if (args[i][0] == '-')
            {
                if (args[i] == "--version" || args[i] == "-v")
                {
                    if (argc > 2)
                        throw Error("--version flag must not have any aditional options");
                    PrintVersion();
                }

                if (args[i] == "--help" || args[i] == "-h")
                {
                    if (argc > 2)
                        throw Error("--help flag must not have any aditional options");
                    PrintHelp();
                }
            }
            else 
            {
                try
                {
                    auto source = Utils::GetTextFromFile(args[i]);
                    Lexer lexer (source);

                    auto tokens = lexer.GetTokens();

                    Utils::PrintTokens(tokens);
                }
                catch (Error error)
                {
                    error.Print();
                }
            }
        }
    }

    status_t Run(int argc, char** argv)
    {
        try 
        {
            if (argc < 2) 
            {
                LogError("no argumets passed");
                Log("rvlang [flags] <filename>", "Usage");
                Log("To see list of flags use 'rvlang --help'");
                return RVLANG_ERROR;
            }

            std::vector<std::string> args;
            for (int i = 0; i < argc; i++)
            {
                args.push_back(argv[i]);
            }

            ParseArgs(args);

            return RVLANG_SUCCESS;
        }
        catch(Error error)
        {
            error.Print();
            return RVLANG_ERROR;
        }
    }
}
