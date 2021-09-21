#include "primitives.h"

const float Segment::tolerance = 1e-5f;

Segment::Segment(const Point &p0, const Point &p1, int id) : _id(id)
{
    /* p0 is the leftist and the lowest end */
    if (p0.x < p1.x)
    {
        this->_p0 = p0;
        this->_p1 = p1;
    }
    else if (p1.x < p0.x)
    {
        this->_p0 = p1;
        this->_p1 = p0;
    }
    else if (p0.y < p1.y)
    {
        this->_p0 = p0;
        this->_p1 = p1;
    }
    else
    {
        this->_p0 = p1;
        this->_p1 = p0;
    }

    orient =
            p0.x - p1.x < tolerance ? Orientation::HORIZONTAL :
            (p0.y - p1.y < tolerance ? Orientation::VERTICAL :
                                       Orientation::NONE);
}

Segment & Segment::operator=(const Segment &seg)
{
    this->_p0 = seg._p0;
    this->_p1 = seg._p1;
    return *this;
}

bool Segment::intersect(const Segment &other, Point &intPt) const
{
    if (orient == Orientation::HORIZONTAL &&
        other.orient == Orientation::HORIZONTAL)
    {
        if (_p1.x >= other._p0.x)
        {
            // any point on common part of collinear edges
            intPt = _p1;
            return true;
        }
        else if (other._p1.x >= _p0.x)
        {
            intPt = _p0;
            return true;
        }
        else
            return false;
    }
    else if (orient == Orientation::VERTICAL &&
             other.orient == Orientation::VERTICAL)
    {
        if (_p1.y >= other._p0.y)
        {
            // any point on common part of collinear edges
            intPt = _p1;
            return true;
        }
        else if (other._p1.y >= _p0.y)
        {
            intPt = _p0;
            return true;
        }
        else
            return false;
    }
    else if (orient == Orientation::VERTICAL &&
             other.orient == Orientation::HORIZONTAL)
    {
        if (other._p0.x <= _p0.x && _p0.x <= other._p1.x &&
            _p0.y <= other._p0.y && other._p0.y <= _p1.y)
        {
            intPt = {_p0.x, other._p0.y};
            return true;
        }
        else
            return false;
    }
    else
    {
        if (_p0.x <= other._p0.x && other._p0.x <= _p1.x &&
            other._p0.y <= _p0.y && _p0.y <= other._p1.y)
        {
            intPt = {other._p0.x, _p0.y};
            return true;
        }
        else
            return false;
    }
}

int Segment::id() const
{
    return _id;
}

Point Segment::p0() const
{
    return _p0;
}

Point Segment::p1() const
{
    return _p1;
}

std::istream &operator>>(std::istream &is, Point &pt)
{
    is >> pt.x >> pt.y;
    return is;
}

std::istream &operator>>(std::istream &is, Segment &seg)
{
    is >> seg._id >> seg._p0 >> seg._p1;
    return is;
}

bool Point::operator==(const Point &rhs)
{
    return
            (rhs.x - x) * (rhs.x - x) +
            (rhs.y - y) * (rhs.y - y) < Segment::tolerance;
}
