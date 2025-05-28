#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include "image/image_io.hpp"
#include "filters/filters.hpp"

int main() {
    const std::string inputFile = "images/input.ppm";
    const std::string scalarFile = "images/output_scalar.ppm";
    const std::string simdFile = "images/output_simd.ppm";
    const std::string logFile = "benchmarks/times.txt";

    unsigned char* imageData = nullptr;
    int width = 0, height = 0;

    if (!readPPM(inputFile, imageData, width, height)) {
        std::cerr << "Failed to read input image.\n";
        return 1;
    }

    std::cout << "Image loaded: " << width << "x" << height << "\n";

    std::ofstream log(logFile);
    if (!log) {
        std::cerr << "Failed to open log file.\n";
    }

    // Scalar Blur
    unsigned char* scalarData = new unsigned char[width * height * 3];
    auto t1 = std::chrono::high_resolution_clock::now();
    blurScalar(imageData, scalarData, width, height);
    auto t2 = std::chrono::high_resolution_clock::now();
    double scalarMs = std::chrono::duration<double, std::milli>(t2 - t1).count();
    std::cout << "Scalar Blur Time: " << scalarMs << " ms\n";
    if (log) log << "Scalar Blur Time: " << scalarMs << " ms\n";

    writePPM(scalarFile, scalarData, width, height);

    // SIMD Blur
    unsigned char* simdData = new unsigned char[width * height * 3];
    auto t3 = std::chrono::high_resolution_clock::now();
    blurSIMD(imageData, simdData, width, height);
    auto t4 = std::chrono::high_resolution_clock::now();
    double simdMs = std::chrono::duration<double, std::milli>(t4 - t3).count();
    std::cout << "SIMD Blur Time: " << simdMs << " ms\n";
    if (log) log << "SIMD Blur Time: " << simdMs << " ms\n";

    writePPM(simdFile, simdData, width, height);

    delete[] imageData;
    delete[] scalarData;
    delete[] simdData;

    if (log) log.close();
    return 0;
}

