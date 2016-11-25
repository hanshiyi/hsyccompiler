//
// Created by hanshiyi on 15/11/2016.
//

#include <iostream>
#include "tableTerm.h"
#include "syntax.h"
#include "signtable.h"

extern signtable st;
tableTerm::tableTerm() {
    typ = notsym;
    id="";
}
tableTerm::tableTerm(symbolSystem symb, std::string idname, int scope) {
    this->typ = symb;
    this->id = idname;
    pos = scope-1;
}

convarTerm::convarTerm(tableTerm tt, int value, int sh, bool isva) {
    id = tt.id;
    constValue = value;
    shift = sh;
    typ = tt.typ;
    isvar = isva;
    pos = tt.pos;
    for(std::vector<convarTerm>::iterator it = st.convarList.begin(); it!=st.convarList.end(); it++)
    {
        if(tt.id == (*it).id && pos == (*it).pos)
        {
            fetal("Redefinition of "+ tt.id);
        }
    }
    if(pos != -1) {
        for (std::vector<tableTerm>::iterator it = st.funcList[pos].paralist.begin();
             it != st.funcList[pos].paralist.end(); it++) {
            if (id == (*it).id) {
                fetal("Redefinition of " + id);
            }
        }
    }
}

arrayTerm::arrayTerm(tableTerm tt,int len, int sh) {
    typ = tt.typ;
    id = tt.id;
    length = len;
    shift = sh;
    pos = tt.pos;
    for(std::vector<arrayTerm>::iterator it = st.arrayList.begin(); it!=st.arrayList.end(); it++)
    {
        if(tt.id == (*it).id && pos == (*it).pos)
        {
            fetal("Redefinition of "+ tt.id);
        }
    }
    for(std::vector<convarTerm>::iterator it = st.convarList.begin(); it!=st.convarList.end(); it++)
    {
        if(tt.id == (*it).id && pos == (*it).pos)
        {
            fetal("Redefinition of "+ tt.id);
        }
    }
    if(pos != -1) {
        for (std::vector<tableTerm>::iterator it = st.funcList[pos].paralist.begin();
             it != st.funcList[pos].paralist.end(); it++) {
            if (id == (*it).id) {
                fetal("Redefinition of " + id);
            }
        }
    }
}

funcTerm::funcTerm(tableTerm tt, int beg) {
    typ = tt.typ;
    id = tt.id;
    pos = tt.pos;
    quadCodeBegin = beg;
    for(std::vector<funcTerm>::iterator it = st.funcList.begin(); it!=st.funcList.end(); it++)
    {
        if(tt.id == (*it).id && pos == (*it).pos)
        {
            fetal("Redefinition of "+ tt.id);
        }
    }
}
void funcTerm::insertPara(tableTerm a)
{
    for(std::vector<tableTerm>::iterator it = paralist.begin(); it!=paralist.end(); it++)
    {
        if(a.id == (*it).id)
        {
            fetal("Redefinition of "+a.id);
        }
    }
    st.termList.push_back(a);
    paralist.push_back(a);
}