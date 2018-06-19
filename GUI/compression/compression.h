#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <cmath>
#include <stdint.h>
#include <vector>
#include <iostream>
#include <QDebug>

using namespace std;
typedef vector<uint8_t> Dim1;
typedef vector<Dim1> Dim2;

typedef vector<float> fDim1;
typedef vector<fDim1> fDim2;


#define M 8
#define N 8
#define pi 3.1415926

class Compressor{

public:
	bool convertToBlock(vector<uint8_t> data, int width,int height);
	bool dctTransform();
	void quantization(uint8_t degree = 50);
	void show(vector<uint8_t>data={},int s=1);

	int width;
	int height;
private:
	// dct: store block data
	float data[M][N];

	//count of 8*8 block of image data stream
	int count;

	// data block store all 8*8 blocks
    vector<Dim2> Imgblock,q_Imgblock;
    vector<fDim2> dct_Imgblock;

	uint8_t Q50[M][N] = {
	{16,11,10,16,24,40,51,61},
	{12,12,14,19,26,58,60,55},
	{14,13,16,24,40,57,69,56},
	{14,17,22,29,51,87,80,62},
	{18,22,37,56,68,109,103,77},
	{24,35,55,64,81,104,113,92},
	{49,64,78,87,103,121,120,101},
	{72,92,95,98,112,100,103,99}
	};
};

#endif