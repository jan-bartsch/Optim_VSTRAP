TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    src/io/input.cpp \
    src/io/output_control_update.cpp \
    src/io/output_diagnostics.cpp \
    src/io/tinyXML/tinystr.cpp \
    src/io/tinyXML/tinyxml.cpp \
    src/io/tinyXML/tinyxmlerror.cpp \
    src/io/tinyXML/tinyxmlparser.cpp \
    src/objects/particle.cpp \
    src/optimization/gradient_calculator.cpp \
    test/test_io.cpp \
    test/test_main.cpp

HEADERS += \
    src/io/input.h \
    src/io/output_control_update.h \
    src/io/output_diagnostics.h \
    src/io/tinyXML/tinystr.h \
    src/io/tinyXML/tinyxml.h \
    src/objects/particle.h \
    src/optimization/gradient_calculator.h \
    test/test_io.h \
    test/test_main.h
