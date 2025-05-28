
#include "filters.hpp"
#include <immintrin.h>
#include <cstring>
#include <iostream>

void blurChannelSIMD(const unsigned char* input, unsigned char* output, int width, int height) {
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 9; x += 8) {
            __m256i sum = _mm256_setzero_si256();

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int idx = (y + dy) * width + (x + dx);
                    __m128i src = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&input[idx]));
                    __m256i ext = _mm256_cvtepu8_epi16(src);
                    sum = _mm256_add_epi16(sum, ext);
                }
            }

            alignas(32) uint16_t temp[16];
            _mm256_store_si256(reinterpret_cast<__m256i*>(temp), sum);

            alignas(16) unsigned char result[16];
            for (int i = 0; i < 8; ++i)
                result[i] = static_cast<unsigned char>(temp[i] / 9);

            std::memcpy(&output[y * width + x], result, 8);
        }
    }
}

void blurSIMD(const unsigned char* input, unsigned char* output, int width, int height) {
    int size = width * height;

    unsigned char* r = new unsigned char[size];
    unsigned char* g = new unsigned char[size];
    unsigned char* b = new unsigned char[size];

    for (int i = 0; i < size; ++i) {
        r[i] = input[i * 3 + 0];
        g[i] = input[i * 3 + 1];
        b[i] = input[i * 3 + 2];
    }

    blurChannelSIMD(r, r, width, height);
    blurChannelSIMD(g, g, width, height);
    blurChannelSIMD(b, b, width, height);

    for (int i = 0; i < size; ++i) {
        output[i * 3 + 0] = r[i];
        output[i * 3 + 1] = g[i];
        output[i * 3 + 2] = b[i];
    }

    delete[] r;
    delete[] g;
    delete[] b;
}
