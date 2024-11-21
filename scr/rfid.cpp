#include "rfid.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"  // Definição completa de Ui::MainWindow

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>


RFID::RFID(MainWindow *mainWindow) : ponteiro_mainwindow(mainWindow) {}

void RFID::readSerialData()
{
    static QString buffer; // Buffer para acumular os dados
    QByteArray data = ponteiro_mainwindow->serial->readAll();  // Use o QSerialPort do MainWindow
    buffer += QString::fromUtf8(data);

    int index;
    while ((index = buffer.indexOf("\r\n")) != -1) {
        QString mensagem = buffer.left(index);
        buffer.remove(0, index + 2);

        qDebug() << "Mensagem recebida:" << mensagem;

        if (ponteiro_mainwindow->ui->stackedWidget->currentWidget() == ponteiro_mainwindow->ui->page_2) {
            if (mensagem.contains("ACESSO_PERMITIDO")) {
                ponteiro_mainwindow->ui->stackedWidget->setCurrentWidget(ponteiro_mainwindow->ui->page_4);
                qDebug() << "Acesso permitido. Mudando para a página 4.";
            }
            else if (mensagem.contains("ACESSO_NEGADO")) {
                ponteiro_mainwindow->ui->status_rfid_label->setText("ACESSO NEGADO!");
                qDebug() << "Acesso negado!";
            }
        } else {
            qDebug() << "Não está na página 2. Aguardando para ler.";
        }
    }
}

