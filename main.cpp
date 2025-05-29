#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <climits>
#include <omp.h>
#include <cfloat>
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
    delete[] scalarData;

    // Auto-thread tuning for SIMD Blur
    int bestThreads = 1;
    double bestTime = DBL_MAX;

    for (int threads = 1; threads <= std::thread::hardware_concurrency(); threads *= 2) {
        omp_set_num_threads(threads);
        unsigned char* temp = new unsigned char[width * height * 3];

        auto start = std::chrono::high_resolution_clock::now();
        blurSIMD(imageData, temp, width, height);
        auto end = std::chrono::high_resolution_clock::now();

        double elapsed = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << "SIMD Blur with " << threads << " threads: " << elapsed << " ms\n";
        if (log) log << "SIMD Blur with " << threads << " threads: " << elapsed << " ms\n";

        if (elapsed < bestTime) {
            bestTime = elapsed;
            bestThreads = threads;
        }

        delete[] temp;
    }

    std::cout << "Best thread count: " << bestThreads << "\n";
    if (log) log << "Best thread count: " << bestThreads << "\n";

    // Final SIMD blur with best thread count
    omp_set_num_threads(bestThreads);
    unsigned char* simdData = new unsigned char[width * height * 3];
    auto t5 = std::chrono::high_resolution_clock::now();
    blurSIMD(imageData, simdData, width, height);
    auto t6 = std::chrono::high_resolution_clock::now();
    double finalSimdMs = std::chrono::duration<double, std::milli>(t6 - t5).count();

    std::cout << "Final SIMD Blur (best " << bestThreads << " threads): " << finalSimdMs << " ms\n";
    if (log) log << "Final SIMD Blur Time: " << finalSimdMs << " ms\n";

    writePPM(simdFile, simdData, width, height);

    delete[] imageData;
    delete[] simdData;
    if (log) log.close();

    return 0;
}

