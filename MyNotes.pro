QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainmenu.cpp \
    mainwindow.cpp \
    newrow.cpp \
    notes.cpp \
    timetable.cpp \
    treemodel/commomtextdelegate.cpp \
    treemodel/namedatedelegate.cpp \
    treemodel/treeitem.cpp \
    treemodel/treemodel.cpp \
    xlsx2model_old.cpp \
    xlsx2model.cpp

HEADERS += \
    mainmenu.h \
    mainwindow.h \
    newrow.h \
    notes.h \
    timetable.h \
    treemodel/commomtextdelegate.h \
    treemodel/namedatedelegate.h \
    treemodel/treeitem.h \
    treemodel/treemodel.h


FORMS += \
    mainmenu.ui \
    mainwindow.ui \
    newrow.ui \
    notes.ui \
    timetable.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include($$PWD/xlsx/qtxlsx.pri)
include($$PWD/quazip/quazip.pri)

