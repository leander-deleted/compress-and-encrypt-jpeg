
/*
DCT：
2D-DCT变换，通过变换公式
从一个域通过频率映射到频率域
是从一个matrix map to another matrix

图片被重新架构，使用IDCT inverser DCT,
每一个块的元素是像素

DCT 被设计作用在范围在[-128,127)的元素上面，需要将[0,255)的元素减128
得到的8*8的块，(0,0)对应图片块最低频的点，(7,7)对应最高频的部分，人眼对低频的点更加的敏感


量化：
8*8 的DCT 系数通过量化表去压缩，根据不同程度的压缩，和质量，通过不同的压缩量化矩阵实现，压缩质量的选择[1,100],1对应最高的压缩率，最差的解压缩图片质量
100对应最小的压缩率，最好的图片质量。压缩量化表有通过实验的得到的矩阵

Q50的标准矩阵已知，想要Q>50的压缩矩阵，通过标准矩阵乘以（100-Q）/50,想要Q<50的压缩矩阵，(50-Q)/50

量化，通过用转变后的图像矩阵的每一元素除以压缩矩阵的每一个对应位置的元素，以8*8的块为作用单位；
将结果约到最近的整数

获得的量化转化的矩阵，左上角为非0的元素，对应低频的元素的值，高频元素被转化成0，0要被舍弃，是有损压缩的丢失部分，只有有数字的部分才会被用于图像的重建
Q10会得到更多的0，Q90会得到更少的0，0能够减少存储空间

编码：
通过z型遍历，将0都紧密在一起，将C的所有元素编码成一个流的二进制数字

解压缩：
先解码，将二进制的流，转化成相对应的C矩阵，C矩阵中的每一个元素乘以量化表的相对应元素，获得矩阵R，

对R矩阵进行IDCT，并进行高斯约进，获得另一个矩阵R'，再对R'的每一个元素加128，获得压缩完毕的图片像素矩阵，将像素矩阵读出Jpeg文件格式

黑白图片，像素只有两个值0 black,1 white，use 1 bit to store
灰度图片，只存一个像素值，8bits，表示亮度[0,255],0是最黑，255是最亮; 对于rgb存储的图片，r=g=b都是同一个值
颜色图片，亮度变化对于三个颜色组成成分，红绿蓝，最后三个组成颜色组成进行混合得到不同颜色的图片

图片存储通过矩阵，有长宽，不同的长宽对应不同尺寸的图片

如果不去解压缩，直接读取Jpeg压缩文件，应该是乱码

problem:
怎么将得到的矩阵，输出成jpeg格式文件？

*/
#include<iostream>
#include <fstream>
#include <cmath>
using namespace std;
#define pi 3.142857

//block width
#define M 8

//block height
#define N 8


class RGB{
public:
	unsigned char R;
	unsigned char G;
	unsigned char B;

	RGB(unsigned char a,unsigned char b,unsigned char c){
		R = a;
		G = b;;
		B = c;
	}

	bool operator == (RGB & rgb){
		return (R == rgb.R&& G==rgb.G && B==rgb.B);
	}
};

class YCrCb{
public:
	float Y;
	float Cr;
	float Cb;

	YCrCb(float a,float b,float c){
		Y = a;
		Cr = b;
		Cb = c;
	}

	bool operator == (YCrCb & ycrcb){
		return (Y == ycrcb.Y&& Cr==ycrcb.Cr && Cb==ycrcb.Cb);
	}

	void convert(RGB & rgb){
		float fr = (float)rgb.R / 255;
		float fg = (float)rgb.G / 255;
		float fb = (float)rgb.B / 255;
	}
};


int Q50[M][N] = {
	{16,11,10,16,24,40,51,61},
	{12,12,14,19,26,58,60,55},
	{14,13,16,24,40,57,69,56},
	{14,17,22,29,51,87,80,62},
	{18,22,37,56,68,109,103,77},
	{24,35,55,64,81,104,113,92},
	{49,64,78,87,103,121,120,101},
	{72,92,95,98,112,100,103,99}
};


