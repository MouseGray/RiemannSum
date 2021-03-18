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
    auto sign = boost::math::sign(event->angleDelta().y());
    setXPPU((XPPU + 50_px*sign).value);
    setYPPU((YPPU + 50_px*sign).value);

    emit XPPUChanged(XPPU);
    emit YPPUChanged(YPPU);
    update();
}

void Chart::mousePressEvent(QMouseEvent *event)
{
    oldOffsetX = offsetX;
    oldOffsetY = offsetY;
    pressPos = event->pos();
}

void Chart::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::MouseButton::LeftButton){
        offsetX = oldOffsetX + (pressPos.x() - event->x())*range(XPPU)*0.04;
        offsetY = oldOffsetY + (pressPos.y() - event->y())*range(YPPU)*0.04;
        emit offsetXChanged(offsetX);
        emit offsetYChanged(offsetY);
    }
    else
        mousePos = event->pos();

    update();
}

void Chart::mouseReleaseEvent(QMouseEvent *)
{
}

void Chart::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    auto width = pixel(this->width());
    auto height = pixel(this->height());

    auto midX = pixel(this->width() >> 1);
    auto midY = pixel(this->height() >> 1);

    painter.setPen(QPen(Qt::white, 1, Qt::PenStyle::DashLine));
    auto a = midX + XPPU*(A - offsetX);
    auto b = midX + XPPU*(B - offsetX);
    auto c = midY + YPPU*(C - offsetY);
    auto d = midY + YPPU*(D - offsetY);

    painter.drawLine(a.value, 0, a.value, height.value);
    painter.drawLine(b.value, 0, b.value, height.value);

    painter.drawLine(0, c.value, width.value, c.value);
    painter.drawLine(0, d.value, width.value, d.value);

    painter.setPen(QPen(QColor(255, 0, 255), 1));

    auto dx = midX + XPPU*(this->dx - offsetX);

    painter.drawLine(dx.value, 0, dx.value, height.value);

    painter.setPen(QPen(Qt::white));

    // Axis
    auto x = -XPPU*offsetX + midX;
    auto y = -YPPU*offsetY + midY;
    painter.drawLine(0, y.value, width.value, y.value);
    painter.drawLine(x.value, 0, x.value, height.value);

    auto xj = (-1_px - midX)/XPPU + offsetX;
    auto x0 = ( 0_px - midX)/XPPU + offsetX;
    auto x1 = ( 1_px - midX)/XPPU + offsetX;

    auto F_yj = f(xj);
    auto F_y0 = f(x0);
    auto F_y1 = f(x1);

    auto N_yj = N1(xj);
    auto N_y0 = N1(x0);
    auto N_y1 = N1(x1);

    auto F_yj_px = midY - YPPU*(F_yj + offsetY);
    auto F_y0_px = midY - YPPU*(F_y0 + offsetY);
    auto F_y1_px = midY - YPPU*(F_y1 + offsetY);

    auto N_yj_px = midY - YPPU*(N_yj + offsetY);
    auto N_y0_px = midY - YPPU*(N_y0 + offsetY);
    auto N_y1_px = midY - YPPU*(N_y1 + offsetY);

    auto F_dif0 = midY - YPPU*(XPPU*(F_y1 - F_yj)/2_px + offsetY);
    auto F_dif1 = 0_px;

    auto N_dif0 = midY - YPPU*(XPPU*(N_y1 - N_yj)/2_px + offsetY);
    auto N_dif1 = 0_px;

    //auto start = clock();
    for(auto p = 0_px; p < width; p++){

        painter.setPen(Qt::red);
        if (F_vision) painter.drawLine(p.value, F_y0_px.value, (p + 1).value, F_y1_px.value);

        painter.setPen(Qt::blue);
        if (P_vision) painter.drawLine(p.value, N_y0_px.value, (p + 1).value, N_y1_px.value);

        auto r0 = midY - YPPU*((F_y0 - N_y0) + offsetY);
        auto r1 = midY - YPPU*((F_y1 - N_y1) + offsetY);

        painter.setPen(QColor(0, 255, 255));
        if (R_vision) painter.drawLine(p.value, r0.value, (p + 1_px).value, r1.value);

        xj = x0;
        x0 = x1;
        x1 = (p + 2_px - midX)/XPPU + offsetX;

        F_yj = F_y0;
        F_y0 = F_y1;
        F_y1 = f(x1);

        N_yj = N_y0;
        N_y0 = N_y1;
        N_y1 = N1(x1);

        F_yj_px = F_y0_px;
        F_y0_px = F_y1_px;
        F_y1_px = midY - YPPU*(F_y1 + offsetY);

        N_yj_px = N_y0_px;
        N_y0_px = N_y1_px;
        N_y1_px = midY - YPPU*(N_y1 + offsetY);

        F_dif1 = midY - YPPU*(XPPU*(F_y1 - F_yj)/2_px + offsetY);

        painter.setPen(Qt::yellow);
        if (dF_vision) painter.drawLine(p.value, F_dif0.value, (p + 1_px).value, F_dif1.value);

        F_dif0 = F_dif1;

        N_dif1 = midY - YPPU*(XPPU*(N_y1 - N_yj)/2_px + offsetY);

        painter.setPen(Qt::green);
        if (dP_vision) painter.drawLine(p.value, N_dif0.value, (p + 1_px).value, N_dif1.value);

        N_dif0 = N_dif1;
    }

    //qDebug() << clock() - start;

    painter.setPen(Qt::white);

    painter.drawText(width.value - 60, 20, "x:" + QString::number((pixel(mousePos.x()) - midX)/XPPU + offsetX));
    painter.drawText(width.value - 60, 40, "y:" + QString::number(-(pixel(mousePos.y()) - midY + offsetY)/YPPU + offsetY));

    painter.drawText(width.value - 120, 20, "x:" + QString::number(mousePos.x()));
    painter.drawText(width.value - 120, 40, "y:" + QString::number(mousePos.y()));

    painter.drawEllipse(mousePos.x(), mousePos.y(), 3, 3);



    if (XAxisIsVisible())
        drawXAxis(painter);
    else
        drawLXAxis(painter);
    if (YAxisIsVisible())
        drawYAxis(painter);
    else
        drawLYAxis(painter);



    //painter.drawText(QPoint(width() - 40, 60), "zx:" + QString::number(zoomX));
    //painter.drawText(QPoint(width() - 40, 80), "zy:" + QString::number(boost::math::sign(0)));

}


