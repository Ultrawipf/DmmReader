#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "settingsdialog.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{

    this->settings = new SettingsDialog;
    ui->setupUi(this);
    this->serial=new QSerialPort;
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readData()
{
    current.append(serial->readAll());
    if(current.size()>=14){
        packetData.clear();
        packetData.fill(false,14*8);
        QString text="";
        if(!current[0] & 0x10){
            current.clear();
            this->ui->debugText->setText("Packet Error");
            return;
        }
        for(size_t b=0;b<current.size();b++,text+="\n"){
            for(size_t i = 0; i < 8; i++){
                bool bit = current[b] & (1 << i);
                text+=bit?"1":"0";
                packetData[b+i]=bit;
            }
        }
        this->ui->debugText->setText(text);
        current.clear();
    }
}

void MainWindow::decodeData(QBitArray& data){
    QString text="";
    for(size_t i=0;i<data.size();i++)
        text+=data.testBit(i)?"1":"0";
    this->ui->debugText->setText(text);
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        serial->close();
        ui->debugText->setText("Error");
    }
}

void MainWindow::on_settingsButton_clicked()
{
    settings->show();
}

void MainWindow::on_connectButton_clicked()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    ui->debugText->setText("Connected");
    this->connected=serial->open(QIODevice::ReadWrite);
}
