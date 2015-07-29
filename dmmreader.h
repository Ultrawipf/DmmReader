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
    double value; //Current Value
    QByteArray currentPacket; //Currently reading

public:
    DmmReader(QSerialPort* serial);
    ~DmmReader();
    QSerialPort* serial;

    enum Mode{AC,DC,Temp,Beep,Diode} mode;
    bool dispAuto;
    bool dispHold;
    enum Prefix {u=-6,n=-9,m=-3,k=3,M=6,none=0};
    Prefix prefix;
    enum Unit {percent,Farad,Ohm,A,V,Hz,Celsius};
    Unit unit;
    double getValue(){
        return value*std::pow(10,int(prefix));
    }
    double getRawValue(){
        return value;
    }

signals:
    void valueChanged();

private slots:
    void readData();
    double decodeData(QByteArray &d);
    void handleError(QSerialPort::SerialPortError error);
};

#endif // DMMREADER_H
