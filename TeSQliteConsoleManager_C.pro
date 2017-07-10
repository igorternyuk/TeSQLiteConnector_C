TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    sqlite3.c \
    manager.c

HEADERS += \
    sqlite3.h \
    manager.h

LIBS += -lpthread -ldl

QMAKE_CFLAGS += -std=c11

