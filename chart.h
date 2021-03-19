#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <vector>

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/special_functions.hpp>

#include "pixels.h"

using bigdouble = long double;//boost::multiprecision::cpp_dec_float_100;

enum class Direction : bool {
    Horizontal,
    Vertical
};

template<Direction Dir>
constexpr Direction notDir() {
    if constexpr (Dir == Direction::Horizontal) return Direction::Vertical;
    else return Direction::Horizontal;
}

class Chart : public QWidget
{
    Q_OBJECT
public:
    explicit Chart(QWidget *parent = nullptr);

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

    template<Direction Dir>
    double getUPP() {
        if constexpr (Dir == Direction::Horizontal) return XUPP;
        else return YUPP;
    }

    template<Direction Dir>
    double getOffset() {
        if constexpr (Dir == Direction::Horizontal) return A;
        else return C;
    }

    template<Direction Dir>
    double getLimit() {
        if constexpr (Dir == Direction::Horizontal) return B;
        else return D;
    }
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
    template<Direction Dir>
    int valueToPixel(double value){
        if constexpr (Dir == Direction::Vertical) return (height() - 20) - (value - getOffset<Dir>())/getUPP<Dir>();
        return (value - getOffset<Dir>())/getUPP<Dir>();
    }

    template<Direction Dir>
    double pixelToValue(int pixel) {
        if constexpr (Dir == Direction::Vertical) pixel = height() - pixel;
        return pixel*getUPP<Dir>() + getOffset<Dir>();
    }

    template<Direction Dir>
    bool AxisIsVisible()
    {
        auto pos = valueToPixel<Direction::Vertical>(0.0);
        return pos > 0 && pos < (Dir == Direction::Horizontal ? height() : width());
    }

    template<Direction Dir>
    void drawAxis(QPainter& painter, bool visible);

    void drawLine(QPainter& painter, int x1, int y1, int x2, int y2);

    void fillFiniteDifferences();

    // QPoint pressPos;

    bigdouble f(bigdouble x);

    bigdouble t_result = 0;

    double f(double x);
    double N1(double x);

    double alpha = 1.0;
    double beta = 1.0;
    double gamma = 1.0;
    double delta = 1.0;
    double epsilon = 1.0;

    // double oldOffsetX = 0.0;
    // double oldOffsetY = 0.0;

    // double offsetX = 0.0;
    // double offsetY = 0.0;

    double XUPP = 1.0;
    double YUPP = 1.0;

    // pixel XPPU = 1_px;
    // pixel YPPU = 1_px;

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

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};

template<Direction Dir>
void Chart::drawAxis(QPainter& painter, bool visible)
{
    auto w = getLimit<Dir>() - getOffset<Dir>();
    auto offset = w/10;

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
                painter.drawText(pos, height(), QString::number(pvalue, 'g', 6));
            }
        }
        else {
            if (visible) {
                drawLine(painter, axisPos + 2, pos, axisPos - 2, pos);
                painter.drawText(axisPos + 20, pos + 10, QString::number(pvalue, 'g', 6));
            }
            else {
                painter.drawText(30, pos, QString::number(pvalue, 'g', 6));
            }
        }
        value += offset;
    }
}

#endif // CHART_H
