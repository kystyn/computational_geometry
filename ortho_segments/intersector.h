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

std::ostream & operator<<( std::ostream &os, Intersection const& inter );

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

    /*!
     * \brief Compute intersections function.
     * \param segments Segment list.
     * \param os output stream.
     */
    void computeIntersections( std::vector<Segment> const &segments, std::ostream *os );
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

    std::vector<Event> events;
    //! Sweep line status
    SegmentSet status;

    std::ostream *os;
};

#endif // INTERSECTOR_H
