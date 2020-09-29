#include "bigint.h"
#include "bitlen.h"
#include <stdio.h>

void BI_Delete(bigint** x)
{
	if (*x == NULL)
		return;

#ifdef ZERORIZE
	array_init((*x)->a, (*x)->wordlen);
#endif

	free((*x)->a);
	free(*x);
	*x = NULL;
}

void BI_New(bigint** x, int wordlen)
{
	if (*x != NULL)
		BI_Delete(x);

	*x = (bigint*)malloc(sizeof(bigint));
	(*x)->sign = NON_NEGATIVE;
	(*x)->wordlen = wordlen;
	(*x)->a = (word*)calloc(wordlen, sizeof(word));
}

void BI_Show_Bin(bigint** x)
{

}

void BI_Show_Dec(bigint** x) 
{
}

void BI_Show_Hex(bigint** x) 
{
}

// Chapter2.8 Set One, Set Zero, Is Zero, Is One
void BI_Set_One(bigint** x) 
{
	BI_New(x, 1);
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x01; // 0x1
}
void BI_Set_Zero(bigint** x)
{
	BI_New(x, 1);
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x00;
}

int Is_One(bigint** x)
{
	int i = 0;
	int* size = 0;
	Bit_Len(size, x);

	if ((*x)->sign == 1 && (*x)->a[0] != 0)
		return -1; // False
	for (i = size - 1; i > 0, i--)
	{
		if
	}
	else
		return 0;

}
void Is_Zero(bigint** x)
{

}
