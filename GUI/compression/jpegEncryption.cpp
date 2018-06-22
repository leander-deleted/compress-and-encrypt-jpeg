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
#include "encryption.h"
bool Encrpytor::readJegData(const char* path){
	FILE * f;
	uint8_t temp0=0;
	//uint8_t temp1 = 0;
	//vector <uint8_t> data;
	f = fopen(path,"rb");
	data.clear();

	if(f==0){
		qDebug()<<"fail to open";
		return false;
	}
	else{
		int result = fread(header,1,HEADER_LENGTH,f);
		if(result!=HEADER_LENGTH){
			qDebug()<<"fail to read header";
			return false;
		}
		while(true){
			if(fread(&temp0,1,1,f)){
				//image data read in
				data.push_back(temp0);
				//temp1 = temp0;
			}
			else{
				qDebug()<<"finish reading";
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
	 

    //生产密钥流  
    this->r.keyStream(data.size()); 
 	
 	ofstream outks; 

 	//read the key stream in 
    outks.open("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\keystream.dat",ios::binary);  
    for(int i=0;i<data.size();++i){  
        outks<<(r.k[i]);  
    }  
    outks.close(); 

    e_data.clear();
	// index on plain text
    for(int i=0;i<data.size();++i){ 
    	//r.k key stream  
        e_data.push_back((unsigned char)(data[i]^r.k[i]));  
    }   

	return true;
}

bool Encrpytor::decrpyt(){
	FILE * ipk;
	uint8_t tem[111111];
	d_data.clear();
	ipk = fopen("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\keystream.dat","rb");
	// if(e_data.size()==0){
	// 	cout<<"e_data is empty";
	// 	return false;
	// }
	fseek(ipk,0,SEEK_END);
	int size = ftell(ipk);
	fseek(ipk,0,SEEK_SET);
	qDebug()<<"read key stream file";
	fread(tem,sizeof(uint8_t),size,ipk);

	for(int i=0;i<data.size();i++){
		//GUI mode
		//data: the encrypted image data
		//d_data.push_back(tem[i]^data[i]);	
		//console mode	
		d_data.push_back(tem[i]^e_data[i]);
	}
	return true;
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
			cout<<"success write header";
			for(int i=0;i<data.size();i++){
				fwrite(&d_data[i],1,1,outf);
			}
			cout<<"write content";
			if(fwrite(&ending,2,1,outf)!=1){
				cout<<"write ending marker fail"<<endl;	
				return false;		
			}
			cout<<endl<<"ok write decrypted image";
			return true;
		}
	}
}



void RC4::initial() {  
	srand((unsigned)time(NULL));  
	keylen = 16;
    for(int i=0;i<keylen;++i){  //随机生产长度为keylen字节的密钥  
        int tmp=rand()%256;  
   		// K is initial random key stream
        K.push_back(uint8_t(tmp));  
    }  
    for(int i=0;i<256;++i){  
        S[i]=i;  
        T[i]=K[i%keylen];  
    }  
}  
/* 
    initialize the order in the state vector S
*/  
void RC4::rangeS() {  
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
void RC4::keyStream(int len) {  
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


/*
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

	// e.decrpyt();
	if(e.writeToJpeg(1)){
		cout<<"success";
	}
	else{
		cout<<"fail";
	}
	return 0;
}
*/
