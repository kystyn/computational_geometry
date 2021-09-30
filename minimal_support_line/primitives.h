#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <istream>

/*!
 * \brief The Point struct
 */
class Vector
{
public:
    /*!
     * \brief Default class constructor.
     */
    Vector();

    /*!
     * \brief Get x coordinate function.
     * \return x coordinate.
     */
    float x() const;

    /*!
     * \brief Get y coordinate function.
     * \return y coordinate.
     */
    float y() const;

    /*!
     * \brief Get id function.
     * \return id.
     */
    int id() const;

    /*!
     * \brief Evaluate square of Euclid norm function.
     * \return Square of Euclid norm.
     */
    float len2() const;

    /*!
     * \brief Per component class constructor function.
     * \param _id _id.
     * \param x x.
     * \param y y.
     */
    Vector( int _id, float _x, float _y );

    Vector & operator=( Vector const &rhs );

    Vector operator-( Vector const &rhs ) const;

    /*!
     * \brief Evaluate cross product function.
     * \param rhs Vector to evaluate cross product to.
     * \return Cross product.
     */
    float crossProd( Vector const &rhs ) const;

    /*!
     * \brief Dot product function.
     * \param rhs Vector to evaluate dot product to.
     * \return Dot product.
     */
    float dotProd( Vector const &rhs ) const;

    /*!
     * \brief Comparator function.
     * \param rhs Point to compare with.
     * \return true if equal, false otherwise.
     */
    bool operator==( Vector const &rhs ) const;

    /*!
     * \brief Strict order operator.
     * \param rhs Point to compare with.
     * \return True if less, false otherwise.
     */
    bool operator<( Vector const &rhs ) const;

    /*!
     * \brief Nonstrict order operator.
     * \param rhs Point to compare with.
     * \return True if less or equal, false otherwise.
     */
    bool operator<=( Vector const &rhs ) const;

    static const float tolerance;

private:
    int _id;
    float _x, _y;
};


/* Input operators */
std::istream & operator>>( std::istream &is, Vector &pt );

/* Output operators */
std::ostream & operator<<( std::ostream &os, Vector const &pt );

#endif // PRIMITIVES_H
