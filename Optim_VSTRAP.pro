TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    src/io/input.cpp \
    src/optimization/gradient_calculator.cpp

HEADERS += \
    src/io/input.h \
    src/optimization/gradient_calculator.h
