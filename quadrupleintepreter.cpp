//
// Created by hanshiyi on 24/11/2016.
//

#include "quadrupleintepreter.h"
#include "syntax.h"
#include <iostream>
#include <fstream>
#include <ios>
#include <map>
using namespace std;

void quadrupleintepreter::delete_public_expression()
{
    int begin,end;
    for(quadCounter = 0; quadCounter < quadrupleVec.size(); quadCounter++) {
        if(quadrupleVec[quadCounter].op == "blockbegin")
        {
            begin = quadCounter;
            while(quadrupleVec[quadCounter].op != "blockend")
            {
//                cout << quadrupleVec[quadCounter].toString() << endl;
                quadCounter++;
            }
//            cout << quadrupleVec[quadCounter].toString() << endl;
            end = quadCounter;
            bool drawable = true,cal = false;
            for (int i = begin; i < end; i++)
            {
                if(quadrupleVec[i].op=="call" || quadrupleVec[i].op=="arrayaddr" || quadrupleVec[i].op=="arrayvalue")
                {
                    drawable=false;
                }
                if(quadrupleVec[i].op=="mult" || quadrupleVec[i].op=="div" || quadrupleVec[i].op=="sub" || quadrupleVec[i].op=="add")
                {
                    cal=true;
                }
                if(quadrupleVec[i].op1=="$0" )
                {
                    drawable=false;
                }
            }
            if(begin == end-1)
            {
                drawable = false;
            }
            if (drawable && cal)
            {
                DAG_optimize(begin, end);
            }
        }
    }
}
int  quadrupleintepreter::stringToInt(std::string s) {
    return atoi(s.c_str())*4;
}
void quadrupleintepreter::DAG_optimize(int begin, int end)
{
    vector<dagnode> nodeVec;
    map<string, int > nodemap;
    int drawbegin = begin, drawend = begin;
    for (int i = begin; i < end; i++)
    {
        if(quadrupleVec[i].op == "move")
        {
            if(quadrupleVec[i].op1.substr(0,7) == "convar_")
            {
                string varname = quadrupleVec[i].op1.substr(7);
                if(nodemap.find(quadrupleVec[i].op1) == nodemap.end())
                {
                    nodeVec.push_back(dagnode());
                    dagnode &lastnode = nodeVec[nodeVec.size()-1];
                    lastnode.isleaf = true;
                    lastnode.cal = quadrupleVec[i].op1;
                    lastnode.temp = quadrupleVec[i].target;
                    nodemap[quadrupleVec[i].op1] = nodeVec.size() - 1;
                    nodemap[quadrupleVec[i].target] = nodeVec.size() - 1;
                } else
                {
                    nodemap[quadrupleVec[i].target] = nodemap[quadrupleVec[i].op1];
                }
            }
            else if (quadrupleVec[i].target.substr(0,7) == "convar_")
            {
                nodemap[quadrupleVec[i].target] = nodemap["result"];
            } else if (quadrupleVec[i].target == "result")
            {
                nodemap["result"] = nodemap[quadrupleVec[i].op1];
            }else if (quadrupleVec[i].op1 == "result")
            {
                nodemap[quadrupleVec[i].target] = nodemap["result"];
            }
        }else if (quadrupleVec[i].op == "LI")
        {
            nodeVec.push_back(dagnode());
            dagnode &lastnode = nodeVec[nodeVec.size()-1];
            lastnode.isleaf = true;
            lastnode.temp = quadrupleVec[i].target;
            lastnode.cal = quadrupleVec[i].op1;
            lastnode.isconstant = true;
            nodemap[quadrupleVec[i].op1] = nodeVec.size() - 1;
            nodemap[quadrupleVec[i].target] = nodeVec.size() - 1;
        }else if (quadrupleVec[i].op == "add" || quadrupleVec[i].op == "sub")
        {
            bool found = false;

            for (int iter = 0; iter < nodeVec[nodemap[quadrupleVec[i].op1]].parent.size(); iter++)
            {
                int lson  = nodeVec[nodeVec[nodemap[quadrupleVec[i].op1]].parent[iter]].leftson;
                int rson  = nodeVec[nodeVec[nodemap[quadrupleVec[i].op1]].parent[iter]].rightson;
                if(lson == nodemap[quadrupleVec[i].op1] && nodemap[quadrupleVec[i].op2] == rson
                   && nodeVec[nodeVec[nodemap[quadrupleVec[i].op1]].parent[iter]].cal == quadrupleVec[i].op)
                {
                    found = true;
                    nodemap[quadrupleVec[i].target] = nodeVec[nodemap[quadrupleVec[i].op1]].parent[iter];
                    break;
                }
            };
            if(!found){
                nodeVec.push_back(dagnode());
                dagnode &lastnode = nodeVec[nodeVec.size()-1];
                lastnode.cal = quadrupleVec[i].op;
                lastnode.temp = quadrupleVec[i].target;
                nodeVec[nodemap[quadrupleVec[i].op1]].parent.push_back(nodeVec.size()-1);
                nodeVec[nodemap[quadrupleVec[i].op2]].parent.push_back(nodeVec.size()-1);
                lastnode.leftson = nodemap[quadrupleVec[i].op1];
                lastnode.rightson = nodemap[quadrupleVec[i].op2];
                nodemap[quadrupleVec[i].target] = nodeVec.size() - 1;
            }

        }
        else if (quadrupleVec[i].op == "mult" || quadrupleVec[i].op == "div")
        {
            bool found = false;

            for (int iter = 0; iter < nodeVec[nodemap[quadrupleVec[i].target]].parent.size(); iter++)
            {
                int lson  = nodeVec[nodeVec[nodemap[quadrupleVec[i].target]].parent[iter]].leftson;
                int rson  = nodeVec[nodeVec[nodemap[quadrupleVec[i].target]].parent[iter]].rightson;
                if(lson == nodemap[quadrupleVec[i].target] && nodemap[quadrupleVec[i].op1] == rson
                   && nodeVec[nodeVec[nodemap[quadrupleVec[i].target]].parent[iter]].cal == quadrupleVec[i].op)
                {
                    found = true;
                    int spot = nodeVec[nodemap[quadrupleVec[i].target]].parent[iter];
                    i++;
                    nodemap[quadrupleVec[i].target] = spot;
                    break;
                }
            };
            if (!found){
                nodeVec.push_back(dagnode());
                dagnode &lastnode = nodeVec[nodeVec.size()-1];
                lastnode.cal = quadrupleVec[i].op;
                nodeVec[nodemap[quadrupleVec[i].target]].parent.push_back(nodeVec.size()-1);
                nodeVec[nodemap[quadrupleVec[i].op1]].parent.push_back(nodeVec.size()-1);
                lastnode.leftson = nodemap[quadrupleVec[i].target];
                lastnode.rightson = nodemap[quadrupleVec[i].op1];
                nodemap[quadrupleVec[i].target] = nodeVec.size() - 1;
                i++;
                lastnode.temp = quadrupleVec[i].target;
                nodemap[quadrupleVec[i].target] = nodeVec.size() - 1;
            }
        }else
        {
//            cout << "fuck" << quadrupleVec[i].toString() << endl;
        }
    }
//    int i =0;
//    for(vector<dagnode>::iterator iter = nodeVec.begin(); iter!=nodeVec.end(); iter++,i++)
//    {
////        cout << i << (*iter).toString() << endl;
//    }
//    for(map<string,int>::iterator iter = nodemap.begin(); iter!=nodemap.end(); iter++)
//    {
//        cout << (*iter).first << "\t" << (*iter).second << endl;
//    }
//    exit(0);
    vector<int> calstack;
    bool finish = true;
    for (int i = nodeVec.size()-1; i >= 0; i--)
    {
        if(nodeVec[i].parent.size() == 0 && !nodeVec[i].isleaf && !nodeVec[i].isadded)
        {
            calstack.push_back(i);
            nodeVec[i].isadded = true;
            int son  = nodeVec[i].leftson;
            int rson = nodeVec[i].rightson;
            for(vector<int>::iterator iter = nodeVec[son].parent.begin(); iter != nodeVec[son].parent.end(); iter++)
            {
                if((*iter) == i)
                {
                    nodeVec[son].parent.erase(iter);
                    break;
                }
            }
            for(vector<int>::iterator iter = nodeVec[rson].parent.begin(); iter != nodeVec[rson].parent.end(); iter++)
            {
                if((*iter) == i)
                {
                    nodeVec[rson].parent.erase(iter);
                    break;
                }
            }
            ;
            i = son;
            while(nodeVec[i].parent.size()==0 && !nodeVec[i].isleaf)
            {
                calstack.push_back(i);
                nodeVec[i].isadded = true;
                son  = nodeVec[i].leftson;
                rson = nodeVec[i].rightson;
                for(vector<int>::iterator iter = nodeVec[son].parent.begin(); iter != nodeVec[son].parent.end(); iter++)
                {
                    if((*iter) == i)
                    {
                        nodeVec[son].parent.erase(iter);
                        break;
                    }
                }
                for(vector<int>::iterator iter = nodeVec[rson].parent.begin(); iter != nodeVec[rson].parent.end(); iter++)
                {
                    if((*iter) == i)
                    {
                        nodeVec[rson].parent.erase(iter);
                        break;
                    }
                }
                i = son;
            }
            i = nodeVec.size();
            finish = true;
            continue;
        }
        if(nodeVec[i].parent.size() != 0 && !nodeVec[i].isleaf && !nodeVec[i].isadded)
        {
            finish = false;
        }
        if(i == 0 && finish)
        {
            break;
        }
    }
//    for (int i = 0; i<calstack.size(); i++)
//    {
//        cout << calstack[i] << endl;
//    }
    bool nonuse[nodeVec.size()];
    for(int i = 0; i< nodeVec.size();i++)
    {
        nonuse[i] = false;
    }
    for(int i = nodeVec.size() - 1; i >= 0; i--)
    {
        if(!nodeVec[i].isleaf)
        {
            nonuse[i] = true;
            if(nodeVec[nodeVec[i].leftson].isleaf)
            {
                nonuse[nodeVec[i].leftson] = true;
            }
            if(nodeVec[nodeVec[i].rightson].isleaf)
            {
                nonuse[nodeVec[i].rightson] = true;
            }
        }
    }

    int quadspot = begin;
    for (int i = nodeVec.size() - 1; i >= 0; i--) {
        if (nodeVec[i].isleaf ) {
            if (nodeVec[i].isconstant) {
                quadrupleVec[quadspot++] = quadruple("LI", nodeVec[i].temp,
                                                     nodeVec[i].cal, "", "");
            } else {
                quadrupleVec[quadspot++] = quadruple("move", nodeVec[i].temp,
                                                     nodeVec[i].cal, "", "");
            }
            for (map<string, int>::iterator iter = nodemap.begin(); iter != nodemap.end(); iter++) {
                if (iter->first.substr(0, 7) == "convar_" && iter->second == i &&
                    iter->first != nodeVec[i].cal) {
                    quadrupleVec[quadspot++] = quadruple("move", "result", nodeVec[i].temp, "",
                                                         "");
                    quadrupleVec[quadspot++] = quadruple("move", iter->first, "result", "", "");
                }
            }
        }
    }

    for (int i = calstack.size() - 1; i >= 0; i--)
    {
        int calnow = calstack[i];
        string cal = nodeVec[calnow].cal;
        bool leftconst = false, rightconst = false;
        if(nodeVec[nodeVec[calnow].leftson].isleaf)
        {
            if(nodeVec[nodeVec[calnow].leftson].isconstant)
            {
                leftconst = true;
//                if(!(quadrupleVec[quadspot-1].op == "LI" && quadrupleVec[quadspot-1].target == nodeVec[nodeVec[calnow].leftson].temp
//                     && nodeVec[nodeVec[calnow].leftson].cal == quadrupleVec[quadspot-1].op1))
//                quadrupleVec[quadspot++] = quadruple("LI",nodeVec[nodeVec[calnow].leftson].temp,nodeVec[nodeVec[calnow].leftson].cal,"","");
            } else
            {
//                if(!(quadrupleVec[quadspot-1].op == "move" && quadrupleVec[quadspot-1].target == nodeVec[nodeVec[calnow].leftson].temp
//                     && nodeVec[nodeVec[calnow].leftson].cal == quadrupleVec[quadspot-1].op1))
//                    quadrupleVec[quadspot++] = quadruple("move",nodeVec[nodeVec[calnow].leftson].temp,nodeVec[nodeVec[calnow].leftson].cal,"","");
            }
//            for(map<string,int>::iterator iter = nodemap.begin(); iter!=nodemap.end(); iter++)
//            {
//                if(iter->first.substr(0,7) == "convar_" && iter->second == nodeVec[calnow].leftson && iter->first != nodeVec[nodeVec[calnow].leftson].cal)
//                {
//                    quadrupleVec[quadspot++] = quadruple("move","result",nodeVec[nodeVec[calnow].rightson].temp,"","");
//                    quadrupleVec[quadspot++] = quadruple("move",iter->first,"result","","");
//                    nodemap[iter->first] = -1;
//                }
//            }
        }
        if(nodeVec[nodeVec[calnow].rightson].isleaf)
        {
            if(nodeVec[nodeVec[calnow].rightson].isconstant)
            {
                rightconst = true;
//                if(!(quadrupleVec[quadspot-1].op == "LI" && quadrupleVec[quadspot-1].target == nodeVec[nodeVec[calnow].rightson].temp
//                     && nodeVec[nodeVec[calnow].rightson].cal == quadrupleVec[quadspot-1].op1))
//                    quadrupleVec[quadspot++] = quadruple("LI",nodeVec[nodeVec[calnow].rightson].temp,nodeVec[nodeVec[calnow].rightson].cal,"","");
            } else
            {
//                if(!(quadrupleVec[quadspot-1].op == "move" && quadrupleVec[quadspot-1].target == nodeVec[nodeVec[calnow].rightson].temp
//                     && nodeVec[nodeVec[calnow].rightson].cal == quadrupleVec[quadspot-1].op1))
//                    quadrupleVec[quadspot++] = quadruple("move",nodeVec[nodeVec[calnow].rightson].temp,nodeVec[nodeVec[calnow].rightson].cal,"","");
            }
//            for(map<string,int>::iterator iter = nodemap.begin(); iter!=nodemap.end(); iter++)
//            {
//                if(iter->first.substr(0,7) == "convar_" && iter->second == nodeVec[calnow].rightson && iter->first != nodeVec[nodeVec[calnow].rightson].cal)
//                {
//                    quadrupleVec[quadspot++] = quadruple("move","result",nodeVec[nodeVec[calnow].rightson].temp,"","");
//                    quadrupleVec[quadspot++] = quadruple("move",iter->first,"result","","");
//                    nodemap[iter->first] = -1;
//                }
//            }
        }
        if(cal == "sub" || cal == "add")
        {
            if(leftconst && rightconst)
            {
                int left = atoi(nodeVec[nodeVec[calnow].leftson].cal.c_str());
                int right = atoi(nodeVec[nodeVec[calnow].rightson].cal.c_str());
                ostringstream ss;
                if(cal == "sub")
                {
                    ss << left - right;
                } else
                    ss << left + right;

                quadrupleVec[quadspot++] = quadruple("LI",nodeVec[calnow].temp,ss.str(),"","");
            } else
            {
                if(cal=="sub")
                    quadrupleVec[quadspot++] = quadruple("sub",nodeVec[calnow].temp,nodeVec[nodeVec[calnow].leftson].temp,nodeVec[nodeVec[calnow].rightson].temp,"");
                else
                    quadrupleVec[quadspot++] = quadruple("add",nodeVec[calnow].temp,nodeVec[nodeVec[calnow].leftson].temp,nodeVec[nodeVec[calnow].rightson].temp,"");
            }
        } else
        {
            if(cal == "mult")
            {
                quadrupleVec[quadspot++] = quadruple("mult",nodeVec[nodeVec[calnow].leftson].temp,nodeVec[nodeVec[calnow].rightson].temp,"","");
                quadrupleVec[quadspot++] = quadruple("mflo",nodeVec[calnow].temp,"","","");
            }else if(cal == "div")
            {
                quadrupleVec[quadspot++] = quadruple("div",nodeVec[nodeVec[calnow].leftson].temp,nodeVec[nodeVec[calnow].rightson].temp,"","");
                quadrupleVec[quadspot++] = quadruple("mflo",nodeVec[calnow].temp,"","","");
            }
        }
        for(map<string,int>::iterator iter = nodemap.begin(); iter!=nodemap.end(); iter++)
        {
            if(iter->first.substr(0,7) == "convar_" && iter->second == calnow)
            {
                quadrupleVec[quadspot++] = quadruple("move","result",nodeVec[calnow].temp,"","");
                quadrupleVec[quadspot++] = quadruple("move",iter->first,"result","","");
            }
        }
    }
    if(quadspot>end)
    {
        cout << "Optimization failure" << endl;
    }
    for(int i = quadspot; i<end; i++)
    {
        quadrupleVec[i] = quadruple("deleted","","","","");
    }
}
void quadrupleintepreter::generate_target_mips() {
    quadCounter = 0;
    std::cout << "the directory that your want to store your output file"<< endl;
    string dir;
    cin >> dir;
    ofstream os;
    vector<int> callfuncpara;
    string newdir = dir + "output.asm";
    os.open(newdir.c_str());
    this->dir = newdir;
    cout << os.is_open();
    vector<int> parastack;
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
    os << "stringalign" << count <<"\t:\t" << ".asciiz\t" << "\"" << "space" << "\"" << endl;
    for(std::vector<string>::iterator striter = st.stringlist.begin() ; striter!= st.stringlist.end(); striter++, count++)
    {
        os << "string_" << count <<"\t:\t" << ".asciiz\t" << "\"" << (*striter) << "\"" << endl;
    }
    os << ".text "<< endl;
    os << "j\tmain" << endl;
    os<<endl;
    os << "\t$$return:\t" << endl;
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
    string funcname;
    callfuncpara.push_back(0);
    vector<string> callfunc;
    map<string,string> regmap;
    for(quadCounter = 0; quadCounter < quadrupleVec.size(); quadCounter++)
    {
            if(quadrupleVec[quadCounter].label.substr(0,10) == "funcbegin_" && quadrupleVec[quadCounter].label != "funcbegin_main")
            {
                funcname = quadrupleVec[quadCounter].label.substr(10);
                regmap = map<string,string>();
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
                            if((*iter).isassigned && (*iter).pos == st.find_funcname(currentFuc.id))
                            {
                                regmap[(*iter).id] = (*iter).reg;
                            }
                        }
                        for(std::vector<convarTerm>::iterator iter = st.convarList.begin(); iter != st.convarList.end(); iter++)
                        {
                            if(!(*iter).isvar && (*iter).pos == st.find_funcname(currentFuc.id))
                            {
                                os << "\taddi\t" << "$t4,\t" << "$0,\t" << (*iter).constValue << endl;
                                os << "\tsw\t" << "$t4,\t" << (*iter).shift << "($fp)" << endl;
                                if((*iter).isassigned)
                                    os << "\taddi\t" << (*iter).reg + ",\t" << "$t4,\t" << 0 << endl;
                            }
                        }
                        os << "\tadd\t" << "$sp,\t" << "$sp,\t" << currentFuc.tempshift*(-1) << endl;
                    }
                }
