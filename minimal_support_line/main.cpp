#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "point_loader.h"
#include "convex_hull_graham.h"
#include "minimal_support_line.h"

using namespace std;

void help()
{
    std::clog << "Usage: -i path/to/input/file [-o path/to/output/file]\n";
}

int main( int argc, char *argv[] )
{
    if (argc != 5 && argc != 3)
    {
        help();
        return 0;
    }

    std::string inputFileName, outputFileName;
    std::ostream *os = &std::cout;
    std::ofstream ofs;
    for (int i = 1; i < argc; i += 2)
        if (!strcmp(argv[i], "-i"))
            inputFileName = argv[i + 1];
        else if (!strcmp(argv[i], "-o"))
        {
            ofs = std::ofstream(argv[i + 1]);

            if (!ofs)
            {
                std::clog << "file " << argv[i + 1] << " not found\n";
            }
            os = &ofs;
        }
        else
        {
            help();
            break;
        }


    PointLoader loader;

    bool ok;
    auto points = loader.loadFromFile(inputFileName, &ok);
    if (!ok)
    {
        std::clog << "Something went wrong while loading input file\n";
        return 0;
    }

    ConvexHullGraham ch(points);
    auto hull = ch.buildConvexHull();

    MinimalSupportLine msl;
    auto optline = msl.findMinimalSupportLine(points, hull);

    std::cout << "Optimal line containt points with id " <<
                 optline.first << " and id " << optline.second << "\n";
    return 0;
}
