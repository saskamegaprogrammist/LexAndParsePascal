cmake_minimum_required(VERSION 3.14)
project(lexer)

set(CMAKE_CXX_STANDARD 14)

add_executable(lexer main.cpp Lexer/Lexer.cpp Lexer/Lexer.h Token/Token.cpp Token/Token.h Parser/Parser.cpp Parser/Parser.h ParseToken/ParseToken.cpp ParseToken/ParseToken.h)