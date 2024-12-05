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

// Construtor da classe grafico_getter, recebe um ponteiro para a classe MainWindow (chamado "ponteiro_mainwindow"), para modificar os elementos da UI
grafico_getter::grafico_getter(MainWindow *mainWindow) : ponteiro_mainwindow(mainWindow){

    //PARA AS COLUNAS DA TABELA:

    // Configura as colunas da tabela para se que se ajustem automaticamente ao tamanho dela
    QHeaderView* header = ponteiro_mainwindow->ui->dados_tableWidget->horizontalHeader();

    // Faz com que as colunas preencham o espaço disponível
    header->setSectionResizeMode(QHeaderView::Stretch);

}




void grafico_getter::processarDadosSensores(const QString& mensagem)
{
    // Variáveis para armazenar os dados
    static QString horarioAtual;
    static double temperatura = 0;
    static double CO2 = 0;
    static double luminosidade = 0;

    // Verifica se a mensagem contém "T:" e processa a temperatura
    if (mensagem.contains("T:")) {
        bool ok;
        QString tempStr = mensagem.split("T:")[1];
        temperatura = tempStr.toDouble(&ok);
        if (!ok) {
            qDebug() << "Erro ao converter os dados da temperatura!";
            return;
        }
    }

    // Verifica se a mensagem contém "CO2:" e processa o CO2
    if (mensagem.contains("CO2:")) {
        bool ok;
        QString SensCO2 = mensagem.split("CO2:")[1];
        CO2 = SensCO2.toDouble(&ok);
        if (!ok) {
            qDebug() << "Erro ao converter os dados da Qualidade do Ar!";
            return;
        }
    }

    // Verifica se a mensagem contém "L:" e processa a luminosidade
    if (mensagem.contains("L:")) {
        bool ok;
        QString Lumin = mensagem.split("L:")[1];
        luminosidade = Lumin.toDouble(&ok);
        if (!ok) {
            qDebug() << "Erro ao converter os dados da Luminosidade!";
            return;
        }
    }

    // Verifica se todos os dados estão completos (Temperatura, CO2, Luminosidade e Horário)
    if (mensagem.contains("T:") || mensagem.contains("CO2:") || mensagem.contains("L:")) {
        if (horarioAtual.isEmpty()) {
            horarioAtual = QTime::currentTime().toString("HH:mm");
        }

        // Verifica se todos os dados estão prontos
        if (!horarioAtual.isEmpty() && temperatura != 0 && CO2 != 0 && luminosidade != 0) {
            // Se todos os dados estão completos, insere uma nova linha na tabela
            int currentRow = ponteiro_mainwindow->ui->dados_tableWidget->rowCount();
            ponteiro_mainwindow->ui->dados_tableWidget->insertRow(currentRow);

            // Preenche as células com os dados e usa um ponteiro para alinhar
            auto horario_item = new QTableWidgetItem(horarioAtual + 'h');
            ponteiro_mainwindow->ui->dados_tableWidget->setItem(currentRow, 0, horario_item);
            horario_item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            auto temperatura_item = new QTableWidgetItem(QString("%1 °C").arg(temperatura));
            ponteiro_mainwindow->ui->dados_tableWidget->setItem(currentRow, 1, temperatura_item);
            temperatura_item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            auto CO2_item = new QTableWidgetItem(QString("%1 ppm").arg(CO2));
            ponteiro_mainwindow->ui->dados_tableWidget->setItem(currentRow, 2, CO2_item);
            CO2_item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            auto luminosidade_item = new QTableWidgetItem(QString("%1 Lux").arg(luminosidade));
            ponteiro_mainwindow->ui->dados_tableWidget->setItem(currentRow, 3, luminosidade_item);
            luminosidade_item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

            // Limpa as variáveis para os próximos dados
            horarioAtual.clear();
            temperatura = 0;
            CO2 = 0;
            luminosidade = 0;
        }
    }

    // Faz a rolagem para a última linha que foi gerada na tabela
    ponteiro_mainwindow->ui->dados_tableWidget->scrollToBottom();
}







void grafico_getter::salvar_dados()
{
    // Abre o diálogo para o usuário escolher onde salvar o arquivo
    auto fileName = QFileDialog::getSaveFileName(
        ponteiro_mainwindow, "Salvar Dados", "Dados_Coletados.csv",
        "Texto CSV (*.csv);;Todos os arquivos (*.*)");

    if (!fileName.isEmpty()) { // Verifica se o nome do arquivo não está vazio
        if (!fileName.endsWith(".csv")) {   // Se o nome do arquivo não termina com .csv, adiciona
            fileName += ".csv"; // Garante que a extensão seja ".csv"
        }

        QFile csv(fileName);
        if (!csv.open(QIODevice::WriteOnly | QIODevice::Text)) { // Abre o arquivo para escrita
            QMessageBox::warning(ponteiro_mainwindow, "Erro", "Não foi possível salvar o arquivo.");
            return;
        }

        QTextStream csvText(&csv);

        // Adiciona um cabeçalho ao arquivo CSV
        csvText << "Hora;Temperatura;CO2 no ar;Luminosidade\n";

        // Itera sobre as linhas da tabela para salvar os dados
        auto rowCount = ponteiro_mainwindow->ui->dados_tableWidget->rowCount();
        auto columnCount = ponteiro_mainwindow->ui->dados_tableWidget->columnCount();

            for (int i = 0; i < rowCount; ++i) {
                bool primeiraColuna = true;
                for (int j = 0; j < columnCount; ++j) {
                    auto item = ponteiro_mainwindow->ui->dados_tableWidget->item(i, j);
                    if (item) {
                        if (!primeiraColuna) {
                            csvText << ";"; // Adiciona delimitador entre os dados
                        }
                        csvText << item->text(); // Adiciona o texto do item na linha
                        primeiraColuna = false; // A partir da segunda coluna, adiciona o delimitador
                    }
                }
                csvText << "\n"; // Nova linha no CSV após cada linha da tabela
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
