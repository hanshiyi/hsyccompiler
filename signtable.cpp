//
// Created by hanshiyi on 15/11/2016.
//
#include <iostream>
#include "signtable.h"
#include "syntax.h"

extern signtable st;

void signtable::insert_array(arrayTerm at, tableTerm tt) {
    for(std::vector<tableTerm>::iterator  it = termList.begin(); it!=termList.end(); it++)
    {
        if((*it).typ != funcsym && (*it).pos == at.pos && (*it).id == at.id)
        {
            fetal("implicit definition of "+ at.id);
        }
    }
    arrayList.push_back(at);
    tt.typ = arraysym;
    termList.push_back(tt);
}

void signtable::insert_convar(convarTerm cvt, tableTerm tt) {
    for(std::vector<tableTerm>::iterator  it = termList.begin(); it!=termList.end(); it++)
    {
        if((*it).typ != funcsym && (*it).pos == cvt.pos && (*it).id == cvt.id)
        {
            fetal("implicit definition of "+ cvt.id);
        }
    }
    convarList.push_back(cvt);
    if(cvt.isvar)
        tt.typ = varsym;
    else
        tt.typ = constsym;
    termList.push_back(tt);
}

void signtable::insert_func(funcTerm ft, tableTerm tt) {
    for(std::vector<tableTerm>::iterator  it = termList.begin(); it!=termList.end(); it++)
    {
        if((*it).typ == funcsym && ft.id==(*it).id)
        {
            fetal("implicit definition of "+ ft.id);
        }
    }
    funcList.push_back(ft);
    tt.typ = funcsym;
    termList.push_back(tt);
}

void signtable::print() {
    std::cout << "Array definition" << std::endl;
    for(std::vector<arrayTerm>::iterator  it = arrayList.begin(); it!=arrayList.end(); it++)
    {
        std::cout << "\tType : "<< (*it).typ <<"; Name: "<< (*it).id <<  "; Scope: " << ((*it).pos==-1 ? "global":funcList[(*it).pos].id) << "; Length :" <<(*it).length << " shift: " << (*it).shift <<std::endl;
    }
    std::cout << "Constant or variable definition" << std::endl;
    for(std::vector<convarTerm>::iterator  it = convarList.begin(); it!=convarList.end(); it++)
    {
        std::cout << "\tType : "<< (*it).typ <<"; Name: "<< (*it).id <<  "; Scope: " << ((*it).pos==-1 ? "global":funcList[(*it).pos].id) << "; Value :" <<((*it).isvar ? 0 : (*it).constValue) << " shift: " << (*it).shift<<std::endl;
    }
    std::cout << "Function definition" << std::endl;
    for(std::vector<funcTerm>::iterator  it = funcList.begin(); it!=funcList.end(); it++)
    {
        std::cout << "\tType : "<< (*it).typ <<"; Name: "<< (*it).id <<std::endl;
        for(std::vector<tableTerm>::iterator iter = (*it).paralist.begin(); iter!=(*it).paralist.end(); iter++)
        {
            std::cout << "\t\tType" << (*iter).typ <<"; Name: "<< (*iter).id <<  "; Scope: " << ((*iter).pos==-1 ? "global":funcList[(*iter).pos].id) << std::endl;
        }
    }
    std::cout<<"All identifier!" << std::endl;
    for(std::vector<tableTerm>::iterator  it = termList.begin(); it!=termList.end(); it++)
    {
        std::cout << "\tType : "<< (*it).typ <<"; Name: "<< (*it).id <<std::endl;

    }
}

