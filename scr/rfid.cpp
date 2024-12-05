#include "rfid.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"  // Definição completa de Ui::MainWindow

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>


// Construtor com um ponteiro para um objeto MainWindow, chamado mainWindow
// O ponteiro é armazenado em um atributo da classe RFID, chamado ponteiro_mainwindow
// Esse ponteiro permite que a classe RFID modifique os elementos da UI definidos na MainWindow
RFID::RFID(MainWindow *mainWindow) : ponteiro_mainwindow(mainWindow) {}

void RFID::processarMensagemRFID(const QString& mensagem)
{
    if (mensagem.contains("ACESSO_PERMITIDO")) {   // Verifica se a mensagem contém "ACESSO_PERMITIDO"
        ponteiro_mainwindow->ui->stackedWidget->setCurrentWidget(ponteiro_mainwindow->ui->page_4);
        qDebug() << "Acesso permitido.";
    } else if (mensagem.contains("ACESSO_NEGADO")) {  // Verifica se a mensagem contém "ACESSO_NEGADO"
        ponteiro_mainwindow->ui->status_rfid_label->setText("ACESSO NEGADO!");   // Muda o texto da label do status do RFID
        qDebug() << "Acesso negado!";
    }
}
