#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QtSerialPort>

// Declaração antecipada de RFID
class RFID;
class grafico_getter;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow *ui;
    QMediaPlayer* player;
    QSerialPort *serial;    // Objeto para manipular a porta serial
    RFID *rfid;            // Objeto RFID
    grafico_getter *grafico;  // Objeto grafico_getter


    // Método para obter acesso ao QSerialPort
    QSerialPort* getSerialPort();

private slots:

    void paintEvent(QPaintEvent *event);

    void on_visualizar_dados_pushButton_clicked();

    void on_controlar_eletronicos_pushButton_clicked();

    void on_pag_anterior4_pushButton_clicked();

    void on_pag_anterior3_pushButton_clicked();

    void on_action_Informacoes_triggered();

    void on_action_Reposit_rio_Git_do_Projeto_triggered();

    void atualizarMenu(int paginaAtual);

    void on_parar_musica_pushButton_clicked();

    void on_ligarLED_pushButton_clicked();

    void on_ligarFAN_pushButton_clicked();

    void on_pag_anterior2_pushButton_clicked();

    void on_actionSalvar_dados_triggered();

    void readSerialData();

    void on_actionFechar_triggered();
};
#endif // MAINWINDOW_H
