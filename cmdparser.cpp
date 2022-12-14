 #include "cmdparser.h"

CmdParser::CmdParser(QObject *parent) : QObject(parent)
{
//    pos = 0;
    line_number = -1;
    is_input = false;
}

Statement CmdParser::read_space()
{
    size_t index = 0;
    for(auto iter : curText)
    {
        index++;
        if(iter != ' ') curOp += iter;
        else{
            if(!curOp.size()){
                continue;
            } else{
                curText = curText.substr(index);
                return judge();
            }
        }
    }
    if(curOp.empty()){
        curText.resize(0);
        return _Eof;
    }
    else{
        curText.resize(0);
        return judge();
    }
}

Statement CmdParser::judge()
{
    if(curOp == "REM") return _Rem;
    else if(curOp == "LET") return _Let;
    else if(curOp == "PRINT") return _Print;
    else if(curOp == "INPUT") return _Input;
    else if(curOp == "GOTO") return _Goto;
    else if(curOp == "IF") return _If;
    else if(curOp == "THEN") return _Then;
    else if(curOp == "END") return _End;
    else if(curOp == "RUN") return _Run;
    else if(curOp == "LOAD") return _Load;
    else if(curOp == "LIST") return _List;
    else if(curOp == "CLEAR") return _Clear;
    else if(curOp == "HELP") return _Help;
    else if(curOp == "QUIT") return _Quit;
    else if(curOp == "<" || curOp == ">" || curOp == "=") return _Condition;
    else {
        for(auto iter : curOp){
            if(iter < '0' || iter > '9') return _Other;
        }
        return _Number;
    }

}

