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
    R.setF(&f);
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

    // Draw axis
    painter.setPen(QPen(Qt::white));

    auto x = valueToPixel<Direction::Horizontal>(0.0);
    auto y = valueToPixel<Direction::Vertical>(0.0);
    drawLine(painter, -10, y, getPixelLimit<Direction::Horizontal>() - 10, y);
    drawLine(painter, x, -10, x, getPixelLimit<Direction::Vertical>() - 10);

    // Calculate primary values
    auto x0 = pixelToValue<Direction::Horizontal>(0);
    auto x1 = pixelToValue<Direction::Horizontal>(1);

    auto F_y0 = f(x0, 0.0, Coefficient::Null);
    auto F_y1 = f(x1, 0.0, Coefficient::Null);

    auto R_y0 = R(x0);
    auto R_y1 = R(x1);

    auto F_y0_px = valueToPixel<Direction::Vertical>(F_y0);
    auto F_y1_px = valueToPixel<Direction::Vertical>(F_y1);

    auto R_y0_px = valueToPixel<Direction::Vertical>(R_y0);
    auto R_y1_px = valueToPixel<Direction::Vertical>(R_y1);

    // Draw charts
    for(auto p = 0; p < getPixelLimit<Direction::Horizontal>(); p++){

        // Draw f
        painter.setPen(Qt::red);
        if (F_isVisible) drawLine(painter, p, F_y0_px, (p + 1), F_y1_px);

        // Draw R
        painter.setPen(Qt::blue);
        if (R_isVisible) drawLine(painter, p, R_y0_px, (p + 1), R_y1_px);

        // Calculate next values
        x0 = x1;
        x1 = pixelToValue<Direction::Horizontal>(p + 2);

        F_y0 = F_y1;
        F_y1 = f(x1, 0.0, Coefficient::Null);

        R_y0 = R_y1;
        R_y1 = R(x1);

        F_y0_px = F_y1_px;
        F_y1_px = valueToPixel<Direction::Vertical>(F_y1);

        R_y0_px = R_y1_px;
        R_y1_px = valueToPixel<Direction::Vertical>(R_y1);
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
    update();
}

void Chart::updateUPP()
{
    XUPP = (B - A)/(width() - 2*BORDER_SIZE);
    YUPP = (D - C)/(height() - 2*BORDER_SIZE);
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
