#include "serialcommunicator.h"
#include <QtSerialPort/QSerialPort>
#include "ui_mainwindow.h"

SerialCommunicator::SerialCommunicator(Ui::MainWindow *window)
{
    this->window=window;

}

void SerialCommunicator::setPort(QString name){
    port->setPortName(name);
    serial->open(QIODevice::ReadWrite);
}

SerialCommunicator::~SerialCommunicator()
{

}

