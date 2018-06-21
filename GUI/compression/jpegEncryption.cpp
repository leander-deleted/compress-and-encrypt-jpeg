/*
soi
2
appo
2+16=18
dqt
2+132=134
sofo
2+17=19
dht
2+418(1a2)=420
sos
2+12=14

154+19+420+14=607

0xFFD9

*/
#include <stdio.h>
#include <iostream>
using namespace std;
#include <vector>
#include<fstream>
#include<time.h>
#define HEADER_LENGTH 607

class Encrpytor{
public:
	bool readJegData(const char* path);
	bool encrpyt();
	bool decrpyt();
	//mode: 0: write encrpytion jpeg, 1: write decrpyted jpeg
	bool writeToJpeg(int mode);
private:

	class RC4 { 

	public:  
	    //kl: key length, length of final key stream to do encryption
	    RC4(int kl);

	    void encryption(const string &,vector<uint8_t>&data,vector<uint8_t>&e_data,vector<uint8_t>&k);  
		bool decryption(vector <uint8_t>&k,vector<uint8_t>&d_data,vector<uint8_t>&e_data);	  	
	private:  
	    //state vector
	    unsigned char S[256]; //状态向量，共256字节 
	    //temporary vector 
	    unsigned char T[256]; //临时向量，共256字节  
	    //length of key stream to encrpyt plain text

	    int keylen;        //密钥长度，keylen个字节，取值范围为1-256  

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
	    void keyStream(int len,vector<uint8_t>&k);  
	 };


	uint8_t header[607];
	//original binary data  of jpeg file

	//k: key stream
   	vector<uint8_t> k;   

    vector <uint8_t> data;
	//encrpytion data 
	vector <uint8_t> e_data;
	//decrpytion
	vector <uint8_t> d_data;
};
bool Encrpytor::readJegData(const char* path){
	FILE * f;
	uint8_t temp0=0;
	uint8_t temp1 = 0;
	//vector <uint8_t> data;
	f = fopen(path,"rb");
	if(f==0){
		cout<<"fail to open";
		return false;
	}
	else{
		int result = fread(header,1,HEADER_LENGTH,f);
		if(result!=HEADER_LENGTH){
			cout<<"fail to read header";
			return false;
		}
		while(true){
			if(fread(&temp0,1,1,f)){
				//cout<<temp0;
				data.push_back(temp0);
				//temp1 = temp0;
			}
			else{
				cout<<"finish reading";
				break;
			}
		}
		//cout<<data.size();
	}
	data.pop_back();
	data.pop_back();
	// for(uint8_t ele:data){
	// 	cout<<int(ele)<<",";
	// }

	return true;
}


bool Encrpytor::encrpyt(){
	/* use simple key do xor encrpytion on each byte
	uint8_t key=0xAA;
	for(int i=0;i<data.size();i++){
		e_data.push_back(data[i]^key);
	}
	for(uint8_t ele:e_data){
		cout<<ele;
	}
	return true;
	*/
	RC4 r(16);
	r.encryption("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\keystream.txt",data,e_data,k);
	return true;
}

bool Encrpytor::decrpyt(){
	/*
	uint8_t key=0xAA;
	for(int i=0;i<data.size();i++){
		d_data.push_back(e_data[i]^key);
	}
	return true;
	*/
	RC4 r(16);
	r.decryption(k,d_data,e_data);
	// cout<<"d_data";
	// for(auto ele:d_data){
	// 	cout<<ele;
	// }
}


bool Encrpytor::writeToJpeg(int mode){
	FILE * outf;
	uint16_t ending= 0xFFD9;
	if(mode==0){
		outf = fopen("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\123-ept.jpeg","wb");
		if(outf==0){
			cout<<"fail to create encrpytion file";
			return false;
		}
		else{
			if(fwrite(header,1,607,outf)!=607){
				cout<<"write header fail"<<endl;
				return false;
			}
			for(int i=0;i<data.size();i++){
				fwrite(&e_data[i],1,1,outf);
			}
			if(fwrite(&ending,2,1,outf)!=1){
				cout<<"write ending marker fail"<<endl;	
				return false;		
			}
			return true;
		}
	}
	else if (mode==1){
		outf = fopen("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\123-decrption.jpeg","wb");
		if(outf==0){
			cout<<"fail to create decrpytion file";
			return false;
		}
		else{
			if(fwrite(header,1,607,outf)!=607){
				cout<<"write header fail"<<endl;
				return false;
			}
			for(int i=0;i<data.size();i++){
				fwrite(&d_data[i],1,1,outf);
			}
			if(fwrite(&ending,2,1,outf)!=1){
				cout<<"write ending marker fail"<<endl;	
				return false;		
			}
			return true;
		}
	}
}


