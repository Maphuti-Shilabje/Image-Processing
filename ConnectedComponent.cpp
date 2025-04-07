#include "ConnectedComponent.h"

// Constructor that initializes a connected component with a given id and zero pixels.
ConnectedComponent::ConnectedComponent(int id) : id(id), numPixels(0) {}

// Copy constructor: creates a new ConnectedComponent as a deep copy of another.
ConnectedComponent::ConnectedComponent(const ConnectedComponent& other)
    : id(other.id), numPixels(other.numPixels), pixels(other.pixels) {}

// Copy assignment operator: assigns the contents of another ConnectedComponent to this one.
ConnectedComponent& ConnectedComponent::operator=(const ConnectedComponent& other) {
    if (this != &other) { // Avoid asigning to itself
        id = other.id;
       numPixels = other.numPixels;
        pixels = other.pixels; // Deep copy of pixel data
    }
    return *this;
}

// Move constructor: transfers ownership of resources from a temporary object (rvalue) to this object.
ConnectedComponent::ConnectedComponent(ConnectedComponent&& other) noexcept
    : id(other.id), numPixels(other.numPixels), pixels(std::move(other.pixels)) {}

// Move assignment operator: transfers ownership of resources from a temporary object (rvalue).
ConnectedComponent& ConnectedComponent::operator=(ConnectedComponent&& other) noexcept {
    if (this != &other) { // Avoid assigning to itself
        id = other.id;
        numPixels = other.numPixels;
        pixels = std::move(other.pixels); 
    }
    return *this;
}

// Adds a new pixel coordinate (x, y) to the connected component and increments the pixel count.
void ConnectedComponent::addPixel(int x, int y) {
    pixels.emplace_back(x, y);
    numPixels++;
}

// Returns the unique identifier of the connected component.
int ConnectedComponent::getId() const { return id; }

// Returns the number of pixels that belong to this component.
int ConnectedComponent::getNumPixels() const { return numPixels; }

// Returns a reference to the vector containing all pixel coordinates in this component.
const std::vector<std::pair<int, int>>& ConnectedComponent::getPixels() const { return pixels; }

