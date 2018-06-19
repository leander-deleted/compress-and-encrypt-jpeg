#ifndef SIMPLEBMP_H
#define SIMPLEBMP_H

#include <vector>
#include <stdio.h>
#include <stdint.h>
#include <QDebug>
#include "rgb.h"
#include "compression.h"
#include <cmath>
using namespace std;
//BMP structure infomation : http://www.cnblogs.com/xiekeli/archive/2012/05/09/2491191.html


#pragma pack(1) // For MSVC,disable struct Pack,or short will take 32bit mem as int32_t
typedef struct
{
	//type of image format ascii code of "BM",which is 0x4D42
	uint16_t bfType;

	//size of bmp file; in windows, it's little endian storage for bmp file, which is (b,g,r)
	uint32_t bfSize;

	//has to be set 0
	uint16_t bfReserved1;

	//has to be set 0
	uint16_t bfReserved2;

	//the offset bytes from the start of bmp file header to actual image data
	uint32_t bfOffBits;
} ClBitMapFileHeader; // header data of bmp

typedef struct
{
	//bytes size of bitmapinfoheader structure
	uint32_t biSize;

	//the width of image, in pixel unit
	int32_t biWidth;

	//heigth of image
	int32_t biHeight;

	//show 2D image, set to 1
	uint16_t biPlanes;

	//bit number /per pixel value can be 1,4,8,16,24,32
	uint16_t biBitCount;
	/*
	type of compression
	BI_RGB: no compression
	BI_RLE8: per pixel 8 bit RLE compression encode
	..
	BI_JPEG: jpeg compression
	*/
	
	uint32_t biCompression;

/*
	//the size of image in bytes, if bicompression = BI_RGB, can be set to 0
	//  = bitwidth*bitheight
*/
	uint32_t biSizeImage;

	//pixel count/per meter in horrizontal direction
	int32_t biXPelsPerMeter;

	//pixel count/per meter in vertical direction
	int32_t biYPelsPerMeter;

	//count of used color index of color pallete; if =0, use all item in color pallete
	uint32_t biClrUsed;
	//count of color index which have significant effect, if 0, all are important
	uint32_t biClrImportant;
} ClBitMapInfoHeader; //info header data of bmp

/*
if we don't use color palette, every pixel needs to be represented in 3 bytes
if we use color palette, the count of color in the image is only 16 number of color,
we only need to store rgb value of 16 number of color, which made up of a table
each row is the rgb value of a color;
eg:
index 0: rgb(255,0,0) store red color


true color image: the image CAN represent not actually display 256*256*256 counts of color

the reading order of bmp file data is from left upper corner to right lower corner, and read row by row

*/

typedef struct
{
	uint8_t rgbBlue;
	uint8_t rgbGreen;
	uint8_t rgbRed;

	//in memory, every pixel of bmp file occupy 4 bytes to speed up computation, if bmp file only use r,g,b byte value, the last byte of value is unused
	uint8_t rgbReserved;
} ClrgbMap; 

class ClImgBMP
{
public:

	//bmp file header
	ClBitMapFileHeader bmpFileHeaderData;

	//bmp file info header
	ClBitMapInfoHeader bmpInfoHeaderData;
	ClrgbMap colorMap[256];

	//store the image data of pixel with rgb three bytes value
	std::vector<uint8_t> imgData;

	//y value 
	std::vector<uint8_t> y_ele_img,
	cb_ele_img,
	cr_ele_img;

	bool LoadImage(const char* path);
	bool SaveImage(const char* path);

	Compressor cp;
};
#pragma pack()// reset to default

#endif
