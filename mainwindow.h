#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pixels.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setd(double value, double pos);

    void on_AValue_valueChanged(double arg1);
//    void on_BValue_valueChanged(double arg1);
    void on_CValue_valueChanged(double arg1);
//    void on_DValue_valueChanged(double arg1);

//    void on_NValue_valueChanged(int arg1);

    void on_FVision_stateChanged(int arg1);
    void on_dFVision_stateChanged(int arg1);
    void on_PVision_stateChanged(int arg1);
    void on_dPVision_stateChanged(int arg1);
    void on_RVision_stateChanged(int arg1);

    void on_Calculate_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