#include <thread>

void Chart::calc()
{
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

bool Chart::YAxisIsVisible()
{
    auto pos = -offsetX*XPPU.value + (width() >> 1);
    return pos > 0 && pos < width();
}

bool Chart::XAxisIsVisible()
{
    auto pos = -offsetY*YPPU.value + (height() >> 1);
    return pos > 0 && pos < height();
}

void Chart::drawXAxis(QPainter &painter)
{
    auto coef = range(XPPU);
    auto offset = XPPU.value*coef;

    auto axisPos = (width() >> 1) - offsetX*XPPU.value;

    auto it = int(axisPos < 0.0 ? -axisPos/offset : 0);
    auto limit = width();
    auto curPos = axisPos + it*offset;

    while(curPos <= limit) {
        painter.drawLine(curPos, -offsetY*YPPU.value + (height() >> 1) + 2, curPos, -offsetY*YPPU.value + (height() >> 1) - 2);
        painter.drawText(curPos - 15, -offsetY*YPPU.value + (height() >> 1) + 15, QString::number(it*coef));
        curPos += offset;
        it++;
    }

    if (axisPos > width()) {
        it = std::max((axisPos-width())/offset, 1.0);
        curPos = axisPos - it*offset;
    }
    else {
        it = 1;
        curPos = axisPos - offset;
    }
    while(curPos >= 0) {
        painter.drawLine(curPos, -offsetY*YPPU.value + (height() >> 1) + 2, curPos, -offsetY*YPPU.value + (height() >> 1) - 2);
        painter.drawText(curPos - 15, -offsetY*YPPU.value + (height() >> 1) + 15, QString::number(-it*coef));
        curPos -= offset;
        it++;
    }
}

void Chart::drawLXAxis(QPainter &painter)
{
    auto coef = range(XPPU);
    int offset = XPPU.value*coef;

    auto axisPos = width()/2 - offsetX*XPPU.value;

    auto it = 0;
    auto limit = 0.0;
    auto curPos = 0.0;

    if (axisPos < 0) {
        limit = width() - axisPos;
        it = -axisPos/offset;
        curPos = axisPos + it*offset;
    }
    else {
        it = 0;
        limit = width();
        curPos = axisPos;
    }

    while(curPos <= limit) {
        painter.drawText(curPos - 15, height() - 15, QString::number(it*coef));
        curPos += offset;
        it++;
    }

    if (axisPos > width()) {
        it = std::max((axisPos-width())/offset, 1.0);
        curPos = axisPos - it*offset;
    }
    else {
        it = 1;
        curPos = axisPos - offset;
    }

    while(curPos >= 0) {
        painter.drawText(curPos - 15, height() - 15, QString::number(it*coef));
        curPos -= offset;
        it++;
    }
}

void Chart::drawYAxis(QPainter &painter)
{
    auto coef = range(YPPU);
    int offset = YPPU.value*coef;

    auto axisPos = height()/2 - offsetY*YPPU.value;

    auto it = 0;
    auto limit = 0.0;
    auto curPos = 0.0;

    if (axisPos < 0) {
        limit = height() - axisPos;
        it = -axisPos/offset;
        curPos = axisPos + it*offset;
    }
    else {
        it = 1;
        limit = height();
        curPos = axisPos + offset;
    }

    while(curPos <= limit) {
        painter.drawLine(-offsetX*XPPU.value + (width() >> 1) + 2, curPos, -offsetX*XPPU.value + (width() >> 1) - 2, curPos);
        painter.drawText(-offsetX*XPPU.value + (width() >> 1) - 25, curPos, QString::number(-it*coef));
        curPos += offset;
        it++;
    }

    if (axisPos > height()) {
        it = std::max((axisPos-height())/offset, 1.0);
        curPos = axisPos - it*offset;
    }
    else {
        it = 1;
        curPos = axisPos - offset;
    }

    while(curPos >= 0) {
        painter.drawLine(-offsetX*XPPU.value + (width() >> 1) + 2, curPos, -offsetX*XPPU.value + (width() >> 1) - 2, curPos);
        painter.drawText(-offsetX*XPPU.value + (width() >> 1) - 25, curPos, QString::number(it*coef));
        curPos -= offset;
        it++;
    }
}

void Chart::drawLYAxis(QPainter &painter)
{
    auto coef = range(YPPU);
    int offset = YPPU.value*coef;

    auto axisPos = height()/2 - offsetY*YPPU.value;

    auto it = 0;
    auto limit = 0.0;
    auto curPos = 0.0;

    if (axisPos < 0) {
        limit = height() - axisPos;
        it = -axisPos/offset;
        curPos = axisPos + it*offset;
    }
    else {
        it = 1;
        limit = height();
        curPos = axisPos + offset;
    }

    while(curPos <= limit) {
        painter.drawText(0, curPos, QString::number(-it*coef));
        curPos += offset;
        it++;
    }

    if (axisPos > height()) {
        it = std::max((axisPos-height())/offset, 1.0);
        curPos = axisPos - it*offset;
    }
    else {
        it = 1;
        curPos = axisPos - offset;
    }
    while(curPos >= 0) {
        painter.drawText(0, curPos, QString::number(it*coef));
        curPos -= offset;
        it++;
    }
}



double Chart::range(pixel value)
{
    if(1_px     <= value && value < 5_px) return 50;
    if(5_px     <= value && value < 20_px) return 10;
    if(20_px    <= value && value < 50_px) return 5;
    if(50_px    <= value && value < 150_px) return 1;

    if(150_px   <= value && value < 400_px) return 0.5;
    if(400_px   <= value && value < 2000_px) return 0.1;
    if(2000_px  <= value && value < 4000_px) return 0.05;
    if(4000_px  <= value && value < 20000_px) return 0.01;
    if(20000_px <= value && value < 40000_px) return 0.005;
    return 0.001;
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
    return alpha*boost::multiprecision::sin(
                boost::multiprecision::tan(beta/((x - gamma == 0) ? bigdouble(0.00000000001) : x - gamma))) + delta*boost::multiprecision::cos(epsilon*x);
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
        accum = (accum*(x - offset))/(k*h);
        //accum = accum*((q - k + 1)/k);
        offset += h;

//        qDebug() << "k:" << k;
//        qDebug() << "res:" << result.str().c_str();
//        qDebug() << "acc:" << accum.str().c_str();
//        qDebug() << "dif:" << finiteDifferences[k-1].str().c_str();

        result += accum*finiteDifferences[k - 1];
    }

    return result.convert_to<double>();
}

double Chart::N1_d(double x)
{
    auto result = t_result;
    double h = double(B-A)/(N);

    //double q = (x - A)/h;
    bigdouble accum = 1.0;
    double offset = A;
    for(auto k = 1; k < N + 1; k++){
        accum = (accum*(x - offset))/(k*h);
        //accum = accum*((q - k + 1)/k);
        offset += h;
        qDebug() << "k:" << k;
        qDebug() << "res:" << result.str().c_str();
        qDebug() << "acc:" << accum.str().c_str();
        qDebug() << "dif:" << finiteDifferences[k-1].str().c_str();
        result += accum*finiteDifferences[k - 1];

        qDebug() << "res2:" << result.str().c_str();
    }
    qDebug() << result.str().c_str();
    return static_cast<double>(result);
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

void Chart::setOffsetY(int value)
{
    offsetY = value;
    update();
}

void Chart::setOffsetX(int value)
{
    offsetX = value;
    update();
}

void Chart::setYPPU(int value)
{
    YPPU = std::clamp(value, 1, 100000);
    update();
}

void Chart::setXPPU(int value)
{
    XPPU = std::clamp(value, 1, 100000);
    update();
}
