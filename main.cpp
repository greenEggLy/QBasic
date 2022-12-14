#include "mainwindow.h"

#include <QApplication>
#include "Utils.h"
//#include "cmdparser.h"
#include "runner.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
//    Runner runner;
//    std::string str = "100 PRINT 4 + 5 * 3";
//    runner.parser.parse(str);
//    runner.printSyntaxTree(runner.parser.cmds[100].expression_root1);
//    qDebug()<<runner.calSyntaxTree(runner.parser.cmds[100].expression_root1);
    return 0;
}
