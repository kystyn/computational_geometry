#include <cassert>
#include <algorithm>
#include <set>
#include <iostream>
#include <cmath>
#include "intersector.h"

IntersectionSet Intersector::computeIntersections(
        const std::vector<Segment> &segments )
{
    result.clear();

    for (auto &s: segments)
    {
        events.insert({s.p0(), s, Event::EndType::LEFT_LOW});
        // we need only one event corresponding to vertical segment
        if (s.orientation() == Segment::Orientation::HORIZONTAL)
            events.insert({s.p1(), s, Event::EndType::RIGHT_UP});
    }

    while (!events.empty())
    {
        Event event = *events.begin();
        events.erase(event);
        processEvent(event);
    }

    return result;
}

Intersector::EventMap Intersector::getSegmentByLeftEnd(const std::vector<Segment> &segments) const
{
    EventMap segmentsByPt;

    for (auto &s : segments)
    {
        if (s.orientation() == Segment::Orientation::HORIZONTAL)
        {
            Event event = {s.p0(), s, Event::EndType::LEFT_LOW};
            segmentsByPt[event].insert(s);
        }
    }

    return segmentsByPt;
}

Intersector::EventMap Intersector::getSegmentByRightEnd(const std::vector<Segment> &segments) const
{
    EventMap segmentsByPt;

    for (auto &s : segments)
    {
        if (s.orientation() == Segment::Orientation::HORIZONTAL)
        {
            Event event = {s.p1(), s, Event::EndType::RIGHT_UP};
            segmentsByPt[event].insert(s);
        }
    }

    return segmentsByPt;
}

void Intersector::fillStatus( Event const &event )
{
    if (event.type == Event::EndType::LEFT_LOW &&
        event.segment.orientation() == Segment::Orientation::HORIZONTAL)
        status.insert(event.segment);
    else if (event.type == Event::EndType::RIGHT_UP)
        status.erase(event.segment);
}


void Intersector::processEvent( Event const &event )
{
    fillStatus(event);

    // find intersection
    if (event.segment.orientation() == Segment::Orientation::VERTICAL)
    {
        auto pt1 = event.segment.p1();
        Point pt2 = {pt1.x + 1, pt1.y};
        Segment hlp_up(pt1, pt2, 0);

        pt1 = event.segment.p0();
        pt2 = {pt1.x + 1, pt1.y};
        Segment hlp_low(pt1, pt2, 0);

        // find all horizontal segments that intersect vertical...
        auto
                low_seg_it = status.lower_bound(hlp_low),
                up_seg_it = status.upper_bound(hlp_up);

        // TODO ??? ++up_seg_it
        for (auto it = low_seg_it; it != status.end() && it != up_seg_it; it++)
        {
            bool has_intersect;
            auto intPt = event.segment.intersect(*it, has_intersect);
            assert(has_intersect);
            result.insert({event.segment.id(), it->id(), intPt});
        }

        // ... and find vertical segments that have common end point with current TODO
    }
}

LessSegment::LessSegment( Event const &event ) : event(event) {}

bool LessSegment::operator()(const Segment &lhs, const Segment &rhs)
{
    assert(lhs.orientation() == Segment::Orientation::HORIZONTAL);
    assert(rhs.orientation() == Segment::Orientation::HORIZONTAL);
    return lhs.p0().y < rhs.p0().y;
}

bool LessEvent::operator()(const Event &lhs, const Event &rhs)
{
    if (std::fabs(lhs.pt.x - rhs.pt.x) < Segment::tolerance)
    {
        if (lhs.segment.orientation() == Segment::Orientation::HORIZONTAL &&
            rhs.segment.orientation() == Segment::Orientation::VERTICAL)
            return lhs.type == Event::EndType::LEFT_LOW;
        if (lhs.segment.orientation() == Segment::Orientation::VERTICAL &&
            rhs.segment.orientation() == Segment::Orientation::HORIZONTAL)
            return rhs.type == Event::EndType::RIGHT_UP;
    }

    if (lhs.pt == rhs.pt &&
            lhs.segment.orientation() == Segment::Orientation::HORIZONTAL &&
            rhs.segment.orientation() == Segment::Orientation::HORIZONTAL)
    {
        if (!(lhs.segment == rhs.segment))
        /* configuration -- */
            return lhs.type == Event::EndType::RIGHT_UP;
        return false;
    }
    return lhs.pt < rhs.pt;
}

bool LessIntersection::operator()(const Intersection &lhs, const Intersection &rhs) const
{
    // the only thing we need is to detect equal intersections
    if (lhs.id1 == rhs.id2 && lhs.id2 == rhs.id1)
        return false;
    // Just to make sure that no intersections will be lost
    return Point(lhs.id1, lhs.id2) < Point(rhs.id1, rhs.id2);
}

void IntersectionSet::insert(const Intersection &inter)
{
    auto inter2 = inter;
    inter2.id1 = inter.id2;
    inter2.id2 = inter.id1;

    if (intersection_set.find(inter) == intersection_set.end() &&
        intersection_set.find(inter2) == intersection_set.end())
        intersection_set.insert(inter);
}

void IntersectionSet::clear()
{
    intersection_set.clear();
}

std::set<Intersection, LessIntersection> const & IntersectionSet::get() const
{
    return intersection_set;
}
