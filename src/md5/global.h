/* GLOBAL.H - RSAREF types and constants
 */

/* PROTOTYPES should be set to one if and only if the compiler supports
  function argument prototyping.
The following makes PROTOTYPES default to 0 if it has not already
  been defined with C compiler flags.
 */
#ifndef PROTOTYPES
#define PROTOTYPES 0
#endif

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */

/**
 * 64位有多种数据模式，最常见的是LP64和LLP64
 * 1. 许多64位编译器使用 LP64 模型（包括 Solaris、AIX、HP、Linux、Mac OS X、IBM z/OS 原生编译器）
 * 2. 微软的 VC++ 编译器使用 LLP64 模型。
 * 具体查看wikipedia： http://zh.wikipedia.org/wiki/64%E4%BD%8Dcpu#64_.E4.BD.8D.E5.85.83.E8.B3.87.E6.96.99.E6.A8.A1.E5.9E.8B
 *
 * LP64下unsigned long long int和unsigned long int都是64位
 * LLP64下unsigned long long int是64位unsigned long int是32位
 *
 * 所以如果应用程序需要特定位数的整数类型，最好使用C99的uint64_t之类的数据类型。
 */

/* typedef unsigned long int UINT4;*/
typedef unsigned int UINT4;

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it
  returns an empty list.
 */
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif
