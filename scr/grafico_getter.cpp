#include "grafico_getter.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"  // Definição completa de Ui::MainWindow

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

grafico_getter::grafico_getter(MainWindow *mainWindow) : ponteiro_mainwindow(mainWindow){}



void grafico_getter::le_dados_sensores(){
    ponteiro_mainwindow->ui->parar_musica_pushButton->setText("LhjvioWEHVEHJIO");

}


