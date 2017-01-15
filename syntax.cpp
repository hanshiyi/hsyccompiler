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
int intercount;
int forloopcount;
int dowhilecount;
symbolSystem scope;
symbolSystem type;
extern signtable st;
extern quadrupleintepreter qi;
bool charexpression;

void skip()
{
    int count=0;
    while(sym!=semicolon && sym!=rightbrace)
    {
        if(count > 5)
        {
            exit(errornum);
        }
        else if(eofFlag)
        {
            count++;
        }
        getsym();
    }
    getsym();
}
void skipcomma()
{
    int count=0;
    while(sym!=semicolon && sym!=comma && sym!=rightbrace)
    {
        if(count > 5)
        {
            exit(errornum);
        }
        else if(eofFlag)
        {
            count++;
        }
        getsym();
    }
    getsym();
}
void skippara()
{
    int count=0;
    while(sym!=semicolon && sym != rightparathe && sym != comma && sym!=rightbrace)
    {
        if(count > 5)
        {
            exit(errornum);
        }
        else if(eofFlag)
        {
            cout << "made" << endl;
            count++;
        }
        getsym();
    }
    if(sym!=rightparathe)
        getsym();
}
void error(std::string s)
{
    cout << "ERROR" << ++errornum << ": in Line" << line << "," << cc  << " : " << s << endl;
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
            error("const unfinished statement");
             skip();
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
                 skipcomma();
                continue;
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
                        arrayTerm at(tt,inum,0);
                        st.insert_array(at, tt);
                        if(sym != number)
                        {
                            error("array definition lack of d");
                            skipcomma();
                            break;
                        }else
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
                            skipcomma();
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
                            error("illegal token");
                            skipcomma();
                            break;
                        }
                    } else
                    {
                        error("Illegal separate token");
                        skipcomma();
                        break;
                    }
                    if(sym == semicolon)
                    {
                        convarTerm cvt(tt,inum,0,true);
                        st.insert_convar(cvt,tt);

                    }
                }
                if(sym == semicolon)
                    getsym();
            }
        }
        else if(sym == voidsym)
        {
            type = voidsym;
            getsym();
            tableTerm tt(type,id,st.funcList.size());
            if(sym != identifier && sym!=mainsym)
            {
                fetal("Lack of identifier");
                continue;
            }else
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
                    break;
                }
            }
            else{
                error("Illegal void declaration");
                skip();
                continue;
            }
        } else
        {
            error("Illegal beginning token");
            skip();
            continue;

        }
    }
if(errornum!=0)
{
    exit(errornum);
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
    }
//    cout << ": Integer";
}
void charactor()
{
    if(id[0]=='\'' && id[2]=='\'')
    {
        inum = id[1];
    }else{
        error("character detection error!");
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
            skip();
            return;
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
                        skip();
                        break;
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
                        skip();
                        break;
                    }
                } else
                {
                    error("Lack of separator");
                    skip();
                    break;
                }
            }
        }
        else  {
            error("Unfinishted constant declaration!!!");
            skip();
            return;
        }

    } else
    {
        error("Unknown type declaration!!!");
        skip();
        return;
    }
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
            skip();
            return;
        }
        tableTerm tt(type,id,st.funcList.size());
        getsym();
        if(sym == semicolon)
        {
            convarTerm cvt(tt,inum,shift,true);
            shift+=4;
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
                        skip();
                        break;
                    }
                    arrayTerm at(tt,inum,shift);
                    shift += 4*inum;
                    st.insert_array(at, tt);
                    getsym();
                    if(sym == rightbrack)
                    {
                        getsym();
                        if(sym == semicolon)
                        {
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
                        error("unknown token, expected identifier or semicolon");
                        skip();
                        break;
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
                        error("unknown token, expected identifier");
                        skip();
                        break;
                    }
                }else
                {
                    error("Unknown token");
                    skip();
                    break;
                }
                if(sym == semicolon)
                {
                    convarTerm cvt(tt,inum,shift,true);
                    shift += 4;
                    st.insert_convar(cvt,tt);
                }
            }
            if(sym == semicolon)
                getsym();
        }

    }
}

