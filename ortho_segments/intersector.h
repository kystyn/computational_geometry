#ifndef INTERSECTOR_H
#define INTERSECTOR_H

#include <map>
#include <set>
#include "primitives.h"

struct Event
{
    Point pt;
    // pt is left or right end of segment
    Segment segment;
    enum class EndType
    {
        LEFT_LOW, RIGHT_UP
    } type;
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

class LessEvent
{
public:
    bool operator()( Event const &lhs, Event const &rhs );
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

class LessIntersection
{
public:
    bool operator()( Intersection const &lhs,
                     Intersection const &rhs ) const;
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

    /*!
     * \brief Fill status function.
     * \param event Current event.
     */
    void fillStatus( Event const &event );

    EventSet events;
    //! Sweep line status
    SegmentSet status;
    //! Set of intersections
    std::vector<Intersection> result;
};

#endif // INTERSECTOR_H
