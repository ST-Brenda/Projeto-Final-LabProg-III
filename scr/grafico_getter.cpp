#include "grafico_getter.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"  // Definição completa de Ui::MainWindow

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>


grafico_getter::grafico_getter(MainWindow *mainWindow) : ponteiro_mainwindow(mainWindow){

    //PARA AS COLUNAS DA TABELA:

    ponteiro_mainwindow->ui->dados_tableWidget->setColumnWidth(0,200);
    ponteiro_mainwindow->ui->dados_tableWidget->setColumnWidth(1,200);
    ponteiro_mainwindow->ui->dados_tableWidget->setColumnWidth(2,200);
    ponteiro_mainwindow->ui->dados_tableWidget->setColumnWidth(3,200);

}


//void grafico_getter::le_dados_sensores(){
//    ponteiro_mainwindow->ui->parar_musica_pushButton->setText("SALVAR");

//}


void grafico_getter::processarMensagemTemperatura(const QString& mensagem)
{
    if (mensagem.contains("T=")) { // Verifica se a mensagem contém "TMP="
        bool ok;
        QString tempStr = mensagem.split("T=")[1]; // Extrai o valor após "T="
        double temperatura = tempStr.toDouble(&ok);

        if (ok) {
            // Obtém o horário atual do sistema operacional
            QString horarioAtual = QTime::currentTime().toString("HH:mm");

            // Adiciona nova linha na tabela
            int currentRow = ponteiro_mainwindow->ui->dados_tableWidget->rowCount();
            ponteiro_mainwindow->ui->dados_tableWidget->insertRow(currentRow);

            // Preenche a coluna "Horário" com o horário atual
            auto horario_item = new QTableWidgetItem(horarioAtual + 'h');
            ponteiro_mainwindow->ui->dados_tableWidget->setItem(currentRow, 0, horario_item);
            horario_item->setTextAlignment(Qt::AlignHCenter| Qt::AlignVCenter);

            // Preenche a coluna "Temperatura" com o valor formatado
            auto temperatura_item = new QTableWidgetItem(QString("%1 °C").arg(temperatura));
            ponteiro_mainwindow->ui->dados_tableWidget->setItem(currentRow, 1, temperatura_item);
            temperatura_item->setTextAlignment(Qt::AlignHCenter| Qt::AlignVCenter);
            ponteiro_mainwindow->ui->dados_tableWidget->scrollToBottom();
        }
        else {
            qDebug() << "Erro ao converter os dados da temperatura!";
        }
    }
}


void grafico_getter::salvar_dados()
{
    // Abre o diálogo para o usuário escolher onde salvar o arquivo
    auto fileName = QFileDialog::getSaveFileName(
        ponteiro_mainwindow, "Salvar Dados", "Temperaturas.csv",
        "Texto CSV (*.csv);;Todos os arquivos (*.*)");

    if (!fileName.isEmpty()) { // Verifica se o nome do arquivo não está vazio
        if (!fileName.endsWith(".csv")) {
            fileName += ".csv"; // Garante que a extensão seja ".csv"
        }

        QFile csv(fileName);
        if (!csv.open(QIODevice::WriteOnly)) { // Verifica se o arquivo foi aberto
            QMessageBox::warning(ponteiro_mainwindow, "Erro", "Não foi possível salvar o arquivo.");
            return;
        }

        QTextStream csvText(&csv);

        // Adiciona o cabeçalho ao arquivo CSV
        csvText << "Horário;Temperatura\n";

        // Itera sobre as linhas da tabela para salvar os dados
        auto rowCount = ponteiro_mainwindow->ui->dados_tableWidget->rowCount();
        auto columnCount = ponteiro_mainwindow->ui->dados_tableWidget->columnCount();

        for (int i = 0; i < rowCount; ++i) {
            for (int j = 0; j < columnCount; ++j) {
                auto item = ponteiro_mainwindow->ui->dados_tableWidget->item(i, j);
                if (item) {
                    csvText << item->text(); // Adiciona o texto do item
                }
                if (j < columnCount - 1) {
                    csvText << ";"; // Adiciona o delimitador
                }
            }
            csvText << "\n"; // Nova linha no CSV
        }

        csv.close();
        QMessageBox::information(ponteiro_mainwindow, "Sucesso", "Arquivo salvo com sucesso!");
    }
}







// usar barmodel grafico

//https://code.qt.io/cgit/qt/qtcharts.git/tree/examples/charts/chartsgallery?h=6.8

//https://doc.qt.io/qt-5/qtcharts-barchart-example.html

//https://doc.qt.io/qt-5/qtcharts-temperaturerecords-example.html ]

//https://doc.qt.io/qt-5/qtcharts-percentbarchart-example.html

//https://doc.qt.io/qt-5/qtcharts-piechartcustomization-example.html

//https://doc.qt.io/qt-5/qtcharts-piechart-example.html
