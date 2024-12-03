#ifndef GRAFICO_GETTER_H
#define GRAFICO_GETTER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "mainwindow.h"

class MainWindow;



class grafico_getter: public QObject  // Herda de QObject
{
    Q_OBJECT  // Macro necessária para classes que herdam de QObject


public:
    explicit grafico_getter(MainWindow *mainWindow);
    void le_dados_sensores();
    void processarMensagemTemperatura(const QString& mensagem);
    void salvar_dados();

private slots:



private:

    MainWindow *ponteiro_mainwindow;
};

#endif // GRAFICO_GETTER_H
