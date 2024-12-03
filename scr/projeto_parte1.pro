QT += core gui
QT += multimedia
QT += serialport
//QT += core gui charts


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    grafico_getter.cpp \
    main.cpp \
    mainwindow.cpp \
    rfid.cpp \
    sobredialog.cpp

HEADERS += \
    grafico_getter.h \
    mainwindow.h \
    rfid.h \
    sobredialog.h

FORMS += \
    mainwindow.ui \
    sobredialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    arquivo_de_recursos.qrc \


DISTFILES += \
    1.jpeg
