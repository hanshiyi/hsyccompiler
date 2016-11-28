//
// Created by hanshiyi on 24/11/2016.
//

#include "quadrupleintepreter.h"
#include <iostream>
#include <fstream>
#include <ios>

using namespace std;

void quadrupleintepreter::generate_target_mips() {
    quadCounter = 0;
    std::cout << "the directory that your want to store your output file"<< endl;
    string dir;
    cin >> dir;
    ofstream os;
    string newdir = dir + "output.asm";
    os.open(newdir.c_str());
    cout << os.is_open();
    os << ".data" << endl;
    for(std::vector<convarTerm>::iterator gloiter = st.convarList.begin(); gloiter!= st.convarList.end(); gloiter++)
    {
        if((*gloiter).pos == -1)
        {
            if((*gloiter).isvar)
            {
                os << (*gloiter).id << "\t:\t" << ".space\t" << 4 << endl;
            } else
            {
                os << (*gloiter).id << "\t:\t" << ".word\t" << (*gloiter).constValue << endl;
            }
        }
    }int count =0;

    for(std::vector<arrayTerm>::iterator gloiter = st.arrayList.begin(); gloiter!=st.arrayList.end(); gloiter++)
    {
        if((*gloiter).pos == -1)
        {
            os << (*gloiter).id << "\t:\t" << ".space\t" << (*gloiter).length*4 << endl;
        }
    }
    for(std::vector<string>::iterator striter = st.stringlist.begin() ; striter!= st.stringlist.end(); striter++, count++)
    {
        os << "string_" << count <<"\t:\t" << ".asciiz\t" << "\"" << (*striter) << "\"" << endl;
    }
    os << ".text "<< endl;
    os << "j\tmain" << endl;
    string funcname;
    int paracount = 0;
    string callfunc;
    for(quadCounter = 0; quadCounter < quadrupleVec.size(); quadCounter++)
    {
            if(quadrupleVec[quadCounter].label.substr(0,10) == "funcbegin_" && quadrupleVec[quadCounter].label != "funcbegin_main")
            {
                funcname = quadrupleVec[quadCounter].label.substr(10);
                os << quadrupleVec[quadCounter].label.substr(10) << ":" << endl;
                os << "\tadd\t" << "$sp,\t" << "$sp,\t" << "-56" << endl;
                os << "\tsw\t" << "$fp,\t" << "52($sp)" << endl;
                os << "\tsw\t" << "$a3,\t" << "48($sp)" << endl;
                os << "\tsw\t" << "$a2,\t" << "44($sp)" << endl;
                os << "\tsw\t" << "$a1,\t" << "40($sp)" << endl;
                os << "\tsw\t" << "$a0,\t" << "36($sp)" << endl;
                os << "\tsw\t" << "$s7,\t" << "32($sp)" << endl;
                os << "\tsw\t" << "$s6,\t" << "28($sp)" << endl;
                os << "\tsw\t" << "$s5,\t" << "24($sp)" << endl;
                os << "\tsw\t" << "$s4,\t" << "20($sp)" << endl;
                os << "\tsw\t" << "$s3,\t" << "16($sp)" << endl;
                os << "\tsw\t" << "$s2,\t" << "12($sp)" << endl;
                os << "\tsw\t" << "$s1,\t" << "8($sp)" << endl;
                os << "\tsw\t" << "$s0,\t" << "4($sp)" << endl;
                os << "\tsw\t" << "$ra,\t" << "0($sp)" << endl;
                for(std::vector<funcTerm>::iterator funciter = st.funcList.begin(); funciter != st.funcList.end(); funciter++)
                {
                    funcTerm &currentFuc = (*funciter);
                    if(quadrupleVec[quadCounter].label.substr(10) == currentFuc.id)
                    {
                        os << "\tadd\t" << "$sp,\t" << "$sp,\t" << currentFuc.shift*(-1) << endl;
                        os << "\tadd\t" << "$fp,\t" << "$sp,\t" << "$0" << endl;
                        for(std::vector<convarTerm>::iterator iter = st.convarList.begin(); iter != st.convarList.end(); iter++)
                        {
                            if(!(*iter).isvar && (*iter).pos == st.find_funcname(currentFuc.id))
                            {
                                os << "\taddi\t" << "$t4,\t" << "$0,\t" << (*iter).constValue << endl;
                                os << "\tsw\t" << "$t4,\t" << (*iter).shift << "($fp)" << endl;
                            }
                        }
                    }
                }
            }
            else if(quadrupleVec[quadCounter].label.substr(0,8) == "funcend_"&& funcname!="main")
            {
                for(std::vector<funcTerm>::iterator funciter = st.funcList.begin(); funciter != st.funcList.end(); funciter++)
                {
                    funcTerm &currentFuc = (*funciter);
                    if(funcname == currentFuc.id)
                    {
                        os << "\tadd\t" << "$sp,\t" << "$sp,\t" << currentFuc.shift << endl;
                    }
                }
                os << "\tlw\t" << "$fp,\t" << "52($sp)" << endl;
                os << "\tlw\t" << "$a3,\t" << "48($sp)" << endl;
                os << "\tlw\t" << "$a2,\t" << "44($sp)" << endl;
                os << "\tlw\t" << "$a1,\t" << "40($sp)" << endl;
                os << "\tlw\t" << "$a0,\t" << "36($sp)" << endl;
                os << "\tlw\t" << "$s7,\t" << "32($sp)" << endl;
                os << "\tlw\t" << "$s6,\t" << "28($sp)" << endl;
                os << "\tlw\t" << "$s5,\t" << "24($sp)" << endl;
                os << "\tlw\t" << "$s4,\t" << "20($sp)" << endl;
                os << "\tlw\t" << "$s3,\t" << "16($sp)" << endl;
                os << "\tlw\t" << "$s2,\t" << "12($sp)" << endl;
                os << "\tlw\t" << "$s1,\t" << "8($sp)" << endl;
                os << "\tlw\t" << "$s0,\t" << "4($sp)" << endl;
                os << "\tlw\t" << "$ra,\t" << "0($sp)" << endl;
                os << "\tadd\t" << "$sp,\t" << "$sp,\t" << "56" << endl;
                os << "\tjr\t" << "$ra" << endl;
                os << "\tnop"<< endl;
            }else if(quadrupleVec[quadCounter].label == "funcbegin_main")
            {
                funcname = quadrupleVec[quadCounter].label.substr(10);
                os << quadrupleVec[quadCounter].label.substr(10) << ":" << endl;
                for(std::vector<funcTerm>::iterator funciter = st.funcList.begin(); funciter != st.funcList.end(); funciter++)
                {
                    funcTerm &currentFuc = (*funciter);
                    if(quadrupleVec[quadCounter].label.substr(10) == currentFuc.id)
                    {
                        os << "\tadd\t" << "$sp,\t" << "$sp,\t" << currentFuc.shift*(-1) << endl;
                        os << "\tadd\t" << "$fp,\t" << "$sp,\t" << "$0" << endl;
                        for(std::vector<convarTerm>::iterator iter = st.convarList.begin(); iter != st.convarList.end(); iter++)
                        {
                            if(!(*iter).isvar && (*iter).pos == st.find_funcname(currentFuc.id))
                            {
                                os << "\taddi\t" << "$t4,\t" << "$0,\t" << (*iter).constValue << endl;
                                os << "\tsw\t" << "$t4,\t" << (*iter).shift << "($fp)" << endl;
                            }
                        }
                    }
                }
            }
            else if(quadrupleVec[quadCounter].label != "" && quadrupleVec[quadCounter].label != "funcend_main" )
            {
                os << "\t" << quadrupleVec[quadCounter].label << ":" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "move")
            {
                if(quadrupleVec[quadCounter].target.substr(0,7) == "convar_")
                {
                    string convarname = quadrupleVec[quadCounter].target.substr(7);
                    bool isglo = st.check_global(convarname,funcname);
                    if(isglo)
                    {
                        os << "\tsw\t" << "$s0,\t" << convarname << endl;
                    } else
                    {
                        int shift = st.findidshift(convarname, funcname);
                        if(shift == -1)
                        {
                            shift = st.findidparano(convarname, funcname);
                            os << "\tadd\t" << "$a"<< shift <<",\t" << "$s0,\t" << "$0" << endl;
                        }
                        else{
                            os << "\tsw\t" << "$s0,\t" << shift << "($fp)" << endl;
                        }
                    }
                }
                else if(quadrupleVec[quadCounter].target.substr(0,6) == "array_")
                {
                    string arrayname = quadrupleVec[quadCounter].target.substr(6);
                    bool isglo = st.check_global(arrayname, funcname);
                    if(isglo)
                    {
                        os << "\tla\t" << "$t1,\t" << arrayname << endl;
                        os << "\tlw\t" << "$t0,\t" << "0($sp)" << endl;
                        os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                        os << "\tsll\t" << "$t0,\t" << "$t0,\t" << 2 << endl;
                        os << "\tadd\t" << "$t0,\t" << "$t0,\t" << "$t1" << endl;
                    }
                    else
                    {
                        int shift = st.findidshift(arrayname, funcname);
                        os << "\tlw\t" << "$t0,\t" << "0($sp)" << endl;
                        os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                        os << "\taddi\t" << "$t1\t" << "$fp,\t" << shift << endl;
                        os << "\tsll\t" << "$t0,\t" << "$t0,\t" << 2 << endl;
                        os << "\tadd\t" << "$t0,\t" << "$t0,\t" << "$t1" << endl;
                    }
                    os << "\tsw\t" << "$s0,\t" << "0($t0)" << endl;
                }
                else if(quadrupleVec[quadCounter].op1.substr(0,7) == "convar_")
                {
                    string convarname = quadrupleVec[quadCounter].op1.substr(7);
                    bool isglo = st.check_global(convarname,funcname);
                    if(isglo)
                    {
                        os << "\tlw\t" << "$s0,\t" << convarname << endl;
                        os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                        os << "\tsw\t" << "$s0,\t" << "0($sp)" << endl;
                    } else
                    {
                        int shift = st.findidshift(convarname, funcname);
                        if(shift == -1)
                        {
                            shift = st.findidparano(convarname,funcname);
                            os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                            os << "\tsw\t" << "$a"<< shift <<",\t" << "0($sp)" << endl;
                        } else
                        {
                            os << "\tlw\t" << "$s0,\t" << shift << "($fp)" << endl;
                            os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                            os << "\tsw\t" << "$s0,\t" << "0($sp)" << endl;
                        }

                    }
                }
                else if(quadrupleVec[quadCounter].target == "result")
                {
                    os << "\tlw\t" << "$s0" <<",\t" << "0($sp)" << endl;
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                }
                else if(quadrupleVec[quadCounter].op1 == "result")
                {
                    if(quadrupleVec[quadCounter].target == "parameter" && paracount<4)
                    {
                        os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                        os << "\tadd\t" << "$a" << paracount << ",\t"<< "$s0" <<",\t" << "$0" << endl;
                        paracount ++;
                    }else{
                        paracount ++;
                        os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                        os << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
                    }
                }
                else if(quadrupleVec[quadCounter].target == "compare1")
                {
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    os << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
                }
                else if(quadrupleVec[quadCounter].target == "compare2")
                {
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    os << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
                }else if(quadrupleVec[quadCounter].op1 == "returnvalue")
                {
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    os << "\tsw\t" << "$v0" <<",\t" << "0($sp)" << endl;
                }

            }else if(quadrupleVec[quadCounter].op == "add")
            {
                os << "\tlw\t" << "$s2,\t" << "0($sp)" << endl;
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                os << "\tlw\t" << "$s1,\t" << "0($sp)" << endl;
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                os << "\tadd\t" << "$s0,\t" <<"$s1,\t" <<"$s2" << endl;
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                os << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;

            }else if(quadrupleVec[quadCounter].op == "sub")
            {
                if(quadrupleVec[quadCounter].op1 == "$0")
                {
                    os << "\tlw\t" << "$s2,\t" << "0($sp)" << endl;
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                    os << "\tsub\t" << "$s0,\t" <<"$0,\t" <<"$s2" << endl;
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    os << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
                } else
                {
                    os << "\tlw\t" << "$s2,\t" << "0($sp)" << endl;
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                    os << "\tlw\t" << "$s1,\t" << "0($sp)" << endl;
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                    os << "\tsub\t" << "$s0,\t" <<"$s1,\t" <<"$s2" << endl;
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    os << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
                }
            }else if(quadrupleVec[quadCounter].op == "mult")
            {
                os << "\tlw\t" << "$s2,\t" << "0($sp)" << endl;
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                os << "\tlw\t" << "$s1,\t" << "0($sp)" << endl;
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                os << "\tmult\t" <<"$s1,\t" <<"$s2" << endl;

            }else if(quadrupleVec[quadCounter].op == "div")
            {
                os << "\tlw\t" << "$s2,\t" << "0($sp)" << endl;
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                os << "\tlw\t" << "$s1,\t" << "0($sp)" << endl;
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                os << "\tdiv\t" <<"$s1,\t" <<"$s2" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "LI")
            {
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                os <<  "\taddi\t" << "$s0,\t" << "$0,\t" << quadrupleVec[quadCounter].op1 << endl;
                os << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "mflo")
            {
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                os << "\tmflo\t" << "$s0" << endl;
                os << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "arrayaddr")
            {
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                os << "\tsw\t" << "$s0" <<",\t" << "0($sp)" << endl;
            }else if(quadrupleVec[quadCounter].op == "arrayvalue")
            {
                string arrayname = quadrupleVec[quadCounter].op1;
                bool isglo = st.check_global(arrayname, funcname);
                if(isglo)
                {
                    os << "\tla\t" << "$t1,\t" << arrayname << endl;
                    os << "\tsll\t" << "$s0,\t" << "$s0,\t" << 2 << endl;
                    os << "\tadd\t" << "$t0,\t" << "$t1,\t" << "$s0" << endl;
                    os << "\tlw\t" << "$s0,\t" << "0($t0)" << endl;
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    os << "\tsw\t" << "$s0,\t" << "0($sp)" << endl;
                }
                else
                {
                    int shift = st.findidshift(arrayname, funcname);
                    os << "\taddi\t" << "$t1,\t" << "$fp,\t" << shift << endl;
                    os << "\tsll\t" << "$s0,\t" << "$s0,\t" << 2 << endl;
                    os << "\tadd\t" << "$t0,\t" << "$t1,\t" << "$s0" << endl;
                    os << "\tlw\t" << "$s0,\t" << "0($t0)" << endl;
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    os << "\tsw\t" << "$s0,\t" << "0($sp)" << endl;
                }
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
                os << "\tjal\t" << callfunc << endl;
                os << "\tnop\t" << endl;
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << paracount*4 << endl;
                for(std::vector<funcTerm>::iterator funciter = st.funcList.begin(); funciter != st.funcList.end(); funciter++)
                {
                    funcTerm &currentFuc = (*funciter);
                    if(funcname == currentFuc.id)
                    {
                        os << "\tadd\t" << "$sp,\t" << "$sp,\t" << currentFuc.shift*(-1) << endl;
                        os << "\tlw\t" << "$a3,\t" << currentFuc.shift + 48 << "($fp)" << endl;
                        os << "\tlw\t" << "$a2,\t" << currentFuc.shift + 44 << "($fp)" << endl;
                        os << "\tlw\t" << "$a1,\t" << currentFuc.shift + 40 << "($fp)" << endl;
                        os << "\tlw\t" << "$a0,\t" << currentFuc.shift + 36 << "($fp)" << endl;
                    }
                }
            }
            else if(quadrupleVec[quadCounter].op == "j")
            {
                os << "\tj\t" << quadrupleVec[quadCounter].target << endl;
            }else if(quadrupleVec[quadCounter].op == "beq" || quadrupleVec[quadCounter].op == "blt" || quadrupleVec[quadCounter].op == "ble" ||
                    quadrupleVec[quadCounter].op == "bne" || quadrupleVec[quadCounter].op == "bgt"  || quadrupleVec[quadCounter].op == "bge")
            {
                os << "\tlw\t" << "$s2,\t" << "0($sp)" << endl;
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                os << "\tlw\t" << "$s1,\t" << "0($sp)" << endl;
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                os << "\t"<< quadrupleVec[quadCounter].op <<"\t" <<"$s1,\t" <<"$s2,\t" << quadrupleVec[quadCounter].op2 << endl;
            }
            else if(quadrupleVec[quadCounter].op == "return")
            {
                os << "\tadd\t" << "$v0,\t" << "$s0,\t" << "$0" << endl;
            }else if(quadrupleVec[quadCounter].op == "scan")
            {
                os << "\tli\t" << "$v0\t" << 5 << endl;
                os << "\tsyscall" << endl;
                string scanname = quadrupleVec[quadCounter].target;
                bool isglo = st.check_global(scanname,funcname);
                if(isglo)
                {
                    os << "\tsw\t" << "$v0,\t" << scanname << endl;
                } else
                {
                    int shift = st.findidshift(scanname, funcname);
                    if(shift == -1)
                    {
                        shift = st.findidparano(scanname, funcname);
                        os << "\tadd\t" << "$a"<< shift <<",\t" << "$v0,\t" << "$0" << endl;
                    }
                    else{
                        os << "\tsw\t" << "$v0,\t" << shift << "($fp)" << endl;
                    }
                }
            }else if(quadrupleVec[quadCounter].op == "print")
            {
                if(quadrupleVec[quadCounter].target == "result")
                {
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    os << "\tsw\t" << "$a0" <<",\t" << "0($sp)" << endl;
                    os << "\taddi\t" << "$a0,\t" << "$s0,\t" << 0 << endl;
                    os << "\tli\t" << "$v0\t" << 1 << endl;
                    os << "\tsyscall" << endl;
                    os << "\tlw\t" << "$a0,\t" << "0($sp)" << endl;
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                } else
                {
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                    os << "\tsw\t" << "$a0" <<",\t" << "0($sp)" << endl;
                    os << "\tla\t" << "$a0\t" << quadrupleVec[quadCounter].target << endl;
                    os << "\tli\t" << "$v0\t" << 4 << endl;
                    os << "\tsyscall" << endl;
                    os << "\tlw\t" << "$a0,\t" << "0($sp)" << endl;
                    os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                }

            }

    }

}