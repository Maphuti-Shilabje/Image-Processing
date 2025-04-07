#ifndef PGM_IMAGE_PROCESSOR_H
#define PGM_IMAGE_PROCESSOR_H

#include <vector>
#include <string>
#include "ConnectedComponent.h"
#include "PGMimage.h"

class PGMimageProcessor {
private:
    PGMimage image;
    std::vector<std::unique_ptr<ConnectedComponent>> components;
    void bfs(int x, int y, std::vector<unsigned char>& binaryImage, std::unique_ptr<ConnectedComponent>& component, int width, int height);

    
public:
    explicit PGMimageProcessor(const std::string& filename);
    ~PGMimageProcessor() = default;

    PGMimageProcessor(const PGMimageProcessor& other);
    PGMimageProcessor& operator=(const PGMimageProcessor& other);
    PGMimageProcessor(PGMimageProcessor&& other) noexcept;
    PGMimageProcessor& operator=(PGMimageProcessor&& other) noexcept;

    int extractComponents(unsigned char threshold, int minValidSize);
    int filterComponentsBySize(int minSize, int maxSize);
    bool writeComponents(const std::string& outFileName) const;
    int getComponentCount() const;
    int getLargestSize() const;
    int getSmallestSize() const;
    void printComponentData(const ConnectedComponent& theComponent) const;
    const std::vector<std::unique_ptr<ConnectedComponent>>& getComponents() const;
};

#endif