unsigned char * readBmpFile(){
	FILE * f;
	f = fopen("./g1.bmp","rb");
	if(f==NULL){
		std::cout<<"error";
	}
	else{
		unsigned char info[54];
		fread(info,sizeof(unsigned char),54,f);
		int width = *(int *)&info[18];
		int height = *(int *)&info[22];
		std::cout<<"width is "<<width<<"heigth is :"<<height;
		int size = 3*width*height;
		unsigned char *data = new unsigned char [size];
		fread(data,sizeof(unsigned char),size,f);
		fclose(f);
		for(int i=0;i<size;i=i+3){
			unsigned char temp = data[i];
			data[i] = data[i+2];
			data[i+2] = temp;
		}
		// for(int i =0;i<size;i++){
		// 	std::cout<<data[i];
		// }
		return data;
	}
}

/*
40*40 dimesion byte string convert char bytes string to 8*8 array
40pi * 40 pi
*/
void convertToBlockArr(unsigned char * data,unsigned char block[][M][N]){
	// index from 0-24
	for (int i = 0;i<25;i++){

		//index from 0-7 row
		for (int j = 0;j<8;j++){

			//index from 0-7 column
			for(int k = 0;k<8;k++){
				block[i][j][k] = data[40*j+8*(i%5)+k]-128;
			}
		}
	}
}


// Function to find discrete cosine transform and print it
//matrix: source image
int dctTransform(unsigned char matrix[][N])
{
	int i, j, k, l;

	// dct: store the data after discrete cosine transform
	float dct[M][N];

	float ci, cj, dct1, sum;

	//i: index on row
	for (i = 0; i < M; i++) {
		//j: index on column
		for (j = 0; j < N; j++) {
			// ci and cj depends on frequency as well as
			// number of row and columns of specified matrix
			if (i == 0)
			ci = 1 / sqrt(M);
			else
			ci = sqrt(2) / sqrt(N);
			if (j == 0)
			cj = 1 / sqrt(N);
			else
			cj = sqrt(2) / sqrt(N);

			// sum: store the sum of result value  of formula on m*n dimension matrix
			sum = 0;

			//k: index on row
			for (k = 0; k < M; k++) {
				//l: index on column
				for (l = 0; l < N; l++) {
					//store result of formula
					dct1 = matrix[k][l] * 
					cos((2 * k + 1) * i * pi / (2 *M)) * 
					cos((2 * l + 1) * j * pi / (2 * N));
					sum = sum + dct1;
				}
			}

			//cosine signal value at (i,j) position 
			dct[i][j] = int(ci * cj * sum);
		}	
	}

	/*
	cout<<endl<<"one 8*8block"<<endl;
	for (i = 0; i < M; i++) {
		//cout<<"dct[]"<<endl;
		for (j = 0; j < N; j++) {
			cout<<dct[i][j]<<",";
		}
			cout<<"\n";
	}
	*/
}

//do quantization 
void quantization(unsigned char data[M][N],int degree = 50){
	for(int j=0;j<M;j++){
		for(int k=0;k<N;k++){
			data[j][k] =  data[j][k]/Q50[j][k];
			//cout<<"j="<<j<<"k="<<k<<endl;
		}
	}
}


void show(unsigned char data[][M][N]){
	for(int i =0;i<25;i++){
		cout<<endl<<"8*8 block"<<endl;
		for(int j=0;j<M;j++){
			for(int k=0;k<N;k++){
				std::cout << data[i][j][k];
			}
		}
	}
}

int main(int argc,char **argv){
	unsigned char block[25][M][N];
	unsigned char * data = readBmpFile();
	convertToBlockArr(data,block);
	for(int i =0;i<25;i++){
		dctTransform(block[i]);
		quantization(block[i]);
	}
	//show(block);
	return 0;
}

