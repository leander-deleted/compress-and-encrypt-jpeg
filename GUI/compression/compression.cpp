
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

获取Bmp图片的长宽
int widthh = *(int*)&info[18]
int height = *(int *)&info[22]

如果不去解压缩，直接读取Jpeg压缩文件，应该是乱码

problem:
怎么将得到的矩阵，输出成jpeg格式文件？

*/


/*
40*40 dimesion byte string convert char bytes string to 8*8 array
40pi * 40 pi
*/

#include "compression.h"

//i_data: y_ele_data or cb_ele_data or cr
bool Compressor::convertToBlock(vector<uint8_t> i_data, int width,int height){
	// index from 0-24
	this->width = width;

	//qDebug()<<"width paramter"<<width;
	//qDebug()<<"member width"<<this->width;
	this->height = height;
	count = (width/N)*(height/M);

	//qDebug()<<"count"<<this->width;
	//temp vector for row of 8*8 block

	vector<uint8_t> vr;
	vector<Dim1> vb;
	int rowBlockCount = width/N;
	//qDebug()<<"y_ele_data"<<i_data;
	//i: index of count of block
	for (int i = 0;i<count;i++){

		//index from 0-7 row of each block
		for (int j = 0;j<M;j++){

			//index from 0-7 column of 8*8 block
			for(int k = 0;k<N;k++){
				vr.push_back(i_data[(trunc(i/5)*N+j)*width+k]);
			}
			//qDebug()<<"vr:"<<vr;
			vb.push_back(vr);
			vr.clear();
		}
		Imgblock.push_back(vb);
		vb.clear();
	}
	return true;
}

// Function to find discrete cosine transform and print it
//matrix: source image
bool Compressor::dctTransform()
{
	int i, j, k, l;

	vector<float> vr;
	vector<vector<float>> vb;
	float ci, cj, dct1, sum;
	qDebug()<<"enter dctTransform";
	for (int c=0;c<count;c++){

		//i: index on row
		for (i = 0; i < M; i++) {

			//j: index on column
			for (j = 0; j < N; j++) {

				// ci and cj depends on frequency as well as
				// number of row and columns of specified matrix
				if (i == 0)
					ci = float(1 / sqrt(2));
				else
					ci = 1;
				if (j == 0)
					cj = float(1 / sqrt(2));
				else
					cj = 1;

				// sum: store the sum of result value  of formula on m*n dimension matrix
				sum = 0;

				//k: index on row
				for (k = 0; k < M; k++) {
					//l: index on column
					for (l = 0; l < N; l++) {
						//store result of formula
						dct1 = Imgblock[c][k][l] * 
						cos((2 * k + 1) * i * pi / (16.0)) * 
						cos((2 * l + 1) * j * pi / (16.0));
						sum = sum + dct1;
					}
				}

				//cosine signal value at (i,j) position 
				//qDebug()<<"round( (1/sqrt(2N)) *ci * cj * sum)"<<round( (float(1/sqrt(2*N))) *ci * cj * sum);
				vr.push_back( float(1/sqrt(2*N)) *ci * cj * sum);
			}
			qDebug()<<"dct_Imgblock vector:"<<vr;
			vb.push_back(vr);	
			vr.clear();
		}
		dct_Imgblock.push_back(vb);
		vb.clear();
	}
	return true;
}

//do quantization 
void Compressor::quantization(uint8_t degree){
		qDebug()<<"enter quantization";
		vector <Dim1>vb;
		vector <uint8_t>vr;
		//i: index of count
		for(int i=0;i<count;i++){

			//j: 0-M-1, row index of 8*8 block
			for(int j=0;j<M;j++){
				
				//k: index of column of block
				for(int k=0;k<N;k++){
					vr.push_back (round(dct_Imgblock[i][j][k]/Q50[j][k]));
					//cout<<"j="<<j<<"k="<<k<<endl;
				}
				qDebug()<<"vr of quantization:"<<vr;
				vb.push_back(vr);
				vr.clear();
			}
			q_Imgblock.push_back(vb);
			vb.clear();
		}
		//show();
}


void Compressor::zigzagConversion(){
	const uint8_t zigZag[M][N] =
	{ 
		{0, 1, 5, 6,14,15,27,28},
		{2, 4, 7,13,16,26,29,42},
		{3, 8,12,17,25,30,41,43},
		{9,11,18,24,31,40,44,53},
		{10,19,23,32,39,45,52,54},
		{20,22,33,38,46,51,55,60},
		{21,34,37,47,50,56,59,61},
		{35,36,48,49,57,58,62,63} 
	}; 
	uint8_t tem_a[64];
	for(int c=0;c<count;c++){
		for(int i=0;i<M;i++){
			for(int j=0;j<N;j++){
				uint8_t index = zigZag[i][j];
				tem_a[index] = uint8_t(q_Imgblock[c][i][j]);
			}
		}
		for(int k=0;k<64;k++){
			z_stream.push_back(tem_a[k]);
		}
	}
	qDebug()<<z_stream;
	qDebug()<<"size of z_stream"<<z_stream.size();
}


void Compressor::show(vector<uint8_t>data,int s){
	QDebug q = qDebug();
	if(s==1){

		for(int i =0;i<count;i++){
			
			if(i>10 &&i<20){
				//qDebug()<<"8*8 block";
				q<<"8*8 block"<<"\n";
				for(int j=0;j<M;j++){
					for(int k=0;k<N;k++){
						//q<< Imgblock[i][j][k];
						q<< dct_Imgblock[i][j][k];
					}
					q<<"\n";
				}
			}
			
		}
	}
	else if(s==2){
		for(auto ele:data){
			q<<ele;
		}
	}
}


