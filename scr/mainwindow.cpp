#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sobredialog.h"
#include "rfid.h"
#include "grafico_getter.h"


#include <QPainter>
#include <QPixmap>
#include <QDesktopServices>
#include <QMediaPlayer>
#include <QUrl>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , player(new QMediaPlayer(this))  // Inicializa player como membro da classe
    , serial(new QSerialPort(this))  // Inicializa o QSerialPort
{
    ui->setupUi(this);

    // Inicializa o objeto RFID, passando o ponteiro da MainWindow
    rfid = new RFID(this);

    // Instanciando o objeto grafico_getter
    grafico = new grafico_getter(this);



    // Configuração dos parâmetros da porta serial
    serial->setPortName("/dev/ttyACM0");
    serial->setBaudRate(QSerialPort::Baud9600);  // Taxa de transmissão
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // Tentando abrir a porta:
    if (!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Erro ao abrir a porta serial:" << serial->errorString();
    } else {
        qDebug() << "Conexão serial estabelecida!";
    }

//    // Conecta o sinal de leitura de dados:
//    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);





    //PARA ALGUMAS PERSONALIZAÇÕES:
    // Personalizar a menu bar com setStyleSheet usando RGB
    ui->menubar->setStyleSheet("QMenuBar { background-color: rgb(165, 160, 166); color: red; }"
                               "QMenuBar::item { background-color: rgb(165, 160, 166); color: black; }"
                               "QMenuBar::item:selected { background-color: rgb(135, 130, 136); }"      //Escurece a cor do item
                               "QMenuBar::item:pressed { background-color: rgb(105, 100, 106); }");    //Escurece mais ainda, quando o item é pressionado

    // Personalizar a ToolBar com setStyleSheet usando RGB
    ui->toolBar->setStyleSheet("QToolBar {"
                               "background-color: rgb(185, 180, 186);"
                               "border-top: 0.5px solid rgb(0, 0, 0);"
                               "border-bottom: 0.5px solid rgb(0, 0, 0);"
                               "}");




    //PARA ESCONDER OPÇÕES A MENU BAR
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &MainWindow::atualizarMenu);

    // Chamada inicial para configurar a visibilidade dos elementos do menu ao iniciar a aplicação
    atualizarMenu(ui->stackedWidget->currentIndex());





    // PARA TOCAR MÚSICA QUANDO ABRIR O APP:
    player->setMedia(QUrl("qrc:/audi/dust_in_the_wind.mp3"));
    player->setVolume(50);  // Define o volume para 50%
    player->play();  // Inicia a reprodução da música



}


//DESTRUTOR
MainWindow::~MainWindow()
{
    delete ui;
    delete player;

    //Para fechar a conecção serial:
    if (serial->isOpen()) {
        serial->close();
    }

    delete serial;
}



// Sobrescrevendo o paintEvent para desenhar a imagem de fundo com QPixmap
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Carrega a imagem que será usada como fundo
    QPixmap background(":/img/2.jpeg");

    // Ajusta a imagem ao tamanho do widget, centralizando e evitando repetição
    painter.drawPixmap(0, 0, width(), height(), background);

    // Evento de pintura padrão pros componentes serem desenhados corretamente
    QMainWindow::paintEvent(event);
}




void MainWindow::on_visualizar_dados_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);  // Troca para a página 2, que irá verificar com o RFID

    // Conectando o sinal aqui, para inicializar somente quando o botão é criado
    connect(serial, &QSerialPort::readyRead, rfid, &RFID::readSerialData);

    // Chama a função readSerialData() do objeto RFID
    rfid->readSerialData();
}

void MainWindow::on_pag_anterior2_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);  // Volta para a página 1
}



void MainWindow::on_controlar_eletronicos_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);  // Troca para a página 4, que irá permitir controlar alguns equipamentos
}

void MainWindow::on_pag_anterior3_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);  // Volta para a página 1
}




void MainWindow::on_action_Sobre_triggered()
{
    sobreDialog *dlg = new sobreDialog(this);  // 'this' é o pai da janela
    dlg->exec();  // Exibe a caixa de diálogo como modal
}

void MainWindow::on_action_Reposit_rio_Git_do_Projeto_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/ST-Brenda/Projeto-Final-LabProg-III"));

}





//ESCONDE ALGUMAS OPÇÕES DA MENU BAR EM PÁGINAS ESPECÍFICAS
void MainWindow::atualizarMenu(int paginaAtual)
{
    if (paginaAtual != ui->stackedWidget->indexOf(ui->page_4) ) {
        ui->actionSalvar_gr_ficos->setVisible(false);
    } else {
        ui->actionSalvar_gr_ficos->setVisible(true);

    }
}



//INSTALAR:
//sudo apt-get install qt5-qtmultimedia
//sudo apt-get install qtmultimedia5-dev
//sudo apt-get install libqt5multimedia5 libqt5multimedia5-plugins libqt5multimediaquick5
//sudo apt-get install gstreamer1.0-libav gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly



// PARA PAUSAR A MÚSICA:
void MainWindow::on_parar_musica_pushButton_clicked()
{
    QString playIcon = "▶️";
    QString pauseIcon = "⏸️";
    if (player->state() == QMediaPlayer::PlayingState) {
        player->pause();
        ui->parar_musica_pushButton->setText(playIcon + " Música");
    } else {
        player->play();
        ui->parar_musica_pushButton->setText(pauseIcon + " Música");
    }
}













//PARTE PARA CONTROLAR OS EQUIPAMENTOS:
// Variáveis para armazenar os estados
bool ledEstado = false;
bool fanEstado = false;

void MainWindow::on_ligarLED_pushButton_clicked()
{
    // Muda  o estado do LED
    ledEstado = !ledEstado;

    // Muda o texto do botão com base no estado
    if (ledEstado) {
        ui->ligarLED_pushButton->setText("Desligar");
        serial->write("1");  // Envia comando '1' para mudar o estado do LED
    } else {
        ui->ligarLED_pushButton->setText("Ligar");
        serial->write("1");  // Envia comando '1' para mudar o estado do LED novamente
    }
}

void MainWindow::on_ligarFAN_pushButton_clicked()
{
    // Muda o estado da FAN
    fanEstado = !fanEstado;

    // Muda o texto do botão com base no estado
    if (fanEstado) {
        ui->ligarFAN_pushButton->setText("Desligar");
        serial->write("2");  // Envia comando '2' para mudar o estado da fan
    } else {
        ui->ligarFAN_pushButton->setText("Ligar");
        serial->write("2");  // Envia comando '2' para mudar o estado da ventilador novamente
    }
}








void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);    // Muda para a página 1
}


void MainWindow::on_TESTE_clicked()
{
    // Conectando o sinal aqui, para inicializar somente quando o botão é criado
    // Conecta o sinal de leitura de dados da porta serial com o slot
    connect(serial, &QSerialPort::readyRead, grafico, &grafico_getter::le_dados_sensores);

    grafico->le_dados_sensores();
}

