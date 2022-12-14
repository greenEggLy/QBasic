#include "runner.h"

Runner::Runner(QObject *parent) : QThread(parent)
{
    now_input_var = "";
    connect(&this->parser,&CmdParser::send_direct_print,this,&Runner::handle_print);
    connect(&this->parser, &CmdParser::send_direct_let, this, &Runner::handle_let);
    connect(&this->parser, &CmdParser::send_direct_input, this, &Runner::handle_input);
    connect(&this->parser, &CmdParser::send_input_number, this, &Runner::set_input_val);
    connect(&this->parser, &CmdParser::send_clear, this, &Runner::clear_info);
}


void Runner::run()
{
    try {
       seq_runner();
    } catch (std::exception& e) {
        emit send_error(QString(e.what()));
//        this->ui->cmdLineEdit->setText(QString(e.what()));
//        this->ui->cmdLineEdit->insert(QString(" PRESS ENTER..."));
    }
}

//new calculation
int Runner::calSyntaxTree(QVector<Expressions>& expr_vec)
{
    QVector<int> ans_vec;
    for(auto &iter: expr_vec){
        if(iter.type == exp_num){
            ans_vec.push_back(std::stoi(iter.val));
        } else if(iter.type == exp_var){
            ans_vec.push_back(Runner::varMap[iter.val]);
        } else {
            int val2 = ans_vec.last();
            ans_vec.pop_back();
            int val1 = ans_vec.last();
            ans_vec.pop_back();
            int ans = cal(iter.val,val2,val1);
            ans_vec.push_back(ans);
        }
    }
    return ans_vec.first();
}

int Runner::cal(std::string op, int val2, int val1)
{
    int v1 = val1, v2 = val2;
    if(op == "+")
        return v1 + v2;
    else if(op == "-")
        return v1 - v2;
    else if(op == "*")
        return v1 * v2;
    else if(op == "/")
        return v1 / v2;
    else if(op == "**")
        return (int)(pow(v1,v2));
    else return 0;//error!
}

void Runner::seq_runner()
{
    std::map<size_t, CmdLines>::iterator line;
    bool changed = false;
    for(line = parser.cmds.begin(); line != parser.cmds.end(); )
    {
        mutex.lock();
        if(is_input)
            input_cond.wait(&mutex);
        mutex.unlock();
        switch (line->second.get_statement()) {
        case _Rem:
            break;
        case _Let:
            handle_let(line->second);
            break;
        case _If:{
            size_t curLine = line->first;
            size_t newLine = handle_if(line->second);
            if(curLine == newLine) break;
            line = parser.cmds.find(newLine);
            if(line == parser.cmds.end()) {

                throw line_number_overflow();
            }
            changed = true;
            break;
        }
        case _Print:{
            handle_print(line->second);
            break;
        }
        case _Goto:
            line = parser.cmds.find(handle_goto(line->second));
            if(line == parser.cmds.end()) throw line_number_overflow();
            changed = true;
            break;
        case _Input:
            set_input(true);
            now_input_var = line->second.get_var();
            emit send_input();
            break;
        case _End:
            return;
        default:
            throw undefined_command();
        }
        if(!changed/* && !is_input*/) line++;
        else changed = false;
    }
}

void Runner::handle_let(CmdLines &Let)
{
//    int val = calSyntaxTree(Let.expression_root1);
    int val = calSyntaxTree(Let.expr_vec1);
    varMap[Let.get_var()] = val;
}


int Runner::handle_if(CmdLines &If)
{
//    int left_val = calSyntaxTree(If.expression_root1);
//    int right_val = calSyntaxTree(If.expression_root2);
    int left_val = calSyntaxTree(If.expr_vec1);
    int right_val = calSyntaxTree(If.expr_vec2);
    bool run = false;
    if(If.get_contrl() == ">"){
        if(left_val > right_val) run = true;
    } else if(If.get_contrl() == "<") {
        if(left_val < right_val) run = true;
    } else if(If.get_contrl() == "=") {
        if(left_val == right_val) run = true;
    }
    if(run) return If.get_newLineNumber();
    else return If.get_lineNumber();
}

int Runner::handle_goto(CmdLines &Goto)
{
    return Goto.get_newLineNumber();
}

void Runner::handle_print(CmdLines &Print)
{
//    int value = varMap[Print.get_var()] = calSyntaxTree(Print.expression_root1);
    int value = varMap[Print.get_var()] = calSyntaxTree(Print.expr_vec1);
    emit send_print(value);
}

void Runner::handle_input(CmdLines& Input)
{
    Q_UNUSED(Input);
//    this->parser.set_input_status();
    now_input_var = Input.get_var();
    emit send_input();
}

void Runner::set_input_val(int n)
{
    if(!now_input_var.empty()){
        varMap[now_input_var] = n;
        now_input_var.resize(0);
    }
}

void Runner::clear_info()
{
    this->parser.cmds.clear();
    varMap.clear();
    this->set_input(false);
    this->parser.set_input_status(false);
}
