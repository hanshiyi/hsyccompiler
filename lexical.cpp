//
// Created by hanshiyi on 15/11/2016.
//
#include <cstdlib>
#include <cstdio>
#include <string>
#include <map>
#include <iostream>
#include "lexical.h"
using namespace std;
extern FILE *sourceFile;
extern char c;
extern bool eofFlag;
extern std::string symbolString[];
extern std::string id;
extern std::map<std::string,symbolSystem > symbolMap;
extern symbolSystem sym;
extern int cc;
extern int ll;
extern int line;
extern int inum;

void getsym()
{

    id = "";
    while(c == '\n' || c == ' ' || c == '\t' || c == '\r')
    {
        if(c == '\n')
        {
            line ++;
            cc = 0;
        }
        getch();
        continue;
    }
    if(c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
    {
        int length = 0;
        while(c == '_' || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        {
            id = id + c;
            length ++;
            getch();
        }
        if(length > 20)
        {
            std::cout << "identifier too long, substitute it with the first 20 characters."<< std::endl;
            id = id.substr(0,20);
        }
        map<std::string,symbolSystem >::iterator it= symbolMap.find(id);
        if(it == symbolMap.end()) {
            sym = identifier;

        } else
        {
            sym = symbolMap[id];
        }
    }
    else if(c > '0' && c <= '9') {
        inum = 0;
        int tmp = 0;
        bool flag = false;
        while (c >= '0' && c <= '9') {
            id = id + c;
            tmp = inum * 10 + c - '0';
            if(tmp <= 0)
            {
                if (!flag)
                    cout << "integer out of range" << endl;
                flag = true;
            }
            else {
                inum = tmp;
            }
            getch();
        }
        sym = number;
    }
    else if(c=='0')
    {
        sym = number;
        inum = 0;
        id += c;
        getch();
    }
    else if(c == '(' || c==')')
    {

        id+=c;
        sym = symbolMap[id];
        getch();
    }
    else if(c == '{' || c == '}')
    {

        id+=c;
        sym = symbolMap[id];
        getch();
    }
    else if(c == '[' || c == ']')
    {
        id+=c;
        sym = symbolMap[id];
        getch();
    }
    else if(c == '<' || c == '=' || c == '>' || c=='!')
    {
        id += c;
        getch();
        if(c == '=')
        {
            id += c;
            sym = symbolMap[id];
            getch();
        } else{
            map<string,symbolSystem >::iterator it= symbolMap.find(id);

            if(it == symbolMap.end()) {
                cout  << "fuck idiot" << endl;
            } else
            {
                sym = symbolMap[id];
            }
        }
    }
    else if(c == '*' || c == '/' || c == '+' || c == '-')
    {
        id+=c;
        sym = symbolMap[id];
        getch();
    }
    else if(c == ',' || c == ';')
    {
        id+=c;
        sym = symbolMap[id];
        getch();
    }
    else if(c == '\'' || c == '\"')
    {
        switch(c)
        {
            case '\'':
                    id += c;
                    getch();
                    if(c!='+' && c!='-' && c!='*' && c!='/' && !(c>='a' && c<='z') && !(c>='A' && c<='Z') && !(c>='0' && c<='9') && c!='_')
                    {
                        cout << "invalid character : " << c;
                    }
                    id+=c;
                    inum = c;
                    getch();
                    if(c=='\'')
                    {
                        id+=c;
                        sym = character;
                        getch();
                    } else{
                        cout << "invalid character : " << c;
                        exit(1);
                    }
                break;
            case '\"':
                id += c;
                getch();
                while(c != '\"')
                {
                    if(c!=32 && c!=33 && !(c>=35 && c<=126))
                    {
                        cout << "invalid character : " << c<< endl;
                    }

                    id = id + c;
                    getch();
                }
                id += c;
                sym = stringsym;
                getch();
                break;
        }
    } else{
        sym = notsym;
        id = c;
        getch();
    }
}
void getch()
{
    c = fgetc(sourceFile);
    cc++;
    if(c == EOF)
    {
        eofFlag = true;
    }
}