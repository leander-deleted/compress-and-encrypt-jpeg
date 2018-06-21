#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;
void test(){
	FILE * ipk;
	uint8_t *tem;
	//d_data.clear();
	ipk = fopen("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\keystream.dat","rb");

	for(int i=0;i<100;i++){
		fread((void *)tem,sizeof(uint8_t),1,ipk);
		// cout<<e_data[i]<<endl;
		// d_data.push_back((*tem)^e_data[i]);
	}
	// char * tem = new char;
	// ifstream f;
	// f.open("C:\\Users\\fangl\\Desktop\\lecture_ppt\\cryptography\\cryptograph-ex\\keystream.dat",ios::binary);
	// if(f.is_open()){
	// 	cout<<"open";
	// }
	// f.seekg (0, f.end);
 //    int length = f.tellg();
 //    f.seekg (0, f.beg);
 //    cout<<length;
	// f.read(tem,length);
	// cout<<endl<<int(tem[0]);
}
int main(){
	test();
}