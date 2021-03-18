QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chart.cpp \
    main.cpp \
    mainwindow.cpp \
    pixels.cpp

HEADERS += \
    chart.h \
    mainwindow.h \
    pixels.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -LD:/boost_1_75_0/boost_gcc_840/lib/ -llibboost_math_tr1-mgw48-mt-x32-1_75.dll

INCLUDEPATH += D:/boost_1_75_0/boost_gcc_840/include/boost-1_75
DEPENDPATH += D:/boost_1_75_0/boost_gcc_840/include/boost-1_75

unix|win32: LIBS += -LD:/boost_1_75_0/boost_gcc_840/lib/ -llibboost_timer-mgw48-mt-x32-1_75.dll

INCLUDEPATH += D:/boost_1_75_0/boost_gcc_840/include/boost-1_75
DEPENDPATH += D:/boost_1_75_0/boost_gcc_840/include/boost-1_75

unix|win32: LIBS += -LD:/boost_1_75_0/boost_gcc_840/lib/ -llibboost_date_time-mgw48-mt-x32-1_75.dll

INCLUDEPATH += D:/boost_1_75_0/boost_gcc_840/include
DEPENDPATH += D:/boost_1_75_0/boost_gcc_840/include
