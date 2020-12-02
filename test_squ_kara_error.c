#include "bigint.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define   zerorize

int main()
{
	squaring_karatsuba_test();
	return 0;
}

int squaring_karatsuba_test()
{
	int i = 0;
	int size1, size2 = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n�� Squaring Karatsuba ���� �����ϱ� */ // 8, 32, 64 ���� 21 �̻���� �� ��

	printf("\n");
	printf("print(\"Squaring Karatsuba\")\n");
	for (i = 0; i < 1; i++)
	{
		size1 = 22;
		word arr[22] = { 0xe7, 0x1b, 0x31, 0x4e, 0x8d, 0xfa, 0x69, 0x69, 0x2c, 0x8b, 0xa6,
						 0x22, 0xf6, 0x8e, 0xb9, 0xd1, 0x75, 0xb4, 0xa1, 0x47, 0xc1, 0x02 };

		BI_New(&bi_1, size1);
		BI_Set_By_Array(&bi_1, NON_NEGATIVE, arr, size1);

		printf("A = ");
		BI_Show(bi_1, 16);

		len = (bi_1->wordlen + 1) >> 1;
		len = 4 * len + 1;

		BI_New(&bi_re, len);

		SQUC_Karatsuba(&bi_re, bi_1); /********************** 1. ���� Ÿ�� ���� **************************/
		BI_Refine(bi_re);

		printf("A * A == ");
		BI_Show(bi_re, 16);

		printf("\n");

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
	return 0;
}

// Chpater 2.1
void array_init(word* a, int len)
{
	memset(a, 0, sizeof(word) * len); // �迭�� ���̸�ŭ �ʱ�ȭ
}


void BI_Delete(bigint** x)
{
	if (*x == NULL) // bigint ����ü�� �ּҰ� NULL ���̸� ������ �ʿ� ���� ������ return
		return;

#ifdef zerorize
	array_init((*x)->a, (*x)->wordlen); // �迭 �ʱ�ȭ
#endif 
	free((*x)->a); // bigint ����ü ���� word �迭 free
	free(*x); // bigint ����ü free
	*x = NULL; // bigint ����ü�� �ּҰ� NULL�� �ʱ�ȭ
}


void BI_New(bigint** x, int wordlen)
{
	if (*x != NULL) //   x�� �ּҰ� NULL�� �ƴϸ�
		BI_Delete(x); // BI_Delete()�� �ʱ�ȭ

	*x = (bigint*)malloc(sizeof(bigint));
	if (*x == NULL)
		return;

	(*x)->sign = NON_NEGATIVE; // ����̰�
	(*x)->wordlen = wordlen;   // 2��° �Ű������� ���� ������
	(*x)->a = (word*)calloc(wordlen, sizeof(word)); // bigint x->a ����
}

void BI_Set_By_Array(bigint** x, int sign, word* a, int wordlen)
{
	int i;

	(*x)->sign = sign; // bigint ����ü�� ��ȣ ����
	(*x)->wordlen = wordlen; // bigint ����ü�� ���忭 ���� ����

	for (i = 0; i < wordlen; i++)
		(*x)->a[i] = a[i]; // ���� ���� �迭 a�� �ִ� �� bigint ����ü ������ ���� ���� �迭�� ����
}

void BI_Set_By_String(bigint** x, int sign, char* str, word base, int size)
{
	char* hex;
	int i, j, k;

	hex = (char*)malloc(size * sizeof(word)); // �Է¹��� string ���� hex ������ ��ȯ��ų �� ������ ��
	if (hex == NULL)
		return;
	Ascii_To_Hex(str, hex); // �Է¹��� string �� hex�� ��ȯ

	if (base == 2) // 2������ ���� �� (11 01101010 10101111 -> 3 byte �濡 a[2] a[1] a[0])
	{
		if ((strlen(str) % WORD_BIT_LEN) != 0) // �Է¹��� string ���̰� WORD_BIT_LEN�� ����� �ƴϸ�
		{
			for (i = 0; i < (int)(strlen(str) / WORD_BIT_LEN); i++) // �Է¹��� string ���̸� WORD_BIT_LEN���� ���� ��ŭ
			{
				for (j = 0; j < WORD_BIT_LEN; j++)
					(*x)->a[i] |= (hex[strlen(str) - 1 - (WORD_BIT_LEN * i) - j] << j);
				// string�� hex�� ��ȯ�� �迭 ������ (little endian) �ϳ��� ������� bigint ����ü �迭�� �־� �ֱ�
			}

			for (k = 0; k < (int)(strlen(str) % WORD_BIT_LEN); k++) // �Է¹��� string ���̸� WORD_BIT_LEN���� ���� ��������ŭ
				(*x)->a[(strlen(str) / WORD_BIT_LEN)] |= (hex[k] << ((strlen(str) % WORD_BIT_LEN) - k - 1));
			// string�� hex�� ��ȯ�� ó�� �迭�� �ִ� �� bigint ����ü ������ �迭�� �־� �ֱ� (���ÿ����� 11) 
		}

		else // �Է¹��� string ���̰� WORD_BIT_LEN�� ����̸�
		{
			for (i = 0; i < (int)(strlen(str) / WORD_BIT_LEN); i++)
			{
				for (j = 0; j < WORD_BIT_LEN; j++)
					(*x)->a[i] |= (hex[strlen(str) - 1 - WORD_BIT_LEN * i - j] << j);
				// string�� hex�� ��ȯ�� �迭 ������ (little endian) �ϳ��� ������� bigint ����ü �迭�� �־� �ֱ�
			}
		}
	}

	else if (base == 10)
		return; // ���Ŀ� ������ ��

	else if (base == 16) // "123456789" -> {0x89, 0x67, 0x45, 0x23, 0x01} / {0x6789, 0x2345, 0x0001}
	{
		// '0x89' �̷� ������ ���� 2���� 1 byte�̹Ƿ� WORD_BIT_LEN�� 8 -> 4, 32 -> 8, 64 -> 16���� ���ڴϱ� WORD_BIT_LEN / 4
		if ((strlen(str)) % (WORD_BIT_LEN / 4) != 0)
		{
			for (int i = 0; i < (int)(strlen(str) / (WORD_BIT_LEN / 4)); i++)
			{
				for (int j = 0; j < WORD_BIT_LEN / 4; j++)
					(*x)->a[i] |= hex[strlen(str) - 1 - (WORD_BIT_LEN / 4) * i - j] << (4 * j);
				// string�� hex�� ��ȯ�� �迭 ������ (little endian) �ϳ��� ������� bigint ����ü �迭�� �־� �ֱ�
			}

			for (int k = 0; k < (int)(strlen(str) % (WORD_BIT_LEN / 4)); k++)
				(*x)->a[strlen(str) / (WORD_BIT_LEN / 4)] |= (hex[(strlen(str) % (WORD_BIT_LEN / 4)) - 1 + k] << (4 * k));
			// string�� hex�� ��ȯ�� ó�� �迭�� �ִ� �� bigint ����ü ������ �迭�� �־� �ֱ� (���ÿ����� 1)
		}

		else
		{
			for (int i = 0; i < (int)(strlen(str) / (WORD_BIT_LEN / 4)); i++)
			{
				for (int j = 0; j < WORD_BIT_LEN / 4; j++)
					(*x)->a[i] |= hex[strlen(str) - 1 - (WORD_BIT_LEN / 4) * i - j] << (4 * j);
				// string�� hex�� ��ȯ�� �迭 ������ (little endian) �ϳ��� ������� bigint ����ü �迭�� �־� �ֱ�
			}
		}
	}

}

// �Է¹��� string -> hex�� ��ȯ
void Ascii_To_Hex(char* str, char* hex)
{
	int len = 0;
	len = strlen(str);

	for (int i = 0; i < len; i++)
	{
		if (str[i] < 0x40)
			hex[i] = str[i] - 0x30; // 0 ~ 9�� -0x30
		else if (str[i] < 0x47)
			hex[i] = str[i] - 0x31; // a ~ f�� -0x31
	}

}

/*
	Show BigInteger function

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void BI_Show(bigint* x, int base)
{
	int i = 0;
	int j = 0;
	int k = 0;

	if (base == 10) // 10������ ��
	{
		printf("%d", x->a[0]);
		for (i = 1; i < x->wordlen; i++)
		{
			printf("%d * 2^%d", x->a[i], i * WORD_BIT_LEN); // 10���� ����� ��� �ؾ����� ����
		}
	}

	else if (base == 16) // 16������ ��
	{
		if (x->sign == 1)
			printf("-"); // test vector �� �� -�� ���� ��Ȯ�� ������ ������ ������ ���� - �ﵵ��
		printf("0x"); // hex�� ���� 0x
		for (i = x->wordlen - 1; i >= 0; i--)
		{
			if (WORD_BIT_LEN == 32)
				for (j = 0; j < 4; j++) // word ���̰� 32�̸�, 4bytes �̹Ƿ� 1byte�� �ﵵ��
					printf("%02x", ((x->a[i]) >> (32 - (j + 1) * 8)) & 255); // �� ����Ʈ�� �ﵵ�� 0xff(255)�� ����ŷ

			else if (WORD_BIT_LEN == 64)
				for (j = 0; j < 8; j++) // word ���̰� 64�̸�, 8bytes �̹Ƿ� 1byte�� �ﵵ��
					printf("%02x", (x->a[i]) >> (64 - (j + 1) * 8) & 255); // �� ����Ʈ�� �ﵵ�� 0xff(255)�� ����ŷ

			else
				printf("%02x", (x->a[i]));
		}
	}

	else if (base == 2) // 2������ ��
	{
		printf("0b");
		for (i = x->wordlen - 1; i >= 0; i--) // �ֻ�����Ʈ���� ����ϵ���
			for (j = WORD_BIT_LEN - 1; j >= 0; j--)
			{
				k = 1;
				k = k << j;
				printf("%d", (x->a[i] & k) >> j);
			}
	}
	printf("\n");
}

//Chapter 2.3
/*
	Refine BigInteger function (Remove Last Zero Words)

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void BI_Refine(bigint* x)
{
	word* temp = NULL;

	if (x == NULL)
		return;

	int new_wordlen = x->wordlen;

	while (new_wordlen > 1) // at least one word needed
	{
		if (x->a[new_wordlen - 1] != 0) // 0�� �ƴ� ���� ã���� ������
			break;
		new_wordlen--; // new_wordlen = 0�� �ƴ� ����θ� ������ word�� ����
	}

	if (x->wordlen != new_wordlen)
	{
		x->wordlen = new_wordlen;
		temp = (word*)realloc(x->a, sizeof(word) * new_wordlen); // new_wordlen��ŭ�� ������ �迭 realloc

		if (temp != NULL)
			x->a = temp;
		else
			return;
	}

	if ((x->wordlen == 1) && (x->a[0] == 0x0))
		x->sign = NON_NEGATIVE; // ��ȣ ����
}

// Chapter 2.4 Assign BigInt
/*
	Assign BigInteger function (Y <- X)

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void BI_Assign(bigint** y, bigint* x)
{
	int i = 0;
	int size = 0; //(�߰�) x->wordlen�� ������� ���� size
	size = x->wordlen;
	if (*y != NULL) // ���Ӱ� ������ y�� NULL�� �ƴϸ�
		BI_Delete(y); // �ʱ�ȭ ��Ű��
	BI_New(y, x->wordlen); // x�� ���̿� ���� y�� ����
	(*y)->sign = x->sign; // x�� ��ȣ�͵� ���� �����
	for (i = 0; i < size; i++)
		(*y)->a[i] = x->a[i]; // x�� ���� y�� �� ������
	//BI_Delete(&x);
}

// Chapter 2.5 Generate Random BigInt
/*
	Generate Random Number BigInteger function

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void BI_Gen_Rand(bigint** x, int sign, int wordlen)
{
	BI_New(x, wordlen); // bigint ����ü ����
	(*x)->sign = sign; // ��ȣ ����
	Array_Rand((*x)->a, wordlen); // random���� �迭 ����

	BI_Refine(*x); // ���� 0���� ä���� �κ� �ڸ���
}

void Array_Rand(word* dst, int wordlen)
{
	unsigned char* p = (unsigned char*)dst; // rand() �Լ��� ��°��� 15bit�̹Ƿ� ���� ������ �ƴ� �� ����Ʈ ������ �迭�� ������ ����
	int cnt = wordlen * sizeof(word);
	while (cnt > 0)
	{
		*p = rand() & 0xff; // cnt��ŭ random�� �� �����ؼ� �迭�� ����
		p++;
		cnt--;
	}
}

// Chapter 2.6 Get Word Length / Bit Length / j-th Bit of Big-Int
/*
	Get Word Length function

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void BI_Get_Word_Length(int* len, bigint** x)
{
	*len = (*x)->wordlen; // Big Integer x�� wordlen�� ����
}

/*
	Get Bit Length function

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void BI_Bit_Length(int* len, bigint* x)
{
	int i = 0;
	int j = 0;
	int k = 0;
	i = (x->wordlen) - 1; // ���� i���� x�� wordlen - 1 ���� ����

	for (j = WORD_BIT_LEN - 1; j >= 0; j--) // �ֻ��� ���忡 �ֻ��� ��Ʈ�� ���� ���ִ°� �ƴϹǷ�, �ֻ��� ���忡 ��Ʈ�� �����ϴ� �� ã��
	{ // �ݺ����� ���� �ֻ��� ������ �ֻ��� ��Ʈ���� ���ʴ�� ���� ���ִ��� Ȯ��
		k = 1;
		k = k << j;
		if ((x->a[i] & k) != 0)
			break; // ���� �� ������( != 0) break;
	}
	j += i * WORD_BIT_LEN + 1; // �ֻ��� ������ ���° ��Ʈ�� �������� ����ϵ��� j�� ����
	*len = j;
}

/*
	j-th bit of BigInteger function

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
int BI_j_th_Bit_of_BI(int j, bigint* x)
{
	char* z = NULL;
	unsigned long long i = 0;
	int k = 0;
	int r = 0; // ���� �߰�
	//printf("j_th bit : ");
	if (j >= (x->wordlen) * WORD_BIT_LEN) // x�� bit len���� ��� ����
		return ERROR;
	else
	{
		k = j / WORD_BIT_LEN; // ���� k�� j��° ��Ʈ�� ��ġ�� ������ ��ġ(�� ��° ����)
		r = j % WORD_BIT_LEN; // x->a[k]���� r��ŭ �������� �̵��ؾ� ��ü���� j��° ��ġ (���� �߰�)
		i = 1;
		//i = i << j; // 1�� j�� left shift. ex) 1�� 3��ŭ left shift --> 1000
		i = i << r; // ���� �߰�

		if (i == (x->a[k] & i)) // shift�� i�� j��° ��Ʈ�� ��ġ�� ����� &������ ���� j��° ��Ʈ�� ���� �˾Ƴ���
		{
			//printf("1\n");
			return 1;
		}
		else
		{
			//printf("0\n");
			return 0;
		}
	}
}

// Chapter 2.7 /* negative: 1, non-negative: 0 */
/*
	Get Sign of BigInteger function

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
int BI_Get_Sign(bigint* x)
{
	if ((x->sign) == NON_NEGATIVE) // bigint ����ü�� ��ȣ�� NON_NEGATIVE�� NON_NEGATIVE return
		return NON_NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint ����ü�� ��ȣ�� NEGATIVE�� NEGATIVE return
		return NEGATIVE;
	else // �� �� �ƴ� �� ERROR return
		return ERROR;
}

/*
	Flip sign of BigInteger function

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void BI_Flip_Sign(bigint* x)
{
	if ((x->sign) == NON_NEGATIVE) // bigint ����ü�� ��ȣ�� NON_NEGATIVE�� ��ȣ�� NEGATIVE�� ����
		x->sign = NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint ����ü�� ��ȣ�� NEGATIVE�� ��ȣ�� NON_NEGATIVE�� ����
		x->sign = NON_NEGATIVE;
}

// Chapter2.8 Set One, Set Zero, Is Zero, Is One
/*
	Set BigInteger to 1

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void BI_Set_One(bigint** x)
{
	BI_New(x, 1);
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x1; // 0x1
}

/*
	Set BigInteger to 0

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void BI_Set_Zero(bigint** x)
{
	BI_New(x, 1); // ���̰� 1�� bigint x ����
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x00; // 0�� �������ϹǷ�
}

/*
	BigInteger = 1 ?

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
int BI_Is_One(bigint** x)
{
	int i = 0;

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 1) // ��ȣ�� �����̰ų�, LSB�� 1�� �ƴϸ� FALSE
		return -1; // False

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen��ŭ �ϳ��� �̵��� ���鼭 LSB�� 1�̰� �迭�� ������ ���� 0���� Ȯ��
	{
		if ((*x)->a[i] != 0)
			return -1;
	}
	return 0; // True
}

/*
	BigInteger = 0 ?

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
int BI_Is_Zero(bigint** x)
{
	int i = 0;

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 0) // ��ȣ�� �����̰� LSB�� 0�� �ƴϸ� FALSE
		return -1; // False

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen��ŭ �ϳ��� �̵��� ���鼭 �迭�� ���� ���� 0���� Ȯ��
	{
		if ((*x)->a[i] != 0)
			return -1;
	}
	return 0; // True
}

// Chapter 2.9 Compare
/*
	Compare BigInt A with BigInt B

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
int BI_Compare(bigint** x, bigint** y) // return : 1(x > y), 0(x == y), -1(x < y) 
{
	int i = 0;
	int len_x, len_y = 0;

	if ((*x)->sign < (*y)->sign) // A�� ���, B�� ������ �翬�� A�� ũ�Ƿ�
		return 1;
	else if ((*x)->sign > (*y)->sign) // A�� ����, B�� ����� �翬�� B�� ũ�Ƿ�
		return -1;
	else // A, B ��ȣ�� ���� ��
	{
		BI_Get_Word_Length(&len_x, x); //len_x = x->wordlen
		BI_Get_Word_Length(&len_y, y); //len_y = y->wordlen
		if ((*x)->sign == 0) // A, B ��ȣ�� ����� �� (��ȣ�� ���� �����Ƿ�, if ���� ���� �ϳ��� ��)
		{
			if (len_x > len_y) // x�� ���̰� y���� ���
				return 1;	   // x�� ��ٴ� 1�� ���
			else if (len_x < len_y) // y�� ���̰� x���� ���
				return -1;			// y�� ��ٴ� -1 ���
			else // x�� ���� = y�� ����
			{
				for (i = len_x - 1; i >= 0; i--) //(int)len_x --> len_x�� �ٲ�.20.11.23.sy // �� ��
				{
					if ((*x)->a[i] > (*y)->a[i])
						return 1;
					else if ((*x)->a[i] < (*y)->a[i])
						return -1;
				}
				return 0;
			}
		}
		else // A, B ��ȣ�� ������ ��
		{
			// ����� ���ʹ� �ݴ밡 �ǵ��� return
			if (len_x > len_y)
				return -1;
			else if (len_x < len_y)
				return 1;
			else
			{
				for (i = (int)len_x - 1; i >= 0; i--)
				{
					if ((*x)->a[i] > (*y)->a[i])
						return 1;
					else if ((*x)->a[i] < (*y)->a[i])
						return -1;
				}
				return 0;
			}
		}

	}
}

// Chapter 2.10 Left/Right Shift
/*
	BigInt << r

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void Left_Shift(bigint* x, int len) // len: �̵��� ��Ʈ ��
{
	int length = 0;
	int add_len = 0;
	int q = 0;
	int new_wordlen = 0;
	int i = 0;
	int wn = 0;
	int count = 0;
	int r = 0;
	word* temp = NULL;

	length = x->wordlen;

	bigint* cp = NULL;
	BI_Assign(&cp, x); // bigint x�� bigint cp�� ����

	wn = WORD_BIT_LEN * x->wordlen;

	if ((len % WORD_BIT_LEN) == 0) // �̵��� ��Ʈ ���� WORD_BIT_LEN�� ����̸�
		add_len = len / WORD_BIT_LEN; // �̵��� ��Ʈ ���� WORD_BIT_LEN�� ���� ��ŭ ���� ���� �߰�
	else
		add_len = (len / WORD_BIT_LEN) + 1; // �̵��� ��Ʈ ���� WORD_BIT_LEN�� ���� �� + 1��ŭ ���� ���� �߰�

	new_wordlen = add_len + x->wordlen; // �߰��� ���� ���� + ���� ���� ����

	temp = (word*)realloc(x->a, sizeof(word) * new_wordlen); // new_wordlen��ŭ bigint ����ü ���Ҵ�
	if (temp != NULL)
		x->a = temp;

	x->wordlen = new_wordlen; // ���Ҵ��� ����ü ���� ����

	for (i = 0; i < add_len; i++)
		x->a[length + i] = 0; // �߰��� ���� ���̸�ŭ 0���� �ʱ�ȭ

	q = len / WORD_BIT_LEN; // �̵��� ��Ʈ ���� WORD_BIT_LEN���� ���� ��
	r = len % WORD_BIT_LEN; // �̵��� ��Ʈ ���� WORD_BIT_LEN���� ���� ������

	if (r == 0) // �̵��� ��Ʈ ���� WORD_BIT_LEN���� ������ ��������
	{
		for (i = length - 1; i >= 0; i--)
			x->a[i + add_len] = x->a[i]; // ������ ���忭 add_len��ŭ �������� �̵�
		for (i = 0; i < add_len; i++)
			x->a[i] = 0; // 0��° �迭 ~ (add_len - 1)��° �迭 0���� ����
	}

	else if (r != 0) // �̵��� ��Ʈ ���� WORD_BIT_LEN���� ������ �������� ������
	{
		for (i = 0; i < q; i++)
			x->a[i] = 0; // 0��° �迭 ~ (a - 1)��° �迭 0���� �ʱ�ȭ

		x->a[q] = cp->a[0] << r; // q��° �迭�� ���� �迭 �� 0��° �迭�� r��ŭ �������� �̵��� �� ����

		for (i = 1; i < x->wordlen - 1; i++) // 1��° �迭 ~ �迭�� �� �ٷ� �� = (Aj << r) || (Aj-1 >> (WORD_BIT_LEN - r))
			x->a[i + q] = (cp->a[i] << r) | (cp->a[i - 1] >> (WORD_BIT_LEN - r));

		x->a[x->wordlen - 1] = cp->a[cp->wordlen - 1] >> (WORD_BIT_LEN - r); // ������ �迭 = An-1 >> (WORD_BIT_LEN - r)
	}
	BI_Delete(&cp);
	BI_Refine(x);
}

/*
	BigInt >> r

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void Right_Shift(bigint* x, int len)
{
	int i = 0;
	int wn = 0;
	int count = 0;
	int r = 0;
	int q = 0;

	wn = WORD_BIT_LEN * x->wordlen;
	q = len / WORD_BIT_LEN;
	r = len % WORD_BIT_LEN;

	bigint* cp = NULL;
	BI_Assign(&cp, x);

	if (len >= wn) // ���������� ����Ʈ��ų ����(len)�� �迭�� ����(wn)���� ũ�� ���� 0���� ����
	{
		for (i = 0; i < x->wordlen; i++)
			x->a[i] = 0;
	}

	else if ((len < wn) && (len % WORD_BIT_LEN == 0)) // ���������� ����Ʈ��ų ���̰� �迭�� ���̺��� �۰� WORD_BIT_LEN�� �����
	{
		count = len / WORD_BIT_LEN; // ���������� ����Ʈ��ų ���� ����
		for (i = 0; i < x->wordlen - count; i++) // �̵��� ���� ���̸�ŭ �̵�
			x->a[i] = x->a[i + count]; // 
		for (i = x->wordlen - count; i < x->wordlen; i++) // ������ 0���� ����
			x->a[i] = 0;
	}

	else if ((len < wn) && (len % WORD_BIT_LEN != 0)) // ���������� ����Ʈ��ų ���̰� �迭�� ���̺��� �۰� WORD_BIT_LEN�� ����� �ƴϸ�
	{
		for (i = q; i < x->wordlen - 1; i++)
			x->a[i] = (cp->a[i + 1] << ((WORD_BIT_LEN - r)) | (cp->a[i] >> r)); // �̵��� ���� ���̸�ŭ �̵�

		x->a[x->wordlen - 1] = cp->a[cp->wordlen - 1] >> r; // ��������ŭ �̵�
	}
	BI_Delete(&cp);
	BI_Refine(x);
}

// Chapter 2.11 Reduction
/*
	Reduction BigInt

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void BI_Reduction(bigint** x, int r)
{
	int i = 0;
	int wn = 0;
	int count = 0;
	int size = 0;

	size = (*x)->wordlen;
	wn = WORD_BIT_LEN * size;

	if (r >= wn)
	{
		return;
	}
	else if ((r < wn) && (r % WORD_BIT_LEN == 0))
	{
		count = r / WORD_BIT_LEN; // ���� ���� ���� �˾Ƴ���

		for (i = count; i < size; i++) // �ش� ������� �ֻ��� ������� 0���� ����
			(*x)->a[i] = 0;
	}
	else
	{
		count = r / WORD_BIT_LEN;
		/*for (i = 0; i < count; i++)
			(*x)->a[i] = (*x)->a[i] & word_mask;*/ //(�߰�)�ּ�ó���ص� �� �� ���Ƽ� ��
		r = r % WORD_BIT_LEN;
		(*x)->a[i] = (*x)->a[i] & (((word)1 << r) - 1);
		for (i = count + 1; i < size; i++) // �ش� ������ �������� �ֻ��� ������� 0���� ����
			(*x)->a[i] = 0;
	}

	BI_Refine(*x); // �߰�
}

