#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_AValue_valueChanged(double arg1);
//    void on_BValue_valueChanged(double arg1);
    void on_CValue_valueChanged(double arg1);
//    void on_DValue_valueChanged(double arg1);

//    void on_NValue_valueChanged(int arg1);

    void on_FVision_stateChanged(int arg1);
    void on_RVision_stateChanged(int arg1);

    void on_Calculate_clicked();
    void on_AlphaFix_clicked();

    void on_BetaFix_clicked();

    void on_GammaFix_clicked();

    void on_DeltaFix_clicked();

    void on_EpsilonFix_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
