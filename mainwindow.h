#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QMainWindow>
#include "Utils.h"
#include "runner.h"
#include "helpwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void init_connect();
    void print_tree(const CmdLines& line,int ln = -1);
    void print_syntax_tree(QVector<Expressions> expr_vec, int t = 1);

private:
    Ui::MainWindow *ui;
    Runner* runner;
    HelpWindow* helpWindow;
//    QThread calThread;
    bool cleared = false, catched = false;

signals:
    void call_help_window();
    void print_cmdLines();
    void run_code();

private slots:
    void print_cmds();

    void load();
    void clear();
    void help();
    void print_val(int val);
    void on_cmdLineEdit_returnPressed();
    void on_btnRunCode_clicked();
    void set_input_operations();

    void handle_error(QString what);
};
#endif // MAINWINDOW_H