//Chapter 3 Addition

/**
 * @brief Addition (ĳ���� ������ ���� ����)
 * @details

	 [pseudo code]
	 Input  : C, A, B, c, i
	 Output : carry (0 or 1)
	 1 : carry <- 0
	 2 : C <- A + B mod(2^word)
	 3 : if C < A then
	 4 :		carry <- 1
	 5 :	end if
	 6 : C <- C + c mod(2^word)
	 7 : if C < c then
	 8 :		carry = carry + 1
	 9 : end if
	10 : return carry

 * @param C ���� ����� ������ bigint �� ���������� ����
 * @param A ������ ������ bigint �� ���������� ����
 * @param B ������ ������ bigint �� ���������� ����
 * @param c ���� ���꿡�� �޾� �� ĳ�� (0 �Ǵ� 1)
 * @param i ������ ������ �迭�� index

 * @return carry ĳ�� ���� (0 �Ǵ� 1)
 */
unsigned int ADD_ABc(bigint** C, bigint** A, bigint** B, unsigned int c, int i)
{
	int carry = 0;
	word temp = 0; //word* --> word�� �ٲ�
	temp = (*A)->a[i];
	(*C)->a[i] = (*A)->a[i] + (*B)->a[i]; // A, B ������ i��° �� ���ؼ� C�� i��° �迭�� ����

	if ((*C)->a[i] < temp) // ������ A�� i��° ���� �տ��� ����� ������� ������ ĳ���� �߻��� ��
		carry = 1;

	(*C)->a[i] = (*C)->a[i] + c; // ���ڷ� �޾ƿ� ĳ�� ��(c) ���� �ֱ�

	if ((*C)->a[i] < c) // ���� ����� c���� ������ ĳ���� �߻��� ��
		carry = carry + 1;

	return carry;
}

