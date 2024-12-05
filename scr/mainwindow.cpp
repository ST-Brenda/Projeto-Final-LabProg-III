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
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    // Inicialização aqui é obrigatória para membros const, referências e classes sem construtor padrão:
    , player(new QMediaPlayer(this))  // Inicializa player como membro da classe
    , serial(new QSerialPort(this))  // Inicializa o QSerialPort
{
    ui->setupUi(this);



    // MainWindow é a classe pai:

    // Instância da classe RFID passando o ponteiro pra MainWindow
    rfid = new RFID(this);

    // Instância da classe grafico_getter passando o ponteiro pra MainWindow
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

    // Conecta o sinal de leitura de dados:
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);





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
    //DESCOMENTAR
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

    // O método é chamado automaticamente quando há dados paara serem lidos via serial, então não adianta chamar
    //readSerialData();
}



void MainWindow::on_pag_anterior4_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);  // Volta para a página 1
}



void MainWindow::on_controlar_eletronicos_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);  // Troca para a página 3, que irá permitir controlar alguns equipamentos
}

void MainWindow::on_pag_anterior3_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);  // Volta para a página 1
}




void MainWindow::on_action_Informacoes_triggered()
{
    sobreDialog *dlg = new sobreDialog(this);  // 'this' é o pai da janela
    dlg->exec();  // Exibe a caixa de diálogo como modal
}

void MainWindow::on_action_Reposit_rio_Git_do_Projeto_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/ST-Brenda/Projeto-Final-LabProg-III"));

}



//ESCONDE A OPÇÃO DE SALVAR DADOS, DA MENU BAR, ENQUANTO O USUÁRIO NÃO ESTIVER NA PÁGINA 4
void MainWindow::atualizarMenu(int paginaAtual)
{
    if (paginaAtual != ui->stackedWidget->indexOf(ui->page_4) ) {
        ui->actionSalvar_dados->setVisible(false);
    } else {
        ui->actionSalvar_dados->setVisible(true);

    }
}


//INSTALAR PARA MÚSICA:
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
// Variável para armazenar o estado do LED
bool ledEstado = false;

void MainWindow::on_ligarLED_pushButton_clicked()
{
    // Muda  o estado do LED
    ledEstado = !ledEstado;

    // Muda o texto do botão com base no estado
    if (ledEstado) {
        ui->ligarLED_pushButton->setText("Desligar");
        qDebug() << "Ligando LED";
        serial->write("1");  // Envia comando '1' para o arduino mudar o estado do LED
    } else {
        ui->ligarLED_pushButton->setText("Ligar");
        qDebug() << "Desligando LED";
        serial->write("1");  // Envia comando '1' para o arduino mudar o estado do LED novamente
    }
}

// Variável para armazenar o estado da FAN
bool fanEstado = false;
void MainWindow::on_ligarFAN_pushButton_clicked()
{
    // Muda o estado da FAN
    fanEstado = !fanEstado;

    // Muda o texto do botão com base no estado
    if (fanEstado) {
        ui->ligarFAN_pushButton->setText("Desligar");
        qDebug() << "Ligando FAN";
        serial->write("2");  // Envia comando '2' para mudar o estado da FAN
    } else {
        ui->ligarFAN_pushButton->setText("Ligar");
        qDebug() << "Desligando FAN";
        serial->write("2");  // Envia comando '2' para mudar o estado da FAN novamente
    }
}





// Botão da página que aguarda a leitura do RFID
void MainWindow::on_pag_anterior2_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_1);    // Muda para a página 1
}



// A FUNÇÃO ABAIXO MANDA OS DADOS RECEBIDOS POR SERIAL PARA AS RESPECTIVAS CLASSES:
// Verifica se o cartão do RFID permite o acesso
// E coleta e processa os dados pegos pelos sensores

//O método "readSerialData()" é chamado automaticamente sempre que o sinal readyRead() do QSerialPort é emitido,
//isso ocorre quando há dados disponíveis para leitura na porta serial.

void MainWindow::readSerialData()
{
    static QString buffer; // Buffer para acumular os dados
    // Objeto que é usado para manipulação de bytes:
    QByteArray data = serial->readAll(); // Lê todos os dados da porta serial
    // Converte os dados recebidos que estão em formato de bytes para uma string no formato UTF-8
    buffer += QString::fromUtf8(data);


    int index;
    // Método que procura pelo delimitador de mensagem (\r\n) no buffer
    // Se index != -1, há uma mensagem completa no buffer e o loop é executado
    while ((index = buffer.indexOf("\r\n")) != -1) {
        QString mensagem = buffer.left(index); // Extrai uma mensagem completa pegando os caracteres à esquerda de "index"
        buffer.remove(0, index + 2);           // Remove a mensagem do buffer excluindo os caracteres do início do buffer até (index + 2, pois "\r\n" tem 2 caracteres)

        qDebug() << mensagem;

        // Encaminhe as mensagens para as funções específicas
        if (mensagem.startsWith("RFID:")) {
            qDebug() << "Passando para RFID";
            rfid->processarMensagemRFID(mensagem);
        } else if (mensagem.startsWith("T:") || mensagem.startsWith("L:") || mensagem.startsWith("CO2:")) {
            //qDebug() << "Passando para LEITURA DE SENSORES";
            grafico->processarDadosSensores(mensagem);
        } else {
            qDebug() << "Mensagem não reconhecida:" << mensagem;
        }
    }
}


//PARA SALVAR OS DADOS:
void MainWindow::on_actionSalvar_dados_triggered()
{
    grafico->salvar_dados();
}



void MainWindow::on_actionFechar_triggered()
{
    QApplication::quit(); // Finaliza a aplicação
}