void func_analyze()
{
    intercount = 0;
    set<symbolSystem> skipset;
    qi.addQua(quadruple("","","","","funcbegin_"+st.funcList[st.funcList.size()-1].id));

    qi.addQua(quadruple("blockbegin","","","",""));
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
        skip();
    }
    st.funcList[st.funcList.size()-1].tempshift = (1+intercount)*4;
    qi.addQua(quadruple("blockend","","","",""));
    qi.addQua(quadruple("","","","","funcend_"+st.funcList[st.funcList.size()-1].id));
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
                error("Lack of identifier in paralist");
                skippara();
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
                error("Illegal token in parameter list");
                skippara();
                return;
            }
        }else
        {
            error("Illegal beginning token!");
            skippara();
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
            error("lack of finishing token");
            skip();
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
            error("lack of finishing token");
            skip();
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
                error("no suck variable!");
                skip();
                return;
            }
            getsym();
            expression();
            qi.addQua(quadruple("move", "convar_" + token, "result", "", ""));
            if(sym == semicolon)
                getsym();
            else
            {
                error("lack of finishing token");
                skip();
            }
        }
        else if(sym == leftparathe)
        {
            //check
            int pos = st.check_func(token,false);
            int count = 0;

            qi.addQua(quadruple("blockend","","","",""));
            qi.addQua(quadruple("call",token,"","",""));
            qi.addQua(quadruple("parabegin","","","",""));
            getsym();
            while(sym!=rightparathe)
            {
                expression();
                ostringstream ss;
                ss << count;
                count ++;
                ostringstream oss;
                oss << intercount;
                qi.addQua(quadruple("move","parameter","result","$t"+oss.str(),""));
                if(sym == comma)
                {
                    getsym();
                    if(sym == rightparathe)
                    {
                        error("lack of arguments");
                        skip();
                        return;
                    }
                }
            }
            ostringstream  ss;
            ss << count;
            qi.addQua(quadruple("paraend",ss.str(),"","",""));

            qi.addQua(quadruple("blockbegin","","","",""));
            if(count != st.funcList[pos].paralist.size())
            {
                error("Unmatched parameters " + ss.str() );
                skip();
                return;
            }
            getsym();
            if (sym != semicolon) {
                error("Unfinished statement!!");
                skip();
                return;
            } else {
                getsym();
            }
        }
        else if(sym == leftbrack)
        {
            st.check_array(token);
            getsym();
            expression();
            int temp = intercount;
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

                } else
                {
                    error("illegal statement");
                    skip();
                }
            } else
            {
                error("lack of right brack");
                skip();
            }stringstream ss;
            ss << temp;
            qi.addQua(quadruple("move", "array_" + token, "result", "$t"+ss.str(), ""));
        }else
        {
            error("illegal statement");
            skip();
        }

    }
    else if(sym == returnsym)
    {
        getsym();

        qi.addQua(quadruple("blockend","","","",""));
        if(sym == semicolon)
        {
            if(st.funcList[st.funcList.size()-1].typ != voidsym)
            {
                fetal("return value missed!");
            }

            qi.addQua(quadruple("return", "", "", "", ""));
            getsym();
        } else {

            if(sym == leftparathe)
            {
                getsym();
                expression();
                if(sym == rightparathe)
                {
                    getsym();
                } else
                {
                    error("unfinished return statement!");
                    skip();
                    return;
                }
            } else
            {
                error("unfinished return statement!");
                skip();
                return;
            }
            if(st.funcList[st.funcList.size()-1].typ == voidsym)
            {
                error("void function should not have return value");
                skip();
                return;
            }
            qi.addQua(quadruple("return", "result", "", "", ""));
            qi.addQua(quadruple("blockbegin","","","",""));
            if (sym != semicolon) {
                error("Unfinished statement!!");
                skip();
                return;
            } else {
                getsym();
            }
        }
    } else if (sym == semicolon){
        getsym();
    }else{
        error("unrecognized statement beginning token");
        skip();
    }
}
void condition(std::string conditionlabel)
{
    set<symbolSystem > skipset;
    int pos = conditionlabel.find('_');
    int leftcompare,rightcompare;
    if(conditionlabel[0] == 'f')
    {
        qi.addQua(quadruple("", "", "", "", "forloopcondition_"+conditionlabel.substr(pos+1)));
    } else
    {
        getsym();
    }

    qi.addQua(quadruple("blockend","","","",""));
    expression();
    leftcompare = intercount;
    ostringstream os;
    os << leftcompare;
    string endlabel;
    qi.addQua(quadruple("move", "compare1", "result", "$t"+os.str(), ""));
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
        rightcompare = intercount;
        ostringstream ros;
        ros << rightcompare;
        qi.addQua(quadruple("move", "compare2", "result", "$t"+ros.str(), ""));
        if(condisym == equ)
        {
            qi.addQua(quadruple("beq", "$s"+os.str(), "$s"+ros.str(), conditionlabel, ""));
            qi.addQua(quadruple("j", endlabel, "", "", ""));
            if(conditionlabel[0] == 'i')
            {
                qi.addQua(quadruple("", "", "", "", conditionlabel));
            }
        } else if(condisym == let)
        {
            qi.addQua(quadruple("blt", "$s"+os.str(), "$s"+ros.str(), conditionlabel, ""));
            qi.addQua(quadruple("j", endlabel, "", "", ""));
            if(conditionlabel[0] == 'i')
            {
                qi.addQua(quadruple("", "", "", "", conditionlabel));
            }
        } else if(condisym == leeqt)
        {
            qi.addQua(quadruple("ble", "$s"+os.str(), "$s"+ros.str(), conditionlabel, ""));
            qi.addQua(quadruple("j", endlabel, "", "", ""));
            if(conditionlabel[0] == 'i')
            {
                qi.addQua(quadruple("", "", "", "", conditionlabel));
            }
        } else if(condisym == neq)
        {
            qi.addQua(quadruple("bne", "$s"+os.str(), "$s"+ros.str(), conditionlabel, ""));
            qi.addQua(quadruple("j", endlabel, "", "", ""));
            if(conditionlabel[0] == 'i')
            {
                qi.addQua(quadruple("", "", "", "", conditionlabel));
            }
        } else if(condisym == grt)
        {
            qi.addQua(quadruple("bgt", "$s"+os.str(), "$s"+ros.str(), conditionlabel, ""));
            qi.addQua(quadruple("j", endlabel, "", "", ""));
            if(conditionlabel[0] == 'i')
            {
                qi.addQua(quadruple("", "", "", "", conditionlabel));
            }
        } else if(condisym == greqt)
        {
            qi.addQua(quadruple("bge", "$s"+os.str(), "$s"+ros.str(), conditionlabel, ""));
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
            error("condition");
            skip();
        }
    }else if(sym == rightparathe || sym == semicolon)
    {
        qi.addQua(quadruple("move", "compare2", "$0", "", ""));
        qi.addQua(quadruple("bne", "$s"+os.str(), "$0", conditionlabel, ""));
        qi.addQua(quadruple("j", endlabel, "", "", ""));
        if(conditionlabel[0] == 'i')
        {
            qi.addQua(quadruple("", "", "", "", conditionlabel));
        }
        getsym();
    }
    else{
        error("Unfinished statement!!");
        skip();
    }
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
        qi.addQua(quadruple("blockbegin","","","",""));
        statement();

        qi.addQua(quadruple("blockend","","","",""));
        qi.addQua(quadruple("j", "elseend_"+ss.str(), "", "", ""));
        qi.addQua(quadruple("", "", "", "", "ifend_" + ss.str()));
        if(sym == elsesym)
        {
            qi.addQua(quadruple("blockbegin","","","",""));
            getsym();
            statement();
            qi.addQua(quadruple("blockend","","","",""));
        }
        qi.addQua(quadruple("", "", "", "", "elseend_" + ss.str()));
        qi.addQua(quadruple("blockbegin","","","",""));

    } else{
        error("Unfinished statement!!");
        skip();
    }
}

