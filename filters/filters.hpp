#ifndef FILTERS_HPP
#define FILTERS_HPP

void blurScalar(const unsigned char* input, unsigned char* output, int width, int height);

void blurSIMD(const unsigned char* input, unsigned char* output, int width, int height);


#endif

