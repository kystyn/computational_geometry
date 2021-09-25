#include <iostream>
#include <cstring>
#include <string>

#include "segment_loader.h"
#include "intersector.h"

using namespace std;

void help()
{
    std::clog << "Usage: -i path/to/input/file -o path/to/output/file\n";
}

int main( int argc, char *argv[] )
{
    if (argc != 5)
    {
        help();
        return 0;
    }

    std::string inputFileName, outputFileName;
    for (int i = 1; i < argc; i += 2)
        if (!strcmp(argv[i], "-i"))
            inputFileName = argv[i + 1];
        else if (!strcmp(argv[i], "-o"))
            outputFileName = argv[i + 1];
        else
        {
            help();
            break;
        }


    SegmentLoader loader;

    bool ok;
    auto segments = loader.loadFromFile(inputFileName, &ok);
    if (!ok)
    {
        std::clog << "Something went wrong while loading input file\n";
        return 0;
    }

    Intersector intersector;
    auto result = intersector.computeIntersections(segments);

    ok = loader.saveToFile(outputFileName, result);

    if (!ok)
        std::clog << "Something went wrong while saving to output file\n";

    return 0;
}
