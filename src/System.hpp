#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <regex>

#include <vector>
#include <array>
#include <unordered_map>

#include <memory>
#include <variant>
#include <optional>
#include <algorithm>
#include <functional>
#include <any>

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#define RVLANG_VERSION "dev 0.1"

//statuses

#define RVLANG_SUCCESS 0
#define RVLANG_ERROR   1

//colors

#define COLOR_NONE   "\033[0m"

#define COLOR_RED    "\033[91m"
#define COLOR_GREEN  "\033[92m"
#define COLOR_YELLOW "\033[93m"
#define COLOR_BLUE   "\033[94m"

namespace Rvlang 
{
    /**
     * @brief Standart rvlang cli log function to log data
     * @param text text to print
     * @param prefix optional parameter to specify prefix, it is text (mostly one word describing line) before actual text followed by semicolon
     * @param prefixColor optional parameter to specify prefix color
     */
    extern void Log(const std::string& text, const std::string& prefix = std::string(""), const std::string& prefixColor = std::string(COLOR_NONE));

    /**
     * @brief Standart rvlang cli function to log errors
     * @param text error text that will be printed with "Error:" prefix
     */
    extern void LogError(const std::string& text);

    struct Error
    {
        std::string Text;

        Error(const std::string& text) : Text(text) {}
        void Print()
        {
            std::cerr << COLOR_RED << "Error: " << COLOR_NONE << Text << std::endl;
        }
    };
}

