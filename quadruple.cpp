//
// Created by hanshiyi on 21/11/2016.
//

#include "quadruple.h"

quadruple::quadruple(std::string _op, std::string _target, std::string _op1, std::string _op2, std::string _label) {
    op = _op;
    target = _target;
    op1 = _op1;
    op2 = _op2;
    label = _label;
}
std::string quadruple::toString() {
    return op+"\t"+target+"\t"+op1+"\t"+op2+"\t"+label;
}