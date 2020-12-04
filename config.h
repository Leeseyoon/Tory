#ifndef _CONFIG_
#define _CONFIG_

#define NON_NEGATIVE	0
#define NEGATIVE		1

#define SUCCESS			0
#define ERROR			-1

#define TRUE			0
#define FALSE			-1

#define WORD_BIT_LEN	32   // 8 or 32 or 64

#define KARATSUBA		10
#define TEXTBOOK		11
#define BINARY_LONG		12
#define MULTI_PRE		13
#define LTOR			14
#define RTOL			15
#define MONTGOMERY		16

#define FLAG			3	

#define TEST_COUNT		10
#define TEST_SIZE		SIZE_2048

#define FLAG_MUL			KARATSUBA	// Multiplication flag: KARATSUBA, TEXTBOOK
#define FLAG_DIV			MULTI_PRE	// Division flag: BINARY_LONG, MULTI_PRE
#define FLAG_SQU			TEXTBOOK	// Squaring flag: KARATSUBA, TEXTBOOK
#define FLAG_EXP			MONTGOMERY		// Modular exponentiation flag: LTOR, RTOL, MONTGOMERY

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#if WORD_BIT_LEN == 8
typedef unsigned char word;
#define word_mask 0xff
#define SIZE_1024 128
#define SIZE_2048 256

#elif WORD_BIT_LEN == 32
typedef unsigned int word;
#define word_mask 0xffffffff
#define SIZE_1024  32
#define SIZE_2048  64

#else
typedef unsigned long long word;
#define word_mask 0xffffffffffffffff
#define SIZE_1024  16
#define SIZE_2048  32

#endif

typedef struct {
	int sign;
	int wordlen;
	word* a;
} bigint;

#endif
