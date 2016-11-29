//
// Created by hanshiyi on 15/11/2016.
//

#include "syntax.h"
#include <iostream>
#include "lexical.h"
#include "tableTerm.h"
#include <cstdlib>
#include "signtable.h"
#include <set>
#include<fstream>
#include <sstream>
#include "quadrupleintepreter.h"

using namespace std;
extern symbolSystem sym;
extern std::string id;
extern bool eofFlag;
extern int cc;
extern int line;
extern int inum;
int errornum;
int shift;
int stringcount;
int ifconditioncount;
int forloopcount;
int dowhilecount;
symbolSystem scope;
symbolSystem type;
extern signtable st;
extern quadrupleintepreter qi;
bool charexpression;

void skip(set<symbolSystem > skipset)
{
    exit(10);

    while(skipset.find(sym) == skipset.end())
    {
        cout << "skip : " << id << endl;
        getsym();
    }
}
void error(std::string s)
{
    cout << "ERROR" << ++errornum << ": in Line" << line << "," << cc  << " : " << s << endl;
    exit(10);
}
void fetal(std::string s)
{
    cout << "Fetal Error: " << ": in Line" << line << "," << cc  << " : " << s << endl;
    exit(1);
}
void syntax_analyze()
{
    getsym();
    stringcount = 0;
    ifconditioncount = 0;
    forloopcount = 0;
    dowhilecount = 0;
    errornum = 0;
    set<symbolSystem> skipset;
    scope = notsym;
    while(sym == constsym)
    {
        constDef();
        if(sym == semicolon)
        {
            getsym();
        } else{
            cout << "const unfinished" << endl;
        }
    }
    while(!eofFlag)
    {
        string idtmp = id;
        if(sym == intsym || sym == charsym)
        {
            type = sym;
            getsym();
            if(sym != identifier)
            {
                error("Lack of identifier");
                symbolSystem  ss[]={semicolon, rightbrace};
                skipset.clear();
                skipset.insert(ss,ss+1);
                skip(skipset);
            }
            tableTerm tt(type,id,st.funcList.size());
            getsym();
            if(sym == leftparathe)
            {
                funcTerm ft(tt,qi.quadNum());
                st.insert_func(ft,tt);
                func_analyze();
            }
            else if(sym == semicolon)
            {
                convarTerm cvt(tt,inum,0,true);
                st.insert_convar(cvt,tt);
                getsym();
            }
            else{
                while(sym!=semicolon )
                {
                    if(sym==leftbrack)
                    {
                        getsym();
                        if(sym != number)
                        {
                            error("array definition lack of appropriate expression");
                            symbolSystem  ss[]={semicolon};
                            skipset.clear();
                            skipset.insert(ss,ss+0);
                            skip(skipset);
                        }
                        arrayTerm at(tt,inum,0); ///address unsupported!!!!!!!!!!!!!!!!!!!!!
                        st.insert_array(at, tt);
                        getsym();
                        if(sym == rightbrack)
                        {
                            getsym();
                            if(sym == comma)
                            {
                                getsym();
                            }
                        }

                        if(sym == identifier)
                        {
                            tt = tableTerm(type, id,st.funcList.size());
                            getsym();
                        } else if(sym == semicolon)
                        {
                            break;
                        }
                        else
                        {
                            error("unknown token111");
                            symbolSystem  ss[]={semicolon};
                            skipset.clear();
                            skipset.insert(ss,ss);
                            skip(skipset);
                        }
                    }
                    else if(sym == comma)
                    {
                        convarTerm cvt(tt,inum,0,true);
                        st.insert_convar(cvt,tt);
                        getsym();
                        if(sym == identifier)
                        {
                            tt = tableTerm(type, id,st.funcList.size());
                            getsym();
                        } else
                        {
                            error("unknown token");
                            symbolSystem  ss[]={semicolon, rightbrace};
                            skipset.clear();
                            skipset.insert(ss,ss);
                            skip(skipset);
                        }
                    } else
                    {
                        error("Illegal begining token");
                        symbolSystem  ss[]={semicolon, rightbrace};
                        skipset.clear();
                        skipset.insert(ss,ss);
                        skip(skipset);
                    }
                    if(sym == semicolon)
                    {
                        convarTerm cvt(tt,inum,0,true);
                        st.insert_convar(cvt,tt);

                    }
                }
//                cout << ": global variable declaration." << endl;
                getsym();
            }
        }
        else if(sym == voidsym)
        {
            type = voidsym;
            getsym();
            if(sym != identifier && sym!=mainsym)
            {
                error("Lack of identifier");
                symbolSystem  ss[]={semicolon, rightbrace};
                skipset.clear();
                skipset.insert(ss,ss+1);
                skip(skipset);
            }
            tableTerm tt(type,id,st.funcList.size());
            getsym();
            if(sym == leftparathe)
            {
                funcTerm ft(tt,qi.quadNum());
                st.insert_func(ft,tt);
                func_analyze();
                if(sym == rightbrace)
                    getsym();
                if(tt.id=="main")
                {
//                   cout << "  : main function"<< endl;
                    break;
                }
            }
            else{
                error("Illegal void declaration");
                symbolSystem  ss[]={semicolon, rightbrace};
                skipset.clear();
                skipset.insert(ss,ss+1);
                skip(skipset);
            }
        }
    }
}
void integer()
{
    set<symbolSystem> skipset;
    int signflag = 1;
    if(sym == addsym)
    {
        getsym();
    }
    else if(sym == minussym)
    {
        getsym();
        signflag = -1;
    }

    if(sym == number)
    {
        inum = inum * signflag;
    } else{
        error("Integer detection error!");
        symbolSystem  ss[]={comma,semicolon};
        skipset.clear();
        skipset.insert(ss,ss+1);
        skip(skipset);
    }
//    cout << ": Integer";
}
void charactor()
{
    if(id[0]=='\'' && id[2]=='\'')
    {
        inum = id[1];
    }
}

