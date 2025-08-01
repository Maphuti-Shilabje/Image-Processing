#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "Image.h"
#include "ConnectedComponent.h"
#include <memory>
#include <vector>
#include <queue>
#include <algorithm>

template <typename ImageType>
class ImageProcessor {
private:
    ImageType image;
    std::vector<std::unique_ptr<ConnectedComponent>> components;

    void bfs(int x, int y, std::vector<unsigned char>& binaryData, 
            std::unique_ptr<ConnectedComponent>& component) {
        std::queue<std::pair<int, int>> q;
        q.emplace(x, y);
        const int width = image.getWidth();
        const int height = image.getHeight();
        const int dx[] = {0, 0, 1, -1};
        const int dy[] = {-1, 1, 0, 0};

        while (!q.empty()) {
            auto [cx, cy] = q.front();
            q.pop();

            for (int i = 0; i < 4; ++i) {
                int nx = cx + dx[i];
                int ny = cy + dy[i];
                if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                    binaryData[ny * width + nx] == 255) {
                    binaryData[ny * width + nx] = 0;
                    component->addPixel(nx, ny);
                    q.emplace(nx, ny);
                }
            }
        }
    }

    unsigned char getGrayValue(int x, int y) const {
        if constexpr (std::is_same<ImageType, PPMImage>::value) {
            const RGBPixel& pixel = image.getBuffer()[y * image.getWidth() + x];
            return static_cast<unsigned char>(0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b);
        } else {
            return image.getBuffer()[y * image.getWidth() + x];
        }
    }

public:
    explicit ImageProcessor(const std::string& filename) {
        image.read(filename);
    }

    int extractComponents(unsigned char threshold, int minValidSize) {
        const int width = image.getWidth();
        const int height = image.getHeight();
        std::vector<unsigned char> binaryData(width * height);

        // Create binary image
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                binaryData[y * width + x] = (getGrayValue(x, y) >= threshold) ? 255 : 0;
            }
        }

        // Extract components
        components.clear();
        int componentId = 0;
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (binaryData[y * width + x] == 255) {
                    auto comp = std::make_unique<ConnectedComponent>(componentId++);
                    comp->addPixel(x, y);
                    bfs(x, y, binaryData, comp);
                    if (comp->getNumPixels() >= minValidSize) {
                        components.push_back(std::move(comp));
                    }
                }
            }
        }
        return components.size();
    }

    int filterComponentsBySize(int minSize, int maxSize) {
        auto it = components.begin();
        while (it != components.end()) {
            int size = (*it)->getNumPixels();
            if (size < minSize || size > maxSize) {
                it = components.erase(it);
            } else {
                ++it;
            }
        }
        return components.size();
    }

    bool writeComponents(const std::string& outFileName) const {
        const int width = image.getWidth();
        const int height = image.getHeight();
        std::vector<unsigned char> output(width * height, 0);

        for (const auto& comp : components) {
            for (const auto& [x, y] : comp->getPixels()) {
                output[y * width + x] = 255;
            }
        }

        PGMImage outImage;
        outImage.setImageData(output.data(), width, height);
        outImage.write(outFileName);
        return true;
    }

    bool writeComponentsWithBoxes(const std::string& outFileName) const {
        const int width = image.getWidth();
        const int height = image.getHeight();
        std::vector<RGBPixel> output;

        // Handle PGM->PPM conversion
        if constexpr (std::is_same<ImageType, PGMImage>::value) {
            const unsigned char* gray = image.getBuffer();
            output.reserve(width * height);
            for (int i = 0; i < width * height; ++i) {
                output.emplace_back(gray[i], gray[i], gray[i]);
            }
        } else {
            const RGBPixel* color = image.getBuffer();
            output.assign(color, color + width * height);
        }

        // Draw bounding boxes
        const RGBPixel red(255, 0, 0);
        for (const auto& comp : components) {
            int minX = width, maxX = 0, minY = height, maxY = 0;
            for (const auto& [x, y] : comp->getPixels()) {
                minX = std::min(minX, x);
                maxX = std::max(maxX, x);
                minY = std::min(minY, y);
                maxY = std::max(maxY, y);
            }

            // Draw box
            for (int x = minX; x <= maxX; ++x) {
                output[minY * width + x] = red;
                output[maxY * width + x] = red;
            }
            for (int y = minY; y <= maxY; ++y) {
                output[y * width + minX] = red;
                output[y * width + maxX] = red;
            }
        }

        PPMImage outImage;
        outImage.setImageData(output.data(), width, height, true);
        outImage.write(outFileName);
        return true;
    }

    int getComponentCount() const { return components.size(); }
    int getLargestSize() const {
        return components.empty() ? 0 : 
            std::max_element(components.begin(), components.end(),
                [](const auto& a, const auto& b) { 
                    return a->getNumPixels() < b->getNumPixels(); 
                })->get()->getNumPixels();
    }
    int getSmallestSize() const {
        return components.empty() ? 0 : 
            std::min_element(components.begin(), components.end(),
                [](const auto& a, const auto& b) { 
                    return a->getNumPixels() < b->getNumPixels(); 
                })->get()->getNumPixels();
    }
    void printComponentData(const ConnectedComponent& comp) const {
        std::cout << "Component ID: " << comp.getId() 
                  << ", Pixels: " << comp.getNumPixels() << "\n";
    }
    const auto& getComponents() const { return components; }
};

using PGMProcessor = ImageProcessor<PGMImage>;
using PPMProcessor = ImageProcessor<PPMImage>;

#endif // IMAGE_PROCESSOR_H