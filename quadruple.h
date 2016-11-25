//
// Created by hanshiyi on 21/11/2016.
//

#ifndef UNTITLED_QUADRUPLE_H
#define UNTITLED_QUADRUPLE_H

#include <string>

class quadruple{
public:
    std::string op;
    std::string label;
    std::string op1;
    std::string op2;
    std::string target;
    quadruple(std::string op,std::string target="",std::string op1="",std::string op2="",std::string label="");
};
#endif //UNTITLED_QUADRUPLE_H
