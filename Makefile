CXX = g++
CXXFLAGS = -std=c++17 -O2 -mavx -mavx2

SRC = main.cpp image/image_io.cpp filters/blur_scalar.cpp filters/blur_simd_rgb.cpp
OUT = filter

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT) *.o

