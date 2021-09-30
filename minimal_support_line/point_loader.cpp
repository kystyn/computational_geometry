#include <fstream>
#include <iostream>

#include "point_loader.h"
#include "primitives.h"

std::vector<Vector> PointLoader::loadFromFile( std::string const& fileName, bool *ok )
{
    std::ifstream ifs(fileName);

    if (!ifs)
    {
        std::clog << "file " << fileName << " not found\n";
        if (ok)
            *ok = false;
        return {};
    }

    std::vector<Vector> points;

    while (ifs.peek() != EOF)
    {
        Vector pt;

        if (!(ifs >> pt))
        {
            if (ifs.peek() != EOF)
            {
                std::clog << "wrong file format\n";
                if (ok)
                    *ok = false;
                return points;
            }
        }
        else
            points.emplace_back(pt);
    }
    *ok = true;
    return points;
}
