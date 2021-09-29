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

    while (ifs.peek() != EOF)
    {
        Segment seg;

        if (!(ifs >> seg))
        {
            if (ifs.peek() != EOF)
            {
                std::clog << "wrong file format\n";
                if (ok)
                    *ok = false;
                return segments;
            }
        }
        else
            segments.emplace_back(seg);
    }
    *ok = true;
    return segments;
}
