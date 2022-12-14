#ifndef UTILS_H
#define UTILS_H

#pragma once

#include <map>
#include <string>
#include <QString>
#include <unordered_map>
#include <map>
#include <cstring>
#include <queue>
#include <stack>
#include <QDebug>
#include <cmath>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <exception>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>



struct wrong_line_number : public std::exception
{
  const char * what () const throw ()
  {
    return "No Line Number!";
  }
};

struct wrong_command : public std::exception
{
  const char * what () const throw ()
  {
    return "Wrong Command";
  }
};

struct wrong_cmd_type : public std::exception
{
  const char * what () const throw ()
  {
    return "Wrong Command Type!";
  }
};

struct wrong_expression : public std::exception
{
  const char * what () const throw ()
  {
    return "Wrong Expression";
  }
};

struct undefined_command : public std::exception
{
  const char * what () const throw ()
  {
    return "Undefined Command!";
  }
};

struct line_number_overflow : public std::exception
{
  const char * what () const throw ()
  {
    return "No Such Line Number!";
  }
};

#endif // UTILS_H
