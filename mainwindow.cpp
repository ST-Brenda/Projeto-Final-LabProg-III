#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sobredialog.h"
#include <QPainter>
#include <QPixmap>
#include <QDesktopServices>
#include <QMediaPlayer>
#include <QUrl>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , player(new QMediaPlayer(this))  // Inicializa player como membro da classe
{
    ui->setupUi(this);

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


    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, &MainWindow::atualizarMenu);

    // Chamada inicial para configurar a visibilidade dos elementos do menu ao iniciar a aplicação
    atualizarMenu(ui->stackedWidget->currentIndex());


    // Pra tocar a música quando abrir o app:
    player = new QMediaPlayer(this);
    player->setMedia(QUrl("qrc:/audi/acoustic-folk-acoustic-guitar-138361.mp3"));
    player->setVolume(20);  // Define o volume para 50% (opcional)
    player->play();  // Inicia a reprodução da música

}

MainWindow::~MainWindow()
{
    delete ui;

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
    ui->stackedWidget->setCurrentWidget(ui->page_2);  // Troca para a página 2, que irá exibir os dados
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
    if (paginaAtual != ui->stackedWidget->indexOf(ui->page_2) ) {
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