//                cout << funcname <<":" << endl;
//                for(map<string,string>::iterator ite = regmap.begin(); ite!= regmap.end(); ite++)
//                {
//                    cout << (*ite).first << "\treg:\t" << (*ite).second << endl;
//                }
            }
            else if(quadrupleVec[quadCounter].label.substr(0,8) == "funcend_"&& funcname!="main")
            {
                os <<  funcname + "_end" << ":" << endl;
                for(std::vector<funcTerm>::iterator funciter = st.funcList.begin(); funciter != st.funcList.end(); funciter++)
                {
                    funcTerm &currentFuc = (*funciter);
                    if(funcname == currentFuc.id)
                    {
                        os << "\tadd\t" << "$sp,\t" << "$sp,\t" << currentFuc.shift << endl;
                        os << "\tadd\t" << "$sp,\t" << "$sp,\t" << currentFuc.tempshift << endl;
                    }
                }
                os << "\tj\t" << "$$return" << endl;

            }else if(quadrupleVec[quadCounter].label == "funcbegin_main")
            {
                funcname = quadrupleVec[quadCounter].label.substr(10);
                regmap = map<string,string>();
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
                            if((*iter).isassigned && (*iter).pos == st.find_funcname(currentFuc.id))
                            {
                                regmap[(*iter).id] = (*iter).reg;
                            }
                        }
                        for(std::vector<convarTerm>::iterator iter = st.convarList.begin(); iter != st.convarList.end(); iter++)
                        {
                            if(!(*iter).isvar && (*iter).pos == st.find_funcname(currentFuc.id))
                            {
                                os << "\taddi\t" << "$t4,\t" << "$0,\t" << (*iter).constValue << endl;
                                os << "\tsw\t" << "$t4,\t" << (*iter).shift << "($fp)" << endl;
                                if((*iter).isassigned)
                                    os << "\taddi\t" << (*iter).reg + ",\t" << "$t4,\t" << 0 << endl;
                            }
                        }
                        os << "\tadd\t" << "$sp,\t" << "$sp,\t" << currentFuc.tempshift*(-1) << endl;
                    }
                }
