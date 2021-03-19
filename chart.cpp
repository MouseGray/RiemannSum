#include "chart.h"

#include <QPainter>
#include <cmath>

#include <QDebug>
#include <string>

using namespace std;

Chart::Chart(QWidget *parent) : QWidget(parent)
{
    updateData();
    //fillFiniteDifferences();
    //N1_d(99);
}

void Chart::wheelEvent(QWheelEvent *event)
{
#ifdef DEPRECATED
    auto sign = boost::math::sign(event->angleDelta().y());
    setXPPU((XPPU + 50_px*sign).value);
    setYPPU((YPPU + 50_px*sign).value);

    emit XPPUChanged(XPPU);
    emit YPPUChanged(YPPU);
    update();
#endif
}

void Chart::mousePressEvent(QMouseEvent *event)
{
#ifdef DEPRECATED
    oldOffsetX = offsetX;
    oldOffsetY = offsetY;
    pressPos = event->pos();
#endif
}

void Chart::mouseMoveEvent(QMouseEvent *event)
{
#ifdef DEPRECATED
    if(event->buttons() & Qt::MouseButton::LeftButton){
        offsetX = oldOffsetX + (pressPos.x() - event->x())*range(XPPU)*0.04;
        offsetY = oldOffsetY + (pressPos.y() - event->y())*range(YPPU)*0.04;
        emit offsetXChanged(offsetX);
        emit offsetYChanged(offsetY);
    }
#endif
    mousePos = event->pos();

    update();
}

void Chart::mouseReleaseEvent(QMouseEvent *)
{
}

void Chart::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    auto width = this->width();
    auto height = this->height();

    painter.setPen(QPen(Qt::white, 1, Qt::PenStyle::DashLine));

    drawLine(painter, valueToPixel<Direction::Horizontal>(A), -10,
                      valueToPixel<Direction::Horizontal>(A), height - 10);
    drawLine(painter, valueToPixel<Direction::Horizontal>(B), -10,
                      valueToPixel<Direction::Horizontal>(B), height - 10);

    drawLine(painter, -10, valueToPixel<Direction::Vertical>(C),
                      width - 10, valueToPixel<Direction::Vertical>(C));
    drawLine(painter, -10, valueToPixel<Direction::Vertical>(D),
                      width - 10, valueToPixel<Direction::Vertical>(D));

    painter.setPen(QPen(QColor(255, 0, 255), 1));

    auto dx = valueToPixel<Direction::Horizontal>(this->dx);

    drawLine(painter, dx, -10, dx, height - 10);

    painter.setPen(QPen(Qt::white));

    // Axis
    auto x = valueToPixel<Direction::Horizontal>(0.0);
    auto y = valueToPixel<Direction::Vertical>(0.0);
    drawLine(painter, -10, y, width - 10, y);
    drawLine(painter, x, -10, x, height - 10);

    auto xj = pixelToValue<Direction::Horizontal>(-1);
    auto x0 = pixelToValue<Direction::Horizontal>(0);
    auto x1 = pixelToValue<Direction::Horizontal>(1);

    auto F_yj = f(xj);
    auto F_y0 = f(x0);
    auto F_y1 = f(x1);

    auto N_yj = N1(xj);
    auto N_y0 = N1(x0);
    auto N_y1 = N1(x1);

    auto F_yj_px = valueToPixel<Direction::Vertical>(F_yj);
    auto F_y0_px = valueToPixel<Direction::Vertical>(F_y0);
    auto F_y1_px = valueToPixel<Direction::Vertical>(F_y1);

    auto N_yj_px = valueToPixel<Direction::Vertical>(N_yj);
    auto N_y0_px = valueToPixel<Direction::Vertical>(N_y0);
    auto N_y1_px = valueToPixel<Direction::Vertical>(N_y1);

    auto F_dif0 = valueToPixel<Direction::Vertical>((F_y1 - F_yj)/(2*XUPP));
    auto F_dif1 = 0;

    auto N_dif0 = valueToPixel<Direction::Vertical>((N_y1 - N_yj)/(2*XUPP));
    auto N_dif1 = 0;

    //auto start = clock();
    for(auto p = 0; p < width; p++){

        painter.setPen(Qt::red);
        if (F_vision) drawLine(painter, p, F_y0_px, (p + 1), F_y1_px);

        painter.setPen(Qt::blue);
        if (P_vision) drawLine(painter, p, N_y0_px, (p + 1), N_y1_px);

        auto r0 = valueToPixel<Direction::Vertical>(F_y0 - N_y0);
        auto r1 = valueToPixel<Direction::Vertical>(F_y1 - N_y1);

        painter.setPen(QColor(0, 255, 255));
        if (R_vision) drawLine(painter, p, r0, (p + 1), r1);

        xj = x0;
        x0 = x1;
        x1 = pixelToValue<Direction::Horizontal>(p + 2);

        F_yj = F_y0;
        F_y0 = F_y1;
        F_y1 = f(x1);

        N_yj = N_y0;
        N_y0 = N_y1;
        N_y1 = N1(x1);

        F_yj_px = F_y0_px;
        F_y0_px = F_y1_px;
        F_y1_px = valueToPixel<Direction::Vertical>(F_y1);

        N_yj_px = N_y0_px;
        N_y0_px = N_y1_px;
        N_y1_px = valueToPixel<Direction::Vertical>(N_y1);

        F_dif1 = valueToPixel<Direction::Vertical>((F_y1 - F_yj)/(2*XUPP));

        painter.setPen(Qt::yellow);
        if (dF_vision) drawLine(painter, p, F_dif0, (p + 1), F_dif1);

        F_dif0 = F_dif1;

        N_dif1 = valueToPixel<Direction::Vertical>((N_y1 - N_yj)/(2*XUPP));

        painter.setPen(Qt::green);
        if (dP_vision) drawLine(painter, p, N_dif0, (p + 1), N_dif1);

        N_dif0 = N_dif1;
    }

    //qDebug() << clock() - start;

    painter.setPen(Qt::white);

    painter.drawText(width - 80, 20, "x:" + QString::number(pixelToValue<Direction::Horizontal>(mousePos.x() - 10)));
    painter.drawText(width - 80, 40, "y:" + QString::number(pixelToValue<Direction::Vertical>(mousePos.y() + 10)));

    painter.drawText(width - 140, 20, "x:" + QString::number(mousePos.x()));
    painter.drawText(width - 140, 40, "y:" + QString::number(mousePos.y()));

    painter.drawEllipse(mousePos.x(), mousePos.y(), 3, 3);


    drawAxis<Direction::Horizontal>(painter, AxisIsVisible<Direction::Vertical>());
    drawAxis<Direction::Vertical>(painter, AxisIsVisible<Direction::Horizontal>());

    //painter.drawText(QPoint(width() - 40, 60), "zx:" + QString::number(zoomX));
    //painter.drawText(QPoint(width() - 40, 80), "zy:" + QString::number(boost::math::sign(0)));

}

