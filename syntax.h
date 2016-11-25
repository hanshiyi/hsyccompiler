//
// Created by hanshiyi on 12/11/2016.
//

#ifndef UNTITLED_SYNTAX_H
#define UNTITLED_SYNTAX_H

#include <string>
#include <set>
#include "lexical.h"
void error(std::string);
void skip(std::set<symbolSystem> );
void func_analyze( );
void paraList();
void constDef();
void varDef();
void compoundStatement();
void integer();
void multiplestatement();
void statement();
void condition(std::string);
void ifcondition();
void dowhileconditon();
void forstatement();
void scanfstatement();
void printfstatement();
void expression();
void factor(int );
void term(int);
void fetal(std::string);

void syntax_analyze();
#endif //UNTITLED_SYNTAX_H