/**
 * @brief Addition Core (WordLen(A) >= WordLen(B), Sign(A) = Sign(B))
 * @details

	 [pseudo code]
	 Input  : C, A, B, sign
	 1 : B[j] <- 0 for j = m, m + 1, ... , n - 1
	 2 : c <- 0
	 3 : for j = 0 to n - 1 do
	 4 :	c, C[j] <- ADD_ABc(A[j], B[j], c)
	 5 : end for
	 6 : C[n] <- c
	 7 : if C[n] = 1 then
	 8 :	C = (-1)^sign * sum(C[j] * W^j) for j = 0 to n
	 9 : else
	10 :	C = (-1)^sign * sum(C[j] * W^j) for j = 0 to n - 1
	11 : end if

 * @param C ���� ����� ������ bigint �� ���������� ����
 * @param A ������ ������ bigint �� ���������� ����
 * @param B ������ ������ bigint �� ���������� ����
 * @param sign ���� ��� C�� ��ȣ
 */
void ADDC(bigint** C, bigint** A, bigint** B, int sign)
{
	int A_Len;
	int B_Len;
	int i;
	unsigned int carry;

	A_Len = (*A)->wordlen;
	B_Len = (*B)->wordlen;

	word* temp;

	// A�� ���忭 ���̰� B�� ���忭 ���̺��� ū ����̹Ƿ� B�� ���忭 ���̸� A�� ���忭 ���̸�ŭ �÷��� realloc
	(*B)->wordlen = A_Len;
	temp = (word*)realloc((*B)->a, sizeof(word) * (*B)->wordlen); /********************** 4. B�� ���忭�� ���̸� �÷� �� �� �ߴ� **************************/
	if (temp != NULL)
		(*B)->a = temp;

	for (i = B_Len; i < A_Len; i++)
		(*B)->a[i] = 0; // �þ ���̸�ŭ 0���� �ʱ�ȭ

	carry = 0;

	for (i = 0; i < A_Len; i++)
		carry = ADD_ABc(C, A, B, carry, i); // ���� ���� A�� ���忭 ���̸�ŭ ����

	if (carry == 1) // ������ carry�� 1�̸�
		(*C)->a[A_Len] = 1; // ������� MSB�� 1 ����
//	else // ������ carry�� 0�̸�
//		(*C)->a[A_Len] = 0; // ������� MSB�� 0 ����

	BI_Refine(*C);

	// ��ȣ ����
	if (sign == 0)
		(*C)->sign = 0;
	else
		(*C)->sign = 1;
}

// ADD �Լ����� A = A + B�� ��츸 ������ ����, WordLen(A) >= WordLen(B)
/*
	Addition core (if argv[0] == argv[1])

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void ADDC_AAB(bigint** C, bigint** A, bigint** B, int sign)
{
	int A_Len;
	int B_Len;
	int i;
	int carry;

	BI_Get_Word_Length(&A_Len, A);
	BI_Get_Word_Length(&B_Len, B);
	//A_Len = (*A)->wordlen;
	//B_Len = (*B)->wordlen;

	word* temp;

	(*B)->wordlen = A_Len;
	temp = (word*)realloc((*B)->a, sizeof(word) * A_Len);
	if (temp != NULL)
		(*B)->a = temp;

	for (i = B_Len; i < A_Len; i++)
		(*B)->a[i] = 0;

	carry = 0;

	for (i = 0; i < A_Len; i++)
		carry = ADD_ABc(C, A, B, carry, i);

	if (carry == 1)
		(*C)->a[A_Len] = 1;

	if (sign == 0)
		(*C)->sign = 0;
	else
		(*C)->sign = 1;
}

/*
	Addition

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void ADD(bigint** C, bigint** A, bigint** B)
{
	int A_Len = 0;
	int B_Len = 0;
	int A_sign;
	int B_sign;
	int i;
	A_sign = BI_Get_Sign(*A);
	B_sign = BI_Get_Sign(*B);

	BI_Get_Word_Length(&A_Len, A);
	BI_Get_Word_Length(&B_Len, B);

	if (BI_Is_Zero(A) == 0) // A is zero
	{
		// A + B���� A�� 0�̸� B return
		(*C)->sign = (*B)->sign;
		(*C)->wordlen = (*B)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*B)->a[i];

		return;
	}

	if (BI_Is_Zero(B) == 0) // B is zero
	{
		// A + B���� B�� 0�̸� A return
		(*C)->sign = (*A)->sign;
		(*C)->wordlen = (*A)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*A)->a[i];

		return;
	}

	if ((A_sign == NON_NEGATIVE) && (B_sign == NEGATIVE)) // A�� ���, B�� �����̸�
	{
		bigint* temp = NULL;
		BI_New(&temp, B_Len);
		BI_Assign(&temp, *B); // temp = B

		BI_Flip_Sign(temp); // temp = |B|
		SUB(C, *A, temp); // C = A - |B|
		BI_Refine(*C);

		BI_Delete(&temp);

		return;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE)) // A�� ����, B�� ����̸�
	{
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		BI_Assign(&temp, *A); // temp = A

		BI_Flip_Sign(temp); // temp = |A|
		SUB(C, *B, temp); // C = B - |A|
		BI_Refine(*C);

		BI_Delete(&temp);

		return;
	}

	// A, B�� ������ ��ȣ�� ��
	if (A_Len >= B_Len) // A�� ���忭�� ���̰� B�� ���忭�� ���̺��� �� ��
	{
		ADDC(C, A, B, A_sign); // ���� ����
		return;
	}
	else // B�� ���忭�� ���̰� A�� ���忭�� ���̺��� �� ��
	{
		ADDC(C, B, A, A_sign); // ���� ����
		return;
	}

}

// ADD �Լ����� A = A + B�� ��츸 ������ ����
/*
	Addition (if argv[0] == argv[1])

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void ADD_AAB(bigint** C, bigint** A, bigint** B) // A = A + B
{
	int A_Len = 0;
	int B_Len = 0;
	int A_sign;
	int B_sign;
	int i;
	A_sign = BI_Get_Sign(*A);
	B_sign = BI_Get_Sign(*B);

	BI_Get_Word_Length(&A_Len, A);
	BI_Get_Word_Length(&B_Len, B);

	if (BI_Is_Zero(A) == 0) // A is zero
	{
		(*C)->sign = (*B)->sign;
		(*C)->wordlen = (*B)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*B)->a[i];

		return;
	}

	if (BI_Is_Zero(B) == 0) // B is zero
	{
		(*C)->sign = (*A)->sign;
		(*C)->wordlen = (*A)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*A)->a[i];

		return;
	}

	if ((A_sign == NON_NEGATIVE) && (B_sign == NEGATIVE))
	{
		bigint* temp = NULL;
		BI_New(&temp, B_Len);
		BI_Assign(&temp, *B);

		BI_Flip_Sign(temp);
		SUB(C, *A, temp); // SUB �Լ�

		BI_Delete(&temp);

		return;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
	{
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		BI_Assign(&temp, *A);

		BI_Flip_Sign(temp);
		SUB(C, *B, temp); // SUB �Լ�

		BI_Delete(&temp);

		return;
	}

	// A, B�� ������ ��ȣ�� ��
	if (A_Len >= B_Len)
	{
		ADDC_AAB(C, A, B, A_sign);
		return;
	}
	else
	{
		ADDC_AAB(C, B, A, A_sign);
		return;
	}

}

int Compare_WordLen(bigint* A, bigint* B) // return wordlen ū ������
{
	int A_Len, B_Len;

	BI_Get_Word_Length(&A_Len, &A); // A�� ���忭 ����
	BI_Get_Word_Length(&B_Len, &B); // B�� ���忭 ����

	if (A_Len > B_Len) // A�� ���忭�� ���̰� �� ���
		return A_Len; // A�� ���忭 ���� return
	else // B�� ���忭�� ���̰� �� ���
		return B_Len; // B�� ���忭 ���� return
}

/**
* @brief Subtraction
* @details
	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :

* @param C ���� ������(A - B) ����� ������ bigint �� ���� ������ ����
* @param A ���� ������(A - B) A�� �ش��ϴ� bigint �� ������ ����
* @param B ���� ������(A - B) B�� �ش��ϴ� bigint �� ������ ����
*/
void SUB(bigint** C, bigint* A, bigint* B)
{
	unsigned int borrow = 0;
	int len = 0;
	int i = 0;
	bigint* d = NULL;

	BI_Get_Word_Length(&len, &A);   // A->wordlen�� len�� ����
	BI_Get_Word_Length(&borrow, &B);// B->wordlen�� borrow�� ���� 

	if (BI_Is_Zero(&A) == 0) // 0-B --> -B
	{
		BI_Assign(C, B); // B�� �� �״��
		(*C)->sign = NEGATIVE; // ��ȣ�� �ݴ��

		return; // memory leackege X
	}

	if (BI_Is_Zero(&B) == 0) // A - 0 --> A
	{
		if (&A == C) // 1st arg == 2nd arg(AAB)�� ���
			return; // �̹� 1st arg = 2nd arg �̹Ƿ� return���� ����.
		BI_Assign(C, A); // 1st arg != 2nd arg(AAB)�� ��� C�� A�� assign
		return;
		//BI_Assign(C, A); // A�� �� �״��
		//if (A == C)
		//	BI_New(A, borrow);
		//(*C)->sign = A->sign;
		//(*C)->wordlen = A->wordlen;
		//for (i = 0; i < (*C)->wordlen; i++)
		//	(*C)->a[i] = A->a[i];
		//return;// memory leackege X
	}


	if ((A->sign ^ B->sign) == 0) // A, B ��ȣ�� ���� ��
	{
		if ((A->sign & B->sign) == 0) // A, B�� ��ȣ�� ��� ����� ��
		{
			if (BI_Compare(&A, &B) < 0) // A, B�� ���ؼ� A < B�� ��. (BI_Compare(A, B)�� return : -1)
			{
				SUBC(&borrow, C, &B, &A); // B - A �� �ϰ�
				BI_Flip_Sign(*C); // ���� ������ ����Ǿ����Ƿ� ������� ��ȣ �ٲ��ֱ�

				return;
			}
			else if (BI_Compare(&A, &B) == 0) // A = B�� ��, C�� ���� 0�� �Ǿ���Ѵ�.
			{
				// BI_Set_Zero(C); �� �ٲٴ°� ���?
				for (i = 0; i < (*C)->wordlen; i++) // C�� ���̸�ŭ
					(*C)->a[i] = 0;  // 0�� ����
				(*C)->sign = 0; // C�� ��ȣ�� NON-NEGATIVE�� ����

				return;
			}
			else // A, B �� ���ؼ� A >= B�� ��. BI_Compare(A, B)'s return : 0, 1
			{
				SUBC(&borrow, C, &A, &B); // A - B ����

				return;
			}
		}

		else // A, B�� ��ȣ�� ��� ������ ��
		{
			BI_Flip_Sign(A); // A�� ��ȣ�� �����̹Ƿ� ��ȣ �ٲ��ֱ�
			BI_Flip_Sign(B); // B�� ��ȣ�� �����̹Ƿ� ��ȣ �ٲ��ֱ�
			if (BI_Compare(&A, &B) < 0)
			{
				SUBC(&borrow, C, &B, &A);
				BI_Flip_Sign(A); // ���� ������ ����Ǿ����Ƿ� ������� ��ȣ ����ġ
				BI_Flip_Sign(B); // ���� ������ ����Ǿ����Ƿ� ������� ��ȣ ����ġ

				return;// memory leACkege X
			}
			else
			{
				SUBC(&borrow, C, &A, &B);
				BI_Flip_Sign(*C);
				BI_Flip_Sign(A); // ��ȣ ����ġ
				BI_Flip_Sign(B); // ��ȣ ����ġ

				return;// memory leACkege X
			}
		}
	}

	else // A,B ��ȣ�� �ٸ� ��
	{
		if (A->sign == 0) // A�� ���, B�� ����
		{
			BI_Flip_Sign(B); // B�� ��ȣ�� �ٲ��ְ�
			ADD(C, &A, &B);  // ADD ����
			BI_Flip_Sign(B); // ��ȣ ����ġ

			return;
		}
		else
		{
			BI_Flip_Sign(A); //
			ADD(C, &A, &B);

			BI_Flip_Sign(A); // ��ȣ ����ġ
			BI_Flip_Sign(*C); // ��ȣ ����ġ

			return;
		}
	}
}

