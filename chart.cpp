#include "chart.h"

#include <QPainter>
#include <cmath>

#include <QDebug>
#include <string>

using namespace std;

template<>
double Chart::getLimit<Direction::Horizontal>() {
    return B;
}

template<>
double Chart::getLimit<Direction::Vertical>() {
    return D;
}

template<>
double Chart::getOffset<Direction::Horizontal>() {
    return A;
}

template<>
double Chart::getOffset<Direction::Vertical>() {
    return C;
}

template<>
double Chart::getUPP<Direction::Horizontal>() {
    return XUPP;
}

template<>
double Chart::getUPP<Direction::Vertical>() {
    return YUPP;
}

template<>
double Chart::getPixelLimit<Direction::Horizontal>() {
    return width();
}

template<>
double Chart::getPixelLimit<Direction::Vertical>() {
    return height();
}

template<>
int Chart::valueToPixel<Direction::Horizontal>(double value){
    return (value - getOffset<Direction::Horizontal>())/getUPP<Direction::Horizontal>();
}

template<>
int Chart::valueToPixel<Direction::Vertical>(double value){
    return (getPixelLimit<Direction::Vertical>() - 2*BORDER_SIZE) - (value - getOffset<Direction::Vertical>())/getUPP<Direction::Vertical>();
    //     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ axis inversion
}

template<>
double Chart::pixelToValue<Direction::Horizontal>(int pixel) {
    return pixel*getUPP<Direction::Horizontal>() + getOffset<Direction::Horizontal>();
}

template<>
double Chart::pixelToValue<Direction::Vertical>(int pixel) {
    pixel = getPixelLimit<Direction::Vertical>() - pixel; // axis inversion
    return pixel*getUPP<Direction::Vertical>() + getOffset<Direction::Vertical>();
}

Chart::Chart(QWidget *parent) : QWidget(parent)
{
    P.setF(&f);
    updateData();
}

void Chart::mouseMoveEvent(QMouseEvent *event)
{
    mousePos = event->pos();
    update();
}

void Chart::resizeEvent(QResizeEvent*)
{
    updateUPP();
}

void Chart::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    // Draw A, B, C and D
    painter.setPen(QPen(Qt::white, 1, Qt::PenStyle::DashLine));

    drawLine(painter, valueToPixel<Direction::Horizontal>(A), -10,
                      valueToPixel<Direction::Horizontal>(A), getPixelLimit<Direction::Vertical>() - 10);
    drawLine(painter, valueToPixel<Direction::Horizontal>(B), -10,
                      valueToPixel<Direction::Horizontal>(B), getPixelLimit<Direction::Vertical>() - 10);

    drawLine(painter, -10, valueToPixel<Direction::Vertical>(C),
                      getPixelLimit<Direction::Horizontal>() - 10, valueToPixel<Direction::Vertical>(C));
    drawLine(painter, -10, valueToPixel<Direction::Vertical>(D),
                      getPixelLimit<Direction::Horizontal>() - 10, valueToPixel<Direction::Vertical>(D));

    // Draw d
    painter.setPen(QPen(QColor(255, 0, 255), 1));
    auto dx = valueToPixel<Direction::Horizontal>(this->dx);
    drawLine(painter, dx, -10, dx, getPixelLimit<Direction::Vertical>() - 10);

    // Draw axis
    painter.setPen(QPen(Qt::white));

    auto x = valueToPixel<Direction::Horizontal>(0.0);
    auto y = valueToPixel<Direction::Vertical>(0.0);
    drawLine(painter, -10, y, getPixelLimit<Direction::Horizontal>() - 10, y);
    drawLine(painter, x, -10, x, getPixelLimit<Direction::Vertical>() - 10);

    // Calculate primary values
    auto xj = pixelToValue<Direction::Horizontal>(-1);
    auto x0 = pixelToValue<Direction::Horizontal>(0);
    auto x1 = pixelToValue<Direction::Horizontal>(1);

    auto F_yj = f(xj);
    auto F_y0 = f(x0);
    auto F_y1 = f(x1);

    auto N_yj = P(xj);
    auto N_y0 = P(x0);
    auto N_y1 = P(x1);

//  auto F_yj_px = valueToPixel<Direction::Vertical>(F_yj);
    auto F_y0_px = valueToPixel<Direction::Vertical>(F_y0);
    auto F_y1_px = valueToPixel<Direction::Vertical>(F_y1);

