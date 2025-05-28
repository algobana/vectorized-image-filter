#include "filters.hpp"
#include <immintrin.h>
#include <cstring>
#include <iostream>

void blurSIMD(const unsigned char* input, unsigned char* output, int width, int height) {
    int size = width * height;
    unsigned char* gray = new unsigned char[size];

    // RGB → Grayscale (scalar)
    for (int i = 0; i < size; ++i) {
        int r = input[i * 3];
        int g = input[i * 3 + 1];
        int b = input[i * 3 + 2];
        gray[i] = static_cast<unsigned char>(0.3 * r + 0.59 * g + 0.11 * b);
    }

    std::memset(output, 0, size * 3); // RGB output

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 9; x += 8) {
            __m256i sum = _mm256_setzero_si256();

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int idx = (y + dy) * width + (x + dx);
                    __m128i src = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&gray[idx]));
                    __m256i extended = _mm256_cvtepu8_epi16(src);
                    sum = _mm256_add_epi16(sum, extended);
                }
            }

            // sum -> temp[16] -> böl -> result[8]
            alignas(32) uint16_t temp[16];
            _mm256_store_si256(reinterpret_cast<__m256i*>(temp), sum);

            alignas(16) unsigned char result[16];
            for (int i = 0; i < 8; ++i) {
                result[i] = static_cast<unsigned char>(temp[i] / 9);
            }

            std::memcpy(&gray[y * width + x], result, 8);
        }
    }

    // Grayscale → RGB
    for (int i = 0; i < size; ++i) {
        output[i * 3 + 0] = gray[i];
        output[i * 3 + 1] = gray[i];
        output[i * 3 + 2] = gray[i];
    }

    delete[] gray;
}

