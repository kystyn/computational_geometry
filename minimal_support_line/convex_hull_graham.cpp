#include <cassert>
#include <algorithm>
#include <cmath>
#include "convex_hull_graham.h"

ConvexHullGraham::ConvexHullGraham(const std::vector<Vector> &points) : points(points)
{}

std::list<Vector> ConvexHullGraham::buildConvexHull()
{
    std::list<Vector> hull;

    auto p0_it = std::min_element(points.begin(), points.end());
    assert(p0_it != points.end());
    auto p0 = *p0_it;
    points.erase(p0_it);

    std::sort(points.begin(), points.end(),
              []( Vector const& lhs, Vector const &rhs )
    {
        auto crossprod = lhs.crossProd(rhs);
        if (std::fabs(crossprod) > Vector::tolerance)
            return crossprod > Vector::tolerance;
        return lhs.len2() < rhs.len2();
    });

    /* top is end */
    hull.insert(hull.end(), p0);
    hull.insert(hull.end(), points[0]);

    for (size_t i = 2; i < points.size(); i++)
    {
        bool is_right_turn = true;
        while (is_right_turn)
        {
            auto top_it = hull.rbegin();
            if (top_it != hull.rend())
            {
                std::list<Vector>::reverse_iterator prev_top_it = hull.rbegin();
                prev_top_it++;
                if (prev_top_it != hull.rend())
                {
                    is_right_turn = !isLeftTurn(*prev_top_it, *top_it, points[i]);

                    if (is_right_turn)
                        hull.remove(*top_it);
                }
            }
        }
        hull.insert(hull.end(), points[i]);
    }

    return hull;
}

bool ConvexHullGraham::isLeftTurn(const Vector &p1, const Vector &p2, const Vector &p3)
{
    // vector p1p2
    Vector
            p1p2 = p1 - p2,
            p2p3 = p3 - p2;
    return p1p2.crossProd(p2p3) >= -Vector::tolerance;
}
