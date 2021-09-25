#include <fstream>
#include <iostream>

#include "segment_loader.h"
#include "primitives.h"
#include "intersector.h"

std::vector<Segment> SegmentLoader::loadFromFile(const std::string &fileName, bool *ok)
{
    std::ifstream ifs(fileName);

    if (!ifs)
    {
        std::clog << "file " << fileName << " not found\n";
        if (ok)
            *ok = false;
        return {};
    }

    std::vector<Segment> segments;

    while (ifs)
    {
        int id;
        Segment seg;
        if (!(ifs >> id))
            break;

        if (!(ifs >> seg))
        {
            std::clog << "wrong file format\n";
            if (ok)
                *ok = false;
            return segments;
        }
        else
            segments.emplace_back(seg);
    }
    return segments;
}

bool SegmentLoader::saveToFile( const std::string &fileName,
                                const std::vector<Intersection> &intersections )
{
    std::ofstream ofs(fileName);

    if (!ofs)
    {
        std::clog << "file " << fileName << " not found\n";
        return false;
    }

    for (auto &inter: intersections)
        ofs << inter.id1 << ' ' << inter.id2 << ' ' << inter.intPt << '\n';

    return true;
}