/**
* @brief Subtraction Core
* @details
	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :

* @param borrow ���� ���� ���꿡�� return �Ǵ� borrow�� �ش��ϴ� unsigned int �� ������ ����
* @param C ���� ���� ������(A - B) ����� ������ bigint �� ���� ������ ����
* @param A ���� ���� ������(A - B) A�� �ش��ϴ� bigint �� ������ ����
* @param B ���� ���� ������(A - B) B�� �ش��ϴ� bigint �� ������ ����
*/
void SUBC(unsigned int* borrow, bigint** C, bigint** A, bigint** B)
{
	int len, i = 0;
	int result = 0;
	bigint* temp = NULL; // A�� b�� ���̰� �ٸ� �� -> bigint** b�� ���̸� �ٲ� �� �����Ƿ� temp�� �������
	bigint* temp3 = NULL; // C�� A�� ���� ���� ����Ͽ�
	BI_Get_Word_Length(&len, A); // b���� ū A�� ���̸� ������
	BI_New(&temp, len);  // A�� ���� ���̿� ���� temp �� ����
	result = BI_Compare(A, B);
	if (result >= 0)
		(*C)->sign = 0; // �Ű����� C�� �̹� ��ȣ�� ������ �� �ٲ��ִ� �� ���
	if (result < 0)
		(*C)->sign = 1;
	if ((*C)->wordlen < len) // Binary Long Division���� C�� ���̰� 1�̰�, A, B�� ���̰� 2�� ���� �־. //A = 0x40bd
	{
		bigint* temp2 = NULL;
		BI_New(&temp2, len);
		for (i = 0; i < (*C)->wordlen; i++)
			temp2->a[i] = (*C)->a[i];
		BI_Assign(C, temp2);
		BI_Delete(&temp2);
	}
	for (i = 0; i < (*B)->wordlen; i++)
		temp->a[i] = (*B)->a[i]; // b�� ���� ���� ������ �־���ϰ�, �� ��� �������� ���� 0�� ���־����.
	// A�� b���� �� �� b�� ���̸� ��������ϴµ� b�� �ǵ��̸� b�� �ٲ�� ������ temp�� �̿�

	for (i = 0; i < len; i++)
	{

		BI_New(&temp3, len); // A�� wordlen�� ���� len�� ���̷� temp3 ����
		BI_Assign(&temp3, *A); // ���� A�� �����ϰ�
		if (i == 0)
			*borrow = 0;

		(*C)->a[i] = temp3->a[i] - (*borrow);//(*C)->a[i] = (*A)->a[i] - (*borrow); // A - b�� ���� C �� ����
		(*C)->a[i] = (*C)->a[i] & word_mask; // mod 2 ^ (WORD_BIT_LEN)
		if (temp3->a[i] < *borrow)//if ((*A)->a[i] < *borrow) // borrow �� ��
			*borrow = 1;
		else // borrow �ȵ� ��
		{
			*borrow = 0;
			if ((*C)->a[i] < temp->a[i])
				*borrow = *borrow + 1;
		}
		(*C)->a[i] -= temp->a[i]; // temp�� �־���� b�� ���� ����
		(*C)->a[i] = (*C)->a[i] & word_mask; // mod 2 ^ (WORD_BIT_LEN)
	}
	BI_Delete(&temp3);
	BI_Delete(&temp);
	BI_Refine(*C);
}

/**
* @brief Multiplication in Word
* @details

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :

* @param C ���� ���� ���� ������ ����� ������ word �� ������ ����
* @param A ���� ���� ���� ������ ���ϴ� ���� word �� ������ ����
* @param B ���� ���� ���� ������ ���ϴ� ���� word �� ������ ����
*/
void MUL_Word(word* C, word* A, word* B) // ���� ���� ����
{
	int len = 0;
	int i = 0;
	int carry0 = 0;
	int carry1 = 0;

	word sum1 = 0;
	word sum2 = 0;
	word mul0 = 0;
	word mul1 = 0;
	word A1 = 0;
	word B1 = 0;
	word A0 = 0;
	word B0 = 0;

	A1 = ((*A) >> (WORD_BIT_LEN >> 1)); // A�� �ֻ�����Ʈ���� �߰���Ʈ����
	B1 = ((*B) >> (WORD_BIT_LEN >> 1)); // B�� �ֻ�����Ʈ���� �߰���Ʈ����
	A0 = (*A) & (((word)1 << (WORD_BIT_LEN >> 1)) - 1); // A�� �߰���Ʈ���� ������ ��Ʈ����
	B0 = (*B) & (((word)1 << (WORD_BIT_LEN >> 1)) - 1); // B�� �߰���Ʈ���� ������ ��Ʈ����

	mul1 = A1 * B1;
	mul0 = A0 * B0;
	sum1 = A1 * B0;
	sum1 += A0 * B1;
	if (sum1 < A0 * B1)
		carry1 += 1;
	sum2 = (sum1 & (((word)1 << (WORD_BIT_LEN >> 1)) - 1));
	sum2 = sum2 << (WORD_BIT_LEN >> 1); //sum1�� �޺κ�
	sum1 = sum1 >> (WORD_BIT_LEN >> 1); // sum1�� �պκ�
	mul0 = sum2 + mul0;
	if (mul0 < sum2)
		carry0 += 1;
	mul1 = sum1 + mul1 + carry0 + ((word)carry1 << (WORD_BIT_LEN >> 1));

	*C = mul0;
	*(C + 1) = mul1;
}

/**
* @brief Multliplication in Multi Word
* @details

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :

* @param result ���� ���� ���� ������ ����� ������ bigint �� ���� ������ ����
* @param A ���� ���� ���� ������ ���ϴ� ���� bigint �� ������ ����
* @param B ���� ���� ���� ������ ���ϴ� ���� bigint �� ������ ����
*/
void MUL_Multi(bigint** result, bigint* A, bigint* B)
{
	int i, j, len = 0;
	int size_a, size_b = 0;

	BI_Get_Word_Length(&size_a, &A);
	BI_Get_Word_Length(&size_b, &B);

	bigint* D = NULL;
	bigint* result2 = NULL;

	BI_New(&D, (*result)->wordlen); // ���� ���� ���� ������ ����� ������ big integer d

	for (i = 0; i < B->wordlen; i++)
	{
		for (j = 0; j < A->wordlen; j++)
		{
			MUL_Word(&D->a[i + j], &A->a[j], &B->a[i]); // A�� ���Ͽ���� B�� ���Ͽ��� ���� �� D�� ���� ���忡 ����
			ADDC_AAB(result, result, &D, 0);
			D->a[i + j] = 0;
			D->a[i + j + 1] = 0;
		}
	}
	BI_Delete(&D);
	(*result)->sign = A->sign ^ B->sign;
	BI_Refine(*result);
}

