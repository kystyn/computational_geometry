#ifndef SEGMENT_LOADER_H
#define SEGMENT_LOADER_H

#include <vector>
#include <string>

#include "intersector.h"

class Segment;

class SegmentLoader
{
public:
    /*!
     * \brief Load segments from file function.
     * \param fileName[IN] File name to load from.
     * \param ok[OUT] true if ok, false otherwise.
     * \return List of segments.
     */
    static std::vector<Segment> loadFromFile( std::string const& fileName, bool *ok=nullptr );

    /*!
     * \brief Save intersections to file function.
     * \param fileName[IN] File name to save to.
     * \param intersections[IN] intersection set.
     * \return True if successfully written, false otherwise.
     */
    static bool saveToFile( std::string const &fileName,
                            std::vector<Intersection> const &intersections );
};

#endif // SEGMENT_LOADER_H
