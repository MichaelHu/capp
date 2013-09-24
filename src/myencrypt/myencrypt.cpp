/**
 * 加密程序
 * 
 * 2010-03-25 the original version 
 * 2010-04-10 process file content in block-buffer
 */
#include <cstdio>
#include <cstdlib>
#include "memory.h"

#include "myencrypt.h"

#define __CSTYLE__
// #include "../logger/Logger.h"

// 静态类成员必须进行初始化，否则可能link不了
int MyEncrypt::m_instance = 0;
// Logger *MyEncrypt::m_logger = NULL;

MyEncrypt::MyEncrypt(MyEncrypt::ET encType){
	m_encType = encType;
	m_instance++;
	// m_logger = Logger::getInstance("e:\\Log\\myencrypt_log", Logger::SUFFIX_PER_HOUR);
}

MyEncrypt::~MyEncrypt(){}

void MyEncrypt::process(const BYTE *str, const BYTE_SIZE_T inLen, 
	BYTE *outBuf, const BYTE_SIZE_T maxOutLen){
	
	switch(m_encType){
		case MyEncrypt::ET_BYTE_SWITCH:
			byteSwitch(str, inLen, outBuf, maxOutLen);
			// m_logger->debug(str);
			// m_logger->debug(outBuf);
			break;
		default:
			byteSwitch(str, inLen, outBuf, maxOutLen);
			break;
	}
	
}

void MyEncrypt::process(const char *inFileName, const char *outFileName){

	switch(m_encType){
		case MyEncrypt::ET_BYTE_SWITCH:
			byteSwitch(inFileName, outFileName);
			break;
		default:
			byteSwitch(inFileName, outFileName);
			break;
	}

}

void MyEncrypt::byteSwitch(const BYTE *str, const BYTE_SIZE_T inLen, 
	BYTE *outBuf, const BYTE_SIZE_T maxOutLen){
	
	memset((void *)outBuf, 0, maxOutLen);
	for(int i=0; i<inLen && i<maxOutLen; i++){
		outBuf[i] = (BYTE)str[i] >> 4 & 0x0F;
		outBuf[i] = (BYTE)outBuf[i] | (BYTE)str[i] << 4;
	}
	
}

void MyEncrypt::byteSwitch(const char *inFileName, const char *outFileName){
	FILE *inFile, *outFile;
	inFile = fopen(inFileName, "rb");
	outFile = fopen(outFileName, "wb");
	
	BYTE b[BUF_SIZE], 
		b_1[BUF_SIZE];
	int pos = 0, count;
	fseek(inFile, 0, SEEK_SET);
	fseek(outFile, 0, SEEK_SET);
		
	// 无损编码，需要保证二进制安全，所以需要使用fread/fwrite函数对
	while( BUF_SIZE == (count = fread(b, 1, BUF_SIZE, inFile)) ){
	
		byteSwitch(b, BUF_SIZE, b_1, BUF_SIZE); 
		
		if(BUF_SIZE != fwrite(b_1, 1, BUF_SIZE, outFile)){
			printf("write error!!");
			abort();
		}
		
		if(fseek(inFile, pos + BUF_SIZE, SEEK_SET)){
			if( ferror( inFile ) ){
				printf("seek error!");
				abort();
			}
			
			if( feof( inFile ) ){
				printf("end of file !");
			}
		}
		
		fseek(outFile, 0, SEEK_END);
		pos += BUF_SIZE;
		
	}
		
	if(ferror(inFile)){
		printf("read error!");
		abort();
	}
	
	if(feof(inFile) && count < BUF_SIZE && count > 0){
		byteSwitch(b, count, b_1, count); 
		if(count != fwrite(b_1, 1, count, outFile)){
			printf("write error!!");
			abort();
		}
	}
	
	fclose(inFile);
	fclose(outFile);
}
