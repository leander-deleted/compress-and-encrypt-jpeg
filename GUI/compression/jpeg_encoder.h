#ifndef __JPEG_ENCODER_HEADER__
#define __JPEG_ENCODER_HEADER__


class JpegEncoder
{
public:
	/** �������� */
	void clean(void);

	/** ��BMP�ļ��ж�ȡ�ļ�����֧��24bit��������8�ı������ļ� */
	bool readFromBMP(const char* fileName);

	/** ѹ����jpg�ļ��У�quality_scale��ʾ������ȡֵ��Χ(0,100), ����Խ��ѹ������Խ��*/
	bool encodeToJPG(const char* fileName, int quality_scale);

private:
	//image width
	int				m_width;
	//image height
	int				m_height;
	//original 1-d image stream
	unsigned char*	m_rgbBuffer;

	//y value  table
	unsigned char	m_YTable[64];

	unsigned char	m_CbCrTable[64];

	//bit representation of non-zero value
	struct BitString
	{
		//length of huffman code
		int length;	
		//huffman code value
		int value;
	};
	
	// Y value DC standard  huffman table 
	BitString m_Y_DC_Huffman_Table[12];

	// Y value AC standard huffman table
	BitString m_Y_AC_Huffman_Table[256];

	//CB CR standard DC huffman table
	BitString m_CbCr_DC_Huffman_Table[12];

	// CBCR standard AC huffman table
	BitString m_CbCr_AC_Huffman_Table[256];

private:
	void _initHuffmanTables(void);
	void _initCategoryAndBitcode(void);
	void _initQualityTables(int quality);
	void _computeHuffmanTable(const char* nr_codes, const unsigned char* std_table, BitString* huffman_table);
	BitString _getBitCode(int value);

	void _convertColorSpace(int xPos, int yPos, char* yData, char* cbData, char* crData);
	void _foword_FDC(const char* channel_data, short* fdc_data);
	void _doHuffmanEncoding(const short* DU, short& prevDC, const BitString* HTDC, const BitString* HTAC, 
		BitString* outputBitString, int& bitStringCounts);

private:
	void _write_jpeg_header(FILE* fp);
	//value: value to be written
	void _write_byte_(unsigned char value, FILE* fp);
	void _write_word_(unsigned short value, FILE* fp);

	void _write_bitstring_(const BitString* bs, int counts, int& newByte, int& newBytePos, FILE* fp);
	void _write_(const void* p, int byteSize, FILE* fp);

public:
	JpegEncoder();
	~JpegEncoder();
};

#endif
