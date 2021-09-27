#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <istream>
#include <vector>
#include <string>

/*!
 * \brief The Point struct
 */
struct Point
{
    float x, y;

    /*!
     * \brief Comparator function.
     * \param rhs Point to compare with.
     * \return true if equal, false otherwise.
     */
    bool operator==( Point const &rhs );
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

    static const float tolerance;

    Segment() {}

    /*!
     * \brief Segment class constructor.
     * \param p0 First end of segment.
     * \param p1 Second end of segment.
     * \param id Segment identifier.
     */
    Segment( Point const &_p0, Point const &_p1, int id );

    /*!
     * \brief Assignment operator.
     * \param seg Segment to assign.
     * \return self-reference.
     */
    Segment & operator=( Segment const &seg );

    /*!
     * \brief Intersect segments function.
     * \param other[IN] segment to intersect.
     * \param has_intersect[OUT] true if has intersection, false otherwise.
     * \return point of intersection.
     */
    Point intersect( Segment const &other, bool &has_intersect ) const;

    /*!
     * \brief Get orientation function.
     * \return orientation
     */
    Orientation orientation() const;

    /*!
     * \brief Get id function.
     * \return id.
     */
    int id() const;

    /*!
     * \brief Get P0 function.
     * \return P0.
     */
    Point p0() const;

    /*!
     * \brief Get P1 function.
     * \return P1.
     */
    Point p1() const;

private:
    /*!
     * \brief Intersect two horizontal segments function.
     * \param other[IN] segment to intersect.
     * \param has_intersect[OUT] true if has intersection, false otherwise.
     * \return point of intersection.
     */
    Point intersect_hor_hor( Segment const &other, bool &has_intersect ) const;

    /*!
     * \brief Intersect two vertical segments function.
     * \param other[IN] segment to intersect.
     * \param has_intersect[OUT] true if has intersection, false otherwise.
     * \return point of intersection.
     */
    Point intersect_ver_ver( Segment const &other, bool &has_intersect ) const;

    /*!
     * \brief Intersect horizontal & vertical segments function.
     * \param other[IN] segment to intersect.
     * \param has_intersect[OUT] true if has intersection, false otherwise.
     * \return point of intersection.
     */
    Point intersect_hor_ver( Segment const &other, bool &has_intersect ) const;

    //! Ends of segment
    Point _p0, _p1;
    //! Identifier
    int _id;
    //! Segment orientation
    Orientation orient;
};

/* Input operators */
std::istream & operator>>( std::istream &is, Point &pt );
std::istream & operator>>( std::istream &is, Segment &seg );

/* Output operators */
std::ostream & operator<<( std::ostream &os, Point const &pt );

#endif // PRIMITIVES_H
