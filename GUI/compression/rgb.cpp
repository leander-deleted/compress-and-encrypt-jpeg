#include "rgb.h"
#include <iostream>
RGB::RGB(uint8_t r, uint8_t g, uint8_t b){
	R = r;
	G = g;
	B = b;
}

bool RGB::Equals(RGB &rgb){
	return (R == rgb.R) && (G == rgb.G) && (B == rgb.B);
}


YCbCr::YCbCr(float y, float cb, float cr){
	Y = y;
	Cb = cb;
	Cr = cr;
}

bool YCbCr::Equals(YCbCr &ycbcr){
	return (Y == ycbcr.Y) && (Cb == ycbcr.Cb) && (Cr == ycbcr.Cr);
}

YCbCr RGBToYCbCr(const RGB &rgb) {
	/*
	float fr = (float)rgb.R / 255;
	float fg = (float)rgb.G / 255;
	float fb = (float)rgb.B / 255;
	
	float Y = (0.2989 * fr + 0.5866 * fg + 0.1145 * fb);
	float Cb = (0.1687 * fr - 0.3313 * fg + 0.5000 * fb);
	float Cr = (float)(0.5000 * fr - 0.4184 * fg - 0.0816 * fb);
	*/
	//+128 to convert to unsigned integer
	uint8_t Y = (0.2989f * rgb.R + 0.5866f * rgb.G + 0.1145f * rgb.B-128) ;
	uint8_t Cb = (-0.1687 * rgb.R - 0.3313 * rgb.G + 0.5000 * rgb.B);
	uint8_t Cr = 0.5000 * rgb.R - 0.4184 * rgb.G - 0.0816 * rgb.B ;
	return YCbCr(Y, Cb, Cr);
}
/*

int main(){
	RGB data = RGB(181,181,179);
	YCbCr value = RGBToYCbCr(data);
	std::cout <<"Y :"<<value.Y<<std::endl<<"Cb:"<<value.Cb<<std::endl<<"Cr:"<<value.Cr;
}
*/