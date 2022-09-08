#include "mainwindow.h"
#include "ui_mainwindow.h"

/*
 * AUTHOR:      Marcin "Yurij" Biczyski
 * CO-AUTHOR:   Kamil Jagodziński
 * AT:          Sad Dinosaur Studios
 * VERSION:     v1.0 Release
 * DATE:        10.12.2015
 *
 * INTERNET TECHNOLOGIES PROJECT
 * POLITECHNIKA ŚLĄSKA 2015
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    port = 8080;
    ip = "197.0.0.1";
    ui->sPortEdit->setText(QString::number(port));
    ui->sIPEdit->setText("IP could not be found");

    transfer.setupIPFinder();

    connect(&transfer, &Transfers::sendClosed, this, &MainWindow::transfersReload);
    connect(&transfer, &Transfers::receiveClosed, this, &MainWindow::receiveReload);
    connect(&transfer, &Transfers::finderClosed, this, &MainWindow::setExternalIP);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSend_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_actionReceive_triggered()
{
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_actionTransfers_triggered()
{
    ui->tabWidget->setCurrentIndex(2);
}

void MainWindow::on_actionAbout_us_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("EightyEighty Team");
    msgBox.setText("Author: Marcin \"Yurij\" Biczyski");
    msgBox.setInformativeText("Co-author: Kamil Jagodziński\nInspiration: woof by Simon Budig\n@Sad Dinosaur Studios 2015");
    msgBox.setIconPixmap(QPixmap("dinosaur100.png"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_actionProject_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("EightyEighty Project");
    msgBox.setText("EightyEighty - Fast File Transfer Project");
    msgBox.setInformativeText("Politechnika Śląska 2015\nInternet Technologies");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_sSendButton_clicked()
{
    QDir tempPath;
    tempPath.setPath(ui->sFileEdit->text());
    if(!QFile::exists(ui->sFileEdit->text()))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("File does not exist");
        msgBox.setText("File does not exist!");
        msgBox.setInformativeText("Please try again");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        return;
    }

    transfer.setupSend(ui->sFileEdit->text(), ui->sPortEdit->text().toInt());

    ui->tTable->setRowCount(ui->tTable->rowCount()+1);
    ui->tTable->setItem(ui->tTable->rowCount()-1, 0, new QTableWidgetItem(tempPath.dirName()));
    ui->tTable->setItem(ui->tTable->rowCount()-1, 1, new QTableWidgetItem(QString::number(port)));

    int i, j;
    for(i = 8080;; i++) {
        for(j = 0; j < ui->tTable->rowCount(); j++) if(ui->tTable->item(j, 1)->text().toInt() == i) break;
        if(j == ui->tTable->rowCount()) break;
    }

    port = i;
    ui->sPortEdit->setText(QString::number(port));
}

void MainWindow::transfersReload(int port)
{
    int i;
    for(i = 0;; i++) {
        if(ui->tTable->item(i, 1)->text().toInt() == port) break;
    }
    ui->tTable->removeRow(i);
}

void MainWindow::receiveReload(int port)
{
    ui->rFileButton->setEnabled(true);
    ui->rFileEdit->setEnabled(true);
    ui->rIPEdit->setEnabled(true);
    ui->rPortEdit->setEnabled(true);
    ui->rReceiveButton->setEnabled(true);

    QMessageBox msgBox;
    msgBox.setWindowTitle("Transfer completed");
    msgBox.setText("File on port " + QString::number(port) + " has been downloaded");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void MainWindow::on_sFileButton_clicked()
{
    QString fileDir = QFileDialog::getOpenFileName(this, "Open File");
    ui->sFileEdit->setText(fileDir);
}

void MainWindow::on_rFileButton_clicked()
{
    QString fileDir = QFileDialog::getExistingDirectory(this, "Open File");
    ui->rFileEdit->setText(fileDir);
}

void MainWindow::setExternalIP(QString ipAddress)
{
    ui->sIPEdit->setText(ipAddress);
}

void MainWindow::on_rReceiveButton_clicked()
{
    if(QFile::exists(ui->rFileEdit->text())) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Bad file path");
        msgBox.setText("Bad file path!");
        msgBox.setInformativeText("Please try again");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        return;
    }

    ui->rFileButton->setEnabled(false);
    ui->rFileEdit->setEnabled(false);
    ui->rIPEdit->setEnabled(false);
    ui->rPortEdit->setEnabled(false);
    ui->rReceiveButton->setEnabled(false);

    transfer.setupReceive(ui->rFileEdit->text(), ui->rIPEdit->text(), ui->rPortEdit->text().toInt());
}
