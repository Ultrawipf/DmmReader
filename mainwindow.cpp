#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "settingsdialog.h"
#include <QMessageBox>
#include <QDebug>
#include <memory>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    this->settings = new SettingsDialog;
    ui->setupUi(this);
    this->reader=std::make_shared<DmmReader>(new QSerialPort);
    connect(reader.get(),SIGNAL(valueChanged()),this,SLOT(valueChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::valueChanged(){
    this->ui->lcdNumber->display(this->reader->value);
    QString unitText="";
    DmmReader::Unit unit=this->reader->unit;
    switch(unit){
        case DmmReader::Unit::A:
            unitText="A";
        break;
        case DmmReader::Unit::Celsius:
            unitText="°C";
        break;
        case DmmReader::Unit::V:
            unitText="V";
        break;
        default:
            unitText="";
        break;
    }
    this->ui->unitLabel->setText(unitText);
}


void MainWindow::on_settingsButton_clicked()
{
    settings->show();
}

void MainWindow::on_connectButton_clicked()
{
    SettingsDialog::Settings p = settings->settings();
    this->reader->serial->setPortName(p.name);
    this->reader->serial->setBaudRate(p.baudRate);
    this->reader->serial->setDataBits(p.dataBits);
    this->reader->serial->setParity(p.parity);
    this->reader->serial->setStopBits(p.stopBits);
    this->reader->serial->setFlowControl(p.flowControl);
    ui->debugText->setText("Connected");
    this->connected=this->reader->serial->open(QIODevice::ReadWrite);
}
