#ifndef INTERSECTOR_H
#define INTERSECTOR_H

#include <map>
#include <set>
#include "primitives.h"

class LessPoint
{
public:
    bool operator()( Point const &lhs, Point const &rhs )
    {
        if (lhs.x + Segment::tolerance < rhs.x)
            return true;
        if (lhs.x > rhs.x + Segment::tolerance)
            return false;
        return lhs.y < rhs.y;
    }
};

class LessSegment
{
public:
    bool operator()( Segment const &lhs, Segment const &rhs );
};

class Intersector
{
public:
    using EventMap = std::map<Point, std::vector<Segment>, LessPoint>;
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
    std::map<Point, std::vector<Segment>, LessPoint>
        getSegmentByLeftEnd( std::vector<Segment> const &segments ) const;

    /*!
     * \brief Get segments list by their right point.
     * \param segments Segment list.
     * \return map: right point -> segment list.
     */
    std::map<Point, std::vector<Segment>, LessPoint>
        getSegmentByRightEnd( std::vector<Segment> const &segments ) const;

    /*!
     * \brief Process sweep line event function.
     * \param[IN] segments Segment array.
     * \param[IN] event Event.
     */
    void processEvent( std::vector<Segment> const &segments,
                       Point const &event );
private:
    EventMap
        //! leftSegMap map: pt -> segment.p0 == pt
        leftSegMap,
        //! rightSegMap map: pt -> segment.p1 == pt
        rightSegMap,
        //! crossSegMap map: pt -> seg1 x ... x segn == pt
        crossSegMap;
    std::set<Point> events;
    //! Sweep line status
    std::set<Segment, LessSegment> status;
    //! Set of intersections
    std::vector<Intersector::Intersection> result;
};

#endif // INTERSECTOR_H
