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

Point Segment::intersect( Segment const &other, bool &has_intersect ) const
{
    if (orient == Orientation::HORIZONTAL &&
        other.orient == Orientation::HORIZONTAL)
        return intersect_hor_hor(other, has_intersect);
    if (orient == Orientation::VERTICAL &&
        other.orient == Orientation::VERTICAL)
        return intersect_ver_ver(other, has_intersect);
    return intersect_hor_ver(other, has_intersect);
}

Segment::Orientation Segment::orientation() const
{
    return orient;
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

Point Segment::intersect_hor_hor( const Segment &other, bool &has_intersect ) const
{
    if (_p1.x >= other._p0.x)
    {
        // any point on common part of collinear edges
        has_intersect = true;
        return _p1;
    }
    else if (other._p1.x >= _p0.x)
    {
        has_intersect = true;
        return _p0;
    }
    else
    {
        has_intersect = false;
        return {};
    }
}

Point Segment::intersect_ver_ver(const Segment &other, bool &has_intersect) const
{
    if (_p1.y >= other._p0.y)
    {
        // any point on common part of collinear edges
        has_intersect = true;
        return _p1;
    }
    else if (other._p1.y >= _p0.y)
    {
        has_intersect = true;
        return _p0;
    }
    else
    {
        has_intersect = false;
        return {};
    }
}

Point Segment::intersect_hor_ver(const Segment &other, bool &has_intersect) const
{
    if (orient == Orientation::VERTICAL &&
                 other.orient == Orientation::HORIZONTAL)
    {
        if (other._p0.x <= _p0.x && _p0.x <= other._p1.x &&
            _p0.y <= other._p0.y && other._p0.y <= _p1.y)
        {
            has_intersect = true;
            return {_p0.x, other._p0.y};
        }
        else
        {
            has_intersect = false;
            return {};
        }
    }
    else
    {
        if (_p0.x <= other._p0.x && other._p0.x <= _p1.x &&
            other._p0.y <= _p0.y && _p0.y <= other._p1.y)
        {
            has_intersect = true;
            return  {other._p0.x, _p0.y};
        }
        else
        {
            has_intersect = false;
            return {};
        }
    }
}

std::istream &operator>>(std::istream &is, Point &pt)
{
    is >> pt.x >> pt.y;
    return is;
}

std::ostream & operator<<(std::ostream &os, Point const &pt)
{
    os << pt.x << ' ' << pt. y;
    return os;
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
