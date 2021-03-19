#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <vector>

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/special_functions.hpp>

#include "pixels.h"
#include "function.h"
#include "polynomial.h"

using bigdouble = long double; //boost::multiprecision::cpp_dec_float_100;

enum class Direction : bool {
    Horizontal,
    Vertical
};

template<Direction Dir>
constexpr Direction notDir() {
    return Dir == Direction::Horizontal ? Direction::Vertical : Direction::Horizontal;
}

class Chart : public QWidget
{
    Q_OBJECT
public:
    explicit Chart(QWidget *parent = nullptr);

    inline void setAlpha  (double value) { f.setAlpha  (value); updateData(); }
    inline void setBeta   (double value) { f.setBeta   (value); updateData(); }
    inline void setGamma  (double value) { f.setGamma  (value); updateData(); }
    inline void setDelta  (double value) { f.setDelta  (value); updateData(); }
    inline void setEpsilon(double value) { f.setEpsilon(value); updateData(); }

    void setA(double value) { P.setA(value); A = value; updateData(); }
    void setB(double value) { P.setB(value); B = value; updateData(); }
    void setC(double value) { C = value; update(); }
    void setD(double value) { D = value; update(); }

    void setN(int value) { P.setN(value); updateData(); }

    void setF_vision (bool value) { F_isVisible  = value; update(); }
    void setP_vision (bool value) { P_isVisible  = value; update(); }
    void setDF_vision(bool value) { dF_isVisible = value; update(); }
    void setDP_vision(bool value) { dP_isVisible = value; update(); }
    void setR_vision (bool value) { R_isVisible  = value; update(); }

    void updateData();

    void updateUPP();
    void calculate_d();

    template<Direction Dir>
    double getUPP();

    template<Direction Dir>
    double getOffset();

    template<Direction Dir>
    double getLimit();

    template<Direction Dir>
    double getPixelLimit();
signals:
    void dChanged(double d, double dx);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void resizeEvent(QResizeEvent *) override;
private:
    template<Direction Dir>
    int valueToPixel(double value);

    template<Direction Dir>
    double pixelToValue(int pixel) {
        if constexpr (Dir == Direction::Vertical) pixel = height() - pixel;
        return pixel*getUPP<Dir>() + getOffset<Dir>();
    }

    template<Direction Dir>
    bool AxisIsVisible();

    template<Direction Dir>
    void drawAxis(QPainter& painter, bool visible);

    void drawLine(QPainter& painter, int x1, int y1, int x2, int y2);

    Function f;
    Polynomial P;

    // unit per pixel
    double XUPP = 1.0;
    double YUPP = 1.0;

    double A = -100.0;
    double B = 100.0;
    double C = -100.0;
    double D = 100.0;

    double d = 0.0;
    double dx = 0.0;

    bool F_isVisible = true;
    bool P_isVisible = true;
    bool dF_isVisible = true;
    bool dP_isVisible = true;
    bool R_isVisible = true;

    QPoint mousePos;

    static constexpr int BORDER_SIZE = 10;
};



#endif // CHART_H