/*
	Karatsuba

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void Karatsuba(bigint** C, bigint* A, bigint* B)
{
	int i, j = 0;
	int len, A_Len, B_Len;
	int S0_sign, S1_sign;
	int flag = 3;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A�� ���忭 ����
	BI_Get_Word_Length(&B_Len, &B); // B_Len = B�� ���忭 ����

	if (flag >= MIN(A_Len, B_Len)) // A�� ���忭�� ���̿� B�� ���忭�� ���� �� �� ���� ���� flag���� ������
	{
		MUL_Multi(C, A, B); // textbook multiplication ����
		return;
	}

	len = (MAX(A_Len, B_Len) + 1) >> 1;

	bigint* A1 = NULL;
	bigint* A0 = NULL;
	bigint* B1 = NULL;
	bigint* B0 = NULL;

	bigint* T0 = NULL;
	bigint* T1 = NULL;
	bigint* S0 = NULL;
	bigint* S1 = NULL;
	bigint* R = NULL;
	bigint* S = NULL;

	bigint* ADD_result1 = NULL;
	bigint* ADD_result2 = NULL;

	BI_Assign(&A1, A);
	BI_Assign(&A0, A);
	BI_Assign(&B1, B);
	BI_Assign(&B0, B);

	Right_Shift(A1, len * WORD_BIT_LEN); // A1 = A >> len word (A�� len ���常ŭ ���������� �̵�)
	BI_Reduction(&A0, len * WORD_BIT_LEN); // A0 = A mod (2^(len * wordlen)) (A�� len ���常ŭ modular ���� ����)
	Right_Shift(B1, len * WORD_BIT_LEN); // B1 = A >> len word (A�� len ���常ŭ ���������� �̵�)
	BI_Reduction(&B0, len * WORD_BIT_LEN); // B0 = A mod (2^(len * wordlen)) (A�� len ���常ŭ modular ���� ����)

	BI_New(&T0, len * 2);
	BI_New(&T1, len * 2);
	BI_New(&S0, Compare_WordLen(B0, B1)); // S0 = B1 - B0�̴ϱ� B1�� B0 �� �� ū ���� ���̸�ŭ bigint ����
	BI_New(&S1, Compare_WordLen(A0, A1)); // S1 = A0 - A1�̴ϱ� A0�� A1 �� �� ū ���� ���̸�ŭ bigint ����
	BI_New(&R, len * 4);
	BI_New(&S, len * 2);

	Karatsuba(&T1, A1, B1); // T1 = A1 * B1
	Karatsuba(&T0, A0, B0); // T0 = A0 * B0

	BI_Refine(T1);
	BI_Refine(T0);

	bigint* T1_tmp = NULL;
	BI_Assign(&T1_tmp, T1);
	Left_Shift(T1_tmp, 2 * WORD_BIT_LEN * len); // T1_tmp = T1�� 2 * len ���常ŭ ���������� �̵�

	// R = T1 || T0
	for (i = 0; i < T1->wordlen; i++)
		R->a[len * 2 + i] = T1_tmp->a[T1_tmp->wordlen - T1->wordlen + i];
	for (i = 0; i < T0->wordlen; i++)
		R->a[i] = T0->a[i];

	BI_Refine(R);

	SUB(&S1, A0, A1); // S1 = A0 - A1
	SUB(&S0, B1, B0); // S0 = B1 - B0

	S0_sign = BI_Get_Sign(S0);
	S1_sign = BI_Get_Sign(S1);

	//S0, S1�� ������ �� �����Ƿ� ���� ���� �ֱ�
	if (S1_sign == S0_sign)
	{
		if (S1_sign == NEGATIVE)
		{
			BI_Flip_Sign(S1);
			BI_Flip_Sign(S0);
		}
	}
	else
	{
		if (S1_sign == NEGATIVE)
			BI_Flip_Sign(S1);
		else
			BI_Flip_Sign(S0);
	}

	Karatsuba(&S, S1, S0); // S = S1 * S0
	BI_Refine(S);

	// S ��ȣ ���� �ֱ�
	if (S1_sign ^ S0_sign)
	{
		S->sign = NEGATIVE;
	}
	else
	{
		S->sign = NON_NEGATIVE;
	}

	BI_New(&ADD_result1, len * 2 + 1);
	BI_New(&ADD_result2, len * 2 + 1);

	ADD(&ADD_result1, &S, &T1); // ADD_result1 = S + T1
	ADD(&ADD_result2, &ADD_result1, &T0); // ADD_result2 = ADD_result1 + T0 = S + T1 + T0
	Left_Shift(ADD_result2, len * WORD_BIT_LEN); // ADD_result2 << len ����
	ADD_AAB(C, &R, &ADD_result2); // C = R + ADD_result2

	BI_Delete(&A0);
	BI_Delete(&A1);
	BI_Delete(&B0);
	BI_Delete(&B1);

	BI_Delete(&T0);
	BI_Delete(&T1);
	BI_Delete(&S0);
	BI_Delete(&S1);
	BI_Delete(&R);
	BI_Delete(&S);

	BI_Delete(&ADD_result1);
	BI_Delete(&ADD_result2);

	BI_Delete(&T1_tmp);
}

/*
	Single Squaring

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void Single_Squaring(bigint* C, bigint* A)
{
	bigint* A1 = NULL;
	bigint* A0 = NULL;
	bigint* C1 = NULL;
	bigint* C0 = NULL;
	bigint* T = NULL;

	BI_Assign(&A1, A);
	BI_Assign(&A0, A);

	BI_New(&C1, 1);
	BI_New(&C0, 1);
	BI_New(&T, 2);

	// A1 = |A| >> WORD_BIT_LEN / 2, A0 = |A| mod (WORD_BIT_LEN / 2) �� ���� |A| ó��
	if (A->sign == NEGATIVE)
	{
		BI_Flip_Sign(A1);
		BI_Flip_Sign(A0);
	}

	Right_Shift(A1, (int)(WORD_BIT_LEN / 2)); // A1 = |A| >> WORD_BIT_LEN / 2
	BI_Reduction(&A0, (int)(WORD_BIT_LEN / 2)); // A0 = |A| mod (WORD_BIT_LEN / 2)

	C1->a[0] = A1->a[0] * A1->a[0]; // C1 = A1^2
	C0->a[0] = A0->a[0] * A0->a[0]; // C0 = A0^2

	// C = (C1 << WORD_BIT_LEN) + C0
	C->a[1] = C1->a[0];
	C->a[0] = C0->a[0];

	MUL_Multi(&T, A0, A1); // T = A0 * A1
	Left_Shift(T, (WORD_BIT_LEN / 2) + 1); // T = T << ((WORD_BIT_LEN / 2) + 1)

	ADD_AAB(&C, &C, &T); // C = C + T

	BI_Delete(&A1);
	BI_Delete(&A0);
	BI_Delete(&C1);
	BI_Delete(&C0);
	BI_Delete(&T);
}

/*
	Squaring Core

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void SQUC(bigint** C, bigint* A)
{
	int i, j, k, m;
	int wordlen_A;
	int carry = 0;
	int len, c2_len;

	bigint* C1 = NULL;
	bigint* C2 = NULL;
	bigint* T1 = NULL;
	bigint* T2 = NULL;
	bigint* A_i = NULL;
	bigint* A_j = NULL;

	bigint* temp1 = NULL;
	bigint* temp2 = NULL;

	word* temp;

	BI_New(&A_i, 1);
	BI_New(&A_j, 1);

	A_i->sign = A->sign;
	A_j->sign = A->sign;

	BI_Get_Word_Length(&wordlen_A, &A);

	//BI_New(C, wordlen_A * 2 + 1);
	BI_New(&C1, 2 * wordlen_A);
	BI_New(&C2, 2 * wordlen_A);

	for (j = 0; j < wordlen_A; j++)
	{
		BI_New(&T1, 2);
		A_j->a[0] = A->a[j];
		Single_Squaring(T1, A_j);
		BI_Assign(&temp1, T1);
		Left_Shift(temp1, (int)(2 * j * WORD_BIT_LEN)); // T1 = T1 << ((2 * j) * WORD_BIT_LEN)

		// C1 = T1 || C1
		if (j == 0)
		{
			for (k = 0; k < temp1->wordlen; k++)
				C1->a[k] = temp1->a[k]; // j = 0�� ���� C1 = T1 || C1 = T1 (C1�� ó���� 0�̴ϱ�)
		}
		else
		{
			for (k = 0; k < (temp1->wordlen - (2 * j)); k++)
			{
				//if(temp1->wordlen < C1->wordlen)
				C1->a[2 * j + k] = temp1->a[2 * j + k]; // �� �������ʹ� T1�� 2 ���� ������ �������� ����Ʈ ������ ����̹Ƿ� 2 * j��°���� �־� �ֱ�

			}
		}

		BI_Delete(&T1);

		for (i = j + 1; i < wordlen_A; i++)
		{
			BI_New(&T2, 2);
			A_i->a[0] = A->a[i];
			MUL_Multi(&T2, A_j, A_i);
			BI_Assign(&temp2, T2);
			Left_Shift(temp2, (int)((i + j) * WORD_BIT_LEN));

			c2_len = C2->wordlen;
			len = MAX(C2->wordlen, temp2->wordlen) + 1;
			C2->wordlen = len;
			temp = (word*)realloc(C2->a, sizeof(word) * len); // new_wordlen��ŭ bigint ����ü ���Ҵ�
			if (temp != NULL)
				C2->a = temp;

			for (m = c2_len; m < len; m++)
				C2->a[m] = 0; // �þ ���̸�ŭ 0���� �ʱ�ȭ

			ADD_AAB(&C2, &C2, &temp2);
			BI_Refine(C2);

			array_init(T2->a, T2->wordlen);
			BI_Delete(&temp2);
			BI_Delete(&T2);
		}
	}
	Left_Shift(C2, 1);
	ADD(C, &C1, &C2);

	BI_Delete(&C1);
	BI_Delete(&C2);
	BI_Delete(&A_i);
	BI_Delete(&A_j);
	BI_Delete(&temp1);
	BI_Delete(&temp2);
}

/*
	Squaring

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void SQU(bigint** C, bigint* A)
{
	int flag0; // A�� 0���� �Ǻ��ϴ� flag
	int flag1; // A�� 1���� �Ǻ��ϴ� flag
	int flag2 = -1; // A�� ������ �� -1���� �Ǻ��ϴ� flag
	int sign;

	bigint* temp = NULL;

	BI_Assign(&temp, A);

	sign = BI_Get_Sign(A);
	if (sign == NEGATIVE) // A�� ��ȣ�� ������ ���
	{
		BI_Flip_Sign(temp); // ��ȣ �ٲ� ��
		flag2 = BI_Is_One(&temp); // 1���� �Ǻ� (��, -1���� �Ǻ�)
	}

	flag0 = BI_Is_Zero(&A); // A�� 0���� �Ǻ�
	flag1 = BI_Is_One(&A); // A�� 1���� �Ǻ�

	if (flag0 == 0) // A = 0�� ���, A^2 = 0 return
	{
		BI_Assign(C, A);
		BI_Delete(&temp);
		return;
	}
	if (flag1 == 0) // A = 1�� ���, A^2 = 1 return
	{
		BI_Assign(C, A);
		BI_Delete(&temp);
		return;
	}
	if (flag2 == 0) // A = -1�� ���, A^2 = 1 return
	{
		BI_Assign(C, temp);
		BI_Delete(&temp);
		return;
	}

	BI_Delete(&temp);
	BI_New(C, A->wordlen * 2 + 1);
	SQUC(C, A); // A = 0. -1, 1�� �ƴ� ��� SQUC ����

}

/*
	Squaring (used Karatsuba)

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void SQUC_Karatsuba(bigint** C, bigint* A) /********************** 2. ����� ���¿��� ��� F10 ������ **************************/
{
	int i;
	int flag = 1;
	int len, A_Len;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A�� ���忭 ����

	if (flag >= A_Len) // A�� ���忭�� ���̿� B�� ���忭�� ���� �� �� ���� ���� flag���� ������
	{
		SQUC(C, A); // textbook squaring ����
		return;
	}

	len = (A_Len + 1) >> 1;

	bigint* A1 = NULL;
	bigint* A0 = NULL;
	bigint* T0 = NULL;
	bigint* T1 = NULL;
	bigint* R = NULL;
	bigint* S = NULL;

	BI_New(&T0, len * 2);
	BI_New(&T1, len * 2);
	BI_New(&R, len * 4);
	BI_New(&S, len * 2);

	BI_Assign(&A1, A);
	BI_Assign(&A0, A);

	if (A->sign == NEGATIVE)
	{
		BI_Flip_Sign(A1);
		BI_Flip_Sign(A0);
	}

	Right_Shift(A1, len * (int)(WORD_BIT_LEN)); // A1 = |A| >> WORD_BIT_LEN / 2
	BI_Reduction(&A0, len * (int)(WORD_BIT_LEN)); // A0 = |A| mod (WORD_BIT_LEN / 2)

	SQUC_Karatsuba(&T1, A1);
	SQUC_Karatsuba(&T0, A0);

	BI_Refine(T1);
	BI_Refine(T0);

	bigint* T1_tmp = NULL;
	BI_Assign(&T1_tmp, T1);
	Left_Shift(T1_tmp, 2 * WORD_BIT_LEN * len); // T1_tmp = T1�� 2 * len ���常ŭ ���������� �̵�

	// R = T1 || T0
	for (i = 0; i < T1->wordlen; i++)
		R->a[len * 2 + i] = T1_tmp->a[T1_tmp->wordlen - T1->wordlen + i];
	for (i = 0; i < T0->wordlen; i++)
		R->a[i] = T0->a[i];

	BI_Refine(R);

	Karatsuba(&S, A1, A0); // S = A1 * A0
	BI_Refine(S);

	Left_Shift(S, len * WORD_BIT_LEN + 1); // S << len * WORD_BIT_LEN + 1 ��Ʈ��ŭ �������� �̵�
	ADDC(C, &R, &S, R->sign); // C = R + S /********************** 3. �� �Լ� ������(F11) ���� **************************/

	BI_Delete(&A0);
	BI_Delete(&A1);
	BI_Delete(&T0);
	BI_Delete(&T1);
	BI_Delete(&R);
	BI_Delete(&S);
	BI_Delete(&T1_tmp);
}

/**
* @brief Binary_Long_Division Algorithm
* @details
	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
* @param Q Binary Long Divsion ������ �� ���� ����� ������ bigint �� ���� ������ ����
* @param R Binary Long Divsion ������ �������� ���� ����� ������ bigint �� ���� ������ ����
* @param A Binary Long Divsion ������ �������� ���� bigint �� ������ ����
* @param B Binary Long Divsion ������ ������ ���� bigint �� ������ ����
*/
void Binary_Long_Div(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int i = 0;
	word temp = 0;
	int size = 0;
	int len = 0;
	int result = 0;
	bigint* T = NULL;
	bigint* U = NULL;
	BI_New(&T, 1);
	BI_Set_Zero(Q);
	BI_Set_Zero(R);

	BI_Get_Word_Length(&size, &A); // size == bigint A's word length.

	size = size * WORD_BIT_LEN; // ���� size �� A�� �� ��Ʈ ����
	for (i = size - 1; i >= 0; i--) // �ֻ��� ��Ʈ����
	{
		len = (int)(i / WORD_BIT_LEN); // len���� ���� �����ϰ� �ִ� ��Ʈ�� ���� ��ġ
		temp = A->a[len]; // ���� ���� ���� ������ ���� temp�� ����
		len = i % WORD_BIT_LEN; // �ش� ���忡 �� ��° ��Ʈ����
		temp = temp >> len; // �ش� ��Ʈ�� left shift�ؼ� ù��° ��Ʈ�� ���̵���
		temp = temp & 1; // 1�� & �ؼ� 
		T->a[0] = temp; // �ش� ��Ʈ�� ���� ���� // j_th_BI_Bit_Length �ٲ㼭 �ϱ�

		Left_Shift(*R, 1); // 2R
		ADD_DIV(R, R, &T); // 2R + a{j}
		result = BI_Compare(&B, R); // R >= B --> 0, -1
		if (result < 1) // R >= B���� ��
		{
			len = (int)(i / WORD_BIT_LEN) + 1;
			BI_New(&U, len);

			U->a[len - 1] = (word)1 << (i % WORD_BIT_LEN);
			ADD_DIV(Q, Q, &U); // Q + 2 ^ j
			SUB(R, *R, B); // R - B
			BI_Delete(&U); // �������� ��ѹ� U(2 ^ j)�� delete ���ش�.

		}
	}
	BI_Refine(*Q);
	BI_Delete(&T);
}

