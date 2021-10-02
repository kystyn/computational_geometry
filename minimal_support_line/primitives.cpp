#include <cmath>
#include <tuple>
#include "primitives.h"

const double Vector::tolerance = 1e-5f;

std::istream &operator>>(std::istream &is, Vector &pt)
{
    int id;
    double x, y;
    is >> id >> x >> y;

    pt = Vector(x, y, id);
    return is;
}

std::ostream & operator<<(std::ostream &os, Vector const &pt)
{
    os << pt.x() << ' ' << pt.y();
    return os;
}

Vector::Vector() : _x(0), _y(0) {}

double Vector::x() const
{
    return _x;
}

double Vector::y() const
{
    return _y;
}

int Vector::id() const
{
    return _id;
}

double Vector::len2() const
{
    return dotProd(*this);
}

Vector::Vector(double x, double y, int _id) : _id(_id), _x(x), _y(y)
{}

Vector &Vector::operator+=(const Vector &rhs)
{
    _x += rhs._x;
    _y += rhs._y;

    return *this;
}

Vector & Vector::operator=(const Vector &rhs)
{
    _id = rhs._id;
    _x = rhs._x;
    _y = rhs._y;
    return *this;
}

Vector Vector::operator-(const Vector &rhs) const
{
    return Vector(_x - rhs._x, _y - rhs._y);
}

Vector Vector::operator/(double num) const
{
    return Vector(_x / num, _y / num);
}

double Vector::distToLine(const std::tuple<double, double, double> &line) const
{
    double
            a = std::get<0>(line),
            b = std::get<1>(line),
            c = std::get<2>(line);
    return std::abs(a * _x + b * _y + c) / std::sqrt(a * a + b * b);
}

double Vector::crossProd(const Vector &rhs) const
{
    return _x * rhs._y - rhs._x * _y;
}

double Vector::dotProd(const Vector &rhs) const
{
    return _x * rhs._x + _y * rhs._y;
}

bool Vector::operator==(const Vector &rhs) const
{
    return
            (rhs._x - _x) * (rhs._x - _x) +
            (rhs._y - _y) * (rhs._y - _y) < tolerance;
}

bool Vector::operator<(const Vector &rhs) const
{
    if (_x + tolerance < rhs._x)
        return true;
    if (_x > rhs._x + tolerance)
        return false;
    return _y < rhs._y;
}

bool Vector::operator<=(const Vector &rhs) const
{
    return *this < rhs || *this == rhs;
}
