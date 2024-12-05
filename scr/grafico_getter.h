#ifndef GRAFICO_GETTER_H
#define GRAFICO_GETTER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "mainwindow.h"

class MainWindow;



class grafico_getter
{

public:
    explicit grafico_getter(MainWindow *mainWindow);
    void le_dados_sensores();
    void processarDadosSensores(const QString& mensagem);
    void salvar_dados();

private slots:



private:

    MainWindow *ponteiro_mainwindow;
};

#endif // GRAFICO_GETTER_H
