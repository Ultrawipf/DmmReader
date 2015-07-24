#include "dmmreader.h"
#include <QDebug>
#include <QObject>


DmmReader::DmmReader(QSerialPort* serial):serial(serial)
{
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

}

DmmReader::~DmmReader()
{

}

void DmmReader::readData()
{
    currentPacket.append(serial->readAll());
    if(currentPacket.size()>=14){
        if(currentPacket.size()>14)
            currentPacket=currentPacket.right(15);

        if((currentPacket[0] & 0xF0) != 0x10){
            //qDebug()<<"Packet error. "<<currentPacket.toHex();
            //Realign packet
            QByteArray::iterator firstByte = std::find_if(currentPacket.begin(),currentPacket.end(),[](char c){return (c & 0xF0)==0x10;});
            if(firstByte==currentPacket.end())
                return;
            std::rotate(currentPacket.begin(),firstByte,currentPacket.end());
        }
        double curValue = decodeData(currentPacket);
        if(!(curValue!=curValue)){ //check if nan
            value=curValue;
            emit valueChanged();
        }
        currentPacket.clear();
    }

}

double DmmReader::decodeData(QByteArray& d){

    this->dispAuto = (d[0]&0x02);
    if(d[0]&0x04)
        this->mode=Mode::DC;
    else if(d[13]&0x02){ //user bit 1
        this->mode=Mode::Temp;
        this->unit=Unit::Celsius;
    }else if(d[0]&0x0F)
        this->mode=Mode::AC;
    else if(d[10]&0x1){
        this->mode = Mode::Beep;
    }

    if(d[9]&0x8){
        this->prefix = Prefix::u;
    }else if(d[9]&0x4){
        this->prefix = Prefix::n;
    }else if(d[9]&0x2){
        this->prefix = Prefix::k;
    }else if(d[10]&0x8){
        this->prefix = Prefix::m;
    }else if(d[10]&0x2){
        this->prefix = Prefix::M;
    }


    if(d[9]&0x1){
        this->unit = Unit::Diode;
    }else if(d[10]&0x4){
        this->unit = Unit::percent;
    }else if(d[11]&0x8){
        this->unit = Unit::Farad;
    }else if(d[11]&0x4){
        this->unit = Unit::Ohm;
    }else if(d[12]&0x8){
        this->unit = Unit::A;
    }else if(d[12]&0x4){
        this->unit = Unit::V;
    }else if(d[12]&0x2){
        this->unit = Unit::Hz;
    }


    /*Decode 7-Segment
    0:C 1:B 2:A 8:G 9:F 10:E 11:D 12:comma
    */

    int divider = 0;
    double curValue=0;
    for(int seg=0;seg<4;seg++){
        int s=seg*2+1;
        int val = 0; //current Value
        bool A=d[s]&0x04,B=d[s]&0x02,C=d[s]&0x01,D=d[s+1]&0x08,E=d[s+1]&0x04,F=d[s+1]&0x02,G=d[s+1]&0x01,comma=(d[s]&0x08);
        //qDebug()<<"A "<<A<<"B "<<B<<"C "<<C<<"D "<<D<<"E "<<E<<"F "<<F<<"G "<<G<<"Komma "<<comma;

        if(comma&&seg!=0)
            divider=4-seg;

        if(G&&E&&!(A||B||C||D||F)) //1
            val=1;
        else if(A&&B&&C&&D&&E&&G&&!F)
            val=0;
        else if(C&&G&&F&&A&&D&&!(B||E)) //2
            val=2;
        else if(C&&G&&F&&E&&D&&!(B||A)) //3
            val=3;
        else if(B&&G&&F&&E&&!(C||D||A)) //4
            val=4;
        else if(C&&B&&F&&E&&D&&!(A||G)) //5
            val=5;
        else if(C&&B&&F&&E&&D&&A&&!G) //6
            val=6;
        else if(C&&G&&E&&!((A||B||D||F)))//7
            val=7;
        else if(A&&B&&C&&D&&E&&F&&G)//8
            val=8;
        else if(!A&&B&&C&&D&&E&&F&&G)//9
            val=9;
        else
            return std::numeric_limits<double>::quiet_NaN();
        curValue+=pow(10,3-seg)*val;
    }

    if(divider!=0)
        curValue/=pow(10,divider);
    if((d[1]&0x08))
        curValue=-curValue;
    //qDebug() << curValue;
    return curValue;

}

void DmmReader::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {

        serial->close();
        qDebug()<<"Serial Error. Closing";
    }
}
