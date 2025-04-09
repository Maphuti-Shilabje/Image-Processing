#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

// Template parameter PixelType can be unsigned char for PGM or a struct for PPM
template <typename PixelType>
class Image {
private:
    PixelType* buffer;
    int width, height;
    bool isPPM;  // Flag to indicate if this is a PPM image

public:
    // Constructor and destructor
    Image() : buffer(nullptr), width(0), height(0), isPPM(false) {}
    ~Image() { if (buffer) delete[] buffer; buffer = nullptr; }
    
    // Copy constructor
    Image(const Image& other) {
        if (other.buffer) {
            buffer = new PixelType[other.width * other.height];
            std::copy(other.buffer, other.buffer + (other.width * other.height), buffer);
        } else {
            buffer = nullptr;
        }
        width = other.width;
        height = other.height;
        isPPM = other.isPPM;
    }
    
    // Copy assignment
    Image& operator=(const Image& other) {
        if (this == &other) return *this;  // Handle self-assignment
        
        if (buffer) {
            delete[] buffer;
            buffer = nullptr;
        }
        
        if (other.buffer) {
            buffer = new PixelType[other.width * other.height];
            std::copy(other.buffer, other.buffer + (other.width * other.height), buffer);
        } else {
            buffer = nullptr;
        }
        
        width = other.width;
        height = other.height;
        isPPM = other.isPPM;
        
        return *this;
    }
    
    // Move constructor
    Image(Image&& other) noexcept 
        : buffer(other.buffer), width(other.width), height(other.height), isPPM(other.isPPM) {
        other.buffer = nullptr;
        other.width = 0;
        other.height = 0;
    }
    
    // Move assignment
    Image& operator=(Image&& other) noexcept {
        if (this != &other) {
            if (buffer) delete[] buffer;
            
            buffer = other.buffer;
            width = other.width;
            height = other.height;
            isPPM = other.isPPM;
            
            other.buffer = nullptr;
            other.width = 0;
            other.height = 0;
        }
        return *this;
    }
	
	// Getters
    PixelType* getBuffer() { return buffer; }
    const PixelType* getBuffer() const { return buffer; }
    
    void getDims(int& wd, int& ht) const { wd = width; ht = height; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool getIsPPM() const { return isPPM; }
    
    // Set image data
    void setImageData(PixelType* data, int wd, int ht, bool isPPMFlag = false) {
        if (data == nullptr || wd < 1 || ht < 1) {
            std::cerr << "setImageData() invalid data specified - aborted.\n";
            return;
        }
        
        if (buffer) {
            delete[] buffer;
            buffer = nullptr;
        }
        
        buffer = new PixelType[wd * ht];
        if (!buffer) {
            std::cerr << "Failed to allocate memory for buffer.\n";
            return;
        }
        
        width = wd;
        height = ht;
        isPPM = isPPMFlag;
        
        for (int i = 0; i < wd * ht; ++i) {
            buffer[i] = data[i];
        }
    }