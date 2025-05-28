# Vectorized Image Filter (C++ - SIMD Accelerated)

This project demonstrates the application of a 3x3 blur filter using both scalar (classic loop) and SIMD (vectorized with AVX2) techniques on grayscale-converted images.

## 🧠 Purpose

The aim is to compare traditional loop-based filtering with vectorized processing using AVX2 intrinsics to showcase the performance benefits of SIMD in system-level software development.

## ⚙️ Technologies Used

- C++17
- GCC with `-mavx -mavx2` flags
- AVX2 intrinsics via `<immintrin.h>`
- PPM image format (raw RGB)
- Linux command line (`make`, `g++`)

## 📁 Project Structure


