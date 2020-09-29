#ifndef _BIG_INT_
#define _BIG_INT_

#define NON_NEGATIVE	0
#define WORD_BIT_LEN	8	// 8 or 32 or 64

#if WORD_BIT_LEN == 8
typedef unsigned char word;
#elif WORT_BIT_LEN == 32
typedef unsigned int word;
#else
typedef unsigned long long word;
#endif

typedef struct {
	int sign;
	int wordlen;
	word* a;
}bigint;

void BI_Delete(bigint** x);

//
void BI_New(bigint** x, int wordlen);

void BI_Show_Bin(bigint** x);

void BI_Show_Dec(bigint** x);

void BI_Show_Hex(bigint** x);

//Chapter2.8 Set One, Set Zero, Is Zero, Is One
void BI_Set_One(bigint** x);

void BI_Set_Zero(bigint** x);

void Is_One(bigint** x);

void Is_Zero(bigint** x);
#endif