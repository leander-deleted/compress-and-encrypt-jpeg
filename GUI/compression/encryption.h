#pragma once
#include <stdio.h>
#include <iostream>
using namespace std;
#include <vector>
#include<fstream>
#include<time.h>
#define HEADER_LENGTH 607
#include <QDebug>
class RC4 { 

	public:  
	    //state vector
	    unsigned char S[256]; //状态向量，共256字节 
	    //temporary vector 
	    unsigned char T[256]; //临时向量，共256字节  
	    //length of key stream to encrpyt plain text

	    int keylen;        //密钥长度，keylen个字节，取值范围为1-256  

	    //k: key stream
   		vector<uint8_t> k;  
   		 
	    //K: initial random generated key stream
	    vector<uint8_t> K;      //可变长度密钥   
	  
	    /* 
	        initialize state vector S and temporary vector T
	    */  
	    void initial();
	    /* 
	        initialize the order in the state vector S
	    */  
	    void rangeS();
	    /* 
	        generate key stream
	        len:length of plain text
	    */  
	    void keyStream(int len);  
	 };

class Encrpytor{
public:
	//encrpytion data 
	vector <uint8_t> e_data;
	//decrpytion
	vector <uint8_t> d_data;
	bool readJegData(const char* path);
	bool encrpyt();
	bool decrpyt();
	//mode: 0: write encrpytion jpeg, 1: write decrpyted jpeg
	bool writeToJpeg(int mode);
private:

	uint8_t header[607];
	//original binary data  of jpeg file

    vector <uint8_t> data;
    RC4  r;
};
