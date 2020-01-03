TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    src/io/input.cpp \
    src/io/output_control_update.cpp \
    src/io/output_diagnostics.cpp \
    src/objects/particle.cpp \
    src/optimization/gradient_calculator.cpp \
    test/test_io.cpp

HEADERS += \
    src/io/input.h \
    src/io/output_control_update.h \
    src/io/output_diagnostics.h \
    src/objects/particle.h \
    src/optimization/gradient_calculator.h \
    test/test_io.h
