#ifndef __MYENCRYPT__
#define __MYENCRYPT__

#include <cstdio>
#include "common.h"

#define __CSTYLE__
// #include "../logger/Logger.h"

class MyEncrypt{
private:
	static const int BUF_SIZE = 1024;
	static int m_instance;
	// static Logger *m_logger;
	int m_encType;

public:
	// ��������
	enum ET{ET_BYTE_SWITCH};
	// declaration: Ĭ�Ϲ��캯��
	MyEncrypt(MyEncrypt::ET encType = MyEncrypt::ET_BYTE_SWITCH);
	// declaration: ��������
	~MyEncrypt();
	
	void process(const BYTE *str, const BYTE_SIZE_T inLen, BYTE *outBuf, const BYTE_SIZE_T maxOutLen);
	// void process(const FILE *inFile, FILE *outFile);
	void process(const char *inFileName, const char *outFileName);
	
protected:
	void byteSwitch(const BYTE *str, const BYTE_SIZE_T inLen, BYTE *outBuf, const BYTE_SIZE_T maxOutLen);
	// void byteSwitch(const FILE *inFile, FILE *outFile);
	void byteSwitch(const char *inFileName, const char *outFileName);
};
#endif
