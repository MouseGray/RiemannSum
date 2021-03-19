#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Interpolation N1");

    connect(ui->ChartWidget, SIGNAL(offsetXChanged(double)), this, SLOT(setOffsetX(double)));
    connect(ui->ChartWidget, SIGNAL(offsetYChanged(double)), this, SLOT(setOffsetY(double)));
    connect(ui->ChartWidget, SIGNAL(XPPUChanged(pixel)), this, SLOT(setXPPU(pixel)));
    connect(ui->ChartWidget, SIGNAL(YPPUChanged(pixel)), this, SLOT(setYPPU(pixel)));
    connect(ui->ChartWidget, SIGNAL(dChanged(double, double)), this, SLOT(setd(double, double)));

    connect(ui->ChartWidget, SIGNAL(lock()), this, SLOT(lock()));
    connect(ui->ChartWidget, SIGNAL(unlock()), this, SLOT(unlock()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::lock()
{
    ui->Alpha->setDisabled(true);
    ui->Beta->setDisabled(true);
    ui->Gamma->setDisabled(true);
    ui->Delta->setDisabled(true);
    ui->Epsilon->setDisabled(true);

    ui->AValue->setDisabled(true);
    ui->BValue->setDisabled(true);
    ui->CValue->setDisabled(true);
    ui->DValue->setDisabled(true);

    ui->NValue->setDisabled(true);
}

void MainWindow::unlock()
{
    ui->Alpha->setDisabled(false);
    ui->Beta->setDisabled(false);
    ui->Gamma->setDisabled(false);
    ui->Delta->setDisabled(false);
    ui->Epsilon->setDisabled(false);

    ui->AValue->setDisabled(false);
    ui->BValue->setDisabled(false);
    ui->CValue->setDisabled(false);
    ui->DValue->setDisabled(false);

    ui->NValue->setDisabled(false);

    ui->Calculate->setDisabled(false);
}

void MainWindow::on_Alpha_valueChanged(double arg1)
{

}

void MainWindow::on_Beta_valueChanged(double arg1)
{

}


void MainWindow::on_Delta_valueChanged(double arg1)
{

}

void MainWindow::on_Gamma_valueChanged(double arg1)
{

}

void MainWindow::on_Epsilon_valueChanged(double arg1)
{

}

void MainWindow::setd(double value, double pos)
{
    ui->dValue->setText(QString::number(value) + "\n(x=" + QString::number(pos) + ")");
}

void MainWindow::on_AValue_valueChanged(double arg1)
{
    ui->BValue->setMinimum(arg1);
}

void MainWindow::on_BValue_valueChanged(double arg1)
{

}

void MainWindow::on_CValue_valueChanged(double arg1)
{

    ui->DValue->setMinimum(arg1);
}

void MainWindow::on_DValue_valueChanged(double arg1)
{

}

void MainWindow::on_NValue_valueChanged(int arg1)
{

}

void MainWindow::on_FVision_stateChanged(int arg1)
{
    ui->ChartWidget->setF_vision(arg1);
}

void MainWindow::on_dFVision_stateChanged(int arg1)
{
    ui->ChartWidget->setDF_vision(arg1);
}

void MainWindow::on_PVision_stateChanged(int arg1)
{
    ui->ChartWidget->setP_vision(arg1);
}

void MainWindow::on_dPVision_stateChanged(int arg1)
{
    ui->ChartWidget->setDP_vision(arg1);
}

void MainWindow::on_RVision_stateChanged(int arg1)
{
    ui->ChartWidget->setR_vision(arg1);
}

void MainWindow::on_pushButton_clicked()
{

}

void MainWindow::on_Calculate_clicked()
{
    ui->ChartWidget->setAlpha(ui->Alpha->value());
    ui->ChartWidget->setBeta(ui->Beta->value());
    ui->ChartWidget->setDelta(ui->Delta->value());
    ui->ChartWidget->setGamma(ui->Gamma->value());
    ui->ChartWidget->setEpsilon(ui->Epsilon->value());

    ui->ChartWidget->setA(ui->AValue->value());
    ui->ChartWidget->setB(ui->BValue->value());
    ui->ChartWidget->setC(ui->CValue->value());
    ui->ChartWidget->setD(ui->DValue->value());
    ui->ChartWidget->setN(ui->NValue->value());

    ui->Calculate->setDisabled(true);

    ui->ChartWidget->updateData();
}
