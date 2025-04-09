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
	
	 // Read and write image files
    template <typename T = PixelType>
    typename std::enable_if<std::is_same<T, unsigned char>::value, void>::type
    read(const std::string& fileName) {
        std::ifstream ifs(fileName, std::ios::binary);
        if (!ifs) {
            std::cerr << "Failed to open file for read: " << fileName << std::endl;
            return;
        }
        
        std::string line;
        ifs >> line >> std::ws;
        
        if (line != "P5") {
            std::cerr << "Malformed PGM file - magic is: " << line << std::endl;
            return;
        }
        
        isPPM = false;
        
        while (getline(ifs, line)) {
            if (line[0] != '#') break;
        }
        
        std::istringstream iss(line);
        iss >> width >> height;
        
        if (!iss) {
            std::cerr << "Header not correct - unexpected image sizes found: " << line << std::endl;
            return;
        }
        
        int maxChan = 0;
        ifs >> maxChan >> std::ws;
        
        if (maxChan != 255) {
            std::cerr << "Max grey level incorrect - found: " << maxChan << std::endl;
        }
        
        buffer = new unsigned char[width * height];
        ifs.read(reinterpret_cast<char*>(buffer), width * height);
        
        if (!ifs) {
            std::cerr << "Failed to read binary block\n";
        }
        
        ifs.close();
    }
    
    // Write method for PGM
    template <typename T = PixelType>
    typename std::enable_if<std::is_same<T, unsigned char>::value, void>::type
    write(const std::string& fileName) {
        if (buffer == nullptr || width < 1 || height < 1) {
            std::cerr << "Invalid data for PGM write to " << fileName << std::endl;
            return;
        }
        
        std::ofstream ofs(fileName, std::ios::binary);
        if (!ofs) {
            std::cerr << "Unable to open PGM output file " << fileName << std::endl;
            return;
        }
        
        ofs << "P5\n#File produced by Image Processor\n" << width << " " << height << std::endl << 255 << std::endl;
        ofs.write(reinterpret_cast<char*>(buffer), width * height);
        
        if (!ofs) {
            std::cerr << "Error writing binary block of PGM.\n";
        }
        
        ofs.close();
    }
	
	
	  // Create a grayscale version for PPM -> PGM conversion
    Image<unsigned char> toGrayscale() const {
        static_assert(!std::is_same<PixelType, unsigned char>::value, 
                      "toGrayscale() can only be called on color images");
                      
        Image<unsigned char> result;
        unsigned char* grayBuffer = new unsigned char[width * height];
        
        for (int i = 0; i < width * height; i++) {
            // Convert RGB to grayscale using standard formula
            grayBuffer[i] = static_cast<unsigned char>(
                0.299 * buffer[i].r + 0.587 * buffer[i].g + 0.114 * buffer[i].b
            );
        }
        
        result.setImageData(grayBuffer, width, height);
        delete[] grayBuffer;
        
        return result;
    }
};
 
	
	
#endif