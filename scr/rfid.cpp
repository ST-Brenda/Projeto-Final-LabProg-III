#include "rfid.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"  // Definição completa de Ui::MainWindow

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>


RFID::RFID(MainWindow *mainWindow) : ponteiro_mainwindow(mainWindow) {}

void RFID::processarMensagemRFID(const QString& mensagem)
{
    if (mensagem.contains("ACESSO_PERMITIDO")) {
        ponteiro_mainwindow->ui->stackedWidget->setCurrentWidget(ponteiro_mainwindow->ui->page_4);
        qDebug() << "Acesso permitido.";
    } else if (mensagem.contains("ACESSO_NEGADO")) {
        ponteiro_mainwindow->ui->status_rfid_label->setText("ACESSO NEGADO!");
        qDebug() << "Acesso negado!";
    }
}
