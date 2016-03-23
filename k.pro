#-------------------------------------------------
#
# Project created by QtCreator 2016-02-18T22:21:52
#
#-------------------------------------------------

QT       += core widgets
QT        += sql

QMAKE_CXXFLAGS_WARN_ON = -std=c++11 -Wno-unused-parameter

TARGET = k
TEMPLATE = app


SOURCES +=  main.cpp\
				kmain.cpp \
				kf.cpp \
				d.cpp \
				k.cpp \
				ksetup.cpp \
				g.cpp


HEADERS  +=  \
				kmain.h \
				kf.h \
				d.h \
				k.h \
				ksetup.h \
				g.h

FORMS    +=   kmain.ui \
				kf.ui \
				ksetup.ui
