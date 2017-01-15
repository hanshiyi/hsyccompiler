//
// Created by hanshiyi on 15/11/2016.
//

#ifndef UNTITLED_SIGNTABLE_H
#define UNTITLED_SIGNTABLE_H

#include <list>
#include <vector>
#include "tableTerm.h"

class signtable
{
public:
    std::vector<tableTerm> termList;
    std::vector<convarTerm> convarList;
    std::vector<funcTerm> funcList;
    std::vector<arrayTerm> arrayList;
    std::vector<std::string> stringlist;
    signtable()
    {
        termList =  std::vector<tableTerm>();
        convarList = std::vector<convarTerm>();
        funcList = std::vector<funcTerm>();
        arrayList = std::vector<arrayTerm>();
    }
    void insert_convar(convarTerm cvt, tableTerm tt);
    void insert_func(funcTerm ft, tableTerm tt);
    void insert_array(arrayTerm at, tableTerm tt);
    void print();
    int check_func(std::string id, bool);
    bool check_array(std::string id);
    bool check_variable(std::string id);
    int findidshift(std::string id,std::string funcname);
    bool check_constant(std::string id);
    bool check_global(std::string id, std::string func_id);
    int find_funcname(std::string id);
    int findidparano(std::string id,std::string funcname);
    void assignregister();
    symbolSystem check_type(std::string id);
    symbolSystem check_type1(std::string id, std::string funcname);
};

#endif //UNTITLED_SIGNTABLE_H
