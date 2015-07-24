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


};

#endif // MAINWINDOW_H
