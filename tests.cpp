// tests.cpp
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "PGMimageProcessor.h"
#include "ConnectedComponent.h"
#include <memory>

// Test ConnectedComponent class
TEST_CASE("ConnectedComponent operations", "[component]") {
    ConnectedComponent component(1);
    
    SECTION("New component has correct ID") {
        REQUIRE(component.getId() == 1);
    }
    
    SECTION("New component has zero pixels") {
        REQUIRE(component.getNumPixels() == 0);
    }
    
    SECTION("Add pixels to component") {
        component.addPixel(1, 2);
        REQUIRE(component.getNumPixels() == 1);
        
        component.addPixel(3, 4);
        REQUIRE(component.getNumPixels() == 2);
        
        auto pixels = component.getPixels();
        REQUIRE(pixels.size() == 2);
        REQUIRE(pixels[0].first == 1);
        REQUIRE(pixels[0].second == 2);
        REQUIRE(pixels[1].first == 3);
        REQUIRE(pixels[1].second == 4);
    }
}

// Test PGMimageProcessor with a simple test image
TEST_CASE("PGMimageProcessor operations", "[processor]") {
    // Create a test PGM image programmatically
    const int width = 5;
    const int height = 5;
    unsigned char testImage[width * height] = {
        0, 0, 0, 0, 0,
        0, 200, 200, 0, 0,
        0, 200, 200, 0, 0,
        0, 0, 0, 0, 0,
        0, 0, 0, 150, 150
    };
    
    // Create a test PGM file
    PGMimage testPGM;
    testPGM.setImageData(testImage, width, height);
    testPGM.write("test_image.pgm");
    
    // Test the image processor
    PGMimageProcessor processor("test_image.pgm");
    
    SECTION("Extract components with threshold 100") {
        int count = processor.extractComponents(100, 1);
        REQUIRE(count == 2);  // Should find 2 components
        REQUIRE(processor.getLargestSize() == 4);  // Largest is 2x2 square
        REQUIRE(processor.getSmallestSize() == 2);  // Smallest is 1x2 rectangle
    }
    
    SECTION("Extract components with threshold 170") {
        int count = processor.extractComponents(170, 1);
        REQUIRE(count == 1);  // Only the 200-value component
        REQUIRE(processor.getLargestSize() == 4);
    }
    
    SECTION("Filter components by size") {
        processor.extractComponents(100, 1);
        int filtered = processor.filterComponentsBySize(3, 10);
        REQUIRE(filtered == 1);  // Only the 4-pixel component remains
    }
    
    SECTION("Write components") {
        processor.extractComponents(100, 1);
        bool success = processor.writeComponents("output_test.pgm");
        REQUIRE(success);
        
    }
}