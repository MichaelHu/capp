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
	 * 1. ���·�ʽ��ֱ�ӵ���Ĭ�Ϲ��캯��
	 *    MyEncrypt myEncrypt;
	 * 2. ���·�ʽ��ֱ�ӵ��ô�������Ĭ�Ϲ��캯��
	 *    MyEncrypt myEncrypt(MyEncrypt::ET_BYTE_SWITCH);
	 * 3. ����������������ܰ����·�ʽ���壬�������Ϊ����һ��δ����ĺ���
	 *    MyEncrypt myEncrypt();
	 */
	if(3 == argc){
		// �ļ�����
		myEncrypt.process(argv[1], argv[2]);
	}
	else{
		// �ַ�������
		int len = strlen(argv[1]);
		char *buf = new char[len + 1];
		myEncrypt.process(argv[1], strlen(argv[1]), buf, len+1);
		puts(buf);
		// ��Զ�̬�ڴ�����
		delete [] buf;
	}

	// char *str = "\n";
	// char buf[30];
	
	// myEncrypt.process(str, strlen(str), buf, 30);
	
	// myEncrypt.process("E:\\log\\infile.txt", "E:\\log\\outfile.txt");
	// myEncrypt.process("E:\\log\\outfile.txt", "E:\\log\\afile.txt");
}