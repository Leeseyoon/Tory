#include "bigint.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define   zerorize

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
	(*x)->sign = NON_NEGATIVE; // ����̰�
	(*x)->wordlen = wordlen;   // 2��° �Ű������� ���� ������
	(*x)->a = (word*)calloc(wordlen, sizeof(word)); // bigint x ����
}

// Chapter 2.2 Show BigInt
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
void BI_Refine(bigint* x)
{
	word* temp;

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
	}

	if ((x->wordlen == 1) && (x->a[0] == 0x0))
		x->sign = NON_NEGATIVE; // ��ȣ ����
}

// Chapter 2.4 Assign BigInt
void Assign_BI(bigint** y, bigint* x)
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
void bi_gen_rand(bigint** x, int sign, int wordlen)
{
	BI_New(x, wordlen); // bigint ����ü ����
	(*x)->sign = sign; // ��ȣ ����
	array_rand((*x)->a, wordlen); // random���� �迭 ����

	BI_Refine(*x); // ���� 0���� ä���� �κ� �ڸ���
}

void array_rand(word* dst, int wordlen)
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
void Get_Word_Length(int* len, bigint** x)
{
	*len = (*x)->wordlen; // Big Integer x�� wordlen�� ����
}

void Bit_Length(int* len, bigint* x)
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
	j += i * WORD_BIT_LEN; // �ֻ��� ������ ���° ��Ʈ�� �������� ����ϵ��� j�� ����
	*len = j;
}

void j_th_Bit_of_BI(int j, bigint* x)
{
	char* z = NULL;
	unsigned long long i = 0;
	int k = 0;
	printf("j_th bit : ");
	if (j >= (x->wordlen) * WORD_BIT_LEN) // x�� bit len���� ��� ����
		return;
	else
	{
		k = j / WORD_BIT_LEN; // ���� k�� j��° ��Ʈ�� ��ġ�� ������ ��ġ(�� ��° ����)
		i = 1;
		i = i << j; // 1�� j�� left shift. ex) 1�� 3��ŭ left shift --> 1000

		if (i == (x->a[k] & i)) // shift�� i�� j��° ��Ʈ�� ��ġ�� ����� &������ ���� j��° ��Ʈ�� ���� �˾Ƴ���
			printf("1");
		else
			printf("0");
	}
	printf("\n");
}

// Chapter 2.7 /* negative: 1, non-negative: 0 */
int Get_Sign(bigint* x)
{
	if ((x->sign) == NON_NEGATIVE) // bigint ����ü�� ��ȣ�� NON_NEGATIVE�� NON_NEGATIVE return
		return NON_NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint ����ü�� ��ȣ�� NEGATIVE�� NEGATIVE return
		return NEGATIVE;
	else // �� �� �ƴ� �� ERROR return
		return ERROR;
}

void Flip_Sign(bigint* x)
{
	if ((x->sign) == NON_NEGATIVE) // bigint ����ü�� ��ȣ�� NON_NEGATIVE�� ��ȣ�� NEGATIVE�� ����
		x->sign = NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint ����ü�� ��ȣ�� NEGATIVE�� ��ȣ�� NON_NEGATIVE�� ����
		x->sign = NON_NEGATIVE;
}

// Chapter2.8 Set One, Set Zero, Is Zero, Is One
void BI_Set_One(bigint** x)
{
	BI_New(x, 1);
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x1; // 0x1
}

void BI_Set_Zero(bigint** x)
{
	BI_New(x, 1); // ���̰� 1�� bigint x ����
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x00; // 0�� �������ϹǷ�
}

int Is_One(bigint** x)
{
	int i = 0;

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 1) // ��ȣ�� �����̰ų�, LSB�� 1�� �ƴϸ� FALSE
		return -1; // False

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen��ŭ �ϳ��� �̵��� ���鼭 LSB�� 1�̰� �迭�� ������ ���� 0���� Ȯ��
	{
		if ((*x)->a[i] != 0)
			return -1;
	}
	return 0;
}

int Is_Zero(bigint** x)
{
	int i = 0;

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 0) // ��ȣ�� �����̰� LSB�� 0�� �ƴϸ� FALSE
		return -1; // False

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen��ŭ �ϳ��� �̵��� ���鼭 �迭�� ���� ���� 0���� Ȯ��
	{
		if ((*x)->a[i] != 0)
			return -1;
	}
	return 0;
}

