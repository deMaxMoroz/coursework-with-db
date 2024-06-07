QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    debtorswindow.cpp \
    entitieswindow.cpp \
    functionswindow.cpp \
    helpwindow.cpp \
    invoiceswindow.cpp \
    main.cpp \
    mainwindow.cpp \
    personswindow.cpp \
    stationswindow.cpp

HEADERS += \
    debtorswindow.h \
    entitieswindow.h \
    functionswindow.h \
    helpwindow.h \
    invoiceswindow.h \
    mainwindow.h \
    personswindow.h \
    stationswindow.h

FORMS += \
    debtorswindow.ui \
    entitieswindow.ui \
    functionswindow.ui \
    helpwindow.ui \
    invoiceswindow.ui \
    mainwindow.ui \
    personswindow.ui \
    stationswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    login.png

RESOURCES += \
    img.qrc
