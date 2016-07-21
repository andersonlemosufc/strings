TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    dfa.cpp \
    nfa.cpp

HEADERS += \
    dfa.h \
    nfa.h
