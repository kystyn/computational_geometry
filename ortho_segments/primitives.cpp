#include <iostream>
#include <fstream>
#include "primitives.h"

Segment::Segment(const Point &p0, const Point &p1)
{
    /* p0 is the leftist and the lowest end */
    if (p0.x < p1.x)
    {
        this->p0 = p0;
        this->p1 = p1;
    }
    else if (p1.x < p0.x)
    {
        this->p0 = p1;
        this->p1 = p0;
    }
    else if (p0.y < p1.y)
    {
        this->p0 = p0;
        this->p1 = p1;
    }
    else
    {
        this->p0 = p1;
        this->p1 = p0;
    }

    orient =
            p0.x - p1.x < tolerance ? Orientation::HORIZONTAL :
            (p0.y - p1.y < tolerance ? Orientation::VERTICAL :
                                       Orientation::NONE);
}

Segment & Segment::operator=(const Segment &seg)
{
    this->p0 = seg.p0;
    this->p1 = seg.p1;
    return *this;
}

bool Segment::intersect(const Segment &other, Point &intPt) const
{
    if (orient == Orientation::HORIZONTAL &&
        other.orient == Orientation::HORIZONTAL)
    {
        if (p1.x >= other.p0.x)
        {
            // any point on common part of collinear edges
            intPt = p1;
            return true;
        }
        else if (other.p1.x >= p0.x)
        {
            intPt = p0;
            return true;
        }
        else
            return false;
    }
    else if (orient == Orientation::VERTICAL &&
             other.orient == Orientation::VERTICAL)
    {
        if (p1.y >= other.p0.y)
        {
            // any point on common part of collinear edges
            intPt = p1;
            return true;
        }
        else if (other.p1.y >= p0.y)
        {
            intPt = p0;
            return true;
        }
        else
            return false;
    }
    else if (orient == Orientation::VERTICAL &&
             other.orient == Orientation::HORIZONTAL)
    {
        if (other.p0.x <= p0.x && p0.x <= other.p1.x &&
            p0.y <= other.p0.y && other.p0.y <= p1.y)
        {
            intPt = {p0.x, other.p0.y};
            return true;
        }
        else
            return false;
    }
    else
    {
        if (p0.x <= other.p0.x && other.p0.x <= p1.x &&
            other.p0.y <= p0.y && p0.y <= other.p1.y)
        {
            intPt = {other.p0.x, p0.y};
            return true;
        }
        else
            return false;
    }
}

std::istream &operator>>(std::istream &is, Point &pt)
{
    is >> pt.x >> pt.y;
    return is;
}

std::istream &operator>>(std::istream &is, Segment &seg)
{
    is >> seg.p0 >> seg.p1;
    return is;
}

std::map<int, Segment> SegmentLoader::loadFromFile(const std::string &fileName, bool *ok) const
{
    std::ifstream ifs(fileName);

    if (!ifs)
    {
        std::clog << "file " << fileName << " not found\n";
        if (ok)
            *ok = false;
        return {};
    }

    std::map<int, Segment> segments;

    while (ifs)
    {
        int id;
        Segment seg;
        if (!(ifs >> id))
            break;

        if (!(ifs >> seg))
        {
            std::clog << "wrong file format\n";
            if (ok)
                *ok = false;
            return segments;
        }

        if (segments.find(id) != segments.end())
        {
            std::clog << "identifier " << id << " met twice\n";
            if (ok)
                *ok = false;
        }
        else
            segments[id] = seg;
    }
    return segments;
}
