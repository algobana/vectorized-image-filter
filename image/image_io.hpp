// image_io.hpp
#ifndef IMAGE_IO_HPP
#define IMAGE_IO_HPP

#include <string>

bool readPPM(const std::string& filename, unsigned char*& data, int& width, int& height);
bool writePPM(const std::string& filename, const unsigned char* data, int width, int height);

#endif

