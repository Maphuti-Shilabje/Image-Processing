template <typename PixelType>
template <typename T>
typename std::enable_if<std::is_same<T, RGBPixel>::value, Image<unsigned char>>::type
Image<PixelType>::toGrayscale() const {
    Image<unsigned char> result;
    unsigned char* grayData = new unsigned char[width * height];
    
    for (int i = 0; i < width * height; i++) {
        const RGBPixel& pixel = buffer[i];
        grayData[i] = static_cast<unsigned char>(
            0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b
        );
    }
    
    result.setImageData(grayData, width, height);
    delete[] grayData;
    return result;
}