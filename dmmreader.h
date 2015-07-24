#ifndef DMMREADER_H
#define DMMREADER_H
#include <QtSerialPort/QSerialPort>
#include <QByteArray>
#include <memory>
#include <QObject>

class DmmReader: public QObject
{
    Q_OBJECT
private:

    QByteArray currentPacket; //Currently reading

public:
    DmmReader(QSerialPort* serial);
    ~DmmReader();
    QSerialPort* serial;
    double value; //Current Value
    enum Mode{AC,DC,Temp,Beep} mode;
    bool dispAuto;
    bool dispHold;
    enum Prefix {u,n,m,k,M};
    Prefix prefix;
    enum Unit {percent,Diode,Farad,Ohm,A,V,Hz,Celsius};
    Unit unit;
signals:
    void valueChanged();

private slots:
    void readData();
    double decodeData(QByteArray &d);
    void handleError(QSerialPort::SerialPortError error);
};

#endif // DMMREADER_H
