#ifndef FUNCTION_H
#define FUNCTION_H

#include <QObject>
#include <cmath>

enum class Coefficient {
    Null,
    Alpha,
    Beta,
    Gamma,
    Delta,
    Epsilon
};

#define _coefs(type, ty) template<ty> \
    type getCoefficient()

#define _coef(type, ty, name) template<> \
    type getCoefficient<ty>() { return name; } \
    type name

class Function : public QObject
{
    Q_OBJECT
public:
    using cType = double;

    Function() {}

    cType operator() (const cType& x, const cType& fix, Coefficient type) {
        if (x - getValue<Coefficient::Gamma>(fix, type) == 0) return 0;
        return getValue<Coefficient::Alpha>(fix, type)*
                sin(tan(getValue<Coefficient::Beta>(fix, type)/(x - getValue<Coefficient::Gamma>(fix, type)))) +
                getValue<Coefficient::Delta>(fix, type)*cos(getValue<Coefficient::Epsilon>(fix, type)*x);
    }

    void setAlpha  (const cType &value) { alpha   = value; emit changed(); };
    void setBeta   (const cType &value) { beta    = value; emit changed(); };
    void setGamma  (const cType &value) { gamma   = value; emit changed(); };
    void setDelta  (const cType &value) { delta   = value; emit changed(); };
    void setEpsilon(const cType &value) { epsilon = value; emit changed(); };

signals:
    void changed();
private:
    template<Coefficient C>
    cType getValue(const cType& fix, Coefficient type) {
        if (C == type) return fix;
        return getCoefficient<C>();
    }

    _coefs(cType, Coefficient);

    _coef(cType, Coefficient::Alpha,   alpha)   = 1.0;
    _coef(cType, Coefficient::Beta,    beta)    = 1.0;
    _coef(cType, Coefficient::Gamma,   gamma)   = 1.0;
    _coef(cType, Coefficient::Delta,   delta)   = 1.0;
    _coef(cType, Coefficient::Epsilon, epsilon) = 1.0;
};

#endif // FUNCTION_H