void constDef()
{
    getsym();
    set<symbolSystem> skipset;
    if(sym == intsym || sym == charsym)
    {
        type = sym;
        getsym();
        if(sym != identifier)
        {
            error("Lack of identifier");
            symbolSystem  ss[]={semicolon, rightbrace};
            skipset.clear();
            skipset.insert(ss,ss+1);
            skip(skipset);
        }
        tableTerm tt(type,id,st.funcList.size());
        getsym();
        if(sym == assignsym){
            getsym();
            if(type == intsym)
                integer();
            else
                charactor();
            convarTerm cvt(tt,inum,shift,false);
            shift+=4;
            st.insert_convar(cvt,tt);
            ostringstream ss;
            ss << inum;
            ostringstream shs;
            shs << cvt.shift;
            getsym();
            while(sym != semicolon)
            {
                if(sym==comma)
                {
                    getsym();
                    if(sym != identifier)
                    {
                        error("Lack of identifier");
                        symbolSystem  ss[]={semicolon, rightbrace};
                        skipset.clear();
                        skipset.insert(ss,ss+1);
                        skip(skipset);
                    }
                    tt = tableTerm(type,id,st.funcList.size());
                    getsym();
                    if(sym == assignsym){
                        getsym();
                        if(type == intsym)
                            integer();
                        else
                            charactor();
                        cvt = convarTerm(tt,inum,shift,false);
                        shift += 4;
                        st.insert_convar(cvt,tt);
                        ostringstream ss;
                        ss << inum;
                        ostringstream shs;
                        shs << cvt.shift;
                        getsym();
                    }else  {
                        error("Unfinishted constant declaration!!!");
                        symbolSystem  ss[]={semicolon, rightbrace};
                        skipset.clear();
                        skipset.insert(ss,ss+1);
                    }
                } else
                {
                    error("Lack of separator");
                    symbolSystem  ss[]={semicolon, rightbrace};
                    skipset.clear();
                    skipset.insert(ss,ss+1);
                    skip(skipset);
                }

            }
        }
        else  {
            error("Unfinishted constant declaration!!!");
            symbolSystem  ss[]={semicolon, rightbrace};
            skipset.clear();
            skipset.insert(ss,ss+1);
        }

    } else
    {
        error("Unknown type declaration!!!");
        symbolSystem  ss[]={semicolon, rightbrace};
        skipset.clear();
        skipset.insert(ss,ss+1);
    }
//    cout << "This is a constant definition." << endl;
}
void varDef()
{
    set<symbolSystem> skipset;
    if(sym == intsym || sym == charsym)
    {
        type = sym;
        getsym();
        if(sym != identifier)
        {
            error("Lack of identifier");
            symbolSystem  ss[]={semicolon, rightbrace};
            skipset.clear();
            skipset.insert(ss,ss+1);
            skip(skipset);
        }
        tableTerm tt(type,id,st.funcList.size());
        getsym();
        if(sym == semicolon)
        {
            convarTerm cvt(tt,inum,shift,true);
            shift+=4;
            st.insert_convar(cvt,tt);
            getsym();
//            cout << "This is a variable definition." << endl;
        }
        else{
            while(sym!=semicolon )
            {
                if(sym==leftbrack)
                {
                    getsym();
                    if(sym != number)
                    {
                        error("array definition lack of appropriate expression");
                        symbolSystem  ss[]={semicolon};
                        skipset.clear();
                        skipset.insert(ss,ss+0);

                    }
                    arrayTerm at(tt,inum,shift); ///address unsupported!!!!!!!!!!!!!!!!!!!!!
                    shift += 4*inum;
                    st.insert_array(at, tt);
                    getsym();
                    if(sym == rightbrack)
                    {
                        getsym();
                        if(sym == semicolon)
                        {
//                            cout << "This is a variable definition." << endl;
                            getsym();
                            return;
                        }
                        else if(sym == comma)
                        {
                            getsym();
                        }
                    }
                    if(sym == identifier)
                    {
                        tt = tableTerm(type, id,st.funcList.size());
                        getsym();
                    }else if(sym == semicolon)
                    {
                        break;
                    }
                    else
                    {
                        error("unknown token");
                        symbolSystem  ss[]={semicolon};
                        skipset.clear();
                        skipset.insert(ss,ss);
                    }
                }
                else if(sym == comma)
                {
                    convarTerm cvt(tt,inum,shift,true);
                    shift +=4;
                    st.insert_convar(cvt,tt);
                    getsym();
                    if(sym == identifier)
                    {
                        tt = tableTerm(type, id,st.funcList.size());
                        getsym();
                    } else
                    {
                        error("unknown token");
                        symbolSystem  ss[]={semicolon, rightbrace};
                        skipset.clear();
                        skipset.insert(ss,ss);
                    }
                }else
                {
                    error("Unknown token");
                    symbolSystem  ss[]={semicolon, rightbrace};
                    skipset.clear();
                    skipset.insert(ss,ss);
                    skip(skipset);
                }
                if(sym == semicolon)
                {
                    convarTerm cvt(tt,inum,shift,true);
                    shift += 4;
                    st.insert_convar(cvt,tt);
//                    cout << "This is a variable definition." << endl;
                }
            }
            getsym();
        }

    }
}

