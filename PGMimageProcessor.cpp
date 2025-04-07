#include "PGMimageProcessor.h"
#include <fstream>


PGMimageProcessor::PGMimageProcessor(const std::string& filename) {
    image.read(filename);
    if (image.getBuffer() == nullptr) {
        throw std::runtime_error("Failed to read image");
    }
}