void ADDC_DIV(bigint** C, bigint** A, bigint** B, int sign)
{
	int A_Len;
	int B_Len;
	int i;
	int carry;

	A_Len = (*A)->wordlen;
	B_Len = (*B)->wordlen;

	word* temp;

	(*B)->wordlen = A_Len;
	temp = (word*)realloc((*B)->a, sizeof(word) * A_Len);
	if (temp != NULL)
		(*B)->a = temp;

	for (i = B_Len; i < A_Len; i++)
		(*B)->a[i] = 0;

	carry = 0;

	for (i = 0; i < A_Len; i++)
		carry = ADD_ABc(C, A, B, carry, i);

	if (carry == 1)
		(*C)->a[A_Len] = 1;
	//else
	   //(*C)->a[A_Len] = 0;

	BI_Refine(*B);
	//BI_Refine(*C);

	if (sign == 0)
		(*C)->sign = 0;
	else
		(*C)->sign = 1;
}

void ADD_DIV(bigint** C, bigint** A, bigint** B)
{
	int A_Len = 0;
	int B_Len = 0;
	int A_sign;
	int B_sign;
	int i;
	A_sign = BI_Get_Sign(*A);
	B_sign = BI_Get_Sign(*B);

	BI_Get_Word_Length(&A_Len, A);
	BI_Get_Word_Length(&B_Len, B);

	if (BI_Is_Zero(A) == 0) // A is zero
	{
		if (A == C)
			BI_New(C, B_Len); // Binary Long Divison���� �ʿ�

		(*C)->sign = (*B)->sign;
		(*C)->wordlen = (*B)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*B)->a[i];

		return;
	}

	if (BI_Is_Zero(B) == 0) // B is zero
	{
		(*C)->sign = (*A)->sign;
		(*C)->wordlen = (*A)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*A)->a[i];

		return;
	}

	if ((A_sign == NON_NEGATIVE) && (B_sign == NEGATIVE))
	{
		bigint* temp = NULL;
		BI_New(&temp, B_Len);
		BI_Assign(&temp, *B);

		BI_Flip_Sign(temp);
		SUB(C, *A, temp); // SUB �Լ�
		//BI_Refine(C);

		BI_Delete(&temp);

		return;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
	{
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		BI_Assign(&temp, *A);

		BI_Flip_Sign(temp);
		SUB(C, *B, temp); // SUB �Լ�
		//BI_Refine(C);

		BI_Delete(&temp);

		return;
	}

	// A, B�� ������ ��ȣ�� ��
	if (A_Len >= B_Len)
	{
		ADDC_DIV(C, A, B, A_sign);
		return;
	}
	else
	{
		ADDC_DIV(C, B, A, A_sign);
		return;
	}
}

/**
* @brief Long Division Algorithm (Multi-precision version)
* @details

	[pseudo code]
	Input  : Q, R, A, B
	Output : Q, R
	1 : if B <= 0 then
	2 :		return INVAILD
	3 : end if
	4 : if A < B then
	5 :		return (0, A)
	6 : end if
	7 : (Q, R) <- (0, 0)
	8 : for n-1 downtown 0 to
	9 :		R <- RW + A_{i}
	10:		(Q_{i}, R) <- DIVC(R, B)
	11: end for
	12 : return (Q, R)
* @param Q Multi Long Divsion ������ �� ���� ����� ������ bigint �� ���� ������ ����
* @param R Multi Long Divsion ������ �������� ���� ����� ������ bigint �� ���� ������ ����
* @param A Multi Long Divsion ������ �������� ���� bigint �� ������ ����
* @param B Multi Long Divsion ������ ������ ���� bigint �� ������ ����
*/
void DIV(bigint** Q, bigint** R, bigint* A, bigint* B) // Long Division Algorithm(Multi-precision version)���� DIV(A, B) fucntion.
{
	int i = 0; // for �ݺ������� ����� ���� i.
	int len = 0; // ���� len�� ��ѹ� A�� size�� ���� ����
	int result = 0; // BI_Compare(&A, &B)�� ����� ���� ����
	bigint* Temp = NULL; // A�� j��° ����( A_{j} )�� ���� ��ѹ� Temp �ʱ�ȭ
	bigint* Word = NULL; // Q�� i��° ����( Q_{i} )�� ���� ��ѹ� Word �ʱ�ȭ

	BI_Set_Zero(R); // ���̰� 1�̰� ���� 0�� ��ѹ� *R ����
	//BI_New(&Word, 1); // ���̰� 1�̰� ���� 0�� ��ѹ� Word ����. BI_New(&Word, 1)�� ����. //1201

	BI_Get_Word_Length(&len, &A);
	BI_New(Q, len);
	if (B->sign & 1) // [line 1]
		return; // return INVALID.
	result = BI_Compare(&A, &B);
	if (result < 0) // [line 4] B > A �� ���, Compare(&A, &B)�� return : -1
	{
		BI_Assign(R, A);
		return;
	}

	for (i = len - 1; i >= 0; i--) // line 8.
	{
		BI_Set_Zero(&Word); // ���̰� 1�̰� ���� 0�� ��ѹ� Word ����. BI_New(&Word, 1)�� ����. //1201
		BI_New(&Temp, 1); // A�� j��° ����(A_{j})�� ��� ��ѹ� Temp�� ���� ���̰� 1�� ����. 
		Temp->a[0] = A->a[i]; // Temp�� A_{j}�� ����.
		Left_Shift(*R, WORD_BIT_LEN); // R�� �������� W��ŭ shift ==> R * W
		ADD_DIV(R, R, &Temp); // [line 9] R * W�� A_{j}�� ���� ���� ����. 
		DIVC(&Word, R, *R, B); //��ѹ� Word�� Q_{i}�� �����ϴ� ����. 
		BI_Delete(&Temp); // �ݺ��� ������ ������ ��ѹ� Temp delete.
		(*Q)->a[i] = Word->a[0]; // [line 10] Q�� Q_{i}�� ����. 
		BI_Delete(&Word); // ��ѹ� Word delete
	}

	BI_Refine(*Q);
	//BI_Refine(*R);
}

/*
	Long Division Algorithm Core function (Multi-precision version)

	[pseudo code]
	Input  :
	Output :

	1 : if A < B then
	2 :		return (0, A)
	3 :	end if
	4 :	Compute k��Z+ such that 2^k B_{m-1}��[2^(w-1), 2^w)
	5 :	A', B' <- 2^k * A, 2^k * B
	6 :	Q', R' <- DIVCC(A', B')
	7 : Q, R <- Q', 2^(-k) * R'
	8 : return (Q, R)

*/
void DIVC(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int result = 0; // A, B ��Һ񱳰��� ���� ���� result
	int i = 0; // �ݺ����� ����� ���� i
	int k = 0; // line 4�� ���õ� ���� k
	int len = 0; // B�� wordlen�� �������� ���� len
	bigint* AP = NULL; // A�� left shift �ؼ� ������ ��ѹ� A'(AP, A Prime)
	bigint* BP = NULL; // B�� left shift �ؼ� ������ ��ѹ� B'(BP, B Prime)

	result = BI_Compare(&A, &B);
	if (result == -1) // A < B ���� Ŭ ��
	{
		return;
		// return (Q, R) = (0, A)�� ������ϴµ�
		// DIV������ 2��° arg�� 3��° arg�� ���� R�� ���������Ƿ�
		// R�� A�� �Ǿ���, Q�� DIV �Լ������� 0�̴�. 
	}
	BI_Assign(&AP, A); // ��ѹ� A' �� ��ѹ� A assing
	BI_Assign(&BP, B); // ��ѹ� B' �� ��ѹ� B assing
	// line 4. Compute k.
	BI_Get_Word_Length(&len, &B);
	bigint* comp = NULL; // k�� compute�ϱ� ���� ��ѹ� comp ���� �� �ʱ�ȭ
	BI_New(&comp, 1); // ��ѹ� comp�� ���� 1�� ����
	comp->a[0] = B->a[len - 1]; //comp�� B_{m - 1} ����
	while (1)
	{
		Left_Shift(comp, 1); // comp�� left shift ==> comp * 2
		k++; // comp * 2^k �ϵ���.
		if (comp->wordlen == 2) // 2^w�� �Ѿ��,
		{
			k--; // �Ѿ���� �ϳ� �ٿ��ְ�
			break; // break;
		}
	}
	Left_Shift(AP, k); // AP = AP << k 
	Left_Shift(BP, k); // BP = BP << k //line 5. 
	DIVCC(Q, R, AP, BP); // [line 6]
	Right_Shift(*R, k); // [line 7] (Q' == Q)
	BI_Delete(&AP); // �������� ��ѹ� AP delete.
	BI_Delete(&BP); // �������� ��ѹ� BP delete.
	BI_Delete(&comp); // �������� ��ѹ� comp delete.
}

/*
	Long Division Algorithm Core's Core function (Multi-precision version)

	[pseudo code]
	Input  :
	Output :
	1 : if n == m then    <-- DIVCC_n_m( )
	2 :		Q <- Lower Bound(A_{m-1} / B_{m-1})
	3 : end if
	4 : if n == m + 1 then <-- DIVCC_n_m1( )
	5 :		if A_{m} = B_{m-1} then
	6 :			Q <- W - 1
	7 :		else
	8 :			Q <- Lower Bound(A_{m} * W + A_{m-1} / B_{m-1})
	9 :		end if
	10:	end if
	11: R <- A - BQ
	12:	while R < 0 do
	13:		(Q, R) <- (Q - 1, R + B)
	14:	end while
	15:	return (Q, R)
*/
void DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B) // 7.2.3 DIVCC(A, B)
{
	int n = 0; // ���� n�� A�� size
	int m = 0; // ���� m�� B�� size
	word A_msw = 0; // ��ѹ� A�� �ֻ��� ��Ʈ�� ������ ������� ����
	word B_msw = 0; // ��ѹ� B�� �ֻ��� ��Ʈ�� ������ ������� ����
	// (msw == Most Significant Word)
	bigint* BQ = NULL; // ��ѹ� B�� ��ѹ� Q�� ���� ���� ������ ��ѹ� BQ�� ���� �� �ʱ�ȭ.
	bigint* one = NULL; // ������̰� 1�̰� ���� 1�� ����� ��ѹ� one�� ���� �� �ʱ�ȭ.

	BI_Set_One(&one); // ���� 1�̰� ������̰� 1�� �ǵ��� BI_Set_One() ���
	BI_Get_Word_Length(&n, &A); // ���� n�� A�� size ����
	BI_Get_Word_Length(&m, &B); // ���� m�� B�� size ����


	A_msw = A->a[n - 1]; // ��ѹ� A�� �ֻ��� ���带 A_msw�� ����
	B_msw = B->a[m - 1]; // ��ѹ� B�� �ֻ��� ���带 B_msw�� ����

	if (n == m) // DIVCC ���� if(n == m) �� �� // [line 1]
	{
		DIVCC_n_m(Q, A, B, m); // [line 2]
	}
	else if (n == m + 1) // [line 4] DIVCC ���� if(n == m + 1) �� �� 
	{
		if (A_msw == B_msw) // line 5.
			(*Q)->a[0] = word_mask; // Q�� W-1�� �����ؾ��ϹǷ� word_mask ����.
		else // [line 7]
			DIVCC_n_m1(Q, A, B, m); // [line 8]
	}
	BI_New(&BQ, n + m); // ��ѹ� B�� ��ѹ� Q�� ���� ���� ������ ��ѹ� BQ�� B�� Q�� ���������� ������ ������ n + m �� ����.
	MUL_Multi(&BQ, *Q, B); // B�� Q�� ���� ������ ��ѹ� BQ�� ���� (�� �� Karatsuba�� ���浵 ���� --> �ӵ� �� ����!!)
	SUB(R, A, BQ);// R = A - B * Q // [line 11]
	while ((*R)->sign & 1) // [line 12] R�� ��ȣ�� ������ �� 1 & 1 = 1 
	{
		SUB(Q, *Q, one); // Q = Q - 1.
		ADD_AAB(R, R, &B); // R = R + B. 1st arg, 2nd arg�� �����Ƿ� ADD_AAB �Լ� ���
	}
	BI_Delete(&one); // �������� ��ѹ� one delete.
	BI_Delete(&BQ); // �������� ��ѹ� BQ delete.

}

