#ifndef INTERSECTOR_H
#define INTERSECTOR_H

#include <map>
#include <set>
#include "primitives.h"

class LessSegment
{
public:
    LessSegment() {}
    LessSegment( Point const &event );

    bool operator()( Segment const &lhs, Segment const &rhs );
private:
    Point event;
};

using Event = Point;

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

/*!
 * \brief Intersection set wrapper with unique insert.
 */
class IntersectionSet
{
public:
    /*!
     * \brief Insert into set function.
     * \param inter intersection to insert.
     */
    void insert( Intersection const &inter );

    /*!
     * \brief Clear function.
     */
    void clear();

    /*!
     * \brief Get set function.
     * \return
     */
    std::set<Intersection, LessIntersection> const & get() const;
private:
    std::set<Intersection, LessIntersection> intersection_set;
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
    IntersectionSet computeIntersections( std::vector<Segment> const &segments );
private:
    /*!
     * \brief Find intersection right to sweep line function.
     * \param lhs First segment.
     * \param rhs Second segment.
     * \param event Event that marks sweep line position.
     * \param has_intersect True if intersection exists, false otherwise.
     * \return Point of intersection.
     */
    Point findIntersection( Segment const &lhs,
                            Segment const &rhs,
                            Event const &event,
                            bool &has_intersect ) const;
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
     * \brief Calculate intersections at current event function.
     * \param leftSegments Segments seg: seg.p0 == event.pt.
     * \param rightSegments Segments seg: seg.p1 == event.pt.
     * \param crossSegments Segments seg: event.pt in seg.
     * \param event Current event.
     */
    void calculateCurrentIntersections( SegmentSet &leftSegments,
            SegmentSet &rightSegments, SegmentSet &crossSegments,
            Event const &event );

    /*!
     * \brief Fill status function.
     * \param leftSegments Segments seg: seg.p0 == event.pt.
     * \param crossSegments Segments seg: event.pt in seg.
     * \param event Current event.
     */
    void fillStatus( SegmentSet &leftSegments, SegmentSet &crossSegments,
                     SegmentSet &rightSegments, Event const &event );

    /*!
     * \brief process left and intersection points function.
     * \param leftSegments Segments seg: seg.p0 == event.pt.
     * \param crossSegments Segments seg: event.pt in seg.
     * \param event Current event.
     */
    void processLeftCrossPoints(
            SegmentSet &leftSegments, SegmentSet &crossSegments, Event const &event );

    EventMap
        //! leftSegMap map: pt -> segment.p0 == pt
        leftSegMap,
        //! rightSegMap map: pt -> segment.p1 == pt
        rightSegMap,
        //! crossSegMap map: pt -> seg1 x ... x segn == pt
        crossSegMap,
        //! eventSegments map: event -> seg1 .. segn : event in seg1 .. segn
        eventSegments;
    EventSet events;
    //! Sweep line status
    SegmentSet status;
    //! Set of intersections
    IntersectionSet result;
};

#endif // INTERSECTOR_H
