#include "Bitmap.h"
#include <cuda.h>
#define CHANNEL 3
__global__  void toGreyCuda(int width, int height, unsigned char *pixelsIn, unsigned char *pixelsOut)
{
	int Col = threadIdx.x * blockIdx.x * blockDim.x;
	int Row = threadIdx.y * blockIdx.y * blockDim.y;
	if (Col < width && Row < height){
		int greyOffSet = Row * width + Col;
		unsigned char r = pixelsIn[greyOffSet];
		unsigned char b = pixelsIn[greyOffSet + 1];
		unsigned char g = pixelsIn[greyOffSet + 2];
		pixelsOut[greyOffSet] = r;
		pixelsOut[greyOffSet +  1] = b;
		pixelsOut[greyOffSet +  2] = g;
	}
}

Bitmap::Bitmap(string filename): filename(filename) {}

bool Bitmap::ToGreyscale() {	
	std::ifstream ifs;
	ifs.open(filename);
	BitmapFileHeader *btmp_file_header = new BitmapFileHeader();
	ifs.read((char *)btmp_file_header,sizeof(BitmapFileHeader));
	BitmapInfoHeader *btmp_header = new BitmapInfoHeader();
	ifs.read((char *)btmp_header,sizeof(BitmapInfoHeader));
	cout << btmp_header->height << ", " << btmp_header->width << endl;
	long pixelLength = btmp_header->height * btmp_header->width;
    unsigned char* h_input_image, *h_output_image;
    unsigned char* d_input_image, *d_output_image;

 	h_input_image = new unsigned char[pixelLength*CHANNEL*sizeof(unsigned char)];
	h_output_image = new unsigned char[pixelLength*CHANNEL*sizeof(unsigned char)];
	
	cudaMalloc((void**) &d_input_image, CHANNEL * pixelLength * sizeof(unsigned char));
    cudaMalloc((void**) &d_output_image, pixelLength * sizeof(unsigned char));
	
	ifs.read((char *)h_input_image,CHANNEL*pixelLength*sizeof(unsigned char));
	int block_dim = 2;

	cudaMemcpy(d_input_image, h_input_image, CHANNEL * pixelLength * sizeof(unsigned char), cudaMemcpyHostToDevice);

	dim3 dimBlock(block_dim, block_dim, 1);
    dim3 dimGrid(ceil((float)btmp_header->width/block_dim), ceil((float)btmp_header->height/block_dim), 1);
	toGreyCuda<<<dimGrid,dimBlock>>>(btmp_header->width,btmp_header->height,d_input_image,d_output_image);
	cudaMemcpy(h_output_image, d_output_image, pixelLength * sizeof(unsigned char), cudaMemcpyDeviceToHost);

	ifs.close();
	std::ofstream ofs;
	string greyscaleName = filename + "_greyscale.bmp";
	ofs.open(greyscaleName);
	ofs.write((char*)btmp_file_header,sizeof(BitmapFileHeader));
	ofs.write((char*)btmp_header,sizeof(BitmapInfoHeader));
	ofs.write((char*)h_output_image,sizeof(char)*pixelLength*CHANNEL);
	ofs.close();

	// Free the device memory
    cudaFree(d_input_image);
    cudaFree(d_output_image);

    // Free the host memory
    free(h_input_image);
    free(h_output_image);

	return false;
}

Bitmap::~Bitmap() {
	// TODO Auto-generated destructor stub
}