//                cout << funcname <<":" << endl;
//                for(map<string,string>::iterator ite = regmap.begin(); ite!= regmap.end(); ite++)
//                {
//                    cout << (*ite).first << "\treg:\t" << (*ite).second << endl;
//                }
            }
            else if(quadrupleVec[quadCounter].label != ""  )
            {
                if(quadrupleVec[quadCounter].label == "funcend_main")
                {
                    os << "\tmain_end"  << ":" << endl;

                } else
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
                            if(regmap.find(convarname) != regmap.end() && assignregister)
                            {
                                os << "\tadd\t" <<regmap[convarname] << "\t$s0,\t" << "$0" << endl;
                            } else
                                os << "\tsw\t" << "$s0,\t" << shift << "($fp)" << endl;
                        }
                    }
                }
                else if(quadrupleVec[quadCounter].target.substr(0,6) == "array_")
                {
                    string arrayname = quadrupleVec[quadCounter].target.substr(6);
                    int sh = stringToInt(quadrupleVec[quadCounter].op2.substr(2));
                    bool isglo = st.check_global(arrayname, funcname);
                    if(isglo)
                    {
                        os << "\tla\t" << "$t1,\t" << arrayname << endl;
                        os << "\tlw\t" << "$t0,\t" << sh << "($sp)" << endl;
                        os << "\tsll\t" << "$t0,\t" << "$t0,\t" << 2 << endl;
                        os << "\tadd\t" << "$t0,\t" << "$t0,\t" << "$t1" << endl;
                    }
                    else
                    {
                        int shift = st.findidshift(arrayname, funcname);
                        os << "\tlw\t" << "$t0,\t" << sh << "($sp)" << endl;
                        os << "\taddi\t" << "$t1\t" << "$fp,\t" << shift << endl;
                        os << "\tsll\t" << "$t0,\t" << "$t0,\t" << 2 << endl;
                        os << "\tadd\t" << "$t0,\t" << "$t0,\t" << "$t1" << endl;
                    }
                    os << "\tsw\t" << "$s0,\t" << "0($t0)" << endl;
                }
                else if(quadrupleVec[quadCounter].op1.substr(0,7) == "convar_")
                {
                    string convarname = quadrupleVec[quadCounter].op1.substr(7);
                    int sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                    bool isglo = st.check_global(convarname,funcname);
                    if(isglo)
                    {
                        os << "\tlw\t" << "$s0,\t" << convarname << endl;
                        os << "\tsw\t" << "$s0,\t" << sh << "($sp)" << endl;
                    } else
                    {
                        int shift = st.findidshift(convarname, funcname);
                        if(shift == -1)
                        {
                            shift = st.findidparano(convarname,funcname);
                            os << "\tsw\t" << "$a"<< shift <<",\t" << sh << "($sp)" << endl;
                        } else
                        {
                            if(regmap.find(convarname) != regmap.end() && assignregister)
                            {
                                os << "\tadd\t" << "$s0\t" << regmap[convarname] + ",\t" << "$0" << endl;
                            } else
                            {
                                os << "\tlw\t" << "$s0,\t" << shift << "($fp)" << endl;
                            }
                            os << "\tsw\t" << "$s0,\t" << sh << "($sp)" << endl;
                        }

                    }
                }
                else if(quadrupleVec[quadCounter].target == "result")
                {

                    int sh = stringToInt(quadrupleVec[quadCounter].op1.substr(2));
                    os << "\tlw\t" << "$s0" <<",\t" << sh << "($sp)" << endl;
                }
                else if(quadrupleVec[quadCounter].target == "compare1")
                {
                    int sh = stringToInt(quadrupleVec[quadCounter].op2.substr(2));
//                    os << "\tsw\t" << "$s0" <<",\t" << sh << "($sp)" << endl;
                }
                else if(quadrupleVec[quadCounter].target == "compare2")
                {
                    if(quadrupleVec[quadCounter].op1 == "$0")
                    {
                        os << "\taddi\t" << "$s2" <<"\t$0,\t" << "0" << endl;
                    } else
                    {
                        int sh = stringToInt(quadrupleVec[quadCounter].op2.substr(2));
//                        os << "\tsw\t" << "$s0" <<",\t" << sh << "($sp)" << endl;
                    }
                }
                else if(quadrupleVec[quadCounter].op1 == "result")
                {
                    if(quadrupleVec[quadCounter].target == "parameter" )
                    {
                        parastack.push_back(stringToInt(quadrupleVec[quadCounter].op2.substr(2)));
                        callfuncpara[callfuncpara.size()-1] ++;
                    }else {
                        int sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                        os << "\tsw\t" << "$s0" <<",\t" << sh << "($sp)" << endl;
                    }
                }
                else if(quadrupleVec[quadCounter].op1 == "returnvalue")
                {
                    int sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                    os << "\tsw\t" << "$v0" <<",\t" << sh << "($sp)" << endl;
                }
            }else if(quadrupleVec[quadCounter].op == "add")
            {
                int sh = stringToInt(quadrupleVec[quadCounter].op2.substr(2));
                os << "\tlw\t" << "$s2,\t" << sh << "($sp)" << endl;
                sh = stringToInt(quadrupleVec[quadCounter].op1.substr(2));
                os << "\tlw\t" << "$s1,\t" << sh << "($sp)" << endl;
                os << "\tadd\t" << "$s0,\t" <<"$s1,\t" <<"$s2" << endl;
                sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                os << "\tsw\t" << "$s0" <<",\t" << sh << "($sp)" << endl;

            }else if(quadrupleVec[quadCounter].op == "sub")
            {
                if(quadrupleVec[quadCounter].op1 == "$0")
                {
                    int sh = stringToInt(quadrupleVec[quadCounter].op2.substr(2));
                    os << "\tlw\t" << "$s2,\t" << sh << "($sp)" << endl;
                    os << "\tsub\t" << "$s0,\t" <<"$0,\t" <<"$s2" << endl;
                    sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                    os << "\tsw\t" << "$s0" <<",\t" << sh << "($sp)" << endl;
                } else
                {
                    int sh = stringToInt(quadrupleVec[quadCounter].op2.substr(2));
                    os << "\tlw\t" << "$s2,\t" << sh << "($sp)" << endl;
                    sh = stringToInt(quadrupleVec[quadCounter].op1.substr(2));
                    os << "\tlw\t" << "$s1,\t" << sh << "($sp)" << endl;
                    os << "\tsub\t" << "$s0,\t" <<"$s1,\t" <<"$s2" << endl;
                    sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                    os << "\tsw\t" << "$s0" <<",\t" << sh << "($sp)" << endl;
                }
            }else if(quadrupleVec[quadCounter].op == "mult")
            {
                int sh = stringToInt(quadrupleVec[quadCounter].op1.substr(2));
                os << "\tlw\t" << "$s2,\t" << sh << "($sp)" << endl;
                sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                os << "\tlw\t" << "$s1,\t" << sh << "($sp)" << endl;
                os << "\tmult\t" <<"$s1,\t" <<"$s2" << endl;

            }else if(quadrupleVec[quadCounter].op == "div")
            {
                int sh = stringToInt(quadrupleVec[quadCounter].op1.substr(2));
                os << "\tlw\t" << "$s2,\t" << sh << "($sp)" << endl;
                sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                os << "\tlw\t" << "$s1,\t" << sh << "($sp)" << endl;
                os << "\tdiv\t" <<"$s1,\t" <<"$s2" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "LI")
            {
                os <<  "\taddi\t" << "$s0,\t" << "$0,\t" << quadrupleVec[quadCounter].op1 << endl;
                int sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                os << "\tsw\t" << "$s0" <<",\t" << sh << "($sp)" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "mflo")
            {
                os << "\tmflo\t" << "$s0" << endl;
                int sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                os << "\tsw\t" << "$s0" <<",\t" << sh << "($sp)" << endl;
            }
            else if(quadrupleVec[quadCounter].op == "arrayaddr")
            {
                /////////////////////////////////////
            }else if(quadrupleVec[quadCounter].op == "arrayvalue")
            {
                string arrayname = quadrupleVec[quadCounter].op1;
                int sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                bool isglo = st.check_global(arrayname, funcname);
                if(isglo)
                {
                    os << "\tla\t" << "$t1,\t" << arrayname << endl;
                    os << "\tsll\t" << "$s0,\t" << "$s0,\t" << 2 << endl;
                    os << "\tadd\t" << "$t0,\t" << "$t1,\t" << "$s0" << endl;
                    os << "\tlw\t" << "$s0,\t" << "0($t0)" << endl;
                    os << "\tsw\t" << "$s0,\t" << sh << "($sp)" << endl;
                }
                else
                {
                    int shift = st.findidshift(arrayname, funcname);
                    os << "\taddi\t" << "$t1,\t" << "$fp,\t" << shift << endl;
                    os << "\tsll\t" << "$s0,\t" << "$s0,\t" << 2 << endl;
                    os << "\tadd\t" << "$t0,\t" << "$t1,\t" << "$s0" << endl;
                    os << "\tlw\t" << "$s0,\t" << "0($t0)" << endl;
                    os << "\tsw\t" << "$s0,\t" << sh << "($sp)" << endl;
                }
            }
            else if(quadrupleVec[quadCounter].op == "call")
            {
                callfunc.push_back(quadrupleVec[quadCounter].target);
            }
            else if(quadrupleVec[quadCounter].op == "parabegin")
            {
                callfuncpara.push_back(0) ;
            }
            else if(quadrupleVec[quadCounter].op == "paraend")
            {
                for(int i = 0; i< callfuncpara[callfuncpara.size()-1] && i < 4; i++)
                {
                    os << "\tlw\t" << "$a"<< i <<",\t" << parastack[parastack.size()-callfuncpara[callfuncpara.size()-1]+i] << "($sp)" << endl;
                }
                for(int i = 0; i < callfuncpara[callfuncpara.size()-1]; i++)
                {
                    os << "\taddi\t" << "$sp\t" << "$sp,\t" << "-4" << endl;
                    os << "\tlw\t" << "$s0\t" << parastack[parastack.size()-callfuncpara[callfuncpara.size()-1]+i]+(i+1)*4 << "($sp)" << endl;
                    os << "\tsw\t" << "$s0\t" << "0($sp)" << endl;
                }
                for(int i = 0; i < callfuncpara[callfuncpara.size()-1]; i++)
                {
                    parastack.pop_back();
                }
                os << "\tjal\t" << callfunc[callfunc.size()-1] << endl;
                os << "\tnop\t" << endl;
                os << "\taddi\t" << "$sp,\t" << "$sp,\t" << callfuncpara[callfuncpara.size()-1]*4 << endl;
                callfunc.pop_back();
                callfuncpara.pop_back();
                for(std::vector<funcTerm>::iterator funciter = st.funcList.begin(); funciter != st.funcList.end(); funciter++)
                {
                    funcTerm &currentFuc = (*funciter);
                    if(funcname == currentFuc.id)
                    {
                        ////////////////////////////jixed!!!!LKjwflklwefjklwejfklwejflwkefjklwefj
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
                if(quadrupleVec[quadCounter].op1 == "$0")
                {
                    int sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                    os << "\tlw\t" << "$s1" <<",\t" << sh << "($sp)" << endl;
                } else
                {
                    int sh = stringToInt(quadrupleVec[quadCounter].target.substr(2));
                    os << "\tlw\t" << "$s1" <<",\t" << sh << "($sp)" << endl;
                    sh = stringToInt(quadrupleVec[quadCounter].op1.substr(2));
                    os << "\tlw\t" << "$s2" <<",\t" << sh << "($sp)" << endl;
                }
                os << "\t"<< quadrupleVec[quadCounter].op <<"\t" <<"$s1,\t" <<"$s2,\t" << quadrupleVec[quadCounter].op2 << endl;
            }
            else if(quadrupleVec[quadCounter].op == "return")
            {
                if(quadrupleVec[quadCounter].target == "result"){
                    os << "\tadd\t" << "$v0,\t" << "$s0,\t" << "$0" << endl;
                    os << "\tj\t" << funcname+"_end" << endl;
                } else
                {
                    os << "\tj\t" << funcname+"_end" << endl;
                }
            }else if(quadrupleVec[quadCounter].op == "scan")
            {
                string scanname = quadrupleVec[quadCounter].target;
                symbolSystem  type = st.check_type1(scanname,funcname);
                if( type == charsym)
                {
                    os << "\tli\t" << "$v0\t" << 12 << endl;
                }
                else if(type == intsym)
                {
                    os << "\tli\t" << "$v0\t" << 5 << endl;
                }
                else if(type == notsym)
                {
                    fetal("unknown type " + scanname);
                }
                os << "\tsyscall" << endl;
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
                        if(regmap.find(scanname) != regmap.end() && assignregister)
                        {
                            os << "\tadd\t" <<regmap[scanname] << "\t$v0,\t" << "$0" << endl;
                        } else
                            os << "\tsw\t" << "$v0,\t" << shift << "($fp)" << endl;
                    }
                }
            }else if(quadrupleVec[quadCounter].op == "print")
            {
                if(quadrupleVec[quadCounter].target == "result")
                {
                    if(quadrupleVec[quadCounter].op1 == "") {
                        os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                        os << "\tsw\t" << "$a0" << ",\t" << "0($sp)" << endl;
                        os << "\taddi\t" << "$a0,\t" << "$s0,\t" << 0 << endl;
                        os << "\tli\t" << "$v0\t" << 1 << endl;
                        os << "\tsyscall" << endl;
                        os << "\tlw\t" << "$a0,\t" << "0($sp)" << endl;
                        os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                    } else
                    {
                        os << "\taddi\t" << "$sp,\t" << "$sp,\t" << -4 << endl;
                        os << "\tsw\t" << "$a0" << ",\t" << "0($sp)" << endl;
                        os << "\taddi\t" << "$a0,\t" << "$s0,\t" << 0 << endl;
                        os << "\tli\t" << "$v0\t" << 11 << endl;
                        os << "\tsyscall" << endl;
                        os << "\tlw\t" << "$a0,\t" << "0($sp)" << endl;
                        os << "\taddi\t" << "$sp,\t" << "$sp,\t" << 4 << endl;
                    }
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

void quadrupleintepreter::lsoptimize() {
    ifstream is;
    vector<string> storeline;
    char  buffer[1000];
    is.open(dir.c_str());
    while(!is.eof())
    {
        is.getline(buffer,999);
        storeline.push_back(string(buffer));
    }
    is.close();
    ofstream os;
    os.open(dir.c_str());
    int state = 0;
    for(vector<string>::iterator iter = storeline.begin(); iter!=storeline.end(); iter++)
    {
        if((*iter).substr(0,3)== "\tsw")
        {
            string temp = (*iter).substr(3);
            iter++;
            if((*iter).substr(0,3)== "\tlw" && temp == (*iter).substr(3))
            {
                os << "\tsw" + temp << endl;
            }else{
                os << "\tsw" + temp << endl;
                os << (*iter) << endl;
            }
        } else
        {
            os << (*iter) << endl;
        }
    }

}