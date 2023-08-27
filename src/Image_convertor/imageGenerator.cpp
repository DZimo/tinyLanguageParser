#include "imageGenerator.h"

std::vector<std::vector<imageGenerator::Color>> imageGenerator::astToBMP(const std::vector<std::unique_ptr<astNode>>& ast) {
    int width = 800;
    int height = 600;

    std::vector<std::vector<Color>> bmpData(height, std::vector<Color>(width));

    // Now, you'll iterate over the vector of unique pointers to fill the bmpData.
    // For example:
    for (const auto& node : ast) {
        // Access the node using node->someFunction()...
    }

    return bmpData;
}
// Save BMP to file
void imageGenerator::saveBMP(const std::string& filename, const std::vector<std::vector<Color>>& data) {
    int width = data[0].size();
    int height = data.size();

    std::ofstream file(filename, std::ios::binary);

    // BMP file headers
    file.put('B').put('M'); // BM for Bitmap

    // File size
    int fileSize = 54 + 3 * width * height;
    file.put(fileSize).put(fileSize >> 8).put(fileSize >> 16).put(fileSize >> 24);

    file.put(0).put(0).put(0).put(0); // Reserved
    file.put(54).put(0).put(0).put(0); // Image data offset

    // Image header (DIB header)
    file.put(40).put(0).put(0).put(0); // DIB header size
    file.put(width).put(width >> 8).put(width >> 16).put(width >> 24);
    file.put(height).put(height >> 8).put(height >> 16).put(height >> 24);
    file.put(1).put(0); // Number of color planes
    file.put(24).put(0); // Bits per pixel
    file.put(0).put(0).put(0).put(0); // No compression
    file.put(0).put(0).put(0).put(0); // Image size (no compression)
    file.put(0).put(0).put(0).put(0); // Pixels per meter (not specified)
    file.put(0).put(0).put(0).put(0); // Pixels per meter (not specified)
    file.put(0).put(0).put(0).put(0); // Total colors
    file.put(0).put(0).put(0).put(0); // Important colors

    // Image data
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const imageGenerator::Color& pixel = data[y][x];
            file.put(pixel.b).put(pixel.g).put(pixel.r);
        }
    }

    file.close();
}
