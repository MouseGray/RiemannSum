#ifndef PIXELS_H
#define PIXELS_H


struct pixel
{
    double value;

    constexpr pixel(unsigned long long num) :
        value(static_cast<int>(num)) {}

    pixel(double num) : value(num) {}

    pixel(int num) : value(num) {}

    pixel& operator++() {
        value++;
        return *this;
    }

    pixel operator++(int) {
        return ++value;
    }

    pixel& operator--() {
        value--;
        return *this;
    }

    pixel operator--(int) {
        return --value;
    }

    pixel operator-() {
        return -value;
    }

    pixel operator-(pixel right) {
        return value - right.value;
    }

    pixel operator+(pixel right) {
        return value + right.value;
    }

    pixel operator*(double right) {
        return value*right;
    }

    double operator/(pixel right) {
        return value/static_cast<double>(right.value);
    }

    bool operator<(pixel right) {
        return value < right.value;
    }

    bool operator>(pixel right) {
        return value > right.value;
    }

    bool operator<=(pixel right) {
        return value <= right.value;
    }

    bool operator>=(pixel right) {
        return value >= right.value;
    }

    bool operator==(pixel right) {
        return value == right.value;
    }
};

constexpr pixel operator "" _px(unsigned long long num){
    return pixel(num);
}

#endif // PIXELS_H
