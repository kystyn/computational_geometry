#include <fstream>
#include <iostream>
#include "segment_loader.h"

std::vector<Segment> SegmentLoader::loadFromFile(const std::string &fileName, bool *ok) const
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
