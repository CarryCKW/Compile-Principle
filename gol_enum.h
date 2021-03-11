//
// Created by 蔡小蔚 on 2021/3/10.
//

#ifndef HDA_GOL_ENUM_H
#define HDA_GOL_ENUM_H

#endif //HDA_GOL_ENUM_H
enum {
    Num = 128,Fun,Sys,Glo, Loc, Id,
    Char, Else, Enum, If, Int, Return, Sizeof, While,
    Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};
struct identifier {
    int token;//该标识符返回的标记，理论上所有的变量返回的标记都应该是 Id，但实际上由于我们还将在符号表中加入关键字如 if, while 等，它们都有对应的标记。
    int hash;//就是这个标识符的哈希值，用于标识符的快速比较
    char * name;//存放标识符本身的字符串
    int Class;//该标识符的类别，如数字，全局变量或局部变量等
    int type;//标识符的类型，即如果它是个变量，变量是 int 型、char 型还是指针型
    int value;//存放这个标识符的值，如标识符是函数，刚存放函数的地址
    int Bclass;
    int Btype;
    int Bvalue;
    //B**** :C 语言中标识符可以是全局的也可以是局部的，当局部标识符的名字与全局标识符相同时，用作保存全局标识符的信息
};
