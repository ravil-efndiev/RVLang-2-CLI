#include "Cli.hpp"

void PrintHelp()
{
    rvlang::Log("Rvlang comand line toolchain: ");
    rvlang::Log("rvlang [flags] <filename>", "Usage", COLOR_BLUE);
    rvlang::Log("rvlang <single flag>", "\tOr");
    rvlang::Log("", "Flags", COLOR_BLUE);
    rvlang::Log("\t --help (aka -h)     open this list");
    rvlang::Log("\t --version (aka -v)  see current rvlang version installed");
}

void PrintVersion()
{
    rvlang::Log("Current rvlang version installed: ");
    rvlang::Log(RVLANG_VERSION);
}

void ParseArgs(std::vector<std::string> args) 
{
    auto argc = args.size();
    for (int i = 1; i < argc; i++)
    {
        if (args[i][0] == '-')
        {
            if (args[i] == "--version" || args[i] == "-v")
            {
                if (argc > 2)
                    throw rvlang::Error("--version flag must not have any aditional options");
                PrintVersion();
            }

            if (args[i] == "--help" || args[i] == "-h")
            {
                if (argc > 2)
                    throw rvlang::Error("--help flag must not have any aditional options");
                PrintHelp();
            }
        }
        else 
        {
            auto source = rvlang::utils::GetTextFromFile(args[i]);
            rvlang::Log(source);
        }
    }
}

namespace rvlang
{
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