/*
	Long Division Algorithm Core's Core's condition (if n == m) (Multi-precision version)

	[pseudo code]
	Input  :
	Output :
	1 : Q <- A_{m-1} / B_{m-1}
	2 : Q <- LowerBound(Q)
*/
void DIVCC_n_m(bigint** Q, bigint* A, bigint* B, int m) // DIVCC ���� if(n == m) �� ��
{
	bigint* Temp1 = NULL;
	bigint* Temp2 = NULL;
	bigint* Trash = NULL;

	BI_New(&Temp1, 1);
	BI_New(&Temp2, 1);
	BI_New(&Trash, 1);
	Temp1->a[0] = A->a[m - 1];
	Temp2->a[0] = B->a[m - 1];

	Binary_Long_Div(Q, &Trash, Temp1, Temp2);

	BI_Delete(&Temp1);
	BI_Delete(&Temp2);
	BI_Delete(&Trash);
}

/*
	Long Division Algorithm Core's Core's condition (if n == m + 1) (Multi-precision version)

	[pseudo code]
	Input  :
	Output :
	1 : Q <- A_{m} * W + A_{m-1} / B_{m-1}
	2 : Q <- LowerBound(Q)
*/
void DIVCC_n_m1(bigint** Q, bigint* A, bigint* B, int m) // DIVCC ���� if(n == m + 1) �� ��
{
	bigint* Temp1 = NULL; // A_{m} * W + A_{m - 1}�� ������ ��ѹ� Temp1 ���� �� �ʱ�ȭ
	bigint* Temp2 = NULL; // B_{m}�� ������ ��ѹ� Temp2 ���� �� �ʱ�ȭ
	bigint* Trash = NULL; // ������ �Լ����� ������ �Ű������� ���� ��ѹ� Trash ���� �� �ʱ�ȭ

	BI_New(&Temp1, 2); // ��ѹ� Temp1�� ���̴� 2�� ����
	BI_New(&Temp2, 1); // ��ѹ� Temp2�� ���̴� 1�� ����
	BI_New(&Trash, 1); // ��ѹ� Trash�� ���̴� 1�� ����

	Temp1->a[1] = A->a[m]; // A_{m}�� ���� --> A_{m} * W
	Temp1->a[0] = A->a[m - 1]; // A_{m - 1}�� ����
	Temp2->a[0] = B->a[m - 1]; // B_{m - 1}�� ����

	Binary_Long_Div(Q, &Trash, Temp1, Temp2); // DIVCC()�� line 8

	BI_Delete(&Temp1); // �������� ��ѹ� Temp1 delete.
	BI_Delete(&Temp2); // �������� ��ѹ� Temp2 delete.
	BI_Delete(&Trash); // �������� ��ѹ� Trash delete.
}

// C = A^n
/*
	Montgomery Exponentiation MUL function

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void Montgomery_Exp_Mul(bigint** C, bigint* A, bigint* n)
{
	int i, bit;
	int n_bit_len;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0�� ������ �ӽ� ����
	bigint* temp1 = NULL; // t1�� ������ �ӽ� ����

	if (n->sign == NEGATIVE) // ���� ó�� (������ �����)
		return;

	BI_Bit_Length(&n_bit_len, n); // n�� ��Ʈ�� ���� -> n_bit_len

	BI_Set_One(&t0); // t0 = 1
	BI_Assign(&t1, A); // t1 = A

	for (i = n_bit_len - 1; i >= 0; i--)
	{
		bit = BI_j_th_Bit_of_BI(i, n); // n�� i��° ��Ʈ�� 1���� 0���� �Ǻ�

		if (bit == 1) // n�� i��° ��Ʈ�� 1�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			BI_New(&temp0, t0_len + t1_len); // temp0(t0) = t0 * t1�� �����ϱ� ���� bigint ����
			//BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����

			MUL_Multi(&temp0, t0, t1); // temp0 = t0 * t1
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 ����)
			BI_Delete(&temp0);

			SQU(&temp1, t1); // temp1 = t1^2
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 ����)
			BI_Delete(&temp1);

		}
		else if (bit == 0) // n�� i��° ��Ʈ�� 0�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			//BI_New(&temp0, t0_len * 2 + 1); // temp0(t0) = t0^2�� �����ϱ� ���� bigint ����
			BI_New(&temp1, t0_len + t1_len);  // temp1(t1) = t0 * t1�� �����ϱ� ���� bigint ����

			MUL_Multi(&temp1, t0, t1); // temp1 = t0 * t1
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 ����)
			BI_Delete(&temp1);


			SQU(&temp0, t0); // temp0 = t0^2
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 ����)
			BI_Delete(&temp0);
		}
		else // n�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
			return; // ���� ó��
	}

	BI_Assign(C, t0); // ������ t0 �� C�� �Ҵ�

	BI_Delete(&t0);
	BI_Delete(&t1);
}

// C = n * A
/*
	Montgomery Exponentiation ADD function

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void Montgomery_Exp_Add(bigint** C, bigint* A, bigint* n)
{
	int i, bit;
	int n_bit_len;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0�� ������ �ӽ� ����
	bigint* temp1 = NULL; // t1�� ������ �ӽ� ����

	if (n->sign == NEGATIVE) // ���� ó�� (�����)
		return;

	BI_Bit_Length(&n_bit_len, n); // n�� ��Ʈ�� ���� -> n_bit_len

	BI_Set_Zero(&t0); // t0 = 0
	BI_Assign(&t1, A); // t1 = A

	for (i = n_bit_len - 1; i >= 0; i--)
	{
		bit = BI_j_th_Bit_of_BI(i, n); // n�� i��° ��Ʈ�� 1���� 0���� �Ǻ�

		if (bit == 1) // n�� i��° ��Ʈ�� 1�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 + t1�� �����ϱ� ���� bigint ����

			ADD(&temp0, &t0, &t1); // temp0 = t0 + t1
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 ����)
			BI_Delete(&temp0);

			Left_Shift(t1, 1); // t1 = t1 << 2

		}
		else if (bit == 0) // n�� i��° ��Ʈ�� 0�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			BI_New(&temp1, MAX(t0_len, t1_len) + 1);  // temp1(t1) = t0 + t1�� �����ϱ� ���� bigint ����

			ADD(&temp1, &t0, &t1); // temp1 = t0 + t1
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 ����)
			BI_Delete(&temp1);

			Left_Shift(t0, 1); // t0 = t0 << 2
		}
		else // n�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
			return; // ���� ó��
	}

	BI_Assign(C, t0); // ������ t0 �� C�� �Ҵ�

	BI_Delete(&t0);
	BI_Delete(&t1);
}

// C = A^n (mod M)
/*
	Montgomery Modular Exponentiation MUL function

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void Montgomery_Mod_Exp_Mul(bigint** C, bigint* A, bigint* n, bigint* M)
{
	int i, bit;
	int n_bit_len;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0�� ������ �ӽ� ����
	bigint* temp1 = NULL; // t1�� ������ �ӽ� ����

	bigint* Q = NULL;
	bigint* R = NULL;

	if (n->sign == NEGATIVE) // ���� ó��
		return;
	if (M->sign == NEGATIVE)
		return;
	if (A->sign == NEGATIVE)
		return;

	BI_Bit_Length(&n_bit_len, n); // n�� ��Ʈ�� ���� -> n_bit_len

	BI_Set_One(&t0); // t0 = 1
	BI_Assign(&t1, A); // t1 = A

	for (i = n_bit_len - 1; i >= 0; i--)
	{
		bit = BI_j_th_Bit_of_BI(i, n); // n�� i��° ��Ʈ�� 1���� 0���� �Ǻ�

		if (bit == 1) // n�� i��° ��Ʈ�� 1�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			BI_New(&temp0, t0_len + t1_len); // temp0(t0) = t0 * t1�� �����ϱ� ���� bigint ����
			//BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����

			MUL_Multi(&temp0, t0, t1); // temp0 = t0 * t1
			DIV(&Q, &R, temp0, M); // R = temp0 (mod M)
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 ����)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);

			SQU(&temp1, t1); // temp1 = t1^2
			DIV(&Q, &R, temp1, M); // R = temp1 (mod M)
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 ����)
			BI_Delete(&temp1);
			BI_Delete(&Q);
			BI_Delete(&R);

		}
		else if (bit == 0) // n�� i��° ��Ʈ�� 0�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			//BI_New(&temp0, t0_len * 2 + 1); // temp0(t0) = t0^2�� �����ϱ� ���� bigint ����
			BI_New(&temp1, t0_len + t1_len);  // temp1(t1) = t0 * t1�� �����ϱ� ���� bigint ����

			MUL_Multi(&temp1, t0, t1); // temp1 = t0 * t1
			DIV(&Q, &R, temp1, M); // R = temp1 (mod M)
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 ����)
			BI_Delete(&temp1);
			BI_Delete(&Q);
			BI_Delete(&R);

			SQU(&temp0, t0); // temp0 = t0^2
			DIV(&Q, &R, temp0, M); // R = temp0 (mod M)
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 ����)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);
		}
		else // n�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
			return; // ���� ó��
	}

	BI_Assign(C, t0); // ������ t0 �� C�� �Ҵ�

	BI_Delete(&t0);
	BI_Delete(&t1);
}

// C = n * A (mod M)
/*
	Montgomery Modular Exponentiation ADD function

	[pseudo code]
	Input  :
	Output :

	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void Montgomery_Mod_Exp_Add(bigint** C, bigint* A, bigint* n, bigint* M)
{
	int i, bit;
	int n_bit_len;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0�� ������ �ӽ� ����
	bigint* temp1 = NULL; // t1�� ������ �ӽ� ����

	bigint* Q = NULL;
	bigint* R = NULL;

	if (n->sign == NEGATIVE) // ���� ó��
		return;
	if (M->sign == NEGATIVE)
		return;
	if (A->sign == NEGATIVE)
		return;

	BI_Bit_Length(&n_bit_len, n); // n�� ��Ʈ�� ���� -> n_bit_len

	BI_Set_Zero(&t0); // t0 = 0
	BI_Assign(&t1, A); // t1 = A

	for (i = n_bit_len - 1; i >= 0; i--)
	{
		bit = BI_j_th_Bit_of_BI(i, n); // n�� i��° ��Ʈ�� 1���� 0���� �Ǻ�

		if (bit == 1) // n�� i��° ��Ʈ�� 1�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 + t1�� �����ϱ� ���� bigint ����

			ADD(&temp0, &t0, &t1); // temp0 = t0 + t1
			DIV(&Q, &R, temp0, M); // R = temp0 (mod M)
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 ����)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);

			Left_Shift(t1, 1); // t1 = t1 << 2
			DIV(&Q, &R, t1, M); // t1 = t1 (mod M)
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 ����)
			BI_Delete(&Q);
			BI_Delete(&R);
		}
		else if (bit == 0) // n�� i��° ��Ʈ�� 0�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			BI_New(&temp1, MAX(t0_len, t1_len) + 1);  // temp1(t1) = t0 + t1�� �����ϱ� ���� bigint ����

			ADD(&temp1, &t0, &t1); // temp1 = t0 + t1
			DIV(&Q, &R, temp1, M); // R = temp1 (mod M)
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 ����)
			BI_Delete(&temp1);
			BI_Delete(&Q);
			BI_Delete(&R);

			Left_Shift(t0, 1); // t0 = t0 << 2
			DIV(&Q, &R, t0, M); // t0 = t0 (mod M)
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 ����)
			BI_Delete(&Q);
			BI_Delete(&R);
		}
		else // n�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
			return; // ���� ó��
	}

	BI_Assign(C, t0); // ������ t0 �� C�� �Ҵ�

	BI_Delete(&t0);
	BI_Delete(&t1);
}

/*
	[Exponentiation] Left to Right MUL function

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void EXP_LR_MUL(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int x_len = 0;
	int t0_len = 0;

	bigint* t0 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;

	BI_Set_One(&t0);

	BI_Bit_Length(&len, N);
	BI_Get_Word_Length(&x_len, &X);
	for (i = len - 1; i >= 0; i--)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_New(&temp1, 2 * t0_len + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����
		SQU(&temp1, t0);
		BI_Delete(&t0);

		BI_New(&temp0, 2 * t0_len + x_len + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����
		result = BI_j_th_Bit_of_BI(i, N);

		if (result == 1)
			MUL_Multi(&temp0, temp1, X);

		else
			BI_Assign(&temp0, temp1);

		BI_Delete(&temp1);
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
	BI_Delete(&temp0);
	BI_Delete(&temp1);
}

/*
	[Exponentiation] Left to Right ADD function

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void EXP_LR_ADD(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int x_len = 0;
	int t0_len = 0;

	bigint* t0 = NULL;
	bigint* temp0 = NULL;

	BI_Set_Zero(&t0);

	BI_Bit_Length(&len, N);

	for (i = len - 1; i >= 0; i--)
	{
		Left_Shift(t0, 1);
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&x_len, &X); // t1�� ���忭 ���� -> t1_len
		BI_New(&temp0, t0_len + x_len); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����
		result = BI_j_th_Bit_of_BI(i, N);
		if (result == 1)
			ADD(&temp0, &t0, &X);

		else
			BI_Assign(&temp0, t0);

		BI_Delete(&t0);
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
}

/*
	[Exponentiation] Right to Left MUL function

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void EXP_RL_MUL(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;

	BI_Set_One(&t0);
	BI_Assign(&t1, X); //line. 1. 

	BI_Bit_Length(&len, N);

	if (len == 1)
	{
		BI_Assign(T, X);
		BI_Delete(&t0);
		BI_Delete(&t1);
		return;
	}
	for (i = 0; i <= len - 1; i++)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

		BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����

		result = BI_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			BI_New(&temp0, t0_len + t1_len); // temp0(t0) = t0 * t1�� �����ϱ� ���� bigint ����
			MUL_Multi(&temp0, t0, t1);
		}
		else
		{
			BI_Assign(&temp0, t0);
			BI_Delete(&t0);
		}
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);

		SQU(&temp1, t1);
		BI_Delete(&t1);
		BI_Assign(&t1, temp1);
		BI_Delete(&temp1);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
	BI_Delete(&t1);
	BI_Refine(*T);
}

/*
	[Exponentiation] Right to Left ADD function

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void EXP_RL_ADD(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;

	BI_Set_Zero(&t0);
	BI_Assign(&t1, X); //line. 1. 

	BI_Bit_Length(&len, N);
	if (len == 1)
	{
		BI_Assign(T, X);
		BI_Delete(&t0);
		BI_Delete(&t1);
		return;
	}
	for (i = 0; i <= len - 1; i++)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

		result = BI_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 * t1�� �����ϱ� ���� bigint ����
			ADD(&temp0, &t0, &t1);
		}
		else
		{
			BI_Assign(&temp0, t0);
			BI_Delete(&t0);
		}
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);

		Left_Shift(t1, 1);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
	BI_Delete(&t1);
	BI_Refine(*T);
}

/*
	[Modular Exponentiation] Left to Right MUL function (used Binary Long Division)

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void MOD_EXP_LR_MUL(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int x_len = 0;
	int t0_len = 0;

	bigint* t0 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL;
	bigint* trash_r = NULL;
	BI_Set_One(&t0);

	BI_Bit_Length(&len, N);

	for (i = len - 1; i >= 0; i--)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_New(&temp1, 2 * t0_len + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����
		SQU(&temp1, t0);
		BI_Delete(&t0);
		Binary_Long_Div(&trash_q, &trash_r, temp1, M);
		BI_Assign(&temp1, trash_r);
		BI_Delete(&trash_r);
		BI_Delete(&trash_r);

		BI_Get_Word_Length(&x_len, &X); // t1�� ���忭 ���� -> t1_len
		BI_New(&temp0, 2 * t0_len + x_len + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����
		result = BI_j_th_Bit_of_BI(i, N);
		if (result == 1)
		{
			MUL_Multi(&temp0, temp1, X);
			Binary_Long_Div(&trash_q, &trash_r, temp0, M);
			BI_Assign(&temp0, trash_r);
			BI_Delete(&trash_r);
			BI_Delete(&trash_q);
		}

		else
			BI_Assign(&temp0, temp1);

		BI_Delete(&temp1);
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
}

/*
	[Modular Exponentiation] Left to Right ADD function (used Binary Long Division)

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void MOD_EXP_LR_ADD(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int x_len = 0;
	int t0_len = 0;

	bigint* t0 = NULL;
	bigint* temp0 = NULL;
	bigint* trash_q = NULL;
	bigint* trash_r = NULL;
	BI_Set_Zero(&t0);

	BI_Bit_Length(&len, N);

	for (i = len - 1; i >= 0; i--)
	{
		Left_Shift(t0, 1);
		Binary_Long_Div(&trash_q, &trash_r, t0, M);
		BI_Get_Word_Length(&t0_len, &trash_r); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&x_len, &X); // t1�� ���忭 ���� -> t1_len
		BI_New(&temp0, t0_len + x_len); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����
		result = BI_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			ADD(&temp0, &trash_r, &X);
			Binary_Long_Div(&trash_q, &trash_r, temp0, M);
			BI_Assign(&temp0, trash_r);
			BI_Delete(&trash_r);
		}
		else
		{
			BI_Assign(&temp0, trash_r);
		}

		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
}

/*
	[Modular Exponentiation] Right to Left MUL function (used Binary Long Division)

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void MOD_EXP_RL_MUL(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL;
	bigint* trash_r = NULL;

	BI_Set_One(&t0);
	BI_Assign(&t1, X); //line. 1. 

	BI_Bit_Length(&len, N);
	/*BI_j_th_Bit_of_BI(i, N);
	len = n;*/
	if (len == 1)
	{
		BI_Assign(T, X);
		Binary_Long_Div(&trash_q, &trash_r, *T, M);
		BI_Assign(T, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);
		BI_Delete(&t0);
		BI_Delete(&t1);
		return;
	}
	for (i = 0; i <= len - 1; i++)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

		BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����

		result = BI_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			BI_New(&temp0, t0_len + t1_len); // temp0(t0) = t0 * t1�� �����ϱ� ���� bigint ����
			MUL_Multi(&temp0, t0, t1);
		}
		else
		{
			BI_Assign(&temp0, t0);
			BI_Delete(&t0);
		}
		Binary_Long_Div(&trash_q, &trash_r, temp0, M);
		BI_Assign(&temp0, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);

		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);

		SQU(&temp1, t1);
		Binary_Long_Div(&trash_q, &trash_r, temp1, M);
		BI_Assign(&temp1, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);
		BI_Delete(&t1);
		BI_Assign(&t1, temp1);
		BI_Delete(&temp1);
	}

	BI_Assign(T, t0);
	BI_Delete(&t0);
	BI_Delete(&t1);
	BI_Refine(*T);
}