void func_analyze()
{
    set<symbolSystem> skipset;
    qi.addQua(quadruple("","","","","funcbegin_"+st.funcList[st.funcList.size()-1].id));
    shift = 0;
    if(sym == leftparathe)
    {
        paraList();
        if(sym==rightparathe)
        {
            getsym();
            if(sym==leftbrace)
            {
                compoundStatement();
                if(sym == rightbrace)
                {
                    getsym();
                }
            }
        }
    } else
    {
        error("Illegal void declaration");
        symbolSystem  ss[]={semicolon, rightbrace};
        skipset.clear();
        skipset.insert(ss,ss+1);
    }
    qi.addQua(quadruple("","","","","funcend_"+st.funcList[st.funcList.size()-1].id));
//    cout << " : function definition";
}
void paraList() //empty first give space to the var and constant
{
    funcTerm &ft = st.funcList[st.funcList.size() - 1];
    set<symbolSystem > skipset;
    getsym();
    while(sym!=rightparathe)
    {
        if(sym == intsym || sym == charsym)
        {
            symbolSystem type = sym;
            getsym();
            if(sym != identifier)
            {
                error("Lack of identifier");
                symbolSystem  ss[]={semicolon, rightbrace, rightparathe};
                skipset.clear();
                skipset.insert(ss,ss+2);
                skip(skipset);
                return;
            }
            tableTerm tt(type,id,st.funcList.size());
            ft.insertPara(tt);
            getsym();
            if(sym==comma)
            {
                getsym();
            }else if (sym == rightparathe)
            {
                break;
            } else
            {
                error("Illegal token!111");
                symbolSystem  ss[]={semicolon, rightbrace, rightparathe};
                skipset.clear();
                skipset.insert(ss,ss+2);
                skip(skipset);
                return;
            }
        }else
        {
            error("Illegal token!");
            symbolSystem  ss[]={semicolon, rightbrace, rightparathe};
            skipset.clear();
            skipset.insert(ss,ss+2);
            skip(skipset);
            return;
        }
    }
}
void compoundStatement()
{
    getsym();
    while(sym == constsym)
    {
        constDef();
        getsym();
    }
    while (sym==intsym || sym==charsym)
    {
        varDef();
    }
    st.funcList[st.funcList.size()-1].shift = shift;
    multiplestatement();

}

