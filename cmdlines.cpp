#include "cmdlines.h"

//cmdlines
CmdLines::CmdLines()
{
    lineNumber = -1;
    statement = _Eof;
}

CmdLines::CmdLines(Statement s, int ln, std::string &con)
{
    statement = s;
    lineNumber = ln;
    content = con;
//    expression_root1 = expression_root2 = nullptr;
}

void CmdLines::set_statement(Statement s)
{
    statement = s;
}
Statement CmdLines::get_statement() const
{
    return statement;
}

void CmdLines::set_lineNumber(int ln)
{
    lineNumber = ln;
}
int CmdLines::get_lineNumber() const
{
    return lineNumber;
}

void CmdLines::set_newLineNumber(int ln)
{
    newLineNumber = ln;
}
int CmdLines::get_newLineNumber() const
{
    return newLineNumber;
}

void CmdLines::set_content(std::string& s)
{
    content = s;
}
std::string CmdLines::get_content() const
{
    return content;
}

void CmdLines::set_var(std::string& s)
{
    var = s;
}
std::string CmdLines::get_var() const
{
    return var;
}

void CmdLines::set_contrl(std::string& s)
{
    contrl = s;
}
std::string CmdLines::get_contrl() const
{
    return contrl;
}
