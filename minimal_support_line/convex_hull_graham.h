#ifndef CONVEXHULLGRAHAM_H
#define CONVEXHULLGRAHAM_H

#include <vector>
#include <list>
#include "primitives.h"

class ConvexHullGraham
{
public:
    /*!
     * \brief Class constructor.
     * \param points Points to build convex hull over.
     */
    ConvexHullGraham( std::vector<Vector> const &points );

    /*!
     * \brief Build convex hull function.
     * \return Ordered points of convex hull.
     */
    std::list<Vector> buildConvexHull();
private:
    static bool isLeftTurn( Vector const &p1, Vector const &p2, Vector const &p3);

    std::vector<Vector> points;
};

#endif // CONVEXHULLGRAHAM_H