void multiplestatement()
{
    while(sym != rightbrace)
    {
        statement();
    }
    getsym();
}
int counter = 0;
void statement()
{
    set<symbolSystem > skipset;


    if(sym == ifsym)
    {
        getsym();
        ifcondition();
    }else if(sym  == dosym)
    {
        getsym();
        dowhileconditon();
        if(sym == rightparathe)
            getsym();
        else
        {
            ///error
        }
    }else if(sym == forsym)
    {
        getsym();
        forstatement();

    }else if(sym == leftbrace)
    {
        getsym();
        if(sym != rightbrace)
            multiplestatement();
    }else if(sym == scanfsym)
    {
        getsym();
        scanfstatement();
        if(sym == semicolon)
            getsym();
        else
        {
            ///error
        }
    }
    else if(sym == printfsym)
    {
        getsym();
        printfstatement();
        if(sym == semicolon)
            getsym();
        else
        {
            ///error
        }
    }
    else if(sym == identifier)
    {
        string token = id;
        //check
        getsym();
        if(sym == assignsym)
        {
            bool judge = st.check_variable(token);
            if(!judge)
            {
                fetal("no suck variable!");
            }
            getsym();
            expression();
            qi.addQua(quadruple("move", "convar_" + token, "result", "", ""));
            if(sym == semicolon)
                getsym();
            else
            {
                ///error
            }
        }
        else if(sym == leftparathe)
        {
            //check
            int pos = st.check_func(token,false);
            int count = 0;
            qi.addQua(quadruple("call",token,"","",""));
            qi.addQua(quadruple("parabegin","","","",""));
            getsym();
            while(sym!=rightparathe)
            {
                expression();
                ostringstream ss;
                ss << count;
                count ++;
                qi.addQua(quadruple("move","parameter","result","$0",""));
                if(sym == comma)
                {
                    getsym();
                }else
                {
                }
            }
            ostringstream  ss;
            ss << count;
            qi.addQua(quadruple("paraend",ss.str(),"","",""));
            if(count != st.funcList[pos].paralist.size())
            {
                fetal("Unmatched parameters " + ss.str() );
            }
            getsym();
            if(sym == semicolon)
                getsym();
        }
        else if(sym == leftbrack)
        {
            st.check_array(token);
            getsym();
            expression();
            qi.addQua(quadruple("arrayaddr", token, "result" , "", ""));
            if(sym == rightbrack)
            {
                getsym();
                if(sym == assignsym)
                {
                    getsym();
                    expression();
                    if(sym == semicolon)
                    {
                        getsym();
                    }

                }
            }
            qi.addQua(quadruple("move", "array_" + token, "result", "", ""));
        }

    }
    else if(sym == returnsym)
    {
        getsym();
        if(sym == semicolon)
        {
            if(st.funcList[st.funcList.size()-1].typ != voidsym)
            {
                fetal("return value missed!");
            }

            qi.addQua(quadruple("return", "", "", "", ""));
            getsym();
        } else {
            if(st.funcList[st.funcList.size()-1].typ == voidsym)
            {
                fetal("void function should not have return value");
            }

            expression();
            qi.addQua(quadruple("return", "result", "", "", ""));
//        cout << "return statement"<< endl;
            if (sym != semicolon) {
                error("Unfinished statement!!");
                symbolSystem ss[] = {semicolon, rightbrace, rightparathe};
                skipset.clear();
                skipset.insert(ss, ss + 2);
                skip(skipset);
            } else {
                getsym();
            }
        }
    } else if (sym == semicolon){
        getsym();
    }else{
        fetal("made !");
        getsym();
    }
//    cout << id <<  " : statement" << endl;
}
void condition(std::string conditionlabel)
{
    set<symbolSystem > skipset;
    int pos = conditionlabel.find('_');
    if(conditionlabel[0] == 'f')
    {
        qi.addQua(quadruple("", "", "", "", "forloopcondition_"+conditionlabel.substr(pos+1)));
    } else
    {
        getsym();
    }
    expression();
    string endlabel;
    qi.addQua(quadruple("move", "compare1", "result", "", ""));
    if(conditionlabel[0] == 'i')
    {
        endlabel = "ifend_"+conditionlabel.substr(pos+1);
    }
    else if(conditionlabel[0] == 'f')
    {
        endlabel = "forloopend_"+conditionlabel.substr(pos+1);
    }else if(conditionlabel[0] == 'd')
    {
        endlabel = "dowhileend_"+conditionlabel.substr(pos+1);
    }

    if(sym == equ || sym == let || sym == leeqt || sym == neq || sym == grt || sym == greqt)
    {
        symbolSystem condisym = sym;
        getsym();
        expression();
        qi.addQua(quadruple("move", "compare2", "result", "", ""));
        if(condisym == equ)
        {
            qi.addQua(quadruple("beq", "$s1", "$s2", conditionlabel, ""));
            qi.addQua(quadruple("j", endlabel, "", "", ""));
            if(conditionlabel[0] == 'i')
            {
                qi.addQua(quadruple("", "", "", "", conditionlabel));
            }
        } else if(condisym == let)
        {
            qi.addQua(quadruple("blt", "$s1", "$s2", conditionlabel, ""));
            qi.addQua(quadruple("j", endlabel, "", "", ""));
            if(conditionlabel[0] == 'i')
            {
                qi.addQua(quadruple("", "", "", "", conditionlabel));
            }
        } else if(condisym == leeqt)
        {
            qi.addQua(quadruple("ble", "$s1", "$s2", conditionlabel, ""));
            qi.addQua(quadruple("j", endlabel, "", "", ""));
            if(conditionlabel[0] == 'i')
            {
                qi.addQua(quadruple("", "", "", "", conditionlabel));
            }
        } else if(condisym == neq)
        {
            qi.addQua(quadruple("bne", "$s1", "$s2", conditionlabel, ""));
            qi.addQua(quadruple("j", endlabel, "", "", ""));
            if(conditionlabel[0] == 'i')
            {
                qi.addQua(quadruple("", "", "", "", conditionlabel));
            }
        } else if(condisym == grt)
        {
            qi.addQua(quadruple("bgt", "$s1", "$s2", conditionlabel, ""));
            qi.addQua(quadruple("j", endlabel, "", "", ""));
            if(conditionlabel[0] == 'i')
            {
                qi.addQua(quadruple("", "", "", "", conditionlabel));
            }
        } else if(condisym == greqt)
        {
            qi.addQua(quadruple("bge", "$s1", "$s2", conditionlabel, ""));
            qi.addQua(quadruple("j", endlabel, "", "", ""));
            if(conditionlabel[0] == 'i')
            {
                qi.addQua(quadruple("", "", "", "", conditionlabel));
            }
        }
        if(sym == rightparathe || sym == semicolon)
            getsym();
        else
        {
            fetal("conditon");
            exit(0);
        }
    }else if(sym == rightparathe || sym == semicolon)
    {
        qi.addQua(quadruple("move", "compare2", "$0", "", ""));
        qi.addQua(quadruple("bne", "$s1", "$s2", conditionlabel, ""));
        qi.addQua(quadruple("j", endlabel, "", "", ""));
        if(conditionlabel[0] == 'i')
        {
            qi.addQua(quadruple("", "", "", "", conditionlabel));
        }
        getsym();
    }
    else{
        error("Unfinished statement!!");
        symbolSystem  ss[]={semicolon, rightbrace, rightparathe};
        skipset.clear();
        skipset.insert(ss,ss+2);
        skip(skipset);
    }
//    cout << " : condition" ;
}

