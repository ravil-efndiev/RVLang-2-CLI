cmake_minimum_required(VERSION 3.1)

project(RVLang)

add_executable(rvlang
    src/main.cpp 
    src/System.cpp 
    src/cli/Cli.cpp 
    src/cli/Utils.cpp 
    src/interprieter/Lexer.cpp 
    src/interprieter/Parser.cpp 
    src/interprieter/Runtime.cpp
)

set_property(TARGET rvlang PROPERTY CXX_STANDARD 20)

target_include_directories(
    rvlang PUBLIC .
    rvlang PUBLIC src/
)