void dowhileconditon()
{
    set<symbolSystem > skipset;
    ostringstream ss;
    ss << dowhilecount;
    dowhilecount ++;

    qi.addQua(quadruple("blockend","","","",""));
    qi.addQua(quadruple("", "", "", "", "dowhilebegin_" + ss.str()));
    qi.addQua(quadruple("blockbegin","","","",""));
    statement();

    qi.addQua(quadruple("blockend","","","",""));
    if(sym==whilesym)
    {
        getsym();
        if(sym==leftparathe)
        {
            condition("dowhilebegin_" + ss.str());
            qi.addQua(quadruple("", "", "", "", "dowhileend_" + ss.str()));
            qi.addQua(quadruple("blockbegin","","","",""));
            return;
        }
    } else{
        error("Unfinished statement!!");
        skip();
    }

}

void forstatement()
{
    std::string step1,step2;
    symbolSystem stepsym;
    std::string steplength;
    qi.addQua(quadruple("blockend","","","",""));
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
                bool judge1 = st.check_variable(idp);
                if(!judge1)
                {
                    fetal("no such variable " + idp);
                }
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
                                    fetal("no such identifier");
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

                                            qi.addQua(quadruple("blockbegin","","","",""));
                                            statement();
                                            qi.addQua(quadruple("blockend","","","",""));
                                            if(stepsym == addsym)
                                            {
                                                ostringstream oss;
                                                oss << ++intercount;
                                                ostringstream osss;
                                                osss << ++ intercount;
                                                qi.addQua(quadruple("LI", "$t"+oss.str(),  steplength,"", ""));
                                                qi.addQua(quadruple("move", "$t"+osss.str(),  "convar_"+step2 ,"", ""));
                                                qi.addQua(quadruple("add", "$t"+osss.str(), "$t"+oss.str(), "$t"+osss.str(), ""));
                                                qi.addQua(quadruple("move", "result", "$t"+osss.str(), "", ""));
                                                qi.addQua(quadruple("move", "convar_"+step1, "result", "", ""));
                                            }
                                            else if(stepsym == minussym)
                                            {
                                                ostringstream oss;
                                                oss << ++intercount;
                                                ostringstream osss;
                                                osss << ++ intercount;
                                                qi.addQua(quadruple("move", "$t"+oss.str(),  "convar_"+step2 ,"", ""));
                                                qi.addQua(quadruple("LI", "$t"+osss.str(),  steplength,"", ""));
                                                qi.addQua(quadruple("sub", "$t"+osss.str(), "$t"+oss.str(), "$t"+osss.str(), ""));
                                                qi.addQua(quadruple("move", "result", "$t"+osss.str(), "", ""));
                                                qi.addQua(quadruple("move", "convar_"+step1, "result", "", ""));
                                            }
                                            qi.addQua(quadruple("j", "forloopcondition_" + ss.str(), "", "", ""));
                                            qi.addQua(quadruple("", "", "", "", "forloopend_" + ss.str()));
                                            qi.addQua(quadruple("blockbegin","","","",""));
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
            fetal("illegal beginning token");
        }
    } else
    {
        fetal("unfinished for statement");
        skip();
    }
}

