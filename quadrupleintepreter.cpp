//
// Created by hanshiyi on 24/11/2016.
//

#include "quadrupleintepreter.h"
#include <iostream>
using namespace std;

void quadrupleintepreter::generate_target_mips() {
    quadCounter = 0;
    cout << ".data" << endl;
    for(std::vector<convarTerm>::iterator gloiter = st.convarList.begin(); gloiter!= st.convarList.end(); gloiter++)
    {
        if((*gloiter).pos == -1)
        {
            if((*gloiter).isvar)
            {
                cout << (*gloiter).id << "\t:\t" << ".space\t" << 4 << endl;
            } else
            {
                cout << (*gloiter).id << "\t:\t" << ".word\t" << (*gloiter).constValue << endl;
            }
        }
    }int count =0;
    for(std::vector<string>::iterator striter = st.stringlist.begin() ; striter!= st.stringlist.end(); striter++, count++)
    {
        cout << "string_" << count <<"\t:\t" << ".ascii\t" << "\"" << (*striter) << "\"" << endl;
    }
    for(std::vector<arrayTerm>::iterator gloiter = st.arrayList.begin(); gloiter!=st.arrayList.end(); gloiter++)
    {
        if((*gloiter).pos == -1)
        {
            cout << (*gloiter).id << "\t:\t" << ".space\t" << (*gloiter).length*4 << endl;
        }
    }
    cout << ".text "<< endl;
    string funcname;
    int paracount = 0;
    string callfunc;
    for(quadCounter = 0; quadCounter < quadrupleVec.size(); quadCounter++)
    {
            if(quadrupleVec[quadCounter].label.substr(0,10) == "funcbegin_")
            {
                funcname = quadrupleVec[quadCounter].label.substr(10);
                cout << quadrupleVec[quadCounter].label.substr(10) << ":" << endl;
                cout << "\tadd\t" << "$sp,\t" << "$sp,\t" << "-56" << endl;
                cout << "\tsw\t" << "$fp,\t" << "52($sp)" << endl;
                cout << "\tsw\t" << "$a3,\t" << "48($sp)" << endl;
                cout << "\tsw\t" << "$a2,\t" << "44($sp)" << endl;
                cout << "\tsw\t" << "$a1,\t" << "40($sp)" << endl;
                cout << "\tsw\t" << "$a0,\t" << "36($sp)" << endl;
                cout << "\tsw\t" << "$s7,\t" << "32($sp)" << endl;
                cout << "\tsw\t" << "$s6,\t" << "28($sp)" << endl;
                cout << "\tsw\t" << "$s5,\t" << "24($sp)" << endl;
                cout << "\tsw\t" << "$s4,\t" << "20($sp)" << endl;
                cout << "\tsw\t" << "$s3,\t" << "16($sp)" << endl;
                cout << "\tsw\t" << "$s2,\t" << "12($sp)" << endl;
                cout << "\tsw\t" << "$s1,\t" << "8($sp)" << endl;
                cout << "\tsw\t" << "$s0,\t" << "4($sp)" << endl;
                cout << "\tsw\t" << "$ra,\t" << "0($sp)" << endl;
                for(std::vector<funcTerm>::iterator funciter = st.funcList.begin(); funciter != st.funcList.end(); funciter++)
                {
                    funcTerm &currentFuc = (*funciter);
                    if(quadrupleVec[quadCounter].label.substr(10) == currentFuc.id)
                    {
                        cout << "\tadd\t" << "$sp,\t" << "$sp,\t" << currentFuc.shift*(-1) << endl;
                        cout << "\tadd\t" << "$fp,\t" << "$sp,\t" << "$0" << endl;
                        for(std::vector<convarTerm>::iterator iter = st.convarList.begin(); iter != st.convarList.end(); iter++)
                        {
                            if(!(*iter).isvar && (*iter).pos == st.find_funcname(currentFuc.id))
                            {
                                cout << "\taddi\t" << "$t4,\t" << "$0,\t" << (*iter).constValue << endl;
                                cout << "\tsw\t" << "$t4,\t" << (*iter).shift << "($fp)" << endl;
                            }
                        }
                    }
                }
            }
            else if(quadrupleVec[quadCounter].label.substr(0,8) == "funcend_")
            {
                for(std::vector<funcTerm>::iterator funciter = st.funcList.begin(); funciter != st.funcList.end(); funciter++)
                {
                    funcTerm &currentFuc = (*funciter);
                    if(quadrupleVec[quadCounter].label.substr(10) == currentFuc.id)
                    {
                        cout << "\tadd\t" << "$sp,\t" << "$sp,\t" << currentFuc.shift << endl;
                    }
                }
                cout << "\tlw\t" << "$fp,\t" << "52($sp)" << endl;
                cout << "\tlw\t" << "$a3,\t" << "48($sp)" << endl;
                cout << "\tlw\t" << "$a2,\t" << "44($sp)" << endl;
                cout << "\tlw\t" << "$a1,\t" << "40($sp)" << endl;
                cout << "\tlw\t" << "$a0,\t" << "36($sp)" << endl;
                cout << "\tlw\t" << "$s7,\t" << "32($sp)" << endl;
                cout << "\tlw\t" << "$s6,\t" << "28($sp)" << endl;
                cout << "\tlw\t" << "$s5,\t" << "24($sp)" << endl;
                cout << "\tlw\t" << "$s4,\t" << "20($sp)" << endl;
                cout << "\tlw\t" << "$s3,\t" << "16($sp)" << endl;
                cout << "\tlw\t" << "$s2,\t" << "12($sp)" << endl;
                cout << "\tlw\t" << "$s1,\t" << "8($sp)" << endl;
                cout << "\tlw\t" << "$s0,\t" << "4($sp)" << endl;
                cout << "\tlw\t" << "$ra,\t" << "0($sp)" << endl;
                cout << "\tadd\t" << "$sp,\t" << "$sp,\t" << "56" << endl;
                cout << "\tjr\t" << "$ra" << endl;
                cout << "\tnop"<< endl;
            } else if(quadrupleVec[quadCounter].label != "")
            {
                cout << "\t" << quadrupleVec[quadCounter].label << ":" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "move")
            {
                if(quadrupleVec[quadCounter].target.substr(0,7) == "convar_")
                {
                    string convarname = quadrupleVec[quadCounter].target.substr(7);
                    bool isglo = st.check_global(convarname,funcname);
                    if(isglo)
                    {
                        cout << "\tsw\t" << "$s0,\t" << convarname << endl;
                    } else
                    {
                        int shift = st.findidshift(convarname, funcname);
                        if(shift == -1)
                        {
                            shift = st.findidparano(convarname, funcname);
                            cout << "\tadd\t" << "$a"<< shift <<",\t" << "$s0,\t" << "$0" << endl;
                        }
                        else{
                            cout << "\tsw\t" << "$s0,\t" << shift << "($fp)" << endl;
                        }
                    }
                }
                else if(quadrupleVec[quadCounter].target.substr(0,6) == "array_")
                {
                    string arrayname = quadrupleVec[quadCounter].target.substr(6);
                    bool isglo = st.check_global(arrayname, funcname);
                    if(isglo)
                    {
                        cout << "\tla\t" << "$t1,\t" << arrayname << endl;
                        cout << "\tlw\t" << "$t0,\t" << "0($sp)" << endl;
                        cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                        cout << "\tsll\t" << "$t0,\t" << "$t0,\t" << 2 << endl;
                        cout << "\tadd\t" << "$t0,\t" << "$t0,\t" << "$t1" << endl;
                    }
                    else
                    {
                        int shift = st.findidshift(arrayname, funcname);
                        cout << "\tlw\t" << "$t0,\t" << "0($sp)" << endl;
                        cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                        cout << "\taddi\t" << "$t1\t" << "$fp,\t" << shift << endl;
                        cout << "\tsll\t" << "$t0,\t" << "$t0,\t" << 2 << endl;
                        cout << "\tadd\t" << "$t0,\t" << "$t0,\t" << "$t1" << endl;
                    }
                    cout << "\tsw\t" << "$s0,\t" << "0($t0)" << endl;
                }
                else if(quadrupleVec[quadCounter].op1.substr(0,7) == "convar_")
                {
                    string convarname = quadrupleVec[quadCounter].op1.substr(7);
                    bool isglo = st.check_global(convarname,funcname);
                    if(isglo)
                    {
                        cout << "\tlw\t" << "$s0,\t" << convarname << endl;
                        cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                        cout << "\tsw\t" << "$s0,\t" << "0($sp)" << endl;
                    } else
                    {
                        int shift = st.findidshift(convarname, funcname);
                        if(shift == -1)
                        {
                            shift = st.findidparano(convarname,funcname);
                            cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                            cout << "\tsw\t" << "$a"<< shift <<",\t" << "0($sp)" << endl;
                        } else
                        {
                            cout << "\tlw\t" << "$s0,\t" << shift << "($fp)" << endl;
                            cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                            cout << "\tsw\t" << "$s0,\t" << "0($sp)" << endl;
                        }

                    }
                }
                else if(quadrupleVec[quadCounter].target == "result")
                {
                    cout << "\tlw\t" << "$s0" <<",\t" << "0($sp)" << endl;
                    cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                }
                else if(quadrupleVec[quadCounter].op1 == "result")
                {
                    if(quadrupleVec[quadCounter].target == "parameter" && paracount<4)
                    {
                        cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                        cout << "\tadd\t" << "$a" << paracount << ",\t"<< "$s0" <<",\t" << "$0" << endl;
                        paracount ++;
                    }else{
                        paracount ++;
                        cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                        cout << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
                    }
                }
                else if(quadrupleVec[quadCounter].target == "compare1")
                {
                    cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    cout << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
                }
                else if(quadrupleVec[quadCounter].target == "compare2")
                {
                    cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    cout << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
                }else if(quadrupleVec[quadCounter].op1 == "returnvalue")
                {
                    cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    cout << "\tsw\t" << "$v0" <<",\t" << "0($sp)" << endl;
                }

            }else if(quadrupleVec[quadCounter].op == "add")
            {
                cout << "\tlw\t" << "$s2,\t" << "0($sp)" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                cout << "\tlw\t" << "$s1,\t" << "0($sp)" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                cout << "\tadd\t" << "$s0,\t" <<"$s1,\t" <<"$s2" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                cout << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;

            }else if(quadrupleVec[quadCounter].op == "sub")
            {
                cout << "\tlw\t" << "$s2,\t" << "0($sp)" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                cout << "\tlw\t" << "$s1,\t" << "0($sp)" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                cout << "\tsub\t" << "$s0,\t" <<"$s1,\t" <<"$s2" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                cout << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
            }else if(quadrupleVec[quadCounter].op == "mult")
            {
                cout << "\tlw\t" << "$s2,\t" << "0($sp)" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                cout << "\tlw\t" << "$s1,\t" << "0($sp)" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                cout << "\tmult\t" <<"$s1,\t" <<"$s2" << endl;

            }else if(quadrupleVec[quadCounter].op == "div")
            {
                cout << "\tlw\t" << "$s2,\t" << "0($sp)" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                cout << "\tlw\t" << "$s1,\t" << "0($sp)" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                cout << "\tdiv\t" <<"$s1,\t" <<"$s2" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "LI")
            {
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                cout <<  "\taddi\t" << "$s0,\t" << "$0,\t" << quadrupleVec[quadCounter].op1 << endl;
                cout << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "mflo")
            {
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                cout << "\tmflo\t" << "$s0" << endl;
                cout << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "arrayaddr")
            {
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                cout << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "call")
            {
                callfunc = quadrupleVec[quadCounter].target;
            }
            else if(quadrupleVec[quadCounter].op == "parabegin")
            {
                paracount = 0;
            }
            else if(quadrupleVec[quadCounter].op == "paraend")
            {
                cout << "\tjal\t" << callfunc << endl;
                cout << "\tnop\t" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << paracount*4 << endl;
            }
            else if(quadrupleVec[quadCounter].op == "j")
            {
                cout << "\tj\t" << quadrupleVec[quadCounter].target << endl;
            }else if(quadrupleVec[quadCounter].op == "beq" || quadrupleVec[quadCounter].op == "blt" || quadrupleVec[quadCounter].op == "ble" ||
                    quadrupleVec[quadCounter].op == "bne" || quadrupleVec[quadCounter].op == "bgt"  || quadrupleVec[quadCounter].op == "bge")
            {
                cout << "\tlw\t" << "$s2,\t" << "0($sp)" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                cout << "\tlw\t" << "$s1,\t" << "0($sp)" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                cout << "\t"<< quadrupleVec[quadCounter].op <<"\t" <<"$s1,\t" <<"$s2,\t" << quadrupleVec[quadCounter].op2 << endl;
            }
            else if(quadrupleVec[quadCounter].op == "return")
            {
                cout << "\tadd\t" << "$v0,\t" << "$s0,\t" << "$0" << endl;
                cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << paracount*4 << endl;
            }else if(quadrupleVec[quadCounter].op == "scan")
            {
                cout << "\tli\t" << "$v0" << 5 << endl;
                cout << "syscall" << endl;
                string scanname = quadrupleVec[quadCounter].target;
                bool isglo = st.check_global(scanname,funcname);
                if(isglo)
                {
                    cout << "\tsw\t" << "$v0,\t" << scanname << endl;
                } else
                {
                    int shift = st.findidshift(scanname, funcname);
                    if(shift == -1)
                    {
                        shift = st.findidparano(scanname, funcname);
                        cout << "\tadd\t" << "$a"<< shift <<",\t" << "$v0,\t" << "$0" << endl;
                    }
                    else{
                        cout << "\tsw\t" << "$v0,\t" << shift << "($fp)" << endl;
                    }
                }
            }else if(quadrupleVec[quadCounter].op == "print")
            {
                if(quadrupleVec[quadCounter].target == "result")
                {
                    cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    cout << "\tsw\t" << "$a0" <<",\t" << "0($sp)" << endl;
                    cout << "\taddi\t" << "$a0,\t" << "$s0,\t" << 0 << endl;
                    cout << "\tli\t" << "$v0\t" << 1 << endl;
                    cout << "\tlw\t" << "$a0,\t" << "0($sp)" << endl;
                    cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                } else
                {
                    cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    cout << "\tsw\t" << "$a0" <<",\t" << "0($sp)" << endl;
                    cout << "\tla\t" << "$a0\t" << quadrupleVec[quadCounter].target << endl;
                    cout << "\tli\t" << "$v0\t" << 1 << endl;
                    cout << "\tlw\t" << "$a0,\t" << "0($sp)" << endl;
                    cout << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                }

            }

    }

}