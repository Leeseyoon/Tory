#include <stdio.h>
#include "bitlen.h"
#include "bigint.h"


int main() 
{
	int x = 1024;

	int* y;


	Bit_Len(&y, x);
	printf("Bit Length of %d : %d\n", x, y);
	y = ((int)y / WORD_BIT_LEN) + 1;
	printf("Word Length of %d : %d\n", x, y);
	//BI_New();
	return 0;
}