#include <set>
#include "intersector.h"

std::vector<Intersector::Intersection>
    Intersector::computeIntersections( const std::vector<Segment> &segments )
{
    std::set<Point, LessPoint> events;
    std::vector<Intersector::Intersection> result;

    std::map<Point, std::vector<Segment>, LessPoint>
            leftSegMap = getSegmentByLeftEnd(segments),     // pt -> segment.p0 == pt
            rightSegMap = getSegmentByRightEnd(segments),   // pt -> segment.p1 == pt
            crossSegMap;                                    // pt -> seg1 x ... x segn == pt

    for (auto &s: segments)
    {
        events.insert(s.p0());
        events.insert(s.p1());
    }

    leftSegMap.clear();
    rightSegMap.clear();
    crossSegMap.clear();
    result.clear();

    while (!events.empty())
    {
        Point event = *events.erase(events.begin());
        processEvent(segments, event);
    }
}

std::map<Point, std::vector<Segment>, LessPoint>
Intersector::getSegmentByLeftEnd(const std::vector<Segment> &segments) const
{
    std::map<Point, std::vector<Segment>, LessPoint> segmentsByPt;

    for (auto &s : segments)
        segmentsByPt[s.p0()].push_back(s);

    return segmentsByPt;
}

void Intersector::processEvent(
        const std::vector<Segment> &segments,
        Point const &event)
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
                result.push_back({l.id(), r.id(), event});

    if (crossSegments.size() >= 2)
        for (size_t i = 0, n = crossSegments.size(); i < n; i++)
            for (size_t j = i + 1; j < n; j++)
                result.push_back({crossSegments[i].id(), crossSegments[j].id(), event});

    for (auto &s: leftSegments)
        status.erase(s);
}

std::map<Point, std::vector<Segment>, LessPoint>
Intersector::getSegmentByRightEnd(const std::vector<Segment> &segments) const
{
    std::map<Point, std::vector<Segment>, LessPoint> segmentsByPt;

    for (auto &s : segments)
        segmentsByPt[s.p1()].push_back(s);

    return segmentsByPt;
}

bool LessSegment::operator()(const Segment &lhs, const Segment &rhs)
{
    return lhs.p0().y < rhs.p0().y;
}
