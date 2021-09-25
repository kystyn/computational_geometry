#include <cassert>
#include <algorithm>
#include <set>
#include "intersector.h"

std::vector<Intersector::Intersection>
    Intersector::computeIntersections( const std::vector<Segment> &segments )
{
    leftSegMap = getSegmentByLeftEnd(segments);     // pt -> segment.p0 == pt
    rightSegMap = getSegmentByRightEnd(segments);   // pt -> segment.p1 == pt

    for (auto &s: segments)
    {
        events.insert({s.p0(), s, Event::Type::LEFT_LOW});
        events.insert({s.p1(), s, Event::Type::RIGHT_UP});
    }

    leftSegMap.clear();
    rightSegMap.clear();
    crossSegMap.clear();
    result.clear();

    while (!events.empty())
    {
        Event event = *events.erase(events.begin());
        processEvent(event);
    }

    return result;
}

Intersector::EventMap Intersector::getSegmentByLeftEnd(const std::vector<Segment> &segments) const
{
    EventMap segmentsByPt;

    for (auto &s : segments)
    {
        Event event = {s.p0(), s, Event::Type::LEFT_LOW};
        if (segmentsByPt.find(event) == segmentsByPt.end())
        {
            segmentsByPt[event] = SegmentSet(event);
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
        Event event = {s.p1(), s, Event::Type::RIGHT_UP};
        if (segmentsByPt.find(event) == segmentsByPt.end())
        {
            segmentsByPt[event] = SegmentSet(event);
            segmentsByPt[event].insert(s);
        }
    }

    return segmentsByPt;
}

void Intersector::processEvent( Event const &event )
{
    auto
            leftSegments = leftSegMap.at(event),
            rightSegments = rightSegMap.at(event),
            crossSegments = crossSegMap.at(event);

    // if there are at least 2 segments that have left and right end correspondingly,
    // then event is a point where they are crossed
    if (!leftSegments.empty() && !rightSegments.empty())
        for (auto &l : leftSegments)
            for (auto &r : rightSegments)
                result.push_back({l.id(), r.id(), event.pt});

    if (crossSegments.size() >= 2)
        for (SegmentSet::iterator i = crossSegments.begin(); i != crossSegments.end(); ++i)
            for (auto j = i; j != crossSegments.end(); j++)
                result.push_back({i->id(), j->id(), event.pt});

    // change key. erase all segments and insert required again
    status.clear();
    status = std::set<Segment, LessSegment>(LessSegment(event));

    // status := crossSegments + leftSegments
    for (auto &s : crossSegments)
        status.insert(s);
    for (auto &s : leftSegments)
        status.insert(s);

    if (leftSegments.size() + crossSegments.size() == 0)
//        || crossSegments.size() == rightSegments.size() - 1)
    {
        auto s_it = status.find(event.segment), s_it_copy = s_it;
        auto
                s_it_up = ++s_it,
                s_it_low = --s_it_copy;

        // if both up and low neighbour exist
        if (s_it_up != status.end() && s_it_low != status.end())
        {
            // add intersection event if intersection exists
            bool has_intersect;
            auto intPt = s_it_up->intersect(*s_it_low, has_intersect);
            if (has_intersect)
            {
                Event event = {intPt, *s_it_up, Event::Type::CROSS};
                events.insert(event);
                crossSegMap[event].insert(*s_it_up);
                crossSegMap[event].insert(*s_it_low);
            }
        }
    }
    else
    {
        // s'
        auto up_left_seg_it = leftSegments.rbegin();
        auto up_cross_seg_it = crossSegments.rbegin();
        SegmentSet::reverse_iterator up_seg_it;

        if (up_left_seg_it != leftSegments.rend())
        {
            if (up_cross_seg_it != crossSegments.rend())
                up_seg_it = std::max(up_left_seg_it, up_cross_seg_it,
                                  [event]( SegmentSet::reverse_iterator lhs,
                                                                     SegmentSet::reverse_iterator rhs )
                                  {
                                      LessSegment ls(event);
                                      return ls(*lhs, *rhs);
                                  });
            else
                up_seg_it = up_left_seg_it;
        }
        else
        {
            assert(up_cross_seg_it != crossSegments.rend());
            up_seg_it = up_cross_seg_it;
        }

        // find neighbour and check intersection
        if (up_seg_it != status.rend())
        {
            auto up_seg_neighb_it = up_seg_it;
            --up_seg_neighb_it;
            if (up_seg_neighb_it != status.rend())
            {
                // add intersection event if intersection exists
                bool has_intersect;
                auto intPt = up_seg_it->intersect(*up_seg_neighb_it, has_intersect);
                if (has_intersect)
                {
                    Event event = {intPt, *up_seg_it, Event::Type::CROSS};
                    events.insert(event);
                    crossSegMap[event].insert(*up_seg_it);
                    crossSegMap[event].insert(*up_seg_neighb_it);
                }
            }
        }

        // s''
        auto low_left_seg_it = leftSegments.begin();
        auto low_cross_seg_it = crossSegments.begin();
        SegmentSet::iterator low_seg_it;

        if (low_left_seg_it != leftSegments.end())
        {
            if (low_cross_seg_it != crossSegments.end())
                low_seg_it = std::min(low_left_seg_it, low_cross_seg_it,
                                    [event]( SegmentSet::iterator lhs, SegmentSet::iterator rhs )
                                    {
                                        LessSegment ls(event);
                                        return ls(*lhs, *rhs);
                                    });
            else
                low_seg_it = low_left_seg_it;
        }
        else
        {
            assert(low_cross_seg_it != crossSegments.end());
            low_seg_it = low_cross_seg_it;
        }

        // find neighbour and check intersection
        if (low_seg_it != status.end())
        {
            auto low_seg_neighb_it = low_seg_it;
            --low_seg_neighb_it;
            if (low_seg_neighb_it != status.end())
            {
                // add intersection event if intersection exists
                bool has_intersect;
                auto intPt = low_seg_it->intersect(*low_seg_neighb_it, has_intersect);
                if (has_intersect)
                {
                    Event event = {intPt, *low_seg_it, Event::Type::CROSS};
                    events.insert(event);
                    crossSegMap[event].insert(*low_seg_it);
                    crossSegMap[event].insert(*low_seg_neighb_it);
                }
            }
        }
    }
}

LessSegment::LessSegment( Event const &event ) : event(event) {}

/*LessSegment & LessSegment::operator=( LessSegment &&ls )
{
    event = ls.event;
}*/

bool LessSegment::operator()(const Segment &lhs, const Segment &rhs)
{
    if (lhs.orientation() == Segment::Orientation::HORIZONTAL &&
        rhs.orientation() == Segment::Orientation::HORIZONTAL)
        return lhs.p0().y < rhs.p0().y;

    /* actually current algorithm allows only one-point intersection.
     * intersection overlap is not allowed
     */
    if (lhs.orientation() == Segment::Orientation::VERTICAL &&
        rhs.orientation() == Segment::Orientation::VERTICAL)
        return lhs.p0().y < rhs.p0().y;

    Segment hor, vert;
    if (lhs.orientation() == Segment::Orientation::HORIZONTAL &&
        rhs.orientation() == Segment::Orientation::VERTICAL)
    {
        hor = lhs;
        vert = rhs;
    }
    else
    {
        hor = rhs;
        vert = lhs;
    }

    bool has_intersect;
    auto pt = hor.intersect(vert, has_intersect);
    // must be intersection
    assert(has_intersect);

    if (event.type == Event::Type::LEFT_LOW)
        return vert.p0().y < pt.y;
    if (event.type == Event::Type::RIGHT_UP)
        return pt.y < vert.p1().y;
    // TODO event.type == Event::Type::CROSS
}
