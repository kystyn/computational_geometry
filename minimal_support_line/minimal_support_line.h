#ifndef MINIMAL_SUPPORT_LINE_H
#define MINIMAL_SUPPORT_LINE_H

#include <list>
#include <vector>
#include <tuple>
#include "primitives.h"

class MinimalSupportLine
{
public:
    /*!
     * \brief Find minimal support line function.
     * \param points Point to build minimial support line to.
     * \param conv_hull Convex hull of point set.
     * \return
     */
    std::pair<int, int> findMinimalSupportLine(
            std::vector<Vector> const &points,
            std::list<Vector> const &conv_hull );

private:
    /*!
     * \brief Get canonical line parameters function.
     * \param p0 First point in line.
     * \param p1 Second point in line.
     * \return a, b, c: ax + by + c = 0.
     */
    std::tuple<double, double, double> getCanonicalLine( Vector const &p0, Vector const &p1 );

    /*!
     * \brief Find mass center function.
     * \return Mass center of point set.
     */
    Vector findMassCenter(const std::vector<Vector> &points) const;
};

#endif // MINIMAL_SUPPORT_LINE_H
