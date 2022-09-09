#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "transfers.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionSend_triggered();
    void on_actionReceive_triggered();
    void on_actionTransfers_triggered();
    void on_actionAbout_us_triggered();
    void on_actionProject_triggered();
    void on_sSendButton_clicked();
    void on_sFileButton_clicked();
    void on_rFileButton_clicked();
    void on_rReceiveButton_clicked();

    void transfersReload(int);
    void receiveReload(int);
    void setExternalIP(QString);

private:
    Ui::MainWindow *ui;
    Transfers transfer;
    QString ip;
    int port;
};

#endif // MAINWINDOW_H
