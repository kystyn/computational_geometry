#ifndef SEGMENT_LOADER_H
#define SEGMENT_LOADER_H

#include <vector>
#include <string>

#include "primitives.h"

class SegmentLoader
{
public:
    /*!
     * \brief Load segments from file function.
     * \param fileName[IN] File name to load from.
     * \param ok[OUT] true if ok, false otherwise.
     * \return List of segments.
     */
    std::vector<Segment> loadFromFile( std::string const& fileName, bool *ok=nullptr ) const;
};

#endif // SEGMENT_LOADER_H
