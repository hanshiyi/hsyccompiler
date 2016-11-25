#include <iostream>
#include <cstdlib>
#include "syntax.h"
#include <map>
#include <cstdio>
#include "signtable.h"
#include "quadrupleintepreter.h"

using namespace std;
FILE *sourceFile;
char c;
bool eofFlag;
string symbolString[]  = {"assignsym", "addsym", "minussym", "mulsym", "divsym", "identifier", "constsym", "number", "scanfsym", "printfsym", "mainsym", "returnsym", "comma", "ifsym", "elsesym", "dosym", "whilesym", "intsym", "charsym", "voidsym", "forsym", "leftbrace", "rightbrace", "grt", "let", "greqt", "leeqt", "equ", "neq", "leftparathe", "rightparathe", "notsym", "leftbrack", "rightbrack", "singlequote", "doublequote", "character", "stringsym", "semicolon"};
string id;
map<string,symbolSystem > symbolMap;
symbolSystem sym;
int cc;
int ll;
int line;
int inum;

signtable st;
quadrupleintepreter qi;

int main() {
    string programName;
    cin >> programName;
    symbolMap[","] = comma;
    symbolMap["+"] = addsym;
    symbolMap["-"] = minussym;
    symbolMap["="] = assignsym;
    symbolMap["if"] = ifsym;
    symbolMap["else"] = elsesym;
    symbolMap["int"] = intsym;
    symbolMap["char"] = charsym;
    symbolMap["for"] = forsym;
    symbolMap["void"] = voidsym;
    symbolMap["do"] = dosym;
    symbolMap["while"] = whilesym;
    symbolMap["{"] = leftbrace;
    symbolMap["}"] = rightbrace;
    symbolMap["*"] = mulsym;
    symbolMap["/"] = divsym;
    symbolMap["=="] = equ;
    symbolMap["<"] = let;
    symbolMap["<="] = leeqt;
    symbolMap["!="] = neq;
    symbolMap[">"] = grt;
    symbolMap[">="] = greqt;
    symbolMap["("] = leftparathe;
    symbolMap[")"] = rightparathe;
    symbolMap[";"] = semicolon;
    symbolMap["scanf"] = scanfsym;
    symbolMap["printf"] = printfsym;
    symbolMap["main"] = mainsym;
    symbolMap["return"] = returnsym;
    symbolMap["const"] = constsym;
    symbolMap["["] = leftbrack;
    symbolMap["]"] = rightbrack;
    symbolMap["\'"] = singlequote;
    symbolMap["\""] = doublequote;
    sym = notsym;
    cc = ll = 0; //set c counter and line counter to zero
    line = 1;    // set line number to zero
    sourceFile = fopen(programName.c_str(),"r");
    if(sourceFile == NULL) {
        cout << "fuck it";
        exit(0);
    }
    st = signtable();
    eofFlag = false;
    getch();
    inum = -1;
    syntax_analyze();
//    st.print();
    qi.st = st;
//    qi.print();
    qi.generate_target_mips();
    return 0;
}