Encrpytor::RC4::RC4(int kl):keylen(kl) {  
    srand((unsigned)time(NULL));  
    for(int i=0;i<kl;++i){  //随机生产长度为keylen字节的密钥  
        int tmp=rand()%256;  
   		// K is initial random key stream
        K.push_back(char(tmp));  
    }  
} 

void Encrpytor::RC4::initial() {  
        for(int i=0;i<256;++i){  
            S[i]=i;  
            T[i]=K[i%keylen];  
        }  
    }  
/* 
    initialize the order in the state vector S
*/  
void Encrpytor::RC4::rangeS() {  
    int j=0;  
    for(int i=0;i<256;++i){  
        j=(j+S[i]+T[i])%256;  
        //cout<<"j="<<j<<endl;  
        S[i]=S[i]+S[j];  
        S[j]=S[i]-S[j];  
        S[i]=S[i]-S[j];  
    }  
}  


//len: length of plain text in byte
void Encrpytor::RC4::keyStream(int len,vector<uint8_t>&k) {  
    initial();  
    rangeS();  
  
    int i=0,j=0,t;  
    while(len--){  
        i=(i+1)%256;  
        j=(j+S[i])%256;  
  
        S[i]=S[i]+S[j];  
        S[j]=S[i]-S[j];  
        S[i]=S[i]-S[j];  
  
        t=(S[i]+S[j])%256;  
        //k: final key stream; the length of key stream is the same with length of plain text
        k.push_back(S[t]);  
    }  
}  
//plaintext:
//ks: name of file which store the final key stream
//ciphertext
void Encrpytor::RC4::encryption(const string &ks,vector<uint8_t>&data,vector<uint8_t>&e_data,vector<uint8_t>&k) {  
	ofstream outks;  

    //生产密钥流  
    keyStream(data.size(),k); 

    outks.open(ks);  
    for(int i=0;i<k.size();++i){  
        outks<<(k[i]);  
    }  

    outks.close();  
  
	// FILE * kf;
	// uint8_t t;
	// kf = fopen("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\keystream.txt","rb");
	// fread(&t,1,1,kf);
	// if(t!=k[0]){
	// 	cout<<"k[0]"<<k[0]<<endl;
	// 	cout<<t;
	// }
	// else{
	// 	cout<<endl<<"k[0] and t equal"<<endl;
	// }


	// index on plain text
    for(int i=0;i<data.size();++i){  
        e_data.push_back((unsigned char)(data[i]^k[i]));  
    }   
  
}  

bool Encrpytor::RC4::decryption(vector<uint8_t>&k,vector<uint8_t>&d_data,vector<uint8_t>&e_data){

	FILE * kf;
	kf = fopen("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\keystream.txt","rb");
	if(kf==0){
		cout<<"fail to open key stream file"<<endl;
		return false;
	}

	uint8_t * temp = new uint8_t [10] ;
	uint8_t t;
	//kf = fopen("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\keystream.txt","rb");
	fread(temp,1,8,kf);
	if(temp[8]=k[8]){
		cout<<"k[8]"<<k[8]<<endl;
		cout<<temp[8];
	}
	else{
		cout<<endl<<"k[0] and t equal"<<endl;
	}

	// for(int i=0;i<k.size();i++){
	// 	if(temp[i]=k[i]){
	// 		//cout<<"not equal at"<<i<<endl;
	// 		cout<<"k"<<i<<","<<int(k[i])<<endl;
	// 	}
	// 	if(i==100){
	// 		break;
	// 	}
		//cout<<"temp"<<temp;
		//d_data.push_back(k[i]^e_data[i]);
		
	//}
	//delete temp;
	return true;
}



int main(){
	Encrpytor e;
	if(e.readJegData("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\12-j.jpeg")){
		cout<<"success";
	}
	else{
		cout<<"fail";
	}

	e.encrpyt();

	if(e.writeToJpeg(0)){
		cout<<"success";
	}
	else{
		cout<<"fail";
	}

	e.decrpyt();

	// // e.decrpyt();
	// if(e.writeToJpeg(1)){
	// 	cout<<"success";
	// }
	// else{
	// 	cout<<"fail";
	// }
	return 0;
}