#ifndef CMDPARSER_H
#define CMDPARSER_H

#pragma once
#include <QObject>
#include "Utils.h"
#include "Statements.h"
#include "cmdlines.h"



class CmdParser : public QObject
{
    Q_OBJECT
public:
    explicit CmdParser(QObject *parent = nullptr);
    void setText(std::string& t){text = t; curText = text;}
    void parse(std::string& cmdLine);
    void set_input_status(bool status = true);
    inline bool get_input_status() const
    {
        return is_input;
    }
    inline void set_direct_input(bool status = true){is_direct_input = status;}
    inline bool get_direct_input() const {return is_direct_input;}

private:
    Statement read_space();
    Statement judge();

    QVector<Expressions> parseExpr(std::string expr/*,Expressions*& root*/);
    QVector<Expressions> parse_expr(std::string& expr);
    void check(QVector<Expressions>& expr_vec);
//    void create_expr_link(QVector<Expressions>& expr_vec);

    Operators read_expr(std::string& expr, char& c);
    bool read_next(char &c);
    bool is_pri(Operators t, std::string& t2);
    void del_space(std::string& expr);
    bool is_var(std::string& val);
    bool is_op(Operators op);

public:
    // global
    /*static */std::map<size_t, CmdLines> cmds;

private:
    // temporary
    int line_number;
    bool is_input = false,is_direct_input = false;
    std::string curOp;
    std::string text, curText;
    std::string opArr[7] = {"+","-","*","/","**","(","}"};

signals:
    // to runner
    void send_direct_print(CmdLines& Print);
    void send_direct_let(CmdLines& Let);
    void send_direct_input(CmdLines& Input);
    void send_input_number(int val);
    void send_run();
    // to mainWindow
    void send_load();
    void send_clear();
    void send_help();
    void send_quit();
};


#endif // CMDPARSER_H
