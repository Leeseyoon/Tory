#include "bitlen.h"

void Bit_Len(int* len, int x)
{
	int i = 0;
	while (1)
	{
		if (x == 1 | x == 0)
		{
			i += 1;
			break;
		}
		else
		{
			i += 1;
			x = x >> 1;
		}
	}
	*len = i;
}

int IsZero(int x) 
{

}

int IsOne(int x)
{
}