void Chart::drawLine(QPainter &painter, int x1, int y1, int x2, int y2)
{
    painter.drawLine(x1 + 10, y1 + 10, x2 + 10, y2 + 10);
}

#include <thread>

void Chart::calc()
{
    XUPP = (B - A)/(width() - 20);
    YUPP = (D - C)/(height() - 20);

    fillFiniteDifferences();
    t_result = f(bigdouble(A));

    d = 0;
    for(auto s = A; s <= B; s += 0.01) {
        auto val = abs(f(s) - N1(s));
        if (d < val) {
            d = val;
            dx = s;
        }
    }
    emit dChanged(d, dx);
    unlock();
    update();
}

void Chart::updateData()
{
    emit lock();
    std::thread t(&Chart::calc, this);
    t.detach();
}

void Chart::fillFiniteDifferences()
{
    std::vector<bigdouble> vector_1(N + 1, 0.0);
    std::vector<bigdouble> vector_2(N + 1, 0.0);

    bigdouble h = (B-A)/N;
    bigdouble x = A;
    for (auto i = 0; i < N + 1; i++) {
        vector_1[i] = f(x);
        x += h;
    }

    auto pvec_1 = &vector_1;
    auto pvec_2 = &vector_2;
    auto ptemp = pvec_1;

    finiteDifferences.clear();

    for(auto j = 0; j < N; j++) {
        for(auto i = 0; i < N - j; i++) {
            pvec_2->at(i) = pvec_1->at(i + 1) - pvec_1->at(i);
        }
        finiteDifferences.push_back(pvec_2->at(0));
        ptemp = pvec_1;
        pvec_1 = pvec_2;
        pvec_2 = ptemp;
    }
}

bigdouble Chart::f(bigdouble x)
{
    if (x - gamma == 0) return 0;
    return alpha*sin(tan(beta/(x - gamma))) + delta*cos(epsilon*x);
//    return alpha*boost::multiprecision::sin(
//                boost::multiprecision::tan(beta/((x - gamma == 0) ? bigdouble(0.00000000001) : x - gamma))) + delta*boost::multiprecision::cos(epsilon*x);

}

double Chart::f(double x)
{
    return alpha*sin(tan(beta/((x - gamma == 0) ? 0.0000001 : x - gamma))) + delta*cos(epsilon*x);
}

#include <limits>

double Chart::N1(double x)
{
    auto result = t_result;
    double h = double(B-A)/(N);

    double q = (x - A)/h;
    bigdouble accum = 1.0;
    double offset = A;
    for(auto k = 1; k < N + 1; k++){
        //accum = (accum*(x - offset))/(k*h);
        accum = accum*((q - k + 1)/k);
        offset += h;

//        qDebug() << "k:" << k;
//        qDebug() << "res:" << result.str().c_str();
//        qDebug() << "acc:" << accum.str().c_str();
//        qDebug() << "dif:" << finiteDifferences[k-1].str().c_str();

        result += accum*finiteDifferences[k - 1];
    }

    return result;//.convert_to<double>();
}

void Chart::setR_vision(bool value)
{
    R_vision = value;
    update();
}

void Chart::setDP_vision(bool value)
{
    dP_vision = value;
    update();
}

void Chart::setDF_vision(bool value)
{
    dF_vision = value;
    update();
}

void Chart::setP_vision(bool value)
{
    P_vision = value;
    update();
}

void Chart::setF_vision(bool value)
{
    F_vision = value;
    update();
}

void Chart::setN(int value)
{
    N = value;
}

void Chart::setD(double value)
{
    D = value;
}

void Chart::setC(double value)
{
    C = value;
}

void Chart::setB(double value)
{
    B = value;
}

void Chart::setA(double value)
{
    A = value;
}

void Chart::setEpsilon(double value)
{
    epsilon = value;
}

void Chart::setDelta(double value)
{
    delta = value;
}

void Chart::setGamma(double value)
{
    gamma = value;
}

void Chart::setBeta(double value)
{
    beta = value;
}

void Chart::setAlpha(double value)
{
    alpha = value;
}


void Chart::resizeEvent(QResizeEvent *event)
{
    XUPP = (B - A)/(width() - 20);
    YUPP = (D - C)/(height() - 20);
}
