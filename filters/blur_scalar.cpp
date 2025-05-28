#include "filters.hpp"
#include <cstring> // memcpy

void blurScalar(const unsigned char* input, unsigned char* output, int width, int height) {
    int channels = 3; // RGB

    // İlk olarak output'u input'a kopyalayalım (kenarlar değişmeden kalsın)
    std::memcpy(output, input, width * height * channels);

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            for (int c = 0; c < channels; ++c) {
                int sum = 0;

                // 3x3 bölge içinde dolaş
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int nx = x + dx;
                        int ny = y + dy;
                        int idx = (ny * width + nx) * channels + c;
                        sum += input[idx];
                    }
                }

                int idx = (y * width + x) * channels + c;
                output[idx] = sum / 9;
            }
        }
    }
}

