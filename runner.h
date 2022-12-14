#ifndef RUNNER_H
#define RUNNER_H

#pragma once

#include <QObject>
#include "Utils.h"
#include "cmdparser.h"

class Runner : public QThread
{
    Q_OBJECT
public:
    explicit Runner(QObject *parent = nullptr);

//    void printSyntaxTree(Expressions* root, int t = 1);

    void seq_runner();
//    int calSyntaxTree(Expressions* root);
    int calSyntaxTree(QVector<Expressions>& expr_vec);
    void clear_info();
//    void check_legal_lines();
    inline void set_input(bool status = false)
    {
        is_input = status;
    }
    inline void resume()
    {
        mutex.lock();
        is_input = false;
        mutex.unlock();
        input_cond.wakeAll();
    }
    inline void pause()
    {
        mutex.lock();
        is_input = true;
        mutex.unlock();
    }

private:
    int cal(std::string op, int val2, int val1);
    void run() override;

public:
    std::unordered_map<std::string, int> varMap;
    CmdParser parser;

private:
    std::string now_input_var;
    bool is_input = false;
    QMutex mutex;
    QWaitCondition input_cond;

signals:
    void send_print(int val);

    void send_input();
    void end_runner();

    void send_error(QString);

private slots:

    void handle_let(CmdLines& Let);
    int handle_if(CmdLines& If);
    void handle_print(CmdLines& Print);
    int handle_goto(CmdLines& Goto);
    void handle_input(CmdLines& Input);

    void set_input_val(int n);

public:
//    QObject::connect(parser, )
};

#endif // RUNNER_H
