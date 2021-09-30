#ifndef SEGMENT_LOADER_H
#define SEGMENT_LOADER_H

#include <vector>
#include <string>

#include "primitives.h"

class PointLoader
{
public:
    /*!
     * \brief Load points from file function.
     * \param fileName[IN] File name to load from.
     * \param ok[OUT] true if ok, false otherwise.
     * \return List of segments.
     */
    static std::vector<Vector> loadFromFile( std::string const& fileName, bool *ok=nullptr );
};

#endif // SEGMENT_LOADER_H
