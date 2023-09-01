#include "imageGenerator.h"
#include <algorithm>
#include <functional>

int getMaxDepthRecursive(const std::unique_ptr<astNode>& node) {
    if (!node) {
        return 0;
    }

    int maxChildDepth = 0;
    for (const auto& child : node->getChildren()) {  // Assuming 'children' is a member of astNode
        maxChildDepth = std::max(maxChildDepth, getMaxDepthRecursive(child));
    }

    return 1 + maxChildDepth;  // 1 for the current node + maximum depth of children
}

int getMaxDepth(const std::vector<std::unique_ptr<astNode>>& ast) {
    if (ast.empty()) {
        return 0;  // Return 0 if the AST is empty
    }

    int maxDepth = 0;
    for (const auto& rootNode : ast) {
        maxDepth = std::max(maxDepth, getMaxDepthRecursive(rootNode));
    }
    return maxDepth;
}

// Function to draw a rectangle at (x, y) with dimensions (width, height)
void drawRectangle(int x, int y, int width, int height, std::vector<std::vector<imageGenerator::Color>>& data, imageGenerator::Color color) {
    for (int i = x; i < x + width; ++i) {
        for (int j = y; j < y + height; ++j) {
            if (i >= 0 && i < data[0].size() && j >= 0 && j < data.size()) {
                data[j][i] = color;
            }
        }
    }
}

void drawArrow(int x1, int y1, int x2, int y2, std::vector<std::vector<imageGenerator::Color>>& data, imageGenerator::Color color) {
    // Code to draw a straight line (for simplicity)
    for (int i = std::min(y1, y2); i <= std::max(y1, y2); ++i) {
        if (x1 >= 0 && x1 < data[0].size() && i >= 0 && i < data.size()) {
            data[i][x1] = color;
        }
    }
    // Code to add arrow tip can be added here
}

// Function to draw the AST as a tree
void drawAST(const std::unique_ptr<astNode>& node, int x, int y, int level, std::vector<std::vector<imageGenerator::Color>>& data) {
    if (!node) {
        return;
    }

    // Draw the current node at (x, y)
    std::string type = node->getType();
    imageGenerator::Color color;
    // Set the color based on the type
    // ...

    drawRectangle(x, y, 50, 50, data, color);  // Assume each node is represented as a 50x50 rectangle

    // Draw children
    int childX = x - 100;  // Starting x-coordinate for children
    int childY = y + 100;  // Starting y-coordinate for children
    for (const auto& child : node->getChildren()) {
        drawArrow(x, y, childX, childY, data, {0, 0, 0});  // Draw an arrow with black color
        drawAST(child, childX, childY, level + 1, data);
        childX += 200;  // Move the x-coordinate for the next child
    }
}

std::vector<std::vector<imageGenerator::Color>> imageGenerator::astToBMP(const std::vector<std::unique_ptr<astNode>>& ast) {
    int width = 800;
    int height = 600;

    std::vector<std::vector<Color>> bmpData(height, std::vector<Color>(width, {255, 255, 255}));  // Initialize with white color

    int x = width / 2;  // Starting x-coordinate
    int y = 50;         // Starting y-coordinate

    for (const auto& rootNode : ast) {
        drawAST(rootNode, x, y, 0, bmpData);
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