void ifcondition()
{
    set<symbolSystem > skipset;
    if(sym == leftparathe)
    {
        ostringstream ss;
        ss << ifconditioncount;
        ifconditioncount++;
        condition("ifbegin_" + ss.str());
        statement();
        qi.addQua(quadruple("j", "elseend_"+ss.str(), "", "", ""));
        qi.addQua(quadruple("", "", "", "", "ifend_" + ss.str()));
        if(sym == elsesym)
        {
            getsym();
            statement();
        }
        qi.addQua(quadruple("", "", "", "", "elseend_" + ss.str()));

    } else{
        error("Unfinished statement!!");
        symbolSystem  ss[]={semicolon, rightbrace, rightparathe};
        skipset.clear();
        skipset.insert(ss,ss+2);
        skip(skipset);
    }
//    cout << " : if condition statement "<< endl;
}

void dowhileconditon()
{
    set<symbolSystem > skipset;
    ostringstream ss;
    ss << dowhilecount;
    dowhilecount ++;
    qi.addQua(quadruple("", "", "", "", "dowhilebegin_" + ss.str()));
    statement();
    if(sym==whilesym)
    {
        getsym();
        if(sym==leftparathe)
        {
            condition("dowhilebegin_" + ss.str());
            qi.addQua(quadruple("", "", "", "", "dowhileend_" + ss.str()));
//            cout << " : do while statement" << endl;
            return;
        }
    } else{
        error("Unfinished statement!!");
        symbolSystem  ss[]={semicolon, rightbrace, rightparathe};
        skipset.clear();
        skipset.insert(ss,ss+2);
        skip(skipset);
    }

}