int signtable::check_func(std::string id,bool returncheck ) {
    int count = 0 ;
    for(std::vector<funcTerm>::iterator  it = funcList.begin(); it!=funcList.end(); it++,count++)
    {
        if(id == (*it).id && !((*it).typ==voidsym && returncheck))
        {
            return count;
        }
    }
    fetal(" No such function! ---->" + id);
    return -1;
}
bool signtable::check_array(std::string id) {
    for(std::vector<arrayTerm>::iterator  it = arrayList.begin(); it!=arrayList.end(); it++)
    {
        if(id == (*it).id && ((*it).pos == -1 || (*it).pos == st.funcList.size()-1))
        {
            return true;
        }
    }
    fetal(" No such array! ---->" + id);
    return false;
}
bool signtable::check_variable(std::string id) {
    for(std::vector<convarTerm>::iterator  it = convarList.begin(); it!=convarList.end(); it++)
    {
        if(id == (*it).id && (*it).isvar && ((*it).pos == -1 || (*it).pos == st.funcList.size()-1))
        {
            return true;
        }
    }
    for (std::vector<tableTerm>::iterator it = st.funcList[st.funcList.size()-1].paralist.begin();
         it != st.funcList[st.funcList.size()-1].paralist.end(); it++) {
        if (id == (*it).id) {
            return true;
        }
    }
    return false;
}
symbolSystem signtable::check_type(std::string id) {
    for(std::vector<convarTerm>::iterator  it = convarList.begin(); it!=convarList.end(); it++)
    {
        if(id == (*it).id  && ((*it).pos == st.funcList.size()-1))
        {
            return (*it).typ;
        }
    }
    for (std::vector<tableTerm>::iterator it = st.funcList[st.funcList.size()-1].paralist.begin();
         it != st.funcList[st.funcList.size()-1].paralist.end(); it++) {
        if (id == (*it).id) {
            return (*it).typ;
        }
    }
    for(std::vector<arrayTerm>::iterator  it = arrayList.begin(); it!=arrayList.end(); it++)
    {
        if(id == (*it).id &&  ((*it).pos == st.funcList.size()-1))
        {
            return (*it).typ;
        }
    }
    for(std::vector<convarTerm>::iterator  it = convarList.begin(); it!=convarList.end(); it++)
    {
        if(id == (*it).id  && ((*it).pos == -1))
        {
            return (*it).typ;
        }
    }
    for(std::vector<arrayTerm>::iterator  it = arrayList.begin(); it!=arrayList.end(); it++)
    {
        if(id == (*it).id &&  ((*it).pos == -1))
        {
            return (*it).typ;
        }
    }
    for(std::vector<funcTerm>::iterator  it = funcList.begin(); it!=funcList.end(); it++)
    {
        if(id == (*it).id)
        {
            return (*it).typ;
        }
    }

    return notsym;
}
symbolSystem signtable::check_type1(std::string id, std::string funcname) {
    for(std::vector<convarTerm>::iterator  it = convarList.begin(); it!=convarList.end(); it++)
    {
        if(id == (*it).id  && (*it).pos == find_funcname(funcname))
        {
            return (*it).typ;
        }
    }
    for(std::vector<arrayTerm>::iterator  it = arrayList.begin(); it!=arrayList.end(); it++)
    {
        if(id == (*it).id  && (*it).pos == find_funcname(funcname))
        {
            return (*it).typ;
        }
    }
    for (std::vector<tableTerm>::iterator it = st.funcList[find_funcname(funcname)].paralist.begin();
         it != st.funcList[find_funcname(funcname)].paralist.end(); it++) {
        if (id == (*it).id) {
            return (*it).typ;
        }
    }
    return notsym;
}
bool signtable::check_constant(std::string id) {
    for(std::vector<convarTerm>::iterator  it = convarList.begin(); it!=convarList.end(); it++)
    {
        if(id == (*it).id && !(*it).isvar && ((*it).pos == -1 || (*it).pos == st.funcList.size()-1))
        {
            return true;
        }
    }
    return false;
}

int signtable::find_funcname(std::string id)
{
    int count = 0;
    for(std::vector<funcTerm>::iterator  it = funcList.begin(); it!=funcList.end(); it++,count++)
    {
        if(id == (*it).id)
        {
            return count;
        }
    }
    return -1;
}

int signtable::findidshift(std::string id,std::string funcname)
{

    for(std::vector<convarTerm>::iterator  it = convarList.begin(); it!=convarList.end(); it++)
    {
        if(id == (*it).id  && (*it).pos == find_funcname(funcname))
        {
            return (*it).shift;
        }
    }
    for(std::vector<arrayTerm>::iterator  it = arrayList.begin(); it!=arrayList.end(); it++)
    {
        if(id == (*it).id  && (*it).pos == find_funcname(funcname))
        {
            return (*it).shift;
        }
    }
    int count = 1;
    int size = (int)st.funcList[find_funcname(funcname)].paralist.size();
    for (std::vector<tableTerm>::iterator it = st.funcList[find_funcname(funcname)].paralist.begin();
         it != st.funcList[find_funcname(funcname)].paralist.end(); it++,count++) {
        if (id == (*it).id) {
            if(count <= 4)
            {
                return -1;
            } else{
                return (56+(size - count)*4 + st.funcList[find_funcname(funcname)].shift);
            }
        }
    }
    return -1;

}
int signtable::findidparano(std::string id,std::string funcname)
{

    int count = 0;
    int size = (int)st.funcList[find_funcname(funcname)].paralist.size();
    for (std::vector<tableTerm>::iterator it = st.funcList[find_funcname(funcname)].paralist.begin();
         it != st.funcList[find_funcname(funcname)].paralist.end(); it++,count++) {
        if (id == (*it).id) {
                return count;
        }
    }
    return -1;

}

bool signtable::check_global(std::string id,std::string func_id) {
    for (std::vector<tableTerm>::iterator it = termList.begin(); it != termList.end(); it++) {
        if(id == (*it).id && (*it).pos == find_funcname(func_id) && (*it).pos>=0)
        {
            return false;
        }
    }
    for (std::vector<tableTerm>::iterator it = termList.begin(); it != termList.end(); it++) {
        if(id == (*it).id &&  (*it).pos==-1)
        {
            return true;
        }
    }
    return false;
}