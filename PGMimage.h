#ifndef _PGMI_H
#define _PGMI_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class PGMimage {
private:
    unsigned char* buffer;
    int width, height;
public:
    PGMimage() : buffer(nullptr), width(0), height(0) {}
    ~PGMimage() { if (buffer) delete[] buffer; buffer = nullptr;}

    PGMimage(const PGMimage& other) {
        if (other.buffer) {
            buffer = new unsigned char[other.width * other.height];
            std::copy(other.buffer, other.buffer + (other.width * other.height), buffer);
        } else {
            buffer = nullptr;
        }
        width = other.width;
        height = other.height;
    }
    
    PGMimage& operator=(const PGMimage& other) {
        if (this == &other) return *this;
        if (buffer) delete[] buffer;
        if (other.buffer) {
            buffer = new unsigned char[other.width * other.height];
            std::copy(other.buffer, other.buffer + (other.width * other.height), buffer);
        } else {
            buffer = nullptr;
        }
        width = other.width;
        height = other.height;
        return *this;
    }

    unsigned char* getBuffer() { return buffer; }
    const unsigned char* getBuffer() const { return buffer; }

    void getDims(int& wd, int& ht) const {
        wd = width; ht = height;
    }

    void setImageData(unsigned char* data, int wd, int ht);
    void read(const std::string& fileName);
    void write(const std::string& fileName);
    
    void clear() {
        if (buffer) {
            delete[] buffer;
            buffer = nullptr;
        }
        width = 0;
        height = 0;
    }
};

#endif