void forstatement()
{
    std::string step1,step2;
    symbolSystem stepsym;
    std::string steplength;
    if(sym == leftparathe)
    {
        getsym();
        if(sym == identifier)
        {
            string idp = id;
            getsym();
            if(sym == assignsym)
            {
                getsym();
                expression();
                st.check_variable(idp);
                qi.addQua(quadruple("move", "convar_"+idp, "result", "", ""));
                if(sym == semicolon)
                {
                    getsym();
                    ostringstream ss;
                    ss << forloopcount;
                    forloopcount++;
                    condition("forloopbegin_" + ss.str());
                    if(sym == identifier)
                    {
                        bool judge = st.check_variable(id);
                        if(!judge)
                        {
                            fetal("no such variable " + id);
                        }
                        step1 = id;
                        getsym();
                        if(sym == assignsym)
                        {
                            getsym();
                            if(sym == identifier)
                            {
                                bool isvar = st.check_variable(id);
                                bool isconst = st.check_constant(id);
                                if(!isvar && !isconst)
                                {
                                    fetal("undefined identifier");
                                }
                                step2 = id;
                                getsym();
                                if(sym == addsym || sym == minussym)
                                {
                                    stepsym = sym;
                                    getsym();
                                    if(sym == number)
                                    {
                                        steplength = id;
                                        getsym();
                                        if(sym == rightparathe)
                                        {
                                            getsym();
                                            qi.addQua(quadruple("", "", "", "", "forloopbegin_" + ss.str()));
                                            statement();
                                            if(stepsym == addsym)
                                            {

                                                qi.addQua(quadruple("LI", "$t0",  steplength,"", ""));
                                                qi.addQua(quadruple("move", "$t1",  "convar_"+step2 ,"", ""));
                                                qi.addQua(quadruple("add", "$t1", "$t0", "t1", ""));
                                                qi.addQua(quadruple("move", "result", "$t1", "", ""));
                                                qi.addQua(quadruple("move", "convar_"+step1, "result", "", ""));
                                            }
                                            else if(stepsym == minussym)
                                            {
                                                qi.addQua(quadruple("LI", "$t0",  steplength,"", ""));
                                                qi.addQua(quadruple("move", "$t1",  "convar_"+step2 ,"", ""));
                                                qi.addQua(quadruple("sub", "$t1", "$t0", "t1", ""));
                                                qi.addQua(quadruple("move", "result", "$t1", "", ""));
                                                qi.addQua(quadruple("move", "convar_"+step1, "result", "", ""));
                                            }
                                            qi.addQua(quadruple("j", "forloopcondition_" + ss.str(), "", "", ""));
                                            qi.addQua(quadruple("", "", "", "", "forloopend_" + ss.str()));
//                                            cout  <<" : for statement" << endl;
                                            return;
                                        }
                                    }
                                }
                            }
                        }
                    }

                }
            }
        } else{

        }
    }
}

