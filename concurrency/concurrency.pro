TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        peterson.cpp \
        philosophers.cpp

HEADERS += \
    lamportbakery.hpp \
    peterson.h \
    philosophers.h
