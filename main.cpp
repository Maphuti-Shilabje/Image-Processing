#include <iostream>
#include <string>
#include "PGMimageProcessor.h"

int main(int argc, char* argv[]) {
    // Input/output file names and parameters with default values
    std::string inputFile, outFileName;
    int threshold = 128, minValid = 1, filterMin = -1, filterMax = -1;
    bool print = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-m" && i+1 < argc) {
            // Minimum valid component size
            minValid = std::stoi(argv[++i]);
        } else if (arg == "-f" && i+2 < argc) {
            // Filter components by size range [filterMin, filterMax]
            filterMin = std::stoi(argv[++i]);
            filterMax = std::stoi(argv[++i]);
        } else if (arg == "-t" && i+1 < argc) {
            // Threshold for binarization (clamped between 0 and 255)
            threshold = std::stoi(argv[++i]);
            threshold = std::clamp(threshold, 0, 255);
        } else if (arg == "-p") {
            // Flag to print component information
            print = true;
        } else if (arg == "-w" && i+1 < argc) {
            // Output file name to write extracted components
            outFileName = argv[++i];
        } else if (arg[0] != '-') {
            // Positional argument assumed to be input file name
            inputFile = arg;
        }
    }

    // Check if input file was provided
    if (inputFile.empty()) {
        std::cerr << "No input file specified." << std::endl;
        return 1;
    }

    try {
        // Create processor object and load image
        PGMimageProcessor processor(inputFile);

        // Extract connected components using threshold and minimum size
        int count = processor.extractComponents(threshold, minValid);

        // If size filtering is enabled, apply it
        if (filterMin != -1 && filterMax != -1) {
            count = processor.filterComponentsBySize(filterMin, filterMax);
        }

        // Print stats and component info if requested
        if (print) {
            std::cout << "Total components: " << count << std::endl;
            std::cout << "Largest component size: " << processor.getLargestSize() << std::endl;
            std::cout << "Smallest component size: " << processor.getSmallestSize() << std::endl;
            for (const auto& comp : processor.getComponents()) {
                processor.printComponentData(*comp);
            }
        }

        // Write output image showing components (if requested)
        if (!outFileName.empty()) {
            processor.writeComponents(outFileName);
        }
    } catch (const std::exception& e) {
        // Handle any errors thrown during processing
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
