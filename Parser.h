//
// Created by 刘洋 on 2022/11/14.
//

#ifndef QBASIC_HANDOUT_PARSER_H
#define QBASIC_HANDOUT_PARSER_H

#include "Utils.h"

class Parser {
private:
    std::string contents;
    std::string curOp;
    void read_to_next(char* c);
    bool read(char* c);
public:
    Parser(std::string& s){
        contents = s;
    }
};


#endif //QBASIC_HANDOUT_PARSER_H