//  auto N_yj_px = valueToPixel<Direction::Vertical>(N_yj);
    auto N_y0_px = valueToPixel<Direction::Vertical>(N_y0);
    auto N_y1_px = valueToPixel<Direction::Vertical>(N_y1);

    auto F_dif0 = valueToPixel<Direction::Vertical>((F_y1 - F_yj)/(2*XUPP));
    auto F_dif1 = 0;

    auto N_dif0 = valueToPixel<Direction::Vertical>((N_y1 - N_yj)/(2*XUPP));
    auto N_dif1 = 0;

    // Draw charts
    for(auto p = 0; p < getPixelLimit<Direction::Horizontal>(); p++){

        // Draw f
        painter.setPen(Qt::red);
        if (F_isVisible) drawLine(painter, p, F_y0_px, (p + 1), F_y1_px);

        // Draw P
        painter.setPen(Qt::blue);
        if (P_isVisible) drawLine(painter, p, N_y0_px, (p + 1), N_y1_px);

        // Draw r
        painter.setPen(QColor(0, 255, 255));
        if (R_isVisible) {
            auto r0 = valueToPixel<Direction::Vertical>(F_y0 - N_y0);
            auto r1 = valueToPixel<Direction::Vertical>(F_y1 - N_y1);
            drawLine(painter, p, r0, (p + 1), r1);
        }

        // Calculate next values
        xj = x0;
        x0 = x1;
        x1 = pixelToValue<Direction::Horizontal>(p + 2);

        F_yj = F_y0;
        F_y0 = F_y1;
        F_y1 = f(x1);

        N_yj = N_y0;
        N_y0 = N_y1;
        N_y1 = P(x1);

//      F_yj_px = F_y0_px;
        F_y0_px = F_y1_px;
        F_y1_px = valueToPixel<Direction::Vertical>(F_y1);

//      N_yj_px = N_y0_px;
        N_y0_px = N_y1_px;
        N_y1_px = valueToPixel<Direction::Vertical>(N_y1);

        F_dif1 = valueToPixel<Direction::Vertical>((F_y1 - F_yj)/(2*XUPP));

        N_dif1 = valueToPixel<Direction::Vertical>((N_y1 - N_yj)/(2*XUPP));

        // Draw dF
        painter.setPen(Qt::yellow);
        if (dF_isVisible) drawLine(painter, p, F_dif0, (p + 1), F_dif1);

        // Draw dP
        painter.setPen(Qt::green);
        if (dP_isVisible) drawLine(painter, p, N_dif0, (p + 1), N_dif1);

        F_dif0 = F_dif1;

        N_dif0 = N_dif1;
    }

    // Draw coords text
    painter.setPen(Qt::white);

    painter.drawText(getPixelLimit<Direction::Horizontal>() - 80, 20,
                     "x:" + QString::number(pixelToValue<Direction::Horizontal>(mousePos.x() - 10)));
    painter.drawText(getPixelLimit<Direction::Horizontal>() - 80, 40,
                     "y:" + QString::number(pixelToValue<Direction::Vertical>(mousePos.y() + 10)));

    painter.drawText(getPixelLimit<Direction::Horizontal>() - 140, 20,
                     "x:" + QString::number(mousePos.x()));
    painter.drawText(getPixelLimit<Direction::Horizontal>() - 140, 40,
                     "y:" + QString::number(mousePos.y()));

    painter.drawEllipse(mousePos.x(), mousePos.y(), 3, 3);


    drawAxis<Direction::Horizontal>(painter, AxisIsVisible<Direction::Vertical>());
    drawAxis<Direction::Vertical>(painter, AxisIsVisible<Direction::Horizontal>());
}

void Chart::drawLine(QPainter &painter, int x1, int y1, int x2, int y2)
{
    painter.drawLine(x1 + BORDER_SIZE, y1 + BORDER_SIZE, x2 + BORDER_SIZE, y2 + BORDER_SIZE);
}

void Chart::updateData()
{
    updateUPP();
    calculate_d();
    update();
}

void Chart::updateUPP()
{
    XUPP = (B - A)/(width() - 2*BORDER_SIZE);
    YUPP = (D - C)/(height() - 2*BORDER_SIZE);
}

void Chart::calculate_d()
{
    d = 0;
    for(auto s = A; s <= B; s += 0.01) {
        auto val = abs(f(s) - P(s));
        if (d < val) {
            d = val; dx = s;
        }
    }

    emit dChanged(d, dx);
}

template<Direction Dir>
bool Chart::AxisIsVisible()
{
    auto pos = valueToPixel<Dir>(0.0);
    return pos > 0 && pos < getPixelLimit<Dir>();
}

template<Direction Dir>
void Chart::drawAxis(QPainter& painter, bool visible)
{
    auto w = getLimit<Dir>() - getOffset<Dir>();

    auto offset = w/10;
    // TODO:        ^ do something more serious

    auto value = getOffset<Dir>();

    auto axisPos = valueToPixel<notDir<Dir>()>(0.0);

    for(auto i = 0; i <= 10; i++) {

        auto pos = valueToPixel<Dir>(value);

        auto pvalue = static_cast<int>(value*100'000)/100'000.0;

        if constexpr (Dir == Direction::Horizontal) {
            if (visible) {
                drawLine(painter, pos, axisPos + 2, pos, axisPos - 2);
                painter.drawText(pos, axisPos + 25, QString::number(pvalue, 'g', 6));
            }
            else {
                painter.drawText(pos, getPixelLimit<Direction::Vertical>(), QString::number(pvalue, 'g', 6));
            }
        }
        else {
            if (visible) {
                drawLine(painter, axisPos + 2, pos, axisPos - 2, pos);
                painter.drawText(axisPos + 20, pos + 10, QString::number(pvalue, 'g', 6));
            }
            else {
                painter.drawText(25, pos, QString::number(pvalue, 'g', 6));
            }
        }
        value += offset;
    }
}
