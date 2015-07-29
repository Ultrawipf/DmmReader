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
    uName unit=this->reader->unit;
    pName prefix = this->reader->prefix;
    mName mode = this->reader->mode;
    QString prefixName=this->prefixes.at(prefix);
    QString unitName=this->units.at(unit);
    QString modeName=this->modes.at(mode);

    this->ui->debugText->setText("Value: "+QString::number(this->reader->getValue())+unitName);
    this->ui->unitLabel->setText(prefixName+unitName);
    this->ui->lcdNumber->display(this->reader->getRawValue());
    this->ui->modeLabel->setText("Mode: "+modeName);
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