void scanfstatement()
{
    if(sym == leftparathe)
    {
        getsym();
        if(sym == identifier)
        {
            bool isvar = st.check_variable(id);
            if(!isvar)
            {
                fetal("not a var");
            }
            qi.addQua(quadruple("blockend","","","",""));
            qi.addQua(quadruple("scan", id, "", "", ""));
            getsym();
            while(sym == comma)
            {
                getsym();
                if(sym != identifier)
                {
                    fetal("expected identifier");
                }
                bool isvar = st.check_variable(id);
                if(!isvar)
                {
                    fetal("not a var");
                }
                qi.addQua(quadruple("scan", id, "", "", ""));
                getsym();
            }

            qi.addQua(quadruple("blockbegin","","","",""));
            if(sym == rightparathe)
            {
                getsym();
                return;
            }
        }
    } else
    {
        error("unfinished scanf statement");
        skip();
    }
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
            qi.addQua(quadruple("blockend","","","",""));
            qi.addQua(quadruple("print","string_"+ss.str(),"","",""));
            getsym();
            if(sym == rightparathe)
            {
                getsym();
                qi.addQua(quadruple("blockbegin","","","",""));
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
                    qi.addQua(quadruple("blockbegin","","","",""));
                    getsym();
                    return;
                }
            }
        }else
        {
            qi.addQua(quadruple("blockend","","","",""));
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
                qi.addQua(quadruple("blockbegin","","","",""));
                getsym();
                return;
            }
        }
    } else
    {
        error("unfinished scanf statement");
        skip();
    }
}
void expression()
{
    int count = 0;
    charexpression = false;
    symbolSystem tmp = sym;
    int preinter,posinter;
    if(sym==addsym || sym==minussym)
    {
        getsym();
    }
    term(count); //turn negative
    preinter = posinter =intercount;
    ostringstream tss1,tss2;
    tss1 << preinter;
    if(tmp == minussym)
    {
        tss2 << ++intercount;
        preinter = posinter = intercount;
        qi.addQua(quadruple("sub","$t"+tss2.str(),"$0","$t"+tss1.str(),""));
    }

    while(sym != comma && sym!=semicolon  && sym != rightbrack && sym!=rightparathe && sym != equ && sym != let && sym != leeqt && sym != neq && sym != grt && sym != greqt)
    {
        if(sym == addsym || sym == minussym)
        {
            symbolSystem tmp = sym;
            getsym();
            term(count);
            posinter = intercount;
            if(tmp == addsym)
            {
                ostringstream ts1,ts2,ts3;
                ts1 << preinter;
                ts2 << posinter;
                ts3 << ++intercount ;
                qi.addQua(quadruple("add","$t"+ts3.str(),"$t"+ts1.str(),"$t"+ts2.str(),""));
            }else if(tmp == minussym)
            {
                ostringstream ts1,ts2,ts3;
                ts1 << preinter;
                ts2 << posinter;
                ts3 << ++intercount;
                qi.addQua(quadruple("sub","$t"+ts3.str(),"$t"+ts1.str(),"$t"+ts2.str(),""));
            }
            preinter = posinter = intercount;
        } else
        {
            fetal("expression wrong connector");
        }
    }
    ostringstream ss;
    ss << posinter;
    qi.addQua(quadruple("move","result","$t"+ss.str(),"",""));
}
void term(int term_count)
{
    int count = 0;
    intercount ++ ;
    int preinter,posinter;
    factor(count);
    preinter = posinter =intercount;
    while(sym==mulsym || sym==divsym)
    {
        charexpression = false;
        if(sym == mulsym || sym == divsym)
        {
            symbolSystem tmp = sym;
            getsym();
            intercount++;
            factor(count);
            posinter = intercount;
            if(tmp == mulsym)
            {
                ostringstream f1;
                ostringstream f2;
                ostringstream f3;
                f1 << preinter;
                f2 << posinter;
                f3 << ++intercount;
                qi.addQua(quadruple("mult","$t"+f1.str(),"$t"+f2.str(),"",""));
                qi.addQua(quadruple("mflo","$t"+f3.str(),"","",""));
            }
            else if(tmp == divsym)
            {
                ostringstream f1;
                ostringstream f2;
                ostringstream f3;
                f1 << preinter;
                f2 << posinter ;
                f3 << ++intercount;
                qi.addQua(quadruple("div","$t"+f1.str(),"$t"+f2.str(),"",""));
                qi.addQua(quadruple("mflo","$t"+f3.str(),"","",""));
            }
            preinter = posinter = intercount;
        } else
        {
            fetal("fetal in term");
            exit(0);
        }
    }
    ostringstream ss,fs;
    ss << term_count;
    fs << count;
    if(term_count!=0)
        charexpression = false;
}
void factor(int count)
{
    set<symbolSystem > skipset;
    funcTerm &ft = st.funcList[st.funcList.size() - 1];
    ostringstream co;
    co << intercount;
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
            } else
                charexpression = false;
            ostringstream coi;
            coi << ++intercount;
            qi.addQua(quadruple("arrayvalue","$t"+coi.str(),symbol,"result",""));
            if(sym==rightbrack)
            {
                getsym();
            } else
            {
                error("unfinished arrayvalue statement");
                skip();
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
                ostringstream oss;
                oss << intercount;
                qi.addQua(quadruple("move","parameter","result","$t"+oss.str(),""));
                acount ++;
                if(sym == comma)
                {
                    getsym();
                }else if(sym != rightparathe)
                {
                    error("Illegal token!");
                    symbolSystem  ss[]={semicolon, rightbrace, rightparathe};
                    skip();
                }

            }
            symbolSystem  type = st.check_type(symbol);
            if(type == charsym && count == 0)
            {
                charexpression = true;
            } else
                charexpression = false;
            ostringstream ss;
            ss << acount;
            qi.addQua(quadruple("paraend",ss.str(),"","",""));
            ostringstream coi;
            coi << ++intercount;
            qi.addQua(quadruple("move","$t"+coi.str(),"returnvalue","",""));
            if(acount != st.funcList[pos].paralist.size())
            {
                error("unmatched parameters");
                skippara();
            }
            getsym();
        } else
        {
            int paranum = 0;
            bool isvar = st.check_variable(symbol);
            bool isconstant = st.check_constant(symbol);
            if(!isvar && !isconstant)
            {
                error("no such identifier :" + symbol);
                return;
            }
            symbolSystem type = st.check_type(symbol);
            if(type == notsym)
            {
                fetal("unknown type variable/constant " + symbol);
            }else if(type == charsym)
            {
                charexpression = true;
            } else
                charexpression = false;
            qi.addQua(quadruple("move","$t"+co.str(),"convar_"+symbol,"",""));
            return;
        }
    }
    else if(sym == number || sym==addsym || sym==minussym)
    {
        integer();
        int value = inum;
        getsym();
        ostringstream ss;
        charexpression = false;
        ss << value;
        qi.addQua(quadruple("LI","$t"+co.str(),ss.str(),"",""));

        return;
    }
    else if(sym==character)
    {
        if(count == 0)
            charexpression = true;
        else
            charexpression = false;
        ostringstream ss;
        ss << inum;
        qi.addQua(quadruple("LI","$t"+co.str(),ss.str(),"",""));
        getsym();
        return;
    }
    else if(sym == leftparathe)
    {
        getsym();
        expression();
        if(sym == rightparathe)
        {
            getsym();
            ostringstream coi;
            coi << ++intercount;
            qi.addQua(quadruple("move","$t"+coi.str(),"result","",""));
//            cout << " : factor" ;
            return;
        }
    } else
    {
        error("expected expression");
        return;
    }
}