#include "PGMimageProcessor.h"
#include <fstream>
#include <stdexcept>
#include <queue>
#include <algorithm>


// reade the image from file
PGMimageProcessor::PGMimageProcessor(const std::string& filename) {
    image.read(filename);
    if (image.getBuffer() == nullptr) {
        throw std::runtime_error("Failed to read image");
    }
	image.getDims(imageWidth, imageHeight);
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


    // Create a binary image based on the threshold
    std::vector<unsigned char> binaryImage(imageWidth * imageHeight);
    for (int i = 0; i < imageWidth * imageHeight; ++i) {
        binaryImage[i] = (imageData[i] >= threshold) ? 255 : 0;
    }

    image.clear(); // Free memory from original image

    int componentId = 0;
    // Traverse each pixel to find connected components using BFS
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            int index = y * imageWidth + x;
            if (binaryImage[index] == 255) {
                auto component = std::make_unique<ConnectedComponent>(componentId);
                bfs(x, y, binaryImage, component, imageWidth, imageHeight);
                if (component->getNumPixels() >= minValidSize) {
                    components.push_back(std::move(component));
                    componentId++;
                }
            }
        }
    }
    return components.size(); // Return total valid components found
}

// Breadth-First Search to group pixels into a connected component
void PGMimageProcessor::bfs(int startX, int startY, std::vector<unsigned char>& binaryImage,
                            std::unique_ptr<ConnectedComponent>& component, int width, int height) {
    std::queue<std::pair<int, int>> q;
    q.emplace(startX, startY);
    binaryImage[startY * width + startX] = 0; // Mark as visited
    component->addPixel(startX, startY);

    // 4-connected neighborhood
    const int dx[] = {0, 0, 1, -1};
    const int dy[] = {-1, 1, 0, 0};

    // Explore all connected pixels
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                int index = ny * width + nx;
                if (binaryImage[index] == 255) {
                    binaryImage[index] = 0; // Mark as visited
                    component->addPixel(nx, ny);
                    q.emplace(nx, ny);
                }
            }
        }
    }
}

// Filtering out components smaller or larger than the specified size range
int PGMimageProcessor::filterComponentsBySize(int minSize, int maxSize) {
    auto it = components.begin();
    while (it != components.end()) {
        int size = (*it)->getNumPixels();
        if (size < minSize || size > maxSize) {
            it = components.erase(it); // Remove if outside range
        } else {
            ++it;
        }
    }
    return components.size(); // Return count of remaining components
}

// Writes the binary image showing all components to a PGM file
bool PGMimageProcessor::writeComponents(const std::string& outFileName) const {
    std::vector<unsigned char> outputImage(imageWidth * imageHeight, 0); // Black background

    // Set component pixels to white (255)
     for (const auto& component : components) {
        for (const auto& [x, y] : component->getPixels()) {
            // Validate (x, y) before writing
            if (x >= 0 && x < imageWidth && y >= 0 && y < imageHeight) {
                outputImage[y * imageWidth + x] = 255;
            } else {
                std::cerr << "Warning: Skipping invalid pixel (" << x << ", " << y << ")\n";
            }
        }
    }
    std::ofstream outFile(outFileName, std::ios::binary);
    if (!outFile) return false;

    // Write PGM header and pixel data
    outFile << "P5\n" << imageWidth << " " << imageHeight << "\n255\n";
    outFile.write(reinterpret_cast<const char*>(outputImage.data()), outputImage.size());
    outFile.close();
    return true;
}


// Returns the total number of connected components
int PGMimageProcessor::getComponentCount() const { return components.size(); }

// Returns the size of the largest component
int PGMimageProcessor::getLargestSize() const {
    if (components.empty()) return 0;
    return std::max_element(components.begin(), components.end(),
        [](const auto& a, const auto& b) { return a->getNumPixels() < b->getNumPixels(); })->get()->getNumPixels();
}

// Returns the size of the smallest component
int PGMimageProcessor::getSmallestSize() const {
    if (components.empty()) return 0;
    return std::min_element(components.begin(), components.end(),
        [](const auto& a, const auto& b) { return a->getNumPixels() < b->getNumPixels(); })->get()->getNumPixels();
}

// Prints the ID and pixel count of a given component
void PGMimageProcessor::printComponentData(const ConnectedComponent& theComponent) const {
    std::cout << "Component ID: " << theComponent.getId()
              << ", Number of pixels: " << theComponent.getNumPixels() << std::endl;
}

// Returns a const reference to all extracted connected components
const std::vector<std::unique_ptr<ConnectedComponent>>& PGMimageProcessor::getComponents() const {
    return components;
}