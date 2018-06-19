#ifndef RGB_H
#define RGB_H
#include<stdint.h>
class RGB{
public:
	uint8_t R;
	uint8_t G;
	uint8_t B;

	RGB(uint8_t r, uint8_t g, uint8_t b);
	
	bool Equals(RGB &rgb);
};


class YCbCr{
public:
	float Y;
	float Cb;
	float Cr;

	YCbCr(float y, float cb, float cr);

	bool Equals(YCbCr &ycbcr);

};

YCbCr RGBToYCbCr(const RGB &rgb) ;

#endif