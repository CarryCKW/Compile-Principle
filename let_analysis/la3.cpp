//
// Created by ckw on 2021/3/11.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "iostream"
using namespace std;
enum Symbol{
    Num = 128, Fun, Sys, Id, Reserve,
    Char, Else, Enum, If, Int, Return, Sizeof, While, Open ,Read, Close, Printf, Malloc, Memset, Exit, Void, Main,Do,
    Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak, Hex, Otc, Annotations,Separator,Variable
};
string s[] = {"Num", "Fun","Sys","Id", "Reserve",
              "Char", "Else", "Enum", "If", "Int", "Return", "Sizeof", "While", "Open", "Read", "Close",
              "Printf", "Malloc", "Memset", "Exit", "Void", "Main","Do",
              "Assign", "Cond", "Lor", "Lan", "Or", "Xor", "And", "Eq", "Ne", "Lt", "Gt", "Le", "Ge", "Shl", "Shr", "Add", "Sub", "Mul", "Div"
              , "Mod", "Inc", "Dec", "Brak", "Hex", "Otc", "Annotations", "Separator", "Variable"};

enum { CHAR, INT, PTR };
const int poolsize = 256 * 1024;
struct node {
    int Token;  //当前描述符的开始索引
    int Hash;  //数值
    int Name;  //字符串起始地址
    int Type; //为具体的Class对应的东西
    int Class;  //Num = 128, Fun, Sys, Id, Reserve
}identifier[poolsize];

/*
 * Num:各个进制数
 * Fun： 函数名
 * Id：定义的变量
 * Reserve： 保留字 Char, Else, Enum, If, Int, Return, Sizeof, While, Open ,Read, Close, Printf, Mal。。。
 * Sys：Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak, Hex, Otc
 */

int token;
int token_val;
int idx;
int line;
char *src, *old_src;


void readFile(int type);
void next();
int main() {

    line = 1;
    if (!(src = old_src = (char *)malloc(poolsize))) {
        printf("could not malloc(%d) for old/src \n", poolsize);
        return -1;
    }

    readFile(0);
    printf("%s", src);
    int i = Char;
    idx = 0;
    while (i <= Do) {
//        cout<<"i:"<<i<<endl;
        next();
        identifier[idx].Class = Reserve;
        identifier[idx].Type = i;
        idx++;
        i++;
    }

    readFile(1);
    i = 0;
    while (*src != 0) {
        next();
//        cout<<"( "<<identifier[idx].Class<<" , "<<identifier[idx].Type<<" )"<<endl;
        cout<<"( "<<s[identifier[idx].Class-128]<<" , "<<s[identifier[idx].Type-128]<<" )"<<endl;
        idx++;
    }

    return 0;
}

void next(){
    char* last_pos;
    int hash;
    while (token = *src) {
        src++;
        if (token=='\n') {
            line ++;
            continue;
        }
        else if(token == ' ') {
            continue;
        }
        else if (token == '#') {
            // skip macro, because we will not support it
            while (*src != 0 && *src != '\n') {
                src++;
            }
        }
        else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_')) {
//            cout<<(char)token;
            last_pos = src - 1;
            hash = token;

            while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_')) {
                hash = hash * 47 + *src;
//                cout<<(char)*src;
                src++;
            }

            int s = 0;
            while (s < idx) {
                if (identifier[s].Hash == hash) {
                    //find one
                    identifier[idx].Hash = hash;
                    identifier[idx].Class = identifier[s].Class;
                    identifier[idx].Type = identifier[s].Type;
                    return;
                }
                s++;
            }
            //new one
            identifier[idx].Class = Id;
            identifier[idx].Hash = hash;
            identifier[idx].Type = Variable;

            return;
        }
        else if (token >= '0' && token <= '9') {
            token_val = token - '0';
            if (token_val > 0) {
                // dec, starts with [1-9]
                while (*src >= '0' && *src <= '9') {
                    token_val = token_val*10 + *src++ - '0';
                }
                identifier[idx].Class = Num;
                identifier[idx].Type = Dec;
            } else {
                // starts with 0
                if (*src == 'x' || *src == 'X') {
                    //hex
                    token = *++src;
                    while ((token >= '0' && token <= '9') || (token >= 'a' && token <= 'f') || (token >= 'A' && token <= 'F')) {
                        token_val = token_val * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
                        token = *++src;
                    }
                    identifier[idx].Class = Num;
                    identifier[idx].Type = Hex;
                } else {
                    // oct
                    while (*src >= '0' && *src <= '7') {
                        token_val = token_val*8 + *src++ - '0';
                    }
                    identifier[idx].Class = Num;
                    identifier[idx].Type = Otc;
                }
            }
            return;
        }
        else if (token == '/') {
            if (*src == '/') {
                while (*src != 0 && *src != '\n') {
                    ++src;
                }
//                identifier[idx].Class = Sys;
//                identifier[idx].Type = Annotations;
                continue;
            } else {
                // divide operator
                identifier[idx].Class = Sys;
                identifier[idx].Type = Div;
                return;
            }
        }
        else if (token == '=') {
            // parse '==' and '='
            identifier[idx].Class = Sys;
            if (*src == '=') {
                src ++;
                identifier[idx].Type = Eq;
            } else {
                identifier[idx].Type = Assign;
            }
            return;
        }
        else if (token == '+') {
            // parse '+' and '++'
            identifier[idx].Class = Sys;
            if (*src == '+') {
                src ++;
                identifier[idx].Type = Inc;
            } else {
                identifier[idx].Type = Add;
            }
            return;
        }
        else if (token == '~' || token == ';' || token == '{' || token == '}' || token == '(' || token == ')' || token == ']' || token == ',' || token == ':') {
            // directly return the character as token;
            identifier[idx].Class = Sys;
            identifier[idx].Type = Separator;
            return;
        }
    }
}

void readFile(int type) {
    FILE *fp = NULL;

    if (type == 0) {
        if ((fp = fopen("D:\\AllWorkForIns\\WorkForC++\\CompileTheory\\HDA\\let_analysis\\test\\keys.txt", "r")) == NULL) {
            printf("fail to open file...");
            exit(-1);
        }
    }

    if (type == 1) {
        if ((fp = fopen("D:\\AllWorkForIns\\WorkForC++\\CompileTheory\\HDA\\let_analysis\\test\\test.txt", "r")) == NULL) {
            printf("fail to open file...");
            exit(-1);
        }
    }

    char ch = fgetc(fp);
    int i = 0 ;
    while (ch!=EOF) {
        src[i] = ch;
        ch = fgetc(fp);
        i++;
    }
    src[i] = 0;
//    printf("last:%d", src[i]);

    fclose(fp);
}
