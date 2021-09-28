#include <cassert>
#include <algorithm>
#include <set>
#include "intersector.h"

std::vector<Intersection>
    Intersector::computeIntersections( const std::vector<Segment> &segments )
{
    leftSegMap.clear();
    rightSegMap.clear();
    crossSegMap.clear();
    result.clear();

    leftSegMap = getSegmentByLeftEnd(segments);     // pt -> segment.p0 == pt
    rightSegMap = getSegmentByRightEnd(segments);   // pt -> segment.p1 == pt

    for (auto &s: segments)
    {
        events.insert(s.p0());
        eventSegments[s.p0()].insert(s);

        events.insert(s.p1());
        eventSegments[s.p1()].insert(s);
    }

    while (!events.empty())
    {
        Event event = *events.begin();
        events.erase(event);
        processEvent(event);
    }

    return result;
}

Point Intersector::findIntersection(
        const Segment &lhs, const Segment &rhs,
        const Event &event, bool &has_intersect) const
{
    auto pt = lhs.intersect(rhs, has_intersect);
    if (has_intersect)
        has_intersect = event <= pt;
    return pt;
}

Intersector::EventMap Intersector::getSegmentByLeftEnd(const std::vector<Segment> &segments) const
{
    EventMap segmentsByPt;

    for (auto &s : segments)
    {
        Event event = {s.p0()};//, Event::Type::LEFT_LOW};
        segmentsByPt[event].insert(s);
    }

    return segmentsByPt;
}

Intersector::EventMap Intersector::getSegmentByRightEnd(const std::vector<Segment> &segments) const
{
    EventMap segmentsByPt;

    for (auto &s : segments)
    {
        Event event = {s.p1()};//, Event::Type::RIGHT_UP};
        segmentsByPt[event].insert(s);
    }

    return segmentsByPt;
}

void Intersector::calculateCurrentIntersections(
        SegmentSet &leftSegments,
        SegmentSet &rightSegments,
        SegmentSet &crossSegments,
        Event const &event )
{
    // if there are at least 2 segments that have left and right end correspondingly,
    // then event is a point where they are crossed
    for (auto l_it = leftSegments.begin();
         l_it != leftSegments.end(); ++l_it)
    {
        // maybe there are left x left intersections
        auto l_it_j = l_it;
        for (++l_it_j; l_it_j != leftSegments.end(); l_it_j++)
        {
            bool has_intersect;
            l_it->intersect(*l_it_j, has_intersect);
            if (has_intersect)
                result.push_back({l_it->id(), l_it_j->id(), event});
        }

        // left x right
        for (auto &r : rightSegments)
            result.push_back({l_it->id(), r.id(), event});
    }

    // right x right
    for (auto r_it = rightSegments.begin();
         r_it != rightSegments.end(); ++r_it)
    {
        auto r_it_j = r_it;
        for (++r_it_j;
             r_it_j != rightSegments.end(); ++r_it_j)
        {
            bool has_intersect;
            r_it->intersect(*r_it_j, has_intersect);
            if (has_intersect)
                result.push_back({r_it->id(), r_it_j->id(), event});
        }
    }

    if (crossSegments.size() >= 2)
        for (SegmentSet::iterator i = crossSegments.begin(); i != crossSegments.end(); ++i)
        {
            auto j = i;
            for (++j; j != crossSegments.end(); j++)
                result.push_back({i->id(), j->id(), event});
        }
}

void Intersector::fillStatus(
        SegmentSet &leftSegments, SegmentSet &crossSegments,
        SegmentSet &rightSegments,
        Event const &event )
{
    // change key. erase all segments and insert required again
    for (auto &s : crossSegments)
        status.erase(s);
    for (auto &s : rightSegments)
        status.erase(s);

    auto saveStatus = status;

    status = std::set<Segment, LessSegment>(LessSegment(event));

    // status := crossSegments + leftSegments + saved
    for (auto &s : crossSegments)
        status.insert(s);
    for (auto &s : leftSegments)
        status.insert(s);
    for (auto &s : saveStatus)
        status.insert(s);
}

void Intersector::processRightPoints( Event const &event )
{
    auto
            s_it_up = status.find(*eventSegments.find(event)->second.rbegin()),
            s_it_low = status.find(*eventSegments.find(event)->second.begin());
    if (s_it_up == status.end() || s_it_low == status.end())
        return;

    ++s_it_up;
    --s_it_low;

    // if both up and low neighbour exist
    if (s_it_up != status.end() && s_it_low != status.end())
    {
        // add intersection event if intersection exists
        bool has_intersect;
        auto intPt = findIntersection(*s_it_up, *s_it_low, event, has_intersect);
        if (has_intersect)
        {
            Event cross_event = {intPt};//, Event::Type::CROSS};
            events.insert(cross_event);
            if (crossSegMap.find(cross_event) == crossSegMap.end())
                crossSegMap[cross_event] = SegmentSet(cross_event);
            crossSegMap[cross_event].insert(*s_it_up);
            crossSegMap[cross_event].insert(*s_it_low);
        }
    }
}

