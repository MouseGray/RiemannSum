#ifndef FUNCTION_H
#define FUNCTION_H

#include <QObject>
#include <cmath>

class Function : public QObject
{
    Q_OBJECT
public:
    using cType = long double;

    Function() {}

    cType operator() (const cType& x) {
        if (x - gamma == 0) return 0;
        return alpha*sin(tan(beta/(x - gamma))) + delta*cos(epsilon*x);
    }

    void setAlpha  (const cType &value) { alpha   = value; emit changed(); };
    void setBeta   (const cType &value) { beta    = value; emit changed(); };
    void setGamma  (const cType &value) { gamma   = value; emit changed(); };
    void setDelta  (const cType &value) { delta   = value; emit changed(); };
    void setEpsilon(const cType &value) { epsilon = value; emit changed(); };

signals:
    void changed();
private:
    cType alpha   = 1.0;
    cType beta    = 1.0;
    cType gamma   = 1.0;
    cType delta   = 1.0;
    cType epsilon = 1.0;
};

#endif // FUNCTION_H