void CmdParser::parse(std::string &cmd)
{
    setText(cmd);
//    qDebug() << cmd.c_str();
    Statement statement = _Eof;
    line_number = -1;
//    if(!curOp.empty()) curOp = cmd;
    while(_Eof != (statement = read_space())){
        switch (statement) {
        case _Rem:
            if(-1 != line_number){
                CmdLines rem(_Rem, line_number, text);
                int index = curText.find_first_not_of(" ");
                curText = curText.substr(index);
                rem.set_var(curText);
                cmds[line_number] = rem;
                line_number = -1;
            }
            else{
                throw wrong_line_number();
            }
            curText.resize(0);
            curOp.resize(0);
            break;

        case _Let:
        {
            std::string curVar;
            curOp.resize(0);
            if(_Other == read_space() && curOp != "="){
                curVar = curOp;
                curOp.resize(0);
                if(!is_var(curVar)) throw wrong_expression();

                if(_Condition == read_space() && curOp == "="){
                    CmdLines let(_Let, line_number, text);
                    del_space(curText);
//                    let.expression_root1 = parseExpr(curText);
                    let.expr_vec1 = parseExpr(curText/*,let.expression_root1*/);
                    let.set_var(curVar);
                    if(-1 != line_number){
                        cmds[line_number] = let;
                    } else {
                        emit this->send_direct_let(let);
                    }
                    line_number = -1;
                } else {
                    throw wrong_command();
                }
            } else {
                throw wrong_command();
            }
            curText.resize(0);
            curOp.resize(0);
            break;
        }

        case _Print:
        {
            CmdLines print(_Print, line_number, text);
            del_space(curText);
//            print.expression_root1 = parseExpr(curText);
            print.expr_vec1 = parseExpr(curText/*,print.expression_root1*/);
            if(-1 == line_number){
                emit this->send_direct_print(print);
            } else {
                cmds[line_number] = print;
            }
            line_number = -1;
            curOp.resize(0);
            curText.resize(0);
            break;
        }

        case _Input:
        {
            curOp.resize(0);
            if(_Other == (statement = read_space()) || _Eof == statement){
                CmdLines input(_Input, line_number, text);
                input.set_var(curOp);
                if(-1 != line_number)
                    cmds[line_number] = input;
                else{
                    set_direct_input(true);
                    emit send_direct_input(input);
                }
                line_number = -1;
            } else {
                throw wrong_command();
            }
            curOp.resize(0);
            curText.resize(0);
            return;
        }

        case _Goto:
            if(-1 != line_number){
                curOp.resize(0);
                if(-1 != read_space()){
                    CmdLines Goto(_Goto, line_number, text);
                    Goto.set_newLineNumber(stoi(curOp));
                    cmds[line_number] = Goto;
                    line_number = -1;
                } else {
                    throw wrong_command();
                }
            } else {
                throw wrong_line_number();
            }
            curOp.resize(0);
            curText.resize(0);
            break;

        case _If:
            if(-1 != line_number){
                std::string expr;
                curOp.resize(0);
                CmdLines If(_If, line_number, text);

                while(_Condition != read_space()){
                    expr += curOp;
                    curOp.resize(0);
                }
                If.set_contrl(curOp);
                del_space(expr);
//                If.expression_root1 = parseExpr(expr);
                If.expr_vec1 = parseExpr(expr/*,If.expression_root1*/);
                curOp.resize(0), expr.resize(0);

                while(_Then != read_space()){
                    expr += curOp;
                    curOp.resize(0);
                }
                del_space(expr);
//                If.expression_root2 = parseExpr(expr);
                If.expr_vec2 = parseExpr(expr/*,If.expression_root2*/);
                curOp.resize(0), expr.resize(0);

                if(-1 != read_space()) {
                    If.set_newLineNumber(stoi(curOp));
                    cmds[line_number] = If;
                    line_number = -1;
                } else {
                    throw wrong_command();
                }
            } else {
                throw wrong_line_number();
            }
            curOp.resize(0);
            curText.resize(0);
            break;

        case _End:
            if(-1 != line_number){
                CmdLines End(_End, line_number, text);
                cmds[line_number] = End;
                line_number = -1;
            } else {
                throw wrong_line_number();
            }
            curOp.resize(0);
            curText.resize(0);
            break;

        case _Run:
            curOp.resize(0);
            curText.resize(0);
            emit this->send_run();
            return;

        case _Help:
            curOp.resize(0);
            curText.resize(0);
            emit this->send_help();
            return;

        case _Load:
            curOp.resize(0);
            curText.resize(0);
            emit this->send_load();
            return;

        case _Clear:
            curOp.resize(0);
            curText.resize(0);
            emit this->send_clear();
            return;

        case _Quit:
            curOp.resize(0);
            curText.resize(0);
            emit this->send_quit();
            return;

        case _Number:
            line_number = stoi(curOp);
            curOp.resize(0);
            break;

        default:
            curOp = curOp.substr(curOp.find_first_not_of(" "));
            if(!curOp.empty()) {
                curOp.resize(0);
                throw wrong_cmd_type();
            }
            break;
        }
    }
    if(-1 != line_number && !is_input){
        for(auto t : cmds){
            if(t.first == line_number){
                cmds.erase(line_number);
                break;
            }
        }
        line_number = -1;
        curOp.resize(0);
        curText.resize(0);
    } else if(is_input){
        if(-1 != line_number)
            emit this->send_input_number(line_number);
        else{
            curOp = curOp.substr(curOp.find_first_not_of(" "));
            emit this->send_input_number(stoi(curOp));
        }
        line_number = -1;
        curOp.resize(0);
        curText.resize(0);
    }
}


// parse the expressions [DONE]

void CmdParser::del_space(std::string &expr)
{
    int index = 0;
    if(!expr.empty()){
        while((index = expr.find(' ',index)) != std::string::npos){
            expr.erase(index,1);
        }
    }
}

bool CmdParser::is_pri(Operators t, std::string& t2)
{
    Operators tail;
    if(t2 == "+") tail = _Add;
    else if(t2 == "-") tail = _Minus;
    else if(t2 == "*") tail = _Multi;
    else if(t2 == "/") tail = _Devide;
    else if(t2 == "**") tail = _Power;
    else if(t2 == "(") tail = _Paren_start;
    else tail = _Paren_end;
    switch (t) {
    case _Add: case _Minus:
        if(tail < _Paren_start)
            return false;
        else return true;
    case _Multi: case _Devide:
        if(tail >= _Multi && tail < _Paren_start) return false;
        return true;
    case _Power:
//        if(tail >= _Power) return false;
        return true;
    default:
        //error
        throw wrong_expression();
        return false;
    }
}

Operators CmdParser::read_expr(std::string &expr, char &c)
{
    int index = 0, end = expr.size();
    while(index < end){
        c = expr[index++];
        switch (c) {
        case '+':
            expr = expr.substr(index);
            return _Add;
        case '-':
            expr = expr.substr(index);
            return _Minus;
        case '*':
            if('*' == expr[index]){
                index++;
                expr = expr.substr(index);
                return _Power;
            } else {
                expr = expr.substr(index);
                return _Multi;
            }
        case '/':
            expr = expr.substr(index);
            return _Devide;
        case '(':
            expr = expr.substr(index);
            return _Paren_start;
        case ')':
            expr = expr.substr(index);
            return _Paren_end;
        case ' ':
            break;
        case '1': case '2': case '3': case '4': case '5': case '6':
        case '7': case '8': case '9': case '0':
            expr = expr.substr(index);
            return _Numbers;
        default:
            expr = expr.substr(index);
            return _Var;
        }
    }
    return _Eoe;
}

QVector<Expressions> CmdParser::parseExpr(std::string expr)
{
    QVector<Expressions> expr_vec = parse_expr(expr);
//    root = create_expr_link(expr_vec);
    check(expr_vec);
    return expr_vec;
}

void CmdParser::check(QVector<Expressions> &expr_vec)
{
    int expr_num = expr_vec.size();
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
}

//new parse
QVector<Expressions> CmdParser::parse_expr(std::string &expr)
{
    Operators tmp;
    Exp_Type expType = exp_num;
    char c;
    std::string val;
    QVector<Expressions> exp_vec, op_vec;
    while(_Eoe != (tmp = read_expr(expr, c))){
        switch (tmp) {
        case _Var:
            expType = exp_var;
        case _Numbers:
            val += c;
            break;
        case _Minus:
            if(val.size() == 0){
                Expressions new_var(expType,"0");
                new_var.is_added = true;
                exp_vec.push_back(new_var);
            }
        case _Add : case _Multi: case _Devide: case _Power:{
            if(val.size()){
                Expressions new_var (expType,val);
                expType = exp_num, val.resize(0);
                exp_vec.push_back(new_var);
            }
            Expressions new_op(exp_op, opArr[tmp-_Add]);
            while(!op_vec.empty() && !is_pri(tmp,op_vec.last().val)){
                auto new_push = op_vec.last();
                op_vec.pop_back();
                exp_vec.push_back(new_push);
            }
            op_vec.push_back(new_op);
            break;
        }
        case _Paren_start:
        {
            if(val.size()){
                Expressions new_var(expType,val);
                exp_vec.push_back(new_var);
                val.resize(0); expType = exp_num;
            }
            Expressions new_op(exp_op, opArr[tmp-_Add]);
            op_vec.push_back(new_op);
            break;
        }
        case _Paren_end:
        {
            if(val.size()){
                Expressions new_var(expType,val);
                exp_vec.push_back(new_var);
                val.resize(0); expType = exp_num;
            }
            auto last_one = op_vec.last();
            while(last_one.val != "("){
                exp_vec.push_back(last_one);
                op_vec.pop_back();
                if(!op_vec.empty()) last_one = op_vec.last();
                else throw wrong_expression();
            }
            op_vec.pop_back();
            break;
        }
        default:
            throw wrong_expression();
            break;
        }
    }
    if(val.size()){
        Expressions new_var(expType,val);
        exp_vec.push_back(new_var);
        val.resize(0); expType = exp_num;
    }
    while(!op_vec.empty()){
        exp_vec.push_back(op_vec.last());
        op_vec.pop_back();
    }
    return exp_vec;
}



