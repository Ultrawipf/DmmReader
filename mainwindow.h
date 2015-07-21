#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include "settingsdialog.h"
#include <QBitArray>

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
    void readData();
    void decodeData(QBitArray &data);
    void handleError(QSerialPort::SerialPortError error);
    void on_settingsButton_clicked();
    void on_connectButton_clicked();

private:
    Ui::MainWindow *ui;
    bool connected = false;
    QSerialPort* serial;
    SettingsDialog* settings;
    QByteArray current;
    QBitArray packetData;
    float value;
};

#endif // MAINWINDOW_H