void scanfstatement()
{
    if(sym == leftparathe)
    {
        getsym();
        if(sym == identifier)
        {
            st.check_variable(id);
            qi.addQua(quadruple("scan", id, "", "", ""));
            getsym();
            while(sym == comma)
            {
                getsym();
                if(sym != identifier)
                {
                    fetal("scanf not receive identifier");
                }
                st.check_variable(id);
                qi.addQua(quadruple("scan", id, "", "", ""));
                getsym();
            }
            if(sym == rightparathe)
            {
                getsym();
                return;
            }
        }
    } else
    {

    }
//    cout << "scanf" << endl;
}

void printfstatement()
{
    if(sym == leftparathe)
    {
        getsym();
        if(sym == stringsym)
        {
            st.stringlist.push_back(id.substr(1,id.length()-2));
            stringcount ++;
            ostringstream ss;
            ss << stringcount-1;
            qi.addQua(quadruple("print","string_"+ss.str(),"","",""));
            getsym();
            if(sym == rightparathe)
            {
                getsym();
                return;
            }
            else if(sym == comma){
                getsym();
                expression();
                if(charexpression)
                {
                    qi.addQua(quadruple("print", "result", "char", "", ""));
                } else
                {
                    qi.addQua(quadruple("print", "result", "", "", ""));
                }
                if(sym == rightparathe)
                {
                    getsym();
                    return;
                }
            }
        }else
        {
            expression();
            if(charexpression)
            {
                qi.addQua(quadruple("print", "result", "char", "", ""));
            } else
            {
                qi.addQua(quadruple("print", "result", "", "", ""));
            }
            if(sym == rightparathe)
            {
                getsym();
                return;
            }
        }
    } else
    {

    }
//    cout << "printf statement" << endl;
}
void expression()
{
    int count = 0;
    charexpression = false;
    symbolSystem tmp = sym;
    if(sym==addsym || sym==minussym)
    {
        getsym();
    }
    term(count); //turn negative
    if(tmp == minussym)
        qi.addQua(quadruple("sub","$t0","$0","$t0",""));
    while(sym != comma && sym!=semicolon  && sym != rightbrack && sym!=rightparathe && sym != equ && sym != let && sym != leeqt && sym != neq && sym != grt && sym != greqt)
    {
        if(sym == addsym || sym == minussym)
        {
            count ++;
            symbolSystem tmp = sym;
            getsym();
            term(count);
            if(tmp == addsym)
            {
                ostringstream ts1,ts2;
                ts1 << count-1;
                ts2 << count;
                qi.addQua(quadruple("add","$t"+ts2.str(),"$t"+ts1.str(),"$t"+ts2.str(),""));
            }else if(tmp == minussym)
            {
                ostringstream ts1,ts2;
                ts1 << count-1;
                ts2 << count;
                qi.addQua(quadruple("sub","$t"+ts2.str(),"$t"+ts1.str(),"$t"+ts2.str(),""));
            }
        } else
        {
            cout  << id <<"fetal"<<endl;
            exit(0);
        }
    }
    ostringstream ss;
    ss << count;
    qi.addQua(quadruple("move","result","$t"+ss.str(),"",""));
//    cout << "expression";
}
void term(int term_count)
{
    int count = 0;
    factor(count);
    while(sym==mulsym || sym==divsym)
    {
        charexpression = false;
        if(sym == mulsym || sym == divsym)
        {
            count ++;
            symbolSystem tmp = sym;
            getsym();
            factor(count);
            if(tmp == mulsym)
            {
                ostringstream f1;
                ostringstream f2;
                f1 << count - 1;
                f2 << count;
                qi.addQua(quadruple("mult","$s"+f1.str(),"$s"+f2.str(),"",""));
                qi.addQua(quadruple("mflo","$s"+f2.str(),"","",""));
            }
            else if(tmp == divsym)
            {
                ostringstream f1;
                ostringstream f2;
                f1 << count - 1;
                f2 << count ;
                qi.addQua(quadruple("div","$s"+f1.str(),"$s"+f2.str(),"",""));
                qi.addQua(quadruple("mflo","$s"+f2.str(),"","",""));
            }
        } else
        {
            cout <<"fetal in term"<<endl;
            exit(0);
        }
    }
    ostringstream ss,fs;
    ss << term_count;
    fs << count;
    if(term_count!=0)
        charexpression = false;
    qi.addQua(quadruple("move","$t"+ss.str(),"$s"+fs.str(),"",""));
//    if(charexpression)
//    {
//        cout << line << " :" << cc<< " : safjklajsf" << endl;
//    }
//    cout << " : term";
}
void factor(int count)
{
    set<symbolSystem > skipset;
    funcTerm &ft = st.funcList[st.funcList.size() - 1];
    ostringstream co;
    co << count;
//    cout << "enter factor" << endl;
    if(sym==identifier)
    {
        string symbol = id;
        getsym();
        if(sym == leftbrack)
        {
            st.check_array(symbol);
            getsym();
            expression();
            symbolSystem type = st.check_type(symbol);
            if(type == charsym && count==0)
            {
                charexpression = true;
            }
            qi.addQua(quadruple("arrayvalue","$s"+co.str(),symbol,"result",""));
            if(sym==rightbrack)
            {
                getsym();
//                cout << " : factor" ;
            }
        }
        else if(sym == leftparathe)
        {
            int pos = st.check_func(symbol,true);
            getsym();
            int acount = 0;
            qi.addQua(quadruple("call",symbol,"","",""));
            qi.addQua(quadruple("parabegin","","","",""));
            while(sym!=rightparathe)
            {
                expression();
                ostringstream ss;
                ss << acount;
                qi.addQua(quadruple("move","parameter","result","",""));
                acount ++;
                if(sym == comma)
                {
                    getsym();
                }else if(sym != rightparathe)
                {
                    error("Illegal token!~~~~~~~");
                    symbolSystem  ss[]={semicolon, rightbrace, rightparathe};
                    skipset.clear();
                    skipset.insert(ss,ss+2);
                    skip(skipset);
                }

            }
            symbolSystem  type = st.check_type(symbol);
            if(type == charsym && count == 0)
            {
                charexpression = true;
            }
            ostringstream ss;
            ss << acount;
            qi.addQua(quadruple("paraend",ss.str(),"","",""));
            qi.addQua(quadruple("move","$s"+co.str(),"returnvalue","",""));
            if(acount != st.funcList[pos].paralist.size())
            {
                fetal("unmatched parameters");
            }
            getsym();
        } else
        {
            int paranum = 0;
            bool isvar = st.check_variable(symbol);
            bool isconstant = st.check_constant(symbol);
            if(!isvar && !isconstant)
            {
                fetal("no such identifier");
            }
            symbolSystem type = st.check_type(symbol);
            if(type == notsym)
            {
                fetal("unknown type variable/constant " + symbol);
            }else if(type == charsym)
            {
                charexpression = true;
            }
            qi.addQua(quadruple("move","$s"+co.str(),"convar_"+symbol,"",""));
//            cout << ": factor" ;
            return;
        }
    }
    else if(sym == number || sym==addsym || sym==minussym)
    {
        integer();
        int value = inum;
        getsym();
        ostringstream ss;
        ss << value;
        qi.addQua(quadruple("LI","$s"+co.str(),ss.str(),"",""));

//        cout << " : factor" ;
        return;
    }
    else if(sym==character)
    {
        if(count == 0)
            charexpression = true;
        ostringstream ss;
        ss << inum;
        qi.addQua(quadruple("LI","$s"+co.str(),ss.str(),"",""));
        getsym();
//        cout <<  " : factor" ;
        return;
    }
    else if(sym == leftparathe)
    {
        getsym();
        expression();
        if(sym == rightparathe)
        {
            getsym();
            qi.addQua(quadruple("move","$s"+co.str(),"result","",""));
//            cout << " : factor" ;
            return;
        }
    }
//    cout << " : factor" ;
}