#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Interpolation N1");

    connect(ui->ChartWidget, SIGNAL(dChanged(double, double)), this, SLOT(setd(double, double)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setd(double value, double pos)
{
    ui->dValue->setText(QString::number(value) + "\n(x=" + QString::number(pos) + ")");
}

void MainWindow::on_AValue_valueChanged(double arg1)
{
    ui->BValue->setMinimum(arg1);
}

// void MainWindow::on_BValue_valueChanged(double arg1) { }

void MainWindow::on_CValue_valueChanged(double arg1)
{

    ui->DValue->setMinimum(arg1);
}

// void MainWindow::on_DValue_valueChanged(double arg1) { }

// void MainWindow::on_NValue_valueChanged(int arg1) { }

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

    ui->ChartWidget->updateData();
}
