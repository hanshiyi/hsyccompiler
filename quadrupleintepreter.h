//
// Created by hanshiyi on 21/11/2016.
//

#ifndef UNTITLED_QUADRUPLEINTEPRETER_H
#define UNTITLED_QUADRUPLEINTEPRETER_H

#include <iostream>
#include "signtable.h"
#include "quadruple.h"
#include <cstdio>
#include <sstream>

class quadrupleintepreter
{
public:
    signtable st;
    std::string dir;
    int quadCounter;
    bool assignregister;
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
    void delete_public_expression();
    void generate_target_mips();
    void lsoptimize();
    void DAG_optimize(int begin, int end);
    int stringToInt(std::string s);
};
class dagnode
{
public:
    std::string cal;
    std::string temp;
    bool isleaf;
    bool isadded;
    bool isconstant;
    std::vector<int> parent;
    int leftson;
    int rightson;
    dagnode() {
        isleaf = false;
        isadded = false;
        isconstant = false;
        cal = "";
        temp = "";
        parent = std::vector<int> ();
    }
    std::string toString()
    {
        std::ostringstream ss;
        ss << "[";
        for (int i = 0; i<parent.size(); i++)
        {
            ss << parent[i] << ",";
        }
        ss << "]";
        if(isleaf)
        {
            return  "content:\t" + cal + "\tparent:\t" + ss.str()  +"\ttemp:\t" + temp;
        } else
        {
            std::ostringstream ls; ls<< leftson;
            std::ostringstream rs; rs << rightson;
            return  "content:\t" + cal + "\tparent:\t" + ss.str()  +"\ttemp:\t" + temp + "\tleft:\t" + ls.str() + "\tright:\t" + rs.str();
        }
    }

};

#endif //UNTITLED_QUADRUPLEINTEPRETER_H
