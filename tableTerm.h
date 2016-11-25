//
// Created by hanshiyi on 15/11/2016.
//

#ifndef UNTITLED_TABLETERM_H
#define UNTITLED_TABLETERM_H
#include<list>
#include <string>
#include <vector>
#include "lexical.h"
class tableTerm{
public:
    symbolSystem typ;
    int pos;//0 is global 1 or bigger is the i-1th function
    std::string id;
    tableTerm();
    tableTerm(symbolSystem,std::string,int);
};
class convarTerm:public tableTerm
{
public:
    int constValue;
    int shift;
    bool isvar;
    convarTerm(tableTerm,int,int,bool);
};

class arrayTerm:public tableTerm
{
public:
    int length;
    int shift;
    arrayTerm(tableTerm,int,int);
};

class funcTerm:public tableTerm
{
public:
    std::vector<tableTerm> paralist;
    int quadCodeBegin;
    int shift;
    funcTerm(tableTerm,int);
    void insertPara(tableTerm a);
};
#endif //UNTITLED_TABLETERM_H
