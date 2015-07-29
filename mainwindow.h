#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include "settingsdialog.h"
#include "dmmreader.h"
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void valueChanged();

private slots:
    void on_settingsButton_clicked();
    void on_connectButton_clicked();

private:
    Ui::MainWindow *ui;
    bool connected = false;
    SettingsDialog* settings;
    std::shared_ptr<DmmReader> reader;

    typedef DmmReader::Unit uName;
    const std::map<uName,QString> units{{uName::A,"A"},{uName::Celsius,"°C"},{uName::V,"V"},{uName::Farad,"F"},{uName::Hz,"Hz"},{uName::percent,"%"},{uName::Ohm,"Ω"}};

    typedef DmmReader::Prefix pName;
    const std::map<pName,QString> prefixes{{pName::k,"k"},{pName::u,"µ"},{pName::n,"n"},{pName::m,"m"},{pName::M,"M"},{pName::none,""}};

    typedef DmmReader::Mode mName;
    const std::map<mName,QString> modes{{mName::AC,"AC"},{mName::DC,"DC"},{mName::Beep,"↔"},{mName::Temp,"Temp"},{mName::Diode,"►|"}};
};

#endif // MAINWINDOW_H
