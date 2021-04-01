#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <vector>

#include "function.h"
#include "riemannsum.h"

using bigdouble = long double;

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

    inline void setAlpha  (double value) { f.setAlpha  (value); update(); }
    inline void setBeta   (double value) { f.setBeta   (value); update(); }
    inline void setGamma  (double value) { f.setGamma  (value); update(); }
    inline void setDelta  (double value) { f.setDelta  (value); update(); }
    inline void setEpsilon(double value) { f.setEpsilon(value); update(); }

    void setA(double value) { R.setA(value); A = value; updateData(); }
    void setB(double value) { R.setB(value); B = value; updateData(); }
    void setC(double value) { C = value; updateData(); }
    void setD(double value) { D = value; updateData(); }

    void setN(int value) { R.setN(value); update(); }

    void setd(double value) { R.setd(value); update(); };

    void setAlphaFix   () { R.setFixType(Coefficient::Alpha  ); update(); }
    void setBetaFix    () { R.setFixType(Coefficient::Beta   ); update(); }
    void setGammaFix   () { R.setFixType(Coefficient::Gamma  ); update(); }
    void setDeltaFix   () { R.setFixType(Coefficient::Delta  ); update(); }
    void setEpsilonFix () { R.setFixType(Coefficient::Epsilon); update(); }

    void setF_vision (bool value) { F_isVisible  = value; update(); }
    void setR_vision (bool value) { R_isVisible  = value; update(); }

    void updateData();

    void updateUPP();

    template<Direction Dir>
    double getUPP();

    template<Direction Dir>
    double getOffset();

    template<Direction Dir>
    double getLimit();

    template<Direction Dir>
    double getPixelLimit();
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
    RiemannSum R;

    // unit per pixel
    double XUPP = 1.0;
    double YUPP = 1.0;

    double A = -100.0;
    double B = 100.0;
    double C = -100.0;
    double D = 100.0;

    bool F_isVisible = true;
    bool R_isVisible = true;

    QPoint mousePos;

    static constexpr int BORDER_SIZE = 10;
};



#endif // CHART_H