#if 0
Expressions* CmdParser::parse_expr(std::string& expr)
{
    if(!expr.size()) return nullptr;
    Operators tmp;
    Exp_Type expType = exp_num;
    char c, d;
    bool paren = false;
    std::string val;
    Expressions *now_node = nullptr, *root_node = nullptr; // point to the operators
    while(_Eoe != (tmp = read_expr(expr, c))){
        bool is_added = false;
        switch (tmp) {
        case _Var:
            expType = exp_var;
        case _Numbers:
            val += c;
            break;
        case _Minus:
            if((!root_node || !now_node->child[1]) && val.empty()){
                val = "0";
                is_added = true;
//                val += c;
//                break;
            }
        case _Add: case _Multi: case _Devide: case _Power:
        {
            // num and var node
//            if(val.empty()) throw wrong_expression();
            if(expType == exp_var && !is_var(val))
                throw wrong_expression();
            Expressions *new_var = new Expressions(expType, val, now_node);
            if(is_added){
                new_var->is_added = true;
                is_added = false;
            }
            if(now_node && now_node->type == exp_op){ // has an old tree
                if(!now_node->child[1]) now_node->child[1] = new_var;
            }
            expType = exp_num; val.resize(0);

            // operator node
            if(!is_op(tmp)) throw wrong_expression();
            if(!root_node){// a new tree
                Expressions *new_op = new Expressions(exp_op, opArr[tmp-_Add], new_var, nullptr);
                root_node = new_op;
                now_node = root_node;
            } else {
                while(!paren && now_node && !is_pri(tmp,now_node->val)){
                    now_node = now_node->parent;
                }
                if(!now_node){// should be a new root
                    Expressions *new_op = new Expressions(exp_op, opArr[tmp-_Add], root_node, nullptr);
                    root_node = new_op;
                    now_node = root_node;
                } else {
                    Expressions *new_op = new Expressions(exp_op, opArr[tmp-_Add], now_node->child[1], now_node);
                    now_node->child[1] = new_op;
                    now_node = new_op;
                }
            }
            paren = false;
            break;
        }
        case _Paren_start:
            if(now_node) now_node->child[1] = parse_expr(expr);
            else root_node = parse_expr(expr);
            paren = true;
            break;
        case _Paren_end:
            if(now_node && !now_node->child[1]){
                Expressions* new_var = new Expressions(expType,val,now_node);
                expType = exp_num, val.resize(0);
                now_node->child[1] = new_var;
                return root_node;
            } else if(!now_node){
                if(val.size()){
                    Expressions* new_var = new Expressions(expType, val, now_node);
                    expType = exp_num, val.resize(0);
                    root_node = now_node = new_var;
                }
            }
            return root_node;
        default:
            throw wrong_expression();
        }
    }
    if(!val.empty()){
        Expressions *new_var = new Expressions(expType, val, now_node);
        if(!root_node) root_node = new_var;
        else now_node->child[1] = new_var;
    }
    if(root_node)
        return root_node;
    else
        throw wrong_expression();
}
#endif

void CmdParser::set_input_status(bool status)
{
    is_input = status;
}

bool CmdParser::is_var(std::string &val)
{
    if(val == "*" || val == "+" || val == "-" || val == "/" || val == "**" ||
      (val[0] > '0' && val[0] <= '9'))
        return false;
    return true;
}

bool CmdParser::is_op(Operators op)
{
    if(op == _Add || op == _Minus || op == _Multi || op == _Devide || op == _Power)
        return true;
    else return false;
}



