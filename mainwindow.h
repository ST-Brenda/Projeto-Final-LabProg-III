#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QtSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void paintEvent(QPaintEvent *event);

    void on_visualizar_dados_pushButton_clicked();

    void on_controlar_eletronicos_pushButton_clicked();

    void on_pag_anterior2_pushButton_clicked();

    void on_pag_anterior3_pushButton_clicked();

    void on_action_Sobre_triggered();

    void on_action_Reposit_rio_Git_do_Projeto_triggered();

    void atualizarMenu(int paginaAtual);

    void on_parar_musica_pushButton_clicked();

    void on_ligarLED_pushButton_clicked();

    void on_ligarFAN_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* player;
};
#endif // MAINWINDOW_H
