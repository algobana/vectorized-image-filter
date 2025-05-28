// image_io.cpp
#include "image_io.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

bool readPPM(const std::string& filename, unsigned char*& data, int& width, int& height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Couldn't open " << filename << "\n";
        return false;
    }

    std::string magic;
    file >> magic;
    if (magic != "P6") {
        std::cerr << "Invalid PPM format (must be P6)\n";
        return false;
    }

    file >> width >> height;
    int max_val;
    file >> max_val;
    file.ignore(); // Skip single whitespace after header

    int size = width * height * 3; // RGB
    data = new unsigned char[size];
    file.read(reinterpret_cast<char*>(data), size);

    if (!file) {
        std::cerr << "Error reading pixel data\n";
        delete[] data;
        return false;
    }

    return true;
}

bool writePPM(const std::string& filename, const unsigned char* data, int width, int height) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Couldn't write to " << filename << "\n";
        return false;
    }

    file << "P6\n" << width << " " << height << "\n255\n";
    file.write(reinterpret_cast<const char*>(data), width * height * 3);
    return true;
}

