#ifndef CMDLINES_H
#define CMDLINES_H

#pragma once

#include "Utils.h"
#include "Statements.h"
#include "Expressions.h"

class CmdLines
{
public:
    CmdLines();
    CmdLines(Statement s, int ln, std::string &con);

    void set_statement(Statement s);
    Statement get_statement() const;

    void set_lineNumber(int ln);
    int get_lineNumber() const;

    void set_newLineNumber(int ln);
    int get_newLineNumber() const;

    void set_content(std::string &s);
    std::string get_content() const;

    void set_var(std::string &s);
    std::string get_var() const;

    void set_contrl(std::string& s);
    std::string get_contrl() const;

public:
//    Expressions* expression_root1, *expression_root2;
    QVector<Expressions> expr_vec1, expr_vec2;

protected:
    Statement statement;
    int lineNumber;
    int newLineNumber;
    std::string content;
    std::string var;
    std::string contrl;
};

Q_DECLARE_METATYPE(CmdLines);


#endif // CMDLINES_H
