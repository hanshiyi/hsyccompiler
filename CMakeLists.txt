cmake_minimum_required(VERSION 3.6)
project(untitled)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp lexical.h syntax.h lexical.cpp syntax.cpp tableTerm.h tableTerm.cpp signtable.h signtable.cpp quadruple.h quadruple.cpp quadrupleintepreter.h quadrupleintepreter.cpp)
add_executable(untitled ${SOURCE_FILES})