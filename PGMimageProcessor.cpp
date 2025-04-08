#include "PGMimageProcessor.h"
#include <fstream>

// reade the image from file
PGMimageProcessor::PGMimageProcessor(const std::string& filename) {
    image.read(filename);
    if (image.getBuffer() == nullptr) {
        throw std::runtime_error("Failed to read image");
    }
}

// copy constructor
PGMimageProcessor::PGMimageProcessor(const PGMimageProcessor& other) : image(other.image) {
    for (const auto& comp : other.components) {
        components.push_back(std::make_unique<ConnectedComponent>(*comp));
    }
}

// copy assignment
PGMimageProcessor& PGMimageProcessor::operator=(const PGMimageProcessor& other) {
    if (this != &other) {
        image = other.image;
        components.clear();
        for (const auto& comp : other.components) {
            components.push_back(std::make_unique<ConnectedComponent>(*comp));
        }
    }
    return *this;
}

// move constructor
PGMimageProcessor::PGMimageProcessor(PGMimageProcessor&& other) noexcept
    : image(std::move(other.image)), components(std::move(other.components)) {}

// move assignment
PGMimageProcessor& PGMimageProcessor::operator=(PGMimageProcessor&& other) noexcept {
    if (this != &other) {
        image = std::move(other.image);
        components = std::move(other.components);
    }
    return *this;
}

