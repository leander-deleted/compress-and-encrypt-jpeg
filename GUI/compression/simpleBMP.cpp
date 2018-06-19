#include "simpleBMP.h"

bool ClImgBMP::LoadImage(const char* path)
{
	FILE* pFile;
	pFile = fopen(path, "rb");
	if (!pFile)
	{
		return 0;
	}

	fread(&bmpFileHeaderData, sizeof(ClBitMapFileHeader), 1, pFile);
	if (bmpFileHeaderData.bfType == 0x4D42) // Check is it an RGB file
	{


		// Get Channel num of a pixel
		//channel: grayscale image, value of each pixel is 1 byte; true color image without pallete, value is 3 bytes long
		int channels = 0;
		fread(&bmpInfoHeaderData, sizeof(ClBitMapInfoHeader), 1, pFile);
		if (bmpInfoHeaderData.biBitCount == 8)// grayscale format
		{
			
			channels = 1;

			//
			fread(&colorMap, sizeof(ClrgbMap), 256, pFile);
		}
		else if (bmpInfoHeaderData.biBitCount == 24)// RGB format
		{
			channels = 3;
		}
		// Get offset of every scanline,length(scanline)=length(pixel)+offset 
		int offset = 0;
		//length of row of image in bytes
		int linelength = bmpInfoHeaderData.biWidth * channels;

		/*
		computer read 32k bytes(k\in Z) bytes length of data one time; if a line of data is not integer times of 
		32 bytes, computer will add additional 0 at the end of scan
		*/
		offset = linelength % 4;
		if (offset > 0)
		{
			offset = 4 - offset;
		}
		// Read Pixel
		uint8_t pixVal;
		for (int i = 0; i < bmpInfoHeaderData.biHeight; i++)
		{
			for (int j = 0; j < linelength; j++)
			{
				fread(&pixVal, sizeof(uint8_t), 1, pFile);

				//actual image data
				imgData.push_back(pixVal);

			}
			for (int k = 0; k < offset; k++)
			{
				fread(&pixVal, sizeof(uint8_t), 1, pFile);
			}
		}
		//hhh no notification
		vector<uint8_t> t_rgb;
		//qDebug()<<"imgData"<<imgData;
		for(unsigned int i=0;i<imgData.size();i++){
			t_rgb.push_back(imgData[i]);
			if((i+1)%3==0){
				RGB rgb_o = RGB(t_rgb[2],t_rgb[1],t_rgb[0]);
				YCbCr ycbcr_o = RGBToYCbCr(rgb_o);
				QDebug q = qDebug();
				//q<<"ycbcr_o.Y"<<ycbcr_o.Y;
				//q<<"ycbcr_o.Cb"<<ycbcr_o.Cb;
				//q<<"ycbcr_o.Cr"<<ycbcr_o.Cr;				
				y_ele_img.push_back(uint8_t(ycbcr_o.Y));
				cb_ele_img.push_back(uint8_t(ycbcr_o.Cb));
				cr_ele_img.push_back(uint8_t(ycbcr_o.Cr));
				//qDebug()<<ycbcr_o.Y;
				t_rgb.clear();
			}
		}
	}
	else
	{
		return false;
	}

	fclose(pFile);
	return true;
}






bool ClImgBMP::SaveImage(const char* path)
{
	FILE* pFile;
	pFile = fopen(path, "wb");
	if (!pFile)
	{
		return 0;
	}

	// Processing
	fwrite(&bmpFileHeaderData, sizeof(ClBitMapFileHeader), 1, pFile);
	fwrite(&bmpInfoHeaderData, sizeof(ClBitMapInfoHeader), 1, pFile);
	// Get Channel num of a pixel
	int channels = 0;
	if (bmpInfoHeaderData.biBitCount == 8)
	{
		channels = 1;
		fwrite(&colorMap, sizeof(ClrgbMap), 256, pFile);
	}
	else if (bmpInfoHeaderData.biBitCount == 24)
	{
		channels = 3;
	}
	
	// Get offset of every scanline,length(scanline)=length(pixel)+offset
	int offset = 0;
	int linelength = bmpInfoHeaderData.biWidth * channels;
	offset = (channels * bmpInfoHeaderData.biWidth) % 4;
	if (offset > 0)
	{
		offset = 4 - offset;
	}
	// Write Pixel
	uint8_t pixVal;
	auto iter = imgData.begin();
	for (int i = 0; i < bmpInfoHeaderData.biHeight; i++)
	{
		for (int j = 0; j < linelength; j++)
		{
			pixVal = *iter;
			fwrite(&pixVal, sizeof(uint8_t), 1, pFile);
			iter += 1;
		}
		pixVal = 0;
		for (int k = 0; k < offset; k++)
		{
			fwrite(&pixVal, sizeof(uint8_t), 1, pFile);
		}
	}
	fclose(pFile);
	return true;
}