void Intersector::processLeftCrossPoints(
        SegmentSet &leftSegments, SegmentSet &crossSegments, Event const &event )
{
    // s': choose the leftist segment
    auto up_left_seg_it = leftSegments.rbegin();
    auto up_cross_seg_it = crossSegments.rbegin();
    SegmentSet::iterator up_seg_it;

    if (up_left_seg_it != leftSegments.rend())
    {
        if (up_cross_seg_it != crossSegments.rend())
            up_seg_it = status.find(*std::max(up_left_seg_it, up_cross_seg_it,
                              [&event]( SegmentSet::reverse_iterator lhs,
                                  SegmentSet::reverse_iterator rhs )
                              {
                                  LessSegment ls(event);
                                  return ls(*lhs, *rhs);
                              }));
        else
            up_seg_it = status.find(*up_left_seg_it);
    }
    else
    {
        assert(up_cross_seg_it != crossSegments.rend());
        up_seg_it = status.find(*up_cross_seg_it);
    }

    // find neighbour and check intersection
    if (up_seg_it != status.end())
    {
        auto up_seg_neighb_it = up_seg_it;
        ++up_seg_neighb_it;

        if (up_seg_neighb_it != status.end())
        {
            // add intersection event if intersection exists
            bool has_intersect;
            auto intPt = findIntersection(*up_seg_it, *up_seg_neighb_it, event, has_intersect);
            if (has_intersect)
            {
                Event cross_event = {intPt};//, Event::Type::CROSS};
                events.insert(cross_event);
                if (crossSegMap.find(cross_event) == crossSegMap.end())
                    crossSegMap[cross_event] = SegmentSet(cross_event);
                crossSegMap[cross_event].insert(*up_seg_it);
                crossSegMap[cross_event].insert(*up_seg_neighb_it);
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
            low_seg_it = status.find(*std::min(low_left_seg_it, low_cross_seg_it,
                                [&event]( SegmentSet::iterator lhs, SegmentSet::iterator rhs )
                                {
                                    LessSegment ls(event);
                                    return ls(*lhs, *rhs);
                                }));
        else
            low_seg_it = status.find(*low_left_seg_it);
    }
    else
    {
        assert(low_cross_seg_it != crossSegments.end());
        low_seg_it = status.find(*low_cross_seg_it);
    }

    // find neighbour and check intersection
    if (low_seg_it != status.end())
    {
        if (low_seg_it != status.begin())
        {
            auto low_seg_neighb_it = low_seg_it;
            --low_seg_neighb_it;

            // add intersection event if intersection exists
            bool has_intersect;
            auto intPt = findIntersection(*low_seg_it, *low_seg_neighb_it, event, has_intersect);
            if (has_intersect)
            {
                Event cross_event = {intPt};//, Event::Type::CROSS};
                events.insert(cross_event);
                crossSegMap[cross_event].insert(*low_seg_it);
                crossSegMap[cross_event].insert(*low_seg_neighb_it);
            }
        }
    }
}

void Intersector::processEvent( Event const &event )
{
    auto
            leftSegments_it = leftSegMap.find(event),
            rightSegments_it = rightSegMap.find(event),
            crossSegments_it = crossSegMap.find(event);

    auto leftSegments = leftSegments_it != leftSegMap.end() ? leftSegments_it->second : SegmentSet(event);
    auto rightSegments = rightSegments_it != rightSegMap.end() ? rightSegments_it->second : SegmentSet(event);
    auto crossSegments_saved = crossSegments_it != crossSegMap.end() ? crossSegments_it->second : SegmentSet(event);

    // refill with new key
    // leftSegments and rightSegments do not need such procedure because
    // their order does not depend on event
    SegmentSet crossSegments = SegmentSet(event);
    for (auto &s : crossSegments_saved)
        crossSegments.insert(s);

    calculateCurrentIntersections(leftSegments, rightSegments, crossSegments, event);
    fillStatus(leftSegments, crossSegments, rightSegments, event);

    if (leftSegments.size() + crossSegments.size() == 0)
        processRightPoints(event);
    else
        processLeftCrossPoints(leftSegments, crossSegments, event);
}

LessSegment::LessSegment( Event const &event ) : event(event) {}

bool LessSegment::operator()(const Segment &lhs, const Segment &rhs)
{
    /* actually current algorithm allows only one-point intersection.
     * intersection overlap is not allowed
     */
    if (lhs.orientation() == Segment::Orientation::HORIZONTAL &&
        rhs.orientation() == Segment::Orientation::HORIZONTAL)
        return lhs.p0().y < rhs.p0().y;

    if (lhs.orientation() == Segment::Orientation::VERTICAL &&
        rhs.orientation() == Segment::Orientation::VERTICAL)
        return lhs.p0().y < rhs.p0().y;

    Segment hor, vert;
    bool is_lhs_vert;
    if (lhs.orientation() == Segment::Orientation::HORIZONTAL &&
        rhs.orientation() == Segment::Orientation::VERTICAL)
    {
        is_lhs_vert = false;
        hor = lhs;
        vert = rhs;
    }
    else
    {
        is_lhs_vert = true;
        hor = rhs;
        vert = lhs;
    }

    bool has_intersect;
    auto pt = hor.intersect(vert, has_intersect);
    if (!has_intersect)
        return lhs.p0().y < rhs.p0().y;

    if (event < pt)
        // vert < hor
        return is_lhs_vert;

    if (pt < event)
        // vert > hor
        return !is_lhs_vert;

    // pt == event
    return !is_lhs_vert;// <=> lhs.orientation() == Segment::Orientation::HORIZONTAL;
}

bool LessEvent::operator()(const Event &lhs, const Event &rhs)
{
    return lhs < rhs;
}
