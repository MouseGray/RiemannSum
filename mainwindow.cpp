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

void MainWindow::on_AValue_valueChanged(double arg1)
{
    ui->BValue->setMinimum(arg1);
}

void MainWindow::on_CValue_valueChanged(double arg1)
{

    ui->DValue->setMinimum(arg1);
}

void MainWindow::on_FVision_stateChanged(int arg1)
{
    ui->ChartWidget->setF_vision(arg1);
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

void MainWindow::on_AlphaFix_clicked()
{
    ui->ChartWidget->setAlphaFix();
}

void MainWindow::on_BetaFix_clicked()
{
    ui->ChartWidget->setBetaFix();
}

void MainWindow::on_GammaFix_clicked()
{
    ui->ChartWidget->setGammaFix();
}

void MainWindow::on_DeltaFix_clicked()
{
    ui->ChartWidget->setDeltaFix();
}

void MainWindow::on_EpsilonFix_clicked()
{
    ui->ChartWidget->setEpsilonFix();
}
