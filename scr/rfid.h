#ifndef RFID_H
#define RFID_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "mainwindow.h"


class MainWindow;



class RFID
{
public:
    explicit RFID(MainWindow *mainWindow);
    void processarMensagemRFID(const QString& mensagem);

private:
    MainWindow *ponteiro_mainwindow;
};


#endif // RFID_H
