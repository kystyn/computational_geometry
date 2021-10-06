#include <limits>
#include "minimal_support_line.h"

std::pair<int, int> MinimalSupportLine::findMinimalSupportLine(
        const std::vector<Vector> &points,
        const std::list<Vector> &conv_hull)
{
    auto massCenter = findMassCenter(points);

    int opt_id1, opt_id2;
    double opt_dist = std::numeric_limits<double>::max();

    int i = 0;
    for (auto it = conv_hull.begin(); it != conv_hull.end(); ++it, i++)
    {
        auto it1 = it;
        auto it2 = it;
        ++it2;
        it2 = it2 != conv_hull.end() ? it2 : conv_hull.begin();
        auto line = getCanonicalLine(*it1, *it2);
        auto dist = massCenter.distToLine(line);

        if (dist < opt_dist)
        {
            opt_id1 = it1->id();
            opt_id2 = it2->id();
            opt_dist = dist;
        }
    }
    return {opt_id1, opt_id2};
}

std::tuple<double, double, double>
MinimalSupportLine::getCanonicalLine( Vector const &p0, Vector const &p1 )
{
    return std::make_tuple(
                p1.y() - p0.y(),
                p0.x() - p1.x(),
                p1.x() * p0.y() - p1.y() * p0.x());
}

Vector MinimalSupportLine::findMassCenter(
        const std::vector<Vector> &points ) const
{
    Vector res;

    for (auto &pt : points)
        res += pt;

    return res / points.size();
}
