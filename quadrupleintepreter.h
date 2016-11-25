//
// Created by hanshiyi on 21/11/2016.
//

#ifndef UNTITLED_QUADRUPLEINTEPRETER_H
#define UNTITLED_QUADRUPLEINTEPRETER_H

#include <iostream>
#include "signtable.h"
#include "quadruple.h"

class quadrupleintepreter
{
public:
    signtable st;
    int quadCounter;
    std::vector<quadruple> quadrupleVec;
    void addQua(quadruple a)
    {
        quadrupleVec.push_back(a);
    }
    int quadNum()
    {
        return (int)quadrupleVec.size();
    }
    void print()
    {
        for (std::vector<quadruple>::iterator it = quadrupleVec.begin(); it != quadrupleVec.end(); it++)
        {
            if((*it).label!="")
            {
                std::cout << (*it).label << ":  " << std::endl;
            }
            else
                std::cout << (*it).op<< "   " << (*it).target<< "   " << (*it).op1<< "   " << (*it).op2<< std::endl;
        }
    }
    void generate_target_mips();
};
#endif //UNTITLED_QUADRUPLEINTEPRETER_H
