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
    void lock();
    void unlock();

    void on_XPPU_valueChanged(int arg1);
    void on_YPPU_valueChanged(int arg1);

    void on_XOffset_valueChanged(int arg1);
    void on_YOffset_valueChanged(int arg1);

    void on_Alpha_valueChanged(double arg1);
    void on_Delta_valueChanged(double arg1);
    void on_Beta_valueChanged(double arg1);
    void on_Gamma_valueChanged(double arg1);
    void on_Epsilon_valueChanged(double arg1);


    void setd(double value, double pos);
    void setOffsetX(double value);
    void setOffsetY(double value);

    void setXPPU(pixel value);
    void setYPPU(pixel value);

    void on_AValue_valueChanged(double arg1);
    void on_BValue_valueChanged(double arg1);
    void on_CValue_valueChanged(double arg1);
    void on_DValue_valueChanged(double arg1);
    void on_NValue_valueChanged(int arg1);
    void on_FVision_stateChanged(int arg1);
    void on_dFVision_stateChanged(int arg1);
    void on_PVision_stateChanged(int arg1);
    void on_dPVision_stateChanged(int arg1);
    void on_RVision_stateChanged(int arg1);

    void on_pushButton_clicked();

    void on_Calculate_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
