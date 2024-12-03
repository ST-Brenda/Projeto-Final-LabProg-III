#ifndef RFID_H
#define RFID_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "mainwindow.h"


class MainWindow;



class RFID : public QObject  // Herda de QObject
{
    Q_OBJECT  // Macro necessária para classes que herdam de QObject



public:
    explicit RFID(MainWindow *mainWindow);
    void processarMensagemRFID(const QString& mensagem);


private slots:


private:
    MainWindow *ponteiro_mainwindow;

};

#endif // RFID_H
