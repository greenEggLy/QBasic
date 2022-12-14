#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeDisplay->clear();
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
    runner = new Runner;
    helpWindow = new HelpWindow;
    init_connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::init_connect()
{
    connect(&this->runner->parser,&CmdParser::send_load,this,&MainWindow::load);
    connect(this->ui->btnLoadCode, &QPushButton::clicked, this, &MainWindow::load);
    connect(&this->runner->parser,&CmdParser::send_clear,this,&MainWindow::clear);
    connect(this->ui->btnClearCode, &QPushButton::clicked, this, &MainWindow::clear);
    connect(&this->runner->parser, &CmdParser::send_help, this->helpWindow, &HelpWindow::show);
    connect(&this->runner->parser, &CmdParser::send_quit, this, &MainWindow::close);
    connect(this->runner, &Runner::send_input, this, &MainWindow::set_input_operations);
    connect(this->runner, &Runner::send_print, this, &MainWindow::print_val);
    connect(this, &MainWindow::print_cmdLines, this, &MainWindow::print_cmds);
    connect(&this->runner->parser, &CmdParser::send_run, this, [&](){
        try{
            on_btnRunCode_clicked();
        } catch (std::exception& e){
            this->ui->cmdLineEdit->setText(QString(e.what()));
            this->ui->cmdLineEdit->insert(QString(" PRESS ENTER..."));
            catched = true;
        }
    });
    connect(this->runner, &Runner::send_error, this, &MainWindow::handle_error);
}

void MainWindow::on_cmdLineEdit_returnPressed()
{
    QString cmd = ui->cmdLineEdit->text();
    std::string cmdd = cmd.toStdString();
    ui->cmdLineEdit->clear();
    ui->CodeDisplay->clear();
    ui->treeDisplay->clear();
    if(catched){
        catched = false;
        cmdd = cmdd.substr(cmdd.find_first_of("...")+3);
    }
    if(!cmdd.empty()){
        if(runner->parser.get_input_status()){
            cmdd = cmdd.substr(cmdd.find_first_of("?")+1);
        }
        try {
            runner->parser.parse(cmdd);
        } catch (std::exception& e) {
            this->ui->cmdLineEdit->setText(QString(e.what()));
            this->ui->cmdLineEdit->insert(QString(" PRESS ENTER..."));
            catched = true;
        }
        if(runner->parser.get_input_status()){
            if(runner->parser.get_direct_input()){
                runner->parser.set_direct_input(false);
            } else {
                runner->parser.set_input_status(false);
                runner->resume();
            }
        }
    }
    print_cmds();
}

void MainWindow::print_cmds()
{
    for(auto &iter : runner->parser.cmds){
        try{
            ui->CodeDisplay->append(QString::fromStdString(iter.second.get_content()));
            print_tree(iter.second,iter.second.get_lineNumber());
        } catch(std::exception& e){
            this->ui->cmdLineEdit->setText(QString(e.what()));
            this->ui->cmdLineEdit->insert(QString(" PRESS ENTER..."));
            catched = true;
//            runner->parser.cmds.erase(iter);
//            break;
        }
    }
}


void MainWindow::clear()
{
    this->runner->parser.cmds.clear();
    ui->CodeDisplay->clear();
    ui->treeDisplay->clear();
    ui->textBrowser->clear();
    ui->cmdLineEdit->clear();
    cleared = true;
}

void MainWindow::help()
{
    emit call_help_window();
}


void MainWindow::load()
{
    runner->parser.cmds.clear();
    this->ui->CodeDisplay->clear();
    this->ui->textBrowser->clear();
    this->ui->treeDisplay->clear();
    QString fileName = QFileDialog::getOpenFileName(this,"Open");
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","Failed to open file: " + file.errorString());
        return;
    }
    this->runner->clear_info();
    QTextStream stream(&file);
    QString text;
    while(!stream.atEnd()){
        text = stream.readLine();
        std::string cmd = text.toStdString();
        runner->parser.parse(cmd);
    }
    emit print_cmdLines();
}

void MainWindow::print_tree(const CmdLines& line, int ln)
{
    Statement s = line.get_statement();
    if(-1 == ln) return;
    switch (s) {
    case _Let:
        this->ui->treeDisplay->append(QString::number(ln) + QString(" Let =\n"));
        this->ui->treeDisplay->insertPlainText(QString("    "));
        this->ui->treeDisplay->insertPlainText(QString::fromStdString(line.get_var()));
        print_syntax_tree(line.expr_vec1);
        break;

    case _Rem:
        this->ui->treeDisplay->append(QString::number(ln) + QString(" REM\n"));
        this->ui->treeDisplay->insertPlainText(QString("    "));
        this->ui->treeDisplay->insertPlainText(QString::fromStdString(line.get_var()));
        break;

    case _If:
        this->ui->treeDisplay->append(QString::number(ln) + QString(" IF THEN"));
        print_syntax_tree(line.expr_vec1);
        print_syntax_tree(line.expr_vec2);
        break;

    case _Print:
        this->ui->treeDisplay->append(QString::number(ln) + QString(" Print"));
        print_syntax_tree(line.expr_vec1);
        break;

    case _Input:
        break;

    default:
        break;
    }

}


void MainWindow::print_syntax_tree(QVector<Expressions> expr_vec, int t)
{
    int expr_num = expr_vec.size();
    std::queue<synNode> syntax_queue;
//    QVector<Expressions> new_vec;
    QVector<int> index_vec;

    for(int i = 0; i < expr_num; ++i){
        if(expr_vec[i].type != exp_op){
            index_vec.push_back(i);
            continue;
        } else {
            int ch1 = index_vec.last();
            expr_vec[i].child[1] = &expr_vec[ch1];
            index_vec.pop_back();

            int ch2 = index_vec.last();
            expr_vec[i].child[0] = &expr_vec[ch2];
            index_vec.pop_back();

            index_vec.push_back(i);
        }
    }
    if(index_vec.size() > 1) throw wrong_expression();
    int last = index_vec.last();
    Expressions* root = &expr_vec[last];
//    Expressions* root = &ret;
    index_vec.pop_back();

    syntax_queue.push(std::make_pair(t,root));
    int now_level = t;
    while(!syntax_queue.empty()){
        synNode now_node = syntax_queue.front();
        now_level = now_node.first;
        if(1||!now_node.second->is_added){
            this->ui->treeDisplay->insertPlainText(QString("\n"));
            for(int i = 1; i <= now_level; ++i) {
                this->ui->treeDisplay->insertPlainText(QString("    "));
            }
            this->ui->treeDisplay->insertPlainText(QString::fromStdString(now_node.second->val));
        }
        syntax_queue.pop();
        if(now_node.second->child[0])
            syntax_queue.push(std::make_pair(now_level+1,now_node.second->child[0]));
        if(now_node.second->child[1])
            syntax_queue.push(std::make_pair(now_level+1,now_node.second->child[1]));
    }
}

void MainWindow::print_val(int val)
{
    this->ui->textBrowser->append(QString::number(val));
}

void MainWindow::on_btnRunCode_clicked()
{
    this->ui->textBrowser->clear();
    runner->start();
}

void MainWindow::set_input_operations()
{
    this->ui->cmdLineEdit->setText(QString("    ? "));
    this->runner->pause();
    this->runner->parser.set_input_status(true);
}

void MainWindow::handle_error(QString what)
{
    this->ui->cmdLineEdit->setText(what);
    this->ui->cmdLineEdit->insert(QString(" PRESS ENTER..."));
    catched = true;
}
