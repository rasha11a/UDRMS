QT += core gui widgets

TARGET = UDRMS
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    dormitory.cpp \
    restaurant.cpp \
    room.cpp \
    student.cpp \
    roleselectionwindow.cpp \
    studentloginwindow.cpp \
    studentwindow.cpp \
    splashscreen.cpp \
    adminloginwindow.cpp \
    university.cpp \
    welcomewindow.cpp \
    FileManager.cpp


HEADERS += \
    mainwindow.h \
    dormitory.h \
    restaurant.h \
    roleselectionwindow.h \
    room.h \
    student.h \
    splashscreen.h \
    studentloginwindow.h \
    studentwindow.h \
    adminloginwindow.h \
    university.h \
    welcomewindow.h \
    FileManager.h

RESOURCES += \
    resources.qrc