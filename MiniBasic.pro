QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Expressions.cpp \
    cmdlines.cpp \
    cmdparser.cpp \
    helpwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    runner.cpp

HEADERS += \
    Expressions.h \
    Statements.h \
    Utils.h \
    cmdlines.h \
    cmdparser.h \
    helpwindow.h \
    mainwindow.h \
    runner.h

FORMS += \
    helpwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
