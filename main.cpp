#include <iostream>
#include <cstdlib>
#include "Bitmap.h"
#include "BitmapFileHeader.h"
#include "BitmapInfoHeader.h"
using namespace std;

int main() {
	Bitmap bitmap("sample.bmp");
	bitmap.ToGreyscale();
	return 0;
}
