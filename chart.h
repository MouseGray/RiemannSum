#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QMouseEvent>
#include <vector>

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/special_functions.hpp>

#include "pixels.h"

using bigdouble = boost::multiprecision::cpp_dec_float_100;

class Chart : public QWidget
{
    Q_OBJECT
public:
    explicit Chart(QWidget *parent = nullptr);

    void setXPPU(int value);
    void setYPPU(int value);

    void setOffsetX(int value);
    void setOffsetY(int value);

    void setAlpha(double value);
    void setBeta(double value);
    void setGamma(double value);
    void setDelta(double value);
    void setEpsilon(double value);

    void setA(double value);
    void setB(double value);
    void setC(double value);
    void setD(double value);

    void setN(int value);

    void calc();
    void setF_vision(bool value);
    void setP_vision(bool value);
    void setDF_vision(bool value);
    void setDP_vision(bool value);
    void setR_vision(bool value);

    void updateData();
signals:
    void offsetXChanged(double value);
    void offsetYChanged(double value);

    void XPPUChanged(pixel value);
    void YPPUChanged(pixel value);

    void dChanged(double d, double dx);

    void lock();
    void unlock();
    // QWidget interface
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:


    bool XAxisIsVisible();
    bool YAxisIsVisible();

    void drawXAxis(QPainter& painter);
    void drawLXAxis(QPainter& painter);
    void drawYAxis(QPainter& painter);
    void drawLYAxis(QPainter& painter);

    double range(pixel value);

    void fillFiniteDifferences();

    QPoint pressPos;

    bigdouble f(bigdouble x);

    bigdouble t_result = 0;

    double f(double x);
    double N1(double x);
    double N1_d(double x);

    double alpha = 1.0;
    double beta = 1.0;
    double gamma = 1.0;
    double delta = 1.0;
    double epsilon = 1.0;

    double oldOffsetX = 0.0;
    double oldOffsetY = 0.0;

    double offsetX = 0.0;
    double offsetY = 0.0;

    pixel XPPU = 1_px;
    pixel YPPU = 1_px;

    double A = -100;
    double B = 100;
    double C = -100;
    double D = 100;

    int N = 10;

    double d = 0.0;
    double dx = 0.0;

    bool F_vision = true;
    bool P_vision = true;
    bool dF_vision = true;
    bool dP_vision = true;
    bool R_vision = true;

    QPoint mousePos;

    std::vector<bigdouble> finiteDifferences;
};

#endif // CHART_H
