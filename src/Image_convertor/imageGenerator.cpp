#include "imageGenerator.h"
#include <algorithm>

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

std::vector<std::vector<imageGenerator::Color>> imageGenerator::astToBMP(const std::vector<std::unique_ptr<astNode>>& ast) {
    int width = 800;
    int height = 600;

    std::vector<std::vector<Color>> bmpData(height, std::vector<Color>(width, {255, 255, 255}));  // Initialize with white color

    int maxDepth = getMaxDepth(ast);
    int rectHeight = height / maxDepth;

    std::unordered_map<int, int> nodesProcessedAtDepth;  // To keep track of nodes processed at each depth

    for (const auto& node : ast) {
        std::string type = node->getType();
        Color color;

        // Assign colors based on the type of node
        if (type == "Function") {
            color = {255, 0, 0};  // Red for functions
        } else if (type == "Variable") {
            color = {0, 255, 0};  // Green for variables
        } else if (type == "Operator") {
            color = {0, 0, 255};  // Blue for operators
        } else {
            color = {128, 128, 128};  // Default grey color
        }

        int nodeDepth = ::getMaxDepthRecursive(node) - 1;
        int numNodesAtCurrentDepth = nodesProcessedAtDepth[nodeDepth]++;
        int rectWidth = width / (nodesProcessedAtDepth[nodeDepth] + 1);  // +1 to account for the current node

        int xPos = numNodesAtCurrentDepth * rectWidth;

        for (int y = nodeDepth * rectHeight; y < (nodeDepth + 1) * rectHeight; y++) {
            for (int x = xPos; x < xPos + rectWidth; x++) {
                bmpData[y][x] = color;
            }
        }
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
