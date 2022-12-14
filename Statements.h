//
// Created by 刘洋 on 2022/11/13.
//

#ifndef QBASIC_HANDOUT_STATEMENTS_H
#define QBASIC_HANDOUT_STATEMENTS_H

enum Statement{
    _Rem,
    _Let,
    _Print,
    _Input,
    _Goto,
    _If,
    _Then,
    _End,

    _Run,
    _Load,
    _List,
    _Clear,
    _Help,
    _Quit,

    _Condition,
    _Number,
    _Other,
    _Eof
};

enum Operators{
    _Numbers,
    _Var,

    _Add,
    _Minus,

    _Multi,
    _Devide,

    _Power,

    _Paren_start,
    _Paren_end,
    _Eoe
};


#endif //QBASIC_HANDOUT_STATEMENTS_H