// Chapter 2.9 Compare
int Compare_BI(bigint** x, bigint** y) // return : 1(x > y), 0(x == y), -1(x < y) 
{
	int i = 0;
	int len_x, len_y = 0;

	if ((*x)->sign < (*y)->sign) // A�� ���, B�� ������ �翬�� A�� ũ�Ƿ�
		return 1;
	else if ((*x)->sign > (*y)->sign) // A�� ����, B�� ����� �翬�� B�� ũ�Ƿ�
		return -1;
	else // A, B ��ȣ�� ���� ��
	{
		Get_Word_Length(&len_x, x); //len_x = x->wordlen
		Get_Word_Length(&len_y, y); //len_y = y->wordlen
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
	word* temp;

	length = x->wordlen;

	bigint* cp = NULL;
	Assign_BI(&cp, x); // bigint x�� bigint cp�� ����

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
	Assign_BI(&cp, x);

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
void Reduction_BI(bigint** x, int r)
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
// ĳ�� ������ ���� ����
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

// WordLen(A) >= WordLen(B)
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
	temp = (word*)realloc((*B)->a, sizeof(word) * A_Len);
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
void ADDC_AAB(bigint** C, bigint** A, bigint** B, int sign)
{
	int A_Len;
	int B_Len;
	int i;
	int carry;

	Get_Word_Length(&A_Len, A);
	Get_Word_Length(&B_Len, B);
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

void ADD(bigint** C, bigint** A, bigint** B)
{
	int A_Len = 0;
	int B_Len = 0;
	int A_sign;
	int B_sign;
	int i;
	A_sign = Get_Sign(*A);
	B_sign = Get_Sign(*B);

	Get_Word_Length(&A_Len, A);
	Get_Word_Length(&B_Len, B);

	if (Is_Zero(A) == 0) // A is zero
	{
		// A + B���� A�� 0�̸� B return
		(*C)->sign = (*B)->sign;
		(*C)->wordlen = (*B)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*B)->a[i];

		return;
	}

	if (Is_Zero(B) == 0) // B is zero
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
		Assign_BI(&temp, *B); // temp = B

		Flip_Sign(temp); // temp = |B|
		SUB(C, *A, temp); // C = A - |B|
		BI_Refine(*C);

		BI_Delete(&temp);

		return;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE)) // A�� ����, B�� ����̸�
	{
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		Assign_BI(&temp, *A); // temp = A

		Flip_Sign(temp); // temp = |A|
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
void ADD_AAB(bigint** C, bigint** A, bigint** B) // A = A + B
{
	int A_Len = 0;
	int B_Len = 0;
	int A_sign;
	int B_sign;
	int i;
	A_sign = Get_Sign(*A);
	B_sign = Get_Sign(*B);

	Get_Word_Length(&A_Len, A);
	Get_Word_Length(&B_Len, B);

	if (Is_Zero(A) == 0) // A is zero
	{
		(*C)->sign = (*B)->sign;
		(*C)->wordlen = (*B)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*B)->a[i];

		return;
	}

	if (Is_Zero(B) == 0) // B is zero
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
		Assign_BI(&temp, *B);

		Flip_Sign(temp);
		SUB(C, *A, temp); // SUB �Լ�

		BI_Delete(&temp);

		return;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
	{
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		Assign_BI(&temp, *A);

		Flip_Sign(temp);
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

	Get_Word_Length(&A_Len, &A); // A�� ���忭 ����
	Get_Word_Length(&B_Len, &B); // B�� ���忭 ����

	if (A_Len > B_Len) // A�� ���忭�� ���̰� �� ���
		return A_Len; // A�� ���忭 ���� return
	else // B�� ���忭�� ���̰� �� ���
		return B_Len; // B�� ���忭 ���� return
}

void SUB(bigint** C, bigint* A, bigint* B)
{
	unsigned int borrow = 0;
	int len = 0;
	int i = 0;
	bigint* d = NULL;

	Get_Word_Length(&len, &A);   // A->wordlen�� len�� ����
	Get_Word_Length(&borrow, &B);// B->wordlen�� borrow�� ���� 

	if (Is_Zero(&A) == 0) // 0-B --> -B
	{
		Assign_BI(C, B); // B�� �� �״��
		(*C)->sign = NEGATIVE; // ��ȣ�� �ݴ��

		return; // memory leackege X
	}

	if (Is_Zero(&B) == 0) // A - 0 --> A
	{
		if (&A == C) // 1st arg == 2nd arg(AAB)�� ���
			return; // �̹� 1st arg = 2nd arg �̹Ƿ� return���� ����.
		Assign_BI(C, A); // 1st arg != 2nd arg(AAB)�� ��� C�� A�� assign
		return;
		//Assign_BI(C, A); // A�� �� �״��
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
			if (Compare_BI(&A, &B) < 0) // A, B�� ���ؼ� A < B�� ��. (Compare_BI(A, B)�� return : -1)
			{
				SUBC_BI(&borrow, C, &B, &A); // B - A �� �ϰ�
				Flip_Sign(*C); // ���� ������ ����Ǿ����Ƿ� ������� ��ȣ �ٲ��ֱ�

				return;
			}
			else if (Compare_BI(&A, &B) == 0) // A = B�� ��, C�� ���� 0�� �Ǿ���Ѵ�.
			{
				// BI_Set_Zero(C); �� �ٲٴ°� ���?
				for (i = 0; i < (*C)->wordlen; i++) // C�� ���̸�ŭ
					(*C)->a[i] = 0;  // 0�� ����
				(*C)->sign = 0; // C�� ��ȣ�� NON-NEGATIVE�� ����

				return;
			}
			else // A, B �� ���ؼ� A >= B�� ��. Compare_BI(A, B)'s return : 0, 1
			{
				SUBC_BI(&borrow, C, &A, &B); // A - B ����

				return;
			}
		}

		else // A, B�� ��ȣ�� ��� ������ ��
		{
			Flip_Sign(A); // A�� ��ȣ�� �����̹Ƿ� ��ȣ �ٲ��ֱ�
			Flip_Sign(B); // B�� ��ȣ�� �����̹Ƿ� ��ȣ �ٲ��ֱ�
			if (Compare_BI(&A, &B) < 0)
			{
				SUBC_BI(&borrow, C, &B, &A);
				Flip_Sign(A); // ���� ������ ����Ǿ����Ƿ� ������� ��ȣ ����ġ
				Flip_Sign(B); // ���� ������ ����Ǿ����Ƿ� ������� ��ȣ ����ġ

				return;// memory leACkege X
			}
			else
			{
				SUBC_BI(&borrow, C, &A, &B);
				Flip_Sign(*C);
				Flip_Sign(A); // ��ȣ ����ġ
				Flip_Sign(B); // ��ȣ ����ġ

				return;// memory leACkege X
			}
		}
	}

	else // A,B ��ȣ�� �ٸ� ��
	{
		if (A->sign == 0) // A�� ���, B�� ����
		{
			Flip_Sign(B); // B�� ��ȣ�� �ٲ��ְ�
			ADD(C, &A, &B);  // ADD ����
			Flip_Sign(B); // ��ȣ ����ġ

			return;
		}
		else
		{
			Flip_Sign(A); //
			ADD(C, &A, &B);

			Flip_Sign(A); // ��ȣ ����ġ
			Flip_Sign(*C); // ��ȣ ����ġ

			return;
		}
	}
}

void SUBC_BI(unsigned int* borrow, bigint** C, bigint** A, bigint** B)
{
	int len, i = 0;
	int result = 0;
	bigint* temp = NULL; // A�� b�� ���̰� �ٸ� �� -> bigint** b�� ���̸� �ٲ� �� �����Ƿ� temp�� �������
	bigint* temp3 = NULL; // C�� A�� ���� ���� ����Ͽ�
	Get_Word_Length(&len, A); // b���� ū A�� ���̸� ������
	BI_New(&temp, len);  // A�� ���� ���̿� ���� temp �� ����
	result = Compare_BI(A, B);
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
		Assign_BI(C, temp2);
		BI_Delete(&temp2);
	}
	for (i = 0; i < (*B)->wordlen; i++)
		temp->a[i] = (*B)->a[i]; // b�� ���� ���� ������ �־���ϰ�, �� ��� �������� ���� 0�� ���־����.
	// A�� b���� �� �� b�� ���̸� ��������ϴµ� b�� �ǵ��̸� b�� �ٲ�� ������ temp�� �̿�

	for (i = 0; i < len; i++)
	{

		BI_New(&temp3, len); // A�� wordlen�� ���� len�� ���̷� temp3 ����
		Assign_BI(&temp3, *A); // ���� A�� �����ϰ�
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

void MUL_Test(word* C, word* A, word* B) // ���� ���� ����
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

void MUL_MUL(bigint** result, bigint* A, bigint* B)
{
	int i, j, len = 0;
	int size_a, size_b = 0;

	Get_Word_Length(&size_a, &A);
	Get_Word_Length(&size_b, &B);

	bigint* D = NULL;
	bigint* result2 = NULL;

	BI_New(&D, (*result)->wordlen); // ���� ���� ���� ������ ����� ������ big integer d

	for (i = 0; i < B->wordlen; i++)
	{
		for (j = 0; j < A->wordlen; j++)
		{
			MUL_Test(&D->a[i + j], &A->a[j], &B->a[i]); // A�� ���Ͽ���� B�� ���Ͽ��� ���� �� D�� ���� ���忡 ����
			ADDC_AAB(result, result, &D, 0);
			D->a[i + j] = 0;
			D->a[i + j + 1] = 0;
		}
	}
	BI_Delete(&D);
	(*result)->sign = A->sign ^ B->sign;
	BI_Refine(*result);
}

void Karatsuba(bigint** C, bigint* A, bigint* B)
{
	int i, j = 0;
	int len, A_Len, B_Len;
	int S0_sign, S1_sign;
	int flag = 3;

	Get_Word_Length(&A_Len, &A); // A_Len = A�� ���忭 ����
	Get_Word_Length(&B_Len, &B); // B_Len = B�� ���忭 ����

	if (flag >= MIN(A_Len, B_Len)) // A�� ���忭�� ���̿� B�� ���忭�� ���� �� �� ���� ���� flag���� ������
	{
		MUL_MUL(C, A, B); // textbook multiplication ����
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

	Assign_BI(&A1, A);
	Assign_BI(&A0, A);
	Assign_BI(&B1, B);
	Assign_BI(&B0, B);

	Right_Shift(A1, len * WORD_BIT_LEN); // A1 = A >> len word (A�� len ���常ŭ ���������� �̵�)
	Reduction_BI(&A0, len * WORD_BIT_LEN); // A0 = A mod (2^(len * wordlen)) (A�� len ���常ŭ modular ���� ����)
	Right_Shift(B1, len * WORD_BIT_LEN); // B1 = A >> len word (A�� len ���常ŭ ���������� �̵�)
	Reduction_BI(&B0, len * WORD_BIT_LEN); // B0 = A mod (2^(len * wordlen)) (A�� len ���常ŭ modular ���� ����)

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
	Assign_BI(&T1_tmp, T1);
	Left_Shift(T1_tmp, 2 * WORD_BIT_LEN * len); // T1_tmp = T1�� 2 * len ���常ŭ ���������� �̵�

	// R = T1 || T0
	for (i = 0; i < T1->wordlen; i++)
		R->a[len * 2 + i] = T1_tmp->a[T1_tmp->wordlen - T1->wordlen + i];
	for (i = 0; i < T0->wordlen; i++)
		R->a[i] = T0->a[i];

	BI_Refine(R);

	SUB(&S1, A0, A1); // S1 = A0 - A1
	SUB(&S0, B1, B0); // S0 = B1 - B0

	S0_sign = Get_Sign(S0);
	S1_sign = Get_Sign(S1);

	//S0, S1�� ������ �� �����Ƿ� ���� ���� �ֱ�
	if (S1_sign == S0_sign)
	{
		if (S1_sign == NEGATIVE)
		{
			Flip_Sign(S1);
			Flip_Sign(S0);
		}
	}
	else
	{
		if (S1_sign == NEGATIVE)
			Flip_Sign(S1);
		else
			Flip_Sign(S0);
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

void Single_Squaring(bigint* C, bigint* A)
{
	bigint* A1 = NULL;
	bigint* A0 = NULL;
	bigint* C1 = NULL;
	bigint* C0 = NULL;
	bigint* T = NULL;

	Assign_BI(&A1, A);
	Assign_BI(&A0, A);

	BI_New(&C1, 1);
	BI_New(&C0, 1);
	BI_New(&T, 2);

	// A1 = |A| >> WORD_BIT_LEN / 2, A0 = |A| mod (WORD_BIT_LEN / 2) �� ���� |A| ó��
	if (A->sign == NEGATIVE)
	{
		Flip_Sign(A1);
		Flip_Sign(A0);
	}

	Right_Shift(A1, (int)(WORD_BIT_LEN / 2)); // A1 = |A| >> WORD_BIT_LEN / 2
	Reduction_BI(&A0, (int)(WORD_BIT_LEN / 2)); // A0 = |A| mod (WORD_BIT_LEN / 2)

	C1->a[0] = A1->a[0] * A1->a[0]; // C1 = A1^2
	C0->a[0] = A0->a[0] * A0->a[0]; // C0 = A0^2

	// C = (C1 << WORD_BIT_LEN) + C0
	C->a[1] = C1->a[0];
	C->a[0] = C0->a[0];

	MUL_MUL(&T, A0, A1); // T = A0 * A1
	Left_Shift(T, (WORD_BIT_LEN / 2) + 1); // T = T << ((WORD_BIT_LEN / 2) + 1)

	ADD_AAB(&C, &C, &T); // C = C + T

	BI_Delete(&A1);
	BI_Delete(&A0);
	BI_Delete(&C1);
	BI_Delete(&C0);
	BI_Delete(&T);
}

void SQUC(bigint* C, bigint* A)
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

	Get_Word_Length(&wordlen_A, &A);

	BI_New(&C1, 2 * wordlen_A);
	BI_New(&C2, 2 * wordlen_A);

	for (j = 0; j < wordlen_A; j++)
	{
		BI_New(&T1, 2);
		A_j->a[0] = A->a[j];
		Single_Squaring(T1, A_j);
		//printf("T1 = ");
		//BI_Show(T1, 16);
		Assign_BI(&temp1, T1);
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

		//printf("C1 = ");
		//BI_Show(C1, 16);

		//array_init(T1->a, T1->wordlen);
		BI_Delete(&T1);

		for (i = j + 1; i < wordlen_A; i++)
		{
			BI_New(&T2, 2);
			A_i->a[0] = A->a[i];
			MUL_MUL(&T2, A_j, A_i);
			//printf("T2 = ");
			//BI_Show(T2, 16);
			Assign_BI(&temp2, T2);
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
			//printf("C2 = ");
			//BI_Show(C2, 16);

			array_init(T2->a, T2->wordlen);
			BI_Delete(&temp2);
			BI_Delete(&T2);
		}
	}
	Left_Shift(C2, 1);
	//printf("Lefted C2 = ");
	//BI_Show(C2, 16);
	ADD(&C, &C1, &C2);
	//printf("C = ");
	//BI_Show(C, 16);

	BI_Delete(&C1);
	BI_Delete(&C2);
	//BI_Delete(&T1);
	//BI_Delete(&T2);
	BI_Delete(&A_i);
	BI_Delete(&A_j);
	BI_Delete(&temp1);
	BI_Delete(&temp2);
}

void SQUC_Karatsuba(bigint* C, bigint* A)
{
	int i, j;
	int flag = 1;
	int len, A_Len;

	Get_Word_Length(&A_Len, &A); // A_Len = A�� ���忭 ����

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

	Assign_BI(&A1, A);
	Assign_BI(&A0, A);

	if (A->sign == NEGATIVE)
	{
		Flip_Sign(A1);
		Flip_Sign(A0);
	}

	Right_Shift(A1, len * (int)(WORD_BIT_LEN)); // A1 = |A| >> WORD_BIT_LEN / 2
	Reduction_BI(&A0, len * (int)(WORD_BIT_LEN)); // A0 = |A| mod (WORD_BIT_LEN / 2)

	SQUC_Karatsuba(T1, A1);
	SQUC_Karatsuba(T0, A0);

	BI_Refine(T1);
	BI_Refine(T0);

	bigint* T1_tmp = NULL;
	Assign_BI(&T1_tmp, T1);
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
	ADDC(&C, &R, &S, R->sign); // C = R + S

	BI_Delete(&A0);
	BI_Delete(&A1);
	BI_Delete(&T0);
	BI_Delete(&T1);
	BI_Delete(&R);
	BI_Delete(&S);
	BI_Delete(&T1_tmp);
}

/*
	[Binary_Long_Division] pseudo code

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

	Get_Word_Length(&size, &A); // size == bigint A's word length.

	size = size * WORD_BIT_LEN; // ���� size �� A�� �� ��Ʈ ����
	for (i = size - 1; i >= 0; i--) // �ֻ��� ��Ʈ����
	{
		len = (int)(i / WORD_BIT_LEN); // len���� ���� �����ϰ� �ִ� ��Ʈ�� ���� ��ġ
		temp = A->a[len]; // ���� ���� ���� ������ ���� temp�� ����
		len = i % WORD_BIT_LEN; // �ش� ���忡 �� ��° ��Ʈ����
		temp = temp >> len; // �ش� ��Ʈ�� left shift�ؼ� ù��° ��Ʈ�� ���̵���
		temp = temp & 1; // 1�� & �ؼ� 
		T->a[0] = temp; // �ش� ��Ʈ�� ���� ���� // j_th_Bit_Length �ٲ㼭 �ϱ�

		Left_Shift(*R, 1); // 2R
		ADD_DIV(R, R, &T); // 2R + a{j}
		result = Compare_BI(&B, R); // R >= B --> 0, -1
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
	A_sign = Get_Sign(*A);
	B_sign = Get_Sign(*B);

	Get_Word_Length(&A_Len, A);
	Get_Word_Length(&B_Len, B);

	if (Is_Zero(A) == 0) // A is zero
	{
		if (A == C)
			BI_New(C, B_Len); // Binary Long Divison���� �ʿ�

		(*C)->sign = (*B)->sign;
		(*C)->wordlen = (*B)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*B)->a[i];

		return;
	}

	if (Is_Zero(B) == 0) // B is zero
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
		Assign_BI(&temp, *B);

		Flip_Sign(temp);
		SUB(C, *A, temp); // SUB �Լ�
		//BI_Refine(C);

		BI_Delete(&temp);

		return;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
	{
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		Assign_BI(&temp, *A);

		Flip_Sign(temp);
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

/*
	Long Division Algorithm (Multi-precision version)
*/
void DIV(bigint** Q, bigint** R, bigint* A, bigint* B) // Long Division Algorithm(Multi-precision version)���� DIV(A, B) fucntion.
{
	int i = 0; // for �ݺ������� ����� ���� i.
	int len = 0; // ���� len�� ��ѹ� A�� size�� ���� ����
	int result = 0; // Compare_BI(&A, &B)�� ����� ���� ����
	bigint* Temp = NULL; // A�� j��° ����( A_{j} )�� ���� ��ѹ� Temp �ʱ�ȭ
	bigint* Word = NULL; // Q�� i��° ����( Q_{i} )�� ���� ��ѹ� Word �ʱ�ȭ

	BI_Set_Zero(R); // ���̰� 1�̰� ���� 0�� ��ѹ� *R ����
	BI_Set_Zero(&Word); // ���̰� 1�̰� ���� 0�� ��ѹ� Word ����. BI_New(&Word, 1)�� ����.

	Get_Word_Length(&len, &A);
	BI_New(Q, len);
	if (B->sign & 1) // line 1.
		return; // return INVALID.
	result = Compare_BI(&A, &B);
	if (result < 0) // line 4. B > A �� ���, Compare(&A, &B)�� return : -1
	{
		Assign_BI(R, A);
		return;
	}

	for (i = len - 1; i >= 0; i--) // line 8.
	{
		BI_New(&Temp, 1); // A�� j��° ����(A_{j})�� ��� ��ѹ� Temp�� ���� ���̰� 1�� ����. 
		Temp->a[0] = A->a[i]; // Temp�� A_{j}�� ����.
		Left_Shift(*R, WORD_BIT_LEN); // R�� �������� W��ŭ shift ==> R * W
		ADD_DIV(R, R, &Temp); // R * W�� A_{j}�� ���� ���� ����. // line 9.
		DIVC(&Word, R, *R, B); //��ѹ� Word�� Q_{i}�� �����ϴ� ����. 
		BI_Delete(&Temp); // �ݺ��� ������ ������ ��ѹ� Temp delete.
		(*Q)->a[i] = Word->a[0]; // Q�� Q_{i}�� ����. //line 10.
	}
	BI_Delete(&Word); // ��ѹ� Word delete
	BI_Refine(*Q);
	//BI_Refine(*R);
}

void DIVC(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int result = 0; // A, B ��Һ񱳰��� ���� ���� result
	int i = 0; // �ݺ����� ����� ���� i
	int k = 0; // line 4�� ���õ� ���� k
	int len = 0; // B�� wordlen�� �������� ���� len
	bigint* AP = NULL; // A�� left shift �ؼ� ������ ��ѹ� A'(AP, A Prime)
	bigint* BP = NULL; // B�� left shift �ؼ� ������ ��ѹ� B'(BP, B Prime)

	result = Compare_BI(&A, &B);
	if (result == -1) // A < B ���� Ŭ ��
	{
		return;
		// return (Q, R) = (0, A)�� ������ϴµ�
		// DIV������ 2��° arg�� 3��° arg�� ���� R�� ���������Ƿ�
		// R�� A�� �Ǿ���, Q�� DIV �Լ������� 0�̴�. 
	}
	Assign_BI(&AP, A); // ��ѹ� A' �� ��ѹ� A assing
	Assign_BI(&BP, B); // ��ѹ� B' �� ��ѹ� B assing
	// line 4. Compute k.
	Get_Word_Length(&len, &B);
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
	DIVCC(Q, R, AP, BP); // line 6.
	Right_Shift(*R, k); // line 7. (Q' == Q)
	BI_Delete(&AP); // �������� ��ѹ� AP delete.
	BI_Delete(&BP); // �������� ��ѹ� BP delete.
	BI_Delete(&comp); // �������� ��ѹ� comp delete.
}

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
	Get_Word_Length(&n, &A); // ���� n�� A�� size ����
	Get_Word_Length(&m, &B); // ���� m�� B�� size ����


	A_msw = A->a[n - 1]; // ��ѹ� A�� �ֻ��� ���带 A_msw�� ����
	B_msw = B->a[m - 1]; // ��ѹ� B�� �ֻ��� ���带 B_msw�� ����

	if (n == m) // DIVCC ���� if(n == m) �� �� // line 1.
	{
		DIVCC_n_m(Q, A, B, m); // line 2.
	}
	else if (n == m + 1) // DIVCC ���� if(n == m + 1) �� �� //line 4.
	{
		if (A_msw == B_msw) // line 5.
			(*Q)->a[0] = word_mask; // Q�� W-1�� �����ؾ��ϹǷ� word_mask ����.
		else // line 7.
			DIVCC_n_m1(Q, A, B, m); // line 8.
	}
	BI_New(&BQ, n + m); // ��ѹ� B�� ��ѹ� Q�� ���� ���� ������ ��ѹ� BQ�� B�� Q�� ���������� ������ ������ n + m �� ����.
	MUL_MUL(&BQ, *Q, B); // B�� Q�� ���� ������ ��ѹ� BQ�� ���� (�� �� Karatsuba�� ���浵 ���� --> �ӵ� �� ����!!)
	SUB(R, A, BQ);// R = A - B * Q // line 11.
	while ((*R)->sign & 1) // R�� ��ȣ�� ������ �� 1 & 1 = 1 // line 12. 
	{
		SUB(Q, *Q, one); // Q = Q - 1.
		ADD_AAB(R, R, &B); // R = R + B. 1st arg, 2nd arg�� �����Ƿ� ADD_AAB �Լ� ���
	}
	BI_Delete(&one); // �������� ��ѹ� one delete.
	BI_Delete(&BQ); // �������� ��ѹ� BQ delete.

}

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
