//
// Created by 刘洋 on 2022/11/13.
//

#include "Expressions.h"


//void Expressions::printSyntaxTree(Expressions *root, int t)
//{
//    std::queue<synNode> syntax_queue;
//    syntax_queue.push(std::make_pair(t,root));
//    int now_level = t;
//    while(!syntax_queue.empty()){
//        synNode now_node = syntax_queue.front();
//        now_level = now_node.first;
//        for(int i = 1; i <= now_level; ++i) qDebug()<<'\t';
//        qDebug() << QString::fromStdString(now_node.second->val) << '\n';
//        syntax_queue.pop();
//        if(now_node.second->child[0])
//            syntax_queue.push(std::make_pair(now_level+1,now_node.second->child[0]));
//        if(now_node.second->child[1])
//            syntax_queue.push(std::make_pair(now_level+1,now_node.second->child[1]));
//    }
//}

//int Expressions::calSyntaxTree(Expressions* root)
//{
//    std::stack<synNode> tmp;
//    std::stack<int> res;
//    tmp.push(std::make_pair(0,root));
//    while(!tmp.empty()){
//        synNode t = tmp.top();
//        tmp.pop();
//        if(t.first == 0){
//            tmp.push(std::make_pair(1,t.second));
//            if(t.second->child[0]->type == exp_op){
//                tmp.push(std::make_pair(0,t.second->child[0]));
//            }
//            else if(t.second->child[0]->type == exp_num){
//                res.push(stoi(t.second->child[0]->val));
//            } else {
//                res.push(Runner::varMap[t.second->child[0]->val]);
//            }
////                res.push(t.second->child[0]->val);
//        } else if(t.first == 1){
//            tmp.push(std::make_pair(2,t.second));
//            if(t.second->child[1]->type == exp_op){
//                tmp.push(std::make_pair(0,t.second->child[1]));
//            }
//            else if(t.second->child[1]->type == exp_num){
//                res.push(stoi(t.second->child[1]->val));
//            } else {
//                res.push(Runner::varMap[t.second->child[1]->val]);
//            }
//        } else {
//            Expressions*& val2 = tmp.top().second;
//            tmp.pop();
//            Expressions*& val1 = tmp.top().second;
//            tmp.pop();
//            res.push(cal(t.second->val, val2, val1));
//        }
//    }
//}

//int Expressions::cal(std::string op, Expressions*& val2, Expressions*& val1)
//{
//    int v1, v2;
//    if(val1->type == exp_num) v1 = stoi(val1->val);
//    else if(val1->type == exp_var) v1 = Runner::varMap[val1->val];
//    if(val2->type == exp_num) v2 = stoi(val2->val);
//    else if(val2->type == exp_var) v2 = Runner::varMap[val2->val];
//    if(op == "+")
//        return v1 + v2;
//    else if(op == "-")
//        return v1 - v2;
//    else if(op == "*")
//        return v1 * v2;
//    else if(op == "/")
//        return v1 / v2;
//    else if(op == "**")
//        return (int)(pow(v1,v2));
//    else return 0;//error!
//}
