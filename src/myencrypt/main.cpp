#include <cstdio>
#include <cstring>

#include "myencrypt.h"

int main(int argc, char **argv){

	if(2 != argc && 3 != argc){
		printf("Version: 1.0 2011-03-25\r\n");
		printf("Author: Michael Hu\r\n");
		printf("Usage: myencrypt infile outfile\r\n");
		printf("       myencrypt instr\r\n");
		return 1;
	}
	
	MyEncrypt myEncrypt(MyEncrypt::ET_BYTE_SWITCH);
	/**
	 * 1. 以下方式会直接调用默认构造函数
	 *    MyEncrypt myEncrypt;
	 * 2. 以下方式会直接调用带参数的默认构造函数
	 *    MyEncrypt myEncrypt(MyEncrypt::ET_BYTE_SWITCH);
	 * 3. 如果不传参数，则不能按以下方式定义，否则会认为调用一个未定义的函数
	 *    MyEncrypt myEncrypt();
	 */
	if(3 == argc){
		// 文件编码
		myEncrypt.process(argv[1], argv[2]);
	}
	else{
		// 字符串编码
		int len = strlen(argv[1]);
		char *buf = new char[len + 1];
		myEncrypt.process(argv[1], strlen(argv[1]), buf, len+1);
		puts(buf);
		// 配对动态内存清理
		delete [] buf;
	}

	// char *str = "\n";
	// char buf[30];
	
	// myEncrypt.process(str, strlen(str), buf, 30);
	
	// myEncrypt.process("E:\\log\\infile.txt", "E:\\log\\outfile.txt");
	// myEncrypt.process("E:\\log\\outfile.txt", "E:\\log\\afile.txt");
}