#ifndef INTERSECTOR_H
#define INTERSECTOR_H

#include <map>
#include <set>
#include "primitives.h"

struct Event
{
    Point pt;
    Segment segment;
    enum class Type
    {
        LEFT_LOW, // low or left point (p0)
        RIGHT_UP, // up or right point (p1)
        CROSS     // cross point
    } type;
};

class LessEvent
{
public:
    bool operator()( Event const &lhs, Event const &rhs )
    {
        if (lhs.pt.x + Segment::tolerance < rhs.pt.x)
            return true;
        if (lhs.pt.x > rhs.pt.x + Segment::tolerance)
            return false;
        return lhs.pt.y < rhs.pt.y;
    }
};

class LessSegment
{
public:
    LessSegment() {}
    LessSegment( Event const &event );

    bool operator()( Segment const &lhs, Segment const &rhs );
private:
    Event event;
};

/*!
 * \brief The Intersection struct
 */
struct Intersection
{
    //! identifiers of intersected segments
    int id1, id2;
    //! Intersection point
    Point intPt;
};

class Intersector
{
public:
    using SegmentSet = std::set<Segment, LessSegment>;
    using EventMap = std::map<Event, SegmentSet, LessEvent>;
    using EventSet = std::set<Event, LessEvent>;

    /*!
     * \brief Compute intersections function.
     * \param segments Segment list.
     * \return Intersections list.
     */
    std::vector<Intersection> computeIntersections( std::vector<Segment> const &segments );

private:
    /*!
     * \brief Get segments list by their left point.
     * \param segments Segment list.
     * \return map: left point -> segment list.
     */
    EventMap getSegmentByLeftEnd( std::vector<Segment> const &segments ) const;

    /*!
     * \brief Get segments list by their right point.
     * \param segments Segment list.
     * \return map: right point -> segment list.
     */
    EventMap getSegmentByRightEnd( std::vector<Segment> const &segments ) const;

    /*!
     * \brief Process sweep line event function.
     * \param[IN] event Event.
     */
    void processEvent( Event const &event );

    EventMap
        //! leftSegMap map: pt -> segment.p0 == pt
        leftSegMap,
        //! rightSegMap map: pt -> segment.p1 == pt
        rightSegMap,
        //! crossSegMap map: pt -> seg1 x ... x segn == pt
        crossSegMap;
    EventSet events;
    //! Sweep line status
    SegmentSet status;
    //! Set of intersections
    std::vector<Intersection> result;
};

#endif // INTERSECTOR_H
