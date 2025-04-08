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

// Extracts connected components from the image based on a threshold and minimum valid size
int PGMimageProcessor::extractComponents(unsigned char threshold, int minValidSize) {
    const unsigned char* imageData = image.getBuffer();
    int width, height;
    image.getDims(width, height);

    // Create a binary image based on the threshold
    std::vector<unsigned char> binaryImage(width * height);
    for (int i = 0; i < width * height; ++i) {
        binaryImage[i] = (imageData[i] >= threshold) ? 255 : 0;
    }

    image.clear(); // Free memory from original image

    int componentId = 0;
    // Traverse each pixel to find connected components using BFS
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            if (binaryImage[index] == 255) {
                auto component = std::make_unique<ConnectedComponent>(componentId);
                bfs(x, y, binaryImage, component, width, height);
                if (component->getNumPixels() >= minValidSize) {
                    components.push_back(std::move(component));
                    componentId++;
                }
            }
        }
    }
    return components.size(); // Return total valid components found
}
