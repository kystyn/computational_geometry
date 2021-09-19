#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <istream>
#include <vector>
#include <string>
#include <map>

/*!
 * \brief The Point struct
 */
struct Point
{
    float x, y;
};

/*!
 * \brief The Segment class
 */
class Segment
{
public:

    /*!
     * \brief Segment orientation.
     * \details Possible variants:
     * \details - vertical
     * \details - horizontal
     * \details - none
     */
    enum class Orientation
    {
        VERTICAL,
        HORIZONTAL,
        NONE
    };

    Segment() {}

    /*!
     * \brief Segment class constructor.
     * \param p0 First end of segment.
     * \param p1 Second end of segment.
     */
    Segment( Point const &p0, Point const &p1 );

    /*!
     * \brief Assignment operator.
     * \param seg Segment to assign.
     * \return self-reference.
     */
    Segment & operator=( Segment const &seg );

    /*!
     * \brief Intersect segments function.
     * \param other[IN] segment to intersect.
     * \param intPt[OUT] point of intersection.
     * \return true if has intersection, false otherwise.
     */
    bool intersect( Segment const &other, Point &intPt ) const;

    /*!
     * \brief Get orientation function.
     * \return orientation
     */
    Orientation orientation() const;

private:
    //! Ends of segment
    Point p0, p1;
    //! Segment orientation
    Orientation orient;
    const float tolerance = 1e-5f;

    friend std::istream & operator>>( std::istream &is, Segment &seg );
};

class SegmentLoader
{
public:
    /*!
     * \brief Load segments from file function.
     * \param fileName[IN] File name to load from.
     * \param ok[OUT] true if ok, false otherwise.
     * \return map: id -> segment
     */
    std::map<int, Segment> loadFromFile( std::string const& fileName, bool *ok=nullptr ) const;
};

/* Input operators */
std::istream & operator>>( std::istream &is, Point &pt );
std::istream & operator>>( std::istream &is, Segment &seg );

#endif // PRIMITIVES_H
