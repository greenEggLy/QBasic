//
// Created by 刘洋 on 2022/11/13.
//

#ifndef QBASIC_HANDOUT_EXPRESSIONS_H
#define QBASIC_HANDOUT_EXPRESSIONS_H

#pragma once
#include "Utils.h"
//#include "runner.h"

class Expressions;

typedef std::pair<int, Expressions*> synNode;

enum Exp_Type{
    exp_var,
    exp_num,
    exp_op
};

class Expressions {
public:
    Exp_Type type;
    std::string val;
    bool is_added = false;
    Expressions* child[2];

    Expressions(){
        type = exp_var;
        child[0] = nullptr;
        child[1] = nullptr;
    }
    Expressions(Exp_Type arg_type, std::string arg_val){
        type = arg_type;
        val = arg_val;
        child[0] = nullptr;
        child[1] = nullptr;
    }
//    Expressions(Exp_Type arg_type, std::string arg_val, Expressions* left_child, Expressions* par){
//        type = arg_type;
//        val = arg_val;
//        child[0] = left_child;
//        child[1] = nullptr;
//        parent = par;
//    }


private:
//    int cal(std::string op, Expressions*& val2, Expressions*& val1);
};


#endif //QBASIC_HANDOUT_EXPRESSIONS_H
