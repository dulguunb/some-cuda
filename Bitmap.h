/*
 * Bitmap.h
 *
 *  Created on: Jul 2, 2015
 *      Author: johnpurcell
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include "BitmapFileHeader.h"
#include "BitmapInfoHeader.h"
#include <cuda.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"


using namespace std;

struct Pixel {
	uint8_t red;
	uint8_t blue;
	uint8_t green;
};

class Bitmap {
private:
	string filename;
public:
	Bitmap(string filename);
	// void ToGreyCuda(int x, int y,Pixel *pixelsIn, Pixel *PixelsOut);
	bool ToGreyscale();
	virtual ~Bitmap();
};

#endif /* BITMAP_H_ */