/*
	[Modular Exponentiation] Right to Left ADD function (used Binary Long Division)

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void MOD_EXP_RL_ADD(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL;
	bigint* trash_r = NULL;

	BI_Set_Zero(&t0);
	BI_Assign(&t1, X); //line. 1. 

	BI_Bit_Length(&len, N);

	if (len == 1)
	{
		BI_Assign(T, X);
		Binary_Long_Div(&trash_q, &trash_r, *T, M);
		BI_Assign(T, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);
		BI_Delete(&t0);
		BI_Delete(&t1);
		return;
	}
	for (i = 0; i <= len - 1; i++)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

		result = BI_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 * t1�� �����ϱ� ���� bigint ����
			ADD(&temp0, &t0, &t1);
		}
		else
		{
			BI_Assign(&temp0, t0);
			BI_Delete(&t0);
		}
		Binary_Long_Div(&trash_q, &trash_r, temp0, M);
		BI_Assign(&temp0, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);

		Left_Shift(t1, 1);
		Binary_Long_Div(&trash_q, &trash_r, t1, M);
		BI_Assign(&t1, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
	BI_Delete(&t1);
	BI_Refine(*T);
}

/*
	[Modular Exponentiation] Left to Right MUL function (used Multi Long Division)

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void MOD_EXP_LR_MUL_DIV(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int x_len = 0;
	int t0_len = 0;

	bigint* t0 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL;
	bigint* trash_r = NULL;
	BI_Set_One(&t0);

	BI_Bit_Length(&len, N);

	for (i = len - 1; i >= 0; i--)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_New(&temp1, 2 * t0_len + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����
		SQU(&temp1, t0);
		BI_Delete(&t0);
		DIV(&trash_q, &trash_r, temp1, M);
		BI_Assign(&temp1, trash_r);
		BI_Delete(&trash_r);
		BI_Delete(&trash_r);

		BI_Get_Word_Length(&x_len, &X); // t1�� ���忭 ���� -> t1_len
		BI_New(&temp0, 2 * t0_len + x_len + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����
		result = BI_j_th_Bit_of_BI(i, N);
		if (result == 1)
		{
			MUL_Multi(&temp0, temp1, X);
			DIV(&trash_q, &trash_r, temp0, M);
			BI_Assign(&temp0, trash_r);
			BI_Delete(&trash_r);
			BI_Delete(&trash_q);
		}

		else
			BI_Assign(&temp0, temp1);

		BI_Delete(&temp1);
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
}

/*
	[Modular Exponentiation] Left to Right ADD function (used Multi Long Division)

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void MOD_EXP_LR_ADD_DIV(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int x_len = 0;
	int t0_len = 0;

	bigint* t0 = NULL;
	bigint* temp0 = NULL;
	bigint* trash_q = NULL;
	bigint* trash_r = NULL;
	BI_Set_Zero(&t0);

	BI_Bit_Length(&len, N);

	for (i = len - 1; i >= 0; i--)
	{
		Left_Shift(t0, 1);
		DIV(&trash_q, &trash_r, t0, M);
		BI_Get_Word_Length(&t0_len, &trash_r); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&x_len, &X); // t1�� ���忭 ���� -> t1_len
		BI_New(&temp0, t0_len + x_len); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����
		result = BI_j_th_Bit_of_BI(i, N);
		if (result == 1)
		{
			ADD(&temp0, &trash_r, &X);
			DIV(&trash_q, &trash_r, temp0, M);
			BI_Assign(&temp0, trash_r);
			BI_Delete(&trash_r);
		}

		else
		{
			BI_Assign(&temp0, trash_r);
			BI_Delete(&trash_r);
		}

		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);
		BI_Delete(&trash_q);

	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
}

/*
	[Modular Exponentiation] Right to Left MUL function (used Multi Long Division)

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void MOD_EXP_RL_MUL_DIV(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL;
	bigint* trash_r = NULL;

	BI_Set_One(&t0);
	BI_Assign(&t1, X); //line. 1. 

	BI_Bit_Length(&len, N);

	if (len == 1)
	{
		BI_Assign(T, X);
		DIV(&trash_q, &trash_r, *T, M);
		BI_Assign(T, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);
		BI_Delete(&t0);
		BI_Delete(&t1);
		return;
	}
	for (i = 0; i <= len - 1; i++)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

		BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����

		result = BI_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			BI_New(&temp0, t0_len + t1_len); // temp0(t0) = t0 * t1�� �����ϱ� ���� bigint ����
			MUL_Multi(&temp0, t0, t1);
		}
		else
		{
			BI_Assign(&temp0, t0);
			BI_Delete(&t0);
		}
		DIV(&trash_q, &trash_r, temp0, M);
		BI_Assign(&temp0, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);

		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);

		SQU(&temp1, t1);
		Binary_Long_Div(&trash_q, &trash_r, temp1, M);
		BI_Assign(&temp1, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);
		BI_Delete(&t1);
		BI_Assign(&t1, temp1);
		BI_Delete(&temp1);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
	BI_Delete(&t1);
	BI_Refine(*T);
}

/*
	[Modular Exponentiation] Right to Left ADD function (used Multi Long Division)

	[pseudo code]
	Input  :
	Output :
	1 :
	2 :
	3 :
	4 :
	5 :
	6 :
	7 :
	8 :
	9 :
*/
void MOD_EXP_RL_ADD_DIV(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL;
	bigint* trash_r = NULL;

	BI_Set_Zero(&t0);
	BI_Assign(&t1, X); //line. 1. 

	BI_Bit_Length(&len, N);

	if (len == 1)
	{
		BI_Assign(T, X);
		DIV(&trash_q, &trash_r, *T, M);
		BI_Assign(T, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);
		BI_Delete(&t0);
		BI_Delete(&t1);
		return;
	}
	for (i = 0; i <= len - 1; i++)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

		result = BI_j_th_Bit_of_BI(i, N);
		if (result == 1)
		{
			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 * t1�� �����ϱ� ���� bigint ����
			ADD(&temp0, &t0, &t1);
		}
		else
		{
			BI_Assign(&temp0, t0);
			BI_Delete(&t0);
		}
		DIV(&trash_q, &trash_r, temp0, M);
		BI_Assign(&temp0, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);

		Left_Shift(t1, 1);
		Binary_Long_Div(&trash_q, &trash_r, t1, M);
		BI_Assign(&t1, trash_r);
		BI_Delete(&trash_q);
		BI_Delete(&trash_r);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
	BI_Delete(&t1);
	BI_Refine(*T);
}


void BI_Bit_Length_of_number(int num, int* len)
{
	int j = 0;

	for (j = 1; ; j++)
	{
		num = num / 2;

		if (num == 0)
			break;
	}

	*len = j;
}

int j_th_Bit_of_number(int j, int num)
{
	int i;
	int len;

	BI_Bit_Length_of_number(num, &len);

	if (j > len)
		return ERROR;

	i = 1;
	i = i << j;

	if (i == (num & i))
		return 1;
	else
		return 0;
}
