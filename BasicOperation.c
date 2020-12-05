#include "config.h"
#include "BasicOperation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Initialize Array
 * @details �迭�� �Է� ���̸�ŭ 0���� �ʱ�ȭ
 * @param wordt* a �ʱ�ȭ��ų �迭�� word �� ������ ����
 * @param int len �ʱ�ȭ��ų �迭�� ����
 * @return SUCCESS
 */
int array_init(word* a, int len)
{
	memset(a, 0, sizeof(word) * len); // �迭�� ���̸�ŭ �ʱ�ȭ
	return SUCCESS;
}

/**
 * @brief Create Bigint
 * @details bigint ����ü �� ����ü ��� ����
 * @param bigint** x ������ bigint �� ���������� ����
 * @param int wordlen ������ ����ü ���� �迭�� ���忭 ����
 * @return SUCCESS
 * @throws
	ERROR bigint ����ü �Ҵ� ���� ��
	ERROR bigint ����ü ���� �迭 �Ҵ� ���� ��
 */
int BI_New(bigint** x, int wordlen)
{
	if (*x != NULL) //   x�� �ּҰ� NULL�� �ƴϸ�
		BI_Delete(x); // BI_Delete()�� �ʱ�ȭ

	*x = (bigint*)malloc(sizeof(bigint)); // bigint *x �Ҵ�
	if (*x == NULL)
		return ERROR;

	(*x)->sign = NON_NEGATIVE; // ����̰�
	(*x)->wordlen = wordlen;   // 2��° �Ű������� ���� ������
	(*x)->a = (word*)calloc(wordlen, sizeof(word)); // bigint x ������ �迭 a �Ҵ�
	
	if (x == NULL)
		return ERROR;

	return SUCCESS;
}

/**
 * @brief Delete Bigint
 * @details bigint ����ü �� ����ü ���� �迭 ����
 * @param bigint** x ������ bigint �� ���������� ����
 * @return SUCCESS
 */
int BI_Delete(bigint** x)
{
	if (*x == NULL) // bigint ����ü�� �ּҰ� NULL ���̸� ������ �ʿ� ���� ������ return
		return SUCCESS;

#ifdef zerorize
	array_init((*x)->a, (*x)->wordlen); // �迭 �ʱ�ȭ
#endif 
	free((*x)->a); // bigint ����ü ���� word �迭 free
	free(*x); // bigint ����ü free
	*x = NULL; // bigint ����ü�� �ּҰ� NULL�� �ʱ�ȭ

	return SUCCESS;
}

/**
 * @brief Set Bigint by Array
 * @details �Է¹��� �迭�� big integer ����
 * @param bigint** x ������ bigint �� ���������� ����
 * @param int sign ������ bigint�� ��ȣ
 * @param word* a �迭�� word �� ������ ����
 * @param int wordlen ������ ����ü ���� �迭�� ���忭 ����
 * @return SUCCESS
 * @throws ERROR ������ bigint ����ü ���Ҵ� ��
 */
int BI_Set_By_Array(bigint** x, int sign, word* a, int wordlen)
{
	int i;
	if (*x == NULL)
		return ERROR;

	(*x)->sign = sign; // bigint ����ü�� ��ȣ ����
	(*x)->wordlen = wordlen; // bigint ����ü�� ���忭 ���� ����

	for (i = 0; i < wordlen; i++)
		(*x)->a[i] = a[i]; // ���� ���� �迭 a�� �ִ� �� bigint ����ü ������ ���� ���� �迭�� ����

	return SUCCESS;
}

/**
 * @brief Set Bigint by String
 * @details �Է¹��� ���ڿ��� big integer ����
 * @param bigint** x ������ bigint �� ���������� ����
 * @param int sign ������ bigint�� ��ȣ
 * @param char* str ���ڿ��� char �� ������ ����
 * @param word base ������ ������ base
 * @return SUCCESS
 * @throws
	ERROR ������ bigint ����ü ���Ҵ� ��
	ERROR ���ڿ��� ��ȯ�� hex �� �迭 �Ҵ� ���� ��
	ERROR hex ������ ��ȯ ���� ��
	ERROR base�� 2, 16�� �ƴ� ���
 */
int BI_Set_By_String(bigint** x, int sign, char* str, word base)
{
	char* hex;
	int i, j, k;
	int re;
	int len = strlen(str);

	if (*x == NULL)
		return ERROR;

	if ((base != 16) && (base != 2))
		return ERROR;

	hex = (char*)calloc(sizeof(word), len); // �Է¹��� string ���� hex ������ ��ȯ��ų �� ������ ��
	if (hex == NULL)
		return ERROR;

	re = Ascii_To_Hex(str, hex); // �Է¹��� string �� hex�� ��ȯ
	if (re == ERROR)
		return ERROR;

	if (base == 2) // 2������ ���� �� (11 01101010 10101111 -> 3 byte �濡 a[2] a[1] a[0])
	{
		if ((len % WORD_BIT_LEN) != 0) // �Է¹��� string ���̰� WORD_BIT_LEN�� ����� �ƴϸ�
		{
			for (i = 0; i < len / WORD_BIT_LEN; i++) // �Է¹��� string ���̸� WORD_BIT_LEN���� ���� ��ŭ
			{
				for (j = 0; j < WORD_BIT_LEN; j++)
					(*x)->a[i] |= (hex[len - 1 - (WORD_BIT_LEN * i) - j] << j);
				// string�� hex�� ��ȯ�� �迭 ������ (little endian) �ϳ��� ������� bigint ����ü �迭�� �־� �ֱ�
			}

			for (k = 0; k < len % WORD_BIT_LEN; k++) // �Է¹��� string ���̸� WORD_BIT_LEN���� ���� ��������ŭ
				(*x)->a[len / WORD_BIT_LEN] |= (hex[k] << ((len % WORD_BIT_LEN) - k - 1));
			// string�� hex�� ��ȯ�� ó�� �迭�� �ִ� �� bigint ����ü ������ �迭�� �־� �ֱ� (���ÿ����� 11) 
		}

		else // �Է¹��� string ���̰� WORD_BIT_LEN�� ����̸�
		{
			for (i = 0; i < len / WORD_BIT_LEN; i++)
			{
				for (j = 0; j < WORD_BIT_LEN; j++)
					(*x)->a[i] |= (hex[len - 1 - WORD_BIT_LEN * i - j] << j);
				// string�� hex�� ��ȯ�� �迭 ������ (little endian) �ϳ��� ������� bigint ����ü �迭�� �־� �ֱ�
			}
		}
	}

	else if (base == 16) // "123456789" -> {0x89, 0x67, 0x45, 0x23, 0x01} / {0x6789, 0x2345, 0x0001}
	{
		// '0x89' �̷� ������ ���� 2���� 1 byte�̹Ƿ� WORD_BIT_LEN�� 8 -> 4, 32 -> 8, 64 -> 16���� ���ڴϱ� WORD_BIT_LEN / 4
		if (len % (WORD_BIT_LEN / 4) != 0)
		{
			for (i = 0; i < len / (WORD_BIT_LEN / 4); i++)
			{
				for (j = 0; j < WORD_BIT_LEN / 4; j++)
					(*x)->a[i] |= hex[len - 1 - (WORD_BIT_LEN / 4) * i - j] << (4 * j);
				// string�� hex�� ��ȯ�� �迭 ������ (little endian) �ϳ��� ������� bigint ����ü �迭�� �־� �ֱ�
			}

			for (k = 0; k < len % (WORD_BIT_LEN / 4); k++)
				(*x)->a[len / (WORD_BIT_LEN / 4)] |= (hex[(len % (WORD_BIT_LEN / 4)) - 1 + k] << (4 * k));
			// string�� hex�� ��ȯ�� ó�� �迭�� �ִ� �� bigint ����ü ������ �迭�� �־� �ֱ� (���ÿ����� 1)
		}

		else
		{
			for (i = 0; i < (len / (WORD_BIT_LEN / 4)); i++)
			{
				for (j = 0; j < WORD_BIT_LEN / 4; j++)
					(*x)->a[i] |= hex[len - 1 - (WORD_BIT_LEN / 4) * i - j] << (4 * j);
				// string�� hex�� ��ȯ�� �迭 ������ (little endian) �ϳ��� ������� bigint ����ü �迭�� �־� �ֱ�
			}
		}
	}

	return SUCCESS;
}

/**
 * @brief Transform String to Hex
 * @details �Է¹��� ���ڿ��� hex ������ ��ȯ
 * @param char* str ���ڿ��� ����Ű�� char �� ������ ����
 * @param char* hex ��ȯ�� hex ���� ������ char �� ������ ����
 * @return SUCCESS
 * @throws ERROR 0~9, A~F�� �ƴ� ���� ������ ���
 */
int Ascii_To_Hex(char* str, char* hex)
{
	int len = 0;
	int i;

	len = strlen(str);

	for (i = 0; i < len; i++)
	{
		if (str[i] > 0x29 && str[i] < 0x40) // 0x30~0x39 -> 0~9
			hex[i] = str[i] - 0x30; // 0 ~ 9�� -0x30
		else if (str[i] > 0x40 && str[i] < 0x47) // 0x41~0x46 -> A~F
			hex[i] = str[i] - 0x31; // a ~ f�� -0x31
		else
			return ERROR;
	}

	return SUCCESS;
}

/**
 * @brief Show Big Integer
 * @details �Է¹��� ���ڿ��� hex ������ ��ȯ
 * @param bigint* x ����� ���� ����Ű�� bigint �� ������ ����
 * @param int base ����ϰ� ���� ����(����)
 * @return SUCCESS
 * @throws
	ERROR ����� bigint ����ü ���Ҵ� ��
	ERROR base�� 2, 16�� �ƴ� ���
 */
int BI_Show(bigint* x, int base)
{
	int i = 0;
	int j = 0;
	int k = 0;

	if (x == NULL)
		return ERROR;

	if ((base != 16) && (base != 2))
		return ERROR;

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
	return SUCCESS;
}

/**
 * @brief Refine Big Integer (Remove Last Zero Words)
 * @details ����ü ���Ҵ� (����ü ���� �迭���� MSB���� 0���� ä���� ���忭 ����)
 * @param bigint* x ���Ҵ��� bigint �� ������ ����
 * @return SUCCESS
 * @throws
	ERROR �缳���� bigint ����ü ���Ҵ� ��
	ERROR ����ü ���Ҵ� ���� ��
 */
int BI_Refine(bigint* x)
{
	word* temp = NULL;

	if (x == NULL)
		return ERROR;

	int new_wordlen = x->wordlen;

	while (new_wordlen > 1) // (x�� ���忭 ����-  1)����ŭ �ݺ�
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
			return ERROR;
	}

	if (BI_Is_Zero(&x) == 1) // 0�� ���
		x->sign = NON_NEGATIVE; // ��ȣ ����

	return SUCCESS;
}

/**
 * @brief Assign Big Integer
 * @details bigint ����ü ����
 * @param bigint** x ������ ���� ������ bigint �� ���������� ����
 * @param bigint* y ������ ���� ����Ű�� bigint �� ������ ����
 * @return SUCCESS
 */
int BI_Assign(bigint** y, bigint* x)
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

	return SUCCESS;
}

/**
 * @brief Generate Random Number Big Integer
 * @details ������ big integer ����
 * @param bigint** x ������ �������� ������ bigint �� ���������� ����
 * @param int sign ������ �������� ��ȣ
 * @param int wordlen ������ �������� ����
 * @return SUCCESS
 * @throws
	ERROR ������ ���� ���� ��
	ERROR Refine ���� ��
 */
int BI_Gen_Rand(bigint** x, int sign, int wordlen)
{
	int re;

	BI_New(x, wordlen); // bigint ����ü ����
	(*x)->sign = sign; // ��ȣ ����
	
	re = Array_Rand((*x)->a, wordlen); // random���� �迭 ����
	if (re == ERROR)
		return ERROR;

	re = BI_Refine(*x); // ���� 0���� ä���� �κ� �ڸ���
	if (re == ERROR)
		return ERROR;

	return SUCCESS;
}

/**
 * @brief Generate Random number in Array
 * @details ������ �����Ͽ� �迭�� ����
 * @param word* dst ������ �������� ������ ������ ����
 * @param int wordlen ������ �������� ����
 * @return SUCCESS
 * @throws
	ERROR dst ���Ҵ� ��
 */
int Array_Rand(word* dst, int wordlen)
{
	if (dst == NULL)
		return ERROR;

	unsigned char* p = (unsigned char*)dst; // rand() �Լ��� ��°��� 15bit�̹Ƿ� ���� ������ �ƴ� �� ����Ʈ ������ �迭�� ������ ����
	int cnt = wordlen * sizeof(word);
	
	while (cnt > 0)
	{
		*p = rand() & 0xff; // cnt��ŭ random�� �� �����ؼ� �迭�� ����
		p++;
		cnt--;
	}

	return SUCCESS;
}

/**
 * @brief Get Word Length
 * @details ����ü bigint�� ���忭�� ����
 * @param int* len ���忭�� ���̸� ������ int �� ������ ����
 * @param bigint** x ���忭�� ���̸� ���� bigint �� ���������� ����
 * @return SUCCESS
 * @throws ERROR ����ü ���Ҵ� ��
 */
int BI_Get_Word_Length(int* len, bigint** x)
{
	if (*x == NULL)
		return ERROR;

	*len = (*x)->wordlen; // Big Integer x�� wordlen�� ����
	return SUCCESS;
}

/**
 * @brief Get Bit Length
 * @details ����ü bigint�� ��Ʈ�� ����
 * @param int* len ��Ʈ���� ���̸� ������ int �� ������ ����
 * @param bigint** x ��Ʈ���� ���̸� ���� bigint �� ���������� ����
 * @return SUCCESS
 * @throws ERROR ����ü ���Ҵ� ��
 */
int BI_Get_Bit_Length(int* len, bigint* x)
{
	int i = 0;
	int j = 0;
	int k = 0;

	if (x == NULL)
		return ERROR;

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

	return SUCCESS;
}

/**
 * @brief Get j-th bit of Big Integer
 * @details j��° ��Ʈ �Ǻ� (0 �Ǵ� 1)
 * @param int* len ��Ʈ���� ���̸� ������ int �� ������ ����
 * @param bigint** x ��Ʈ���� ���̸� ���� bigint �� ���������� ����
 * @return j-th bit of Big Integer (0 �Ǵ� 1)
 * @throws
	ERROR ����ü ���Ҵ� ��
	ERROR j�� x�� big length���� Ŭ ��
 */
int BI_Get_j_th_Bit_of_BI(int j, bigint* x)
{
	unsigned long long i = 0;
	int k = 0;
	int r = 0;

	if (x == NULL)
		return ERROR;

	if (j >= (x->wordlen) * WORD_BIT_LEN) // x�� bit len���� ũ�� ����
		return ERROR;

	k = j / WORD_BIT_LEN; // ���� k�� j��° ��Ʈ�� ��ġ�� ������ ��ġ(�� ��° ����)
	r = j % WORD_BIT_LEN; // x->a[k]���� r��ŭ �������� �̵��ؾ� ��ü���� j��° ��ġ
	i = 1;
	i = i << r;

	if (i == (x->a[k] & i)) // shift�� i�� j��° ��Ʈ�� ��ġ�� ����� &������ ���� j��° ��Ʈ�� ���� �˾Ƴ���
		return 1;
	else
		return 0;
}

/**
 * @brief Get Sign of Big Integer
 * @details big integer�� ��ȣ �Ǻ�
 * @param bigint* x ��ȣ�� �Ǻ��� bigint �� ������ ����
 * @return sign (NEGATIVE / NON_NEGATIVE)
 * @throws
	ERROR ����ü ���Ҵ� ��
	ERROR x�� ��ȣ�� NON_NEGATIVE�� NEGATIVE�� �ƴ� ���
 */
int BI_Get_Sign(bigint* x)
{
	if (x == NULL)
		return ERROR;
	
	if ((x->sign) == NON_NEGATIVE) // bigint ����ü�� ��ȣ�� NON_NEGATIVE�� NON_NEGATIVE return
		return NON_NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint ����ü�� ��ȣ�� NEGATIVE�� NEGATIVE return
		return NEGATIVE;
	else // �� �� �ƴ� �� ERROR return
		return ERROR;
}

/**
 * @brief Flip sign of Big Integer
 * @details big integer�� ��ȣ�� �ٲ� �ִ� �Լ�
 * @param bigint* x ��ȣ�� �ٲ� bigint �� ������ ����
 * @return SUCCESS
 * @throws
	ERROR ����ü ���Ҵ� ��
	ERROR x�� ��ȣ�� NON_NEGATIVE�� NEGATIVE�� �ƴ� ���
 */
int BI_Flip_Sign(bigint* x)
{
	if (x == NULL)
		return ERROR;

	if ((x->sign) == NON_NEGATIVE) // bigint ����ü�� ��ȣ�� NON_NEGATIVE�� ��ȣ�� NEGATIVE�� ����
		x->sign = NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint ����ü�� ��ȣ�� NEGATIVE�� ��ȣ�� NON_NEGATIVE�� ����
		x->sign = NON_NEGATIVE;
	else
		return ERROR;

	return SUCCESS;
}

/**
 * @brief Set Big Integer to 1
 * @details big integer�� 1�� ����
 * @param bigint** x 1�� ������ bigint �� ���������� ����
 * @return SUCCESS
 */
int BI_Set_One(bigint** x)
{
	BI_New(x, 1);
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x1; // ù��° �迭���� 1 ���� (1�� ������ �ϹǷ�)

	return SUCCESS;
}

/**
 * @brief Set Big Integer to 0
 * @details big integer�� 0���� ����
 * @param bigint** x 0���� ������ bigint �� ���������� ����
 * @return SUCCESS
 */
int BI_Set_Zero(bigint** x)
{
	BI_New(x, 1); // ���̰� 1�� bigint x ����
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x00; // ù��° �迭���� ���� (0�� ������ �ϹǷ�)

	return SUCCESS;
}

/**
 * @brief Determine Big Integer is 1
 * @details big integer�� 1���� �Ǻ�
 * @param bigint** x �Ǻ��� bigint �� ���������� ����
 * @return TRUE or FALSE
 * @throws
	ERROR ����ü ���Ҵ� ��
 */
int BI_Is_One(bigint** x)
{
	int i = 0;

	if (*x == NULL)
		return ERROR;

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 1) // ��ȣ�� �����̰ų�, LSB�� 1�� �ƴϸ� FALSE
		return FALSE;

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen��ŭ �ϳ��� �̵��� ���鼭 LSB�� 1�̰� �迭�� ������ ���� 0���� Ȯ��
	{
		if ((*x)->a[i] != 0)
			return FALSE;
	}
	
	return TRUE;
}

/**
 * @brief Determine Big Integer is 0
 * @details big integer�� 0���� �Ǻ�
 * @param bigint** x �Ǻ��� bigint �� ���������� ����
 * @return TRUE or FALSE
 * @throws
	ERROR ����ü ���Ҵ� ��
 */
int BI_Is_Zero(bigint** x)
{
	int i = 0;

	if (*x == NULL)
		return ERROR;

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 0) // ��ȣ�� �����̰� LSB�� 0�� �ƴϸ� FALSE
		return FALSE;

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen��ŭ �ϳ��� �̵��� ���鼭 �迭�� ���� ���� 0���� Ȯ��
	{
		if ((*x)->a[i] != 0)
			return FALSE;
	}
	
	return TRUE;
}

/**
 * @brief Compare BigInt x with BigInt y
 * @details
	big integer x�� y�� ũ�� ��
	x > y : 1
	x = y : 0
	x < y : 2
 * @param bigint** x ���� bigint �� ���������� ����
 * @param bigint** y ���� bigint �� ���������� ����
 * @return 0 or 1 or 2
 * @throws
	ERROR ����ü x(y) ���Ҵ� ��
 */
int BI_Compare(bigint** x, bigint** y) // return : 1(x > y), 0(x == y), 2(x < y) 
{
	int i = 0;
	int len_x, len_y = 0;

	if ((*x == NULL) | (*y == NULL))
		return ERROR;

	if ((*x)->sign < (*y)->sign) // A�� ���, B�� ������ �翬�� A�� ũ�Ƿ�
		return 1;

	else if ((*x)->sign > (*y)->sign) // A�� ����, B�� ����� �翬�� B�� ũ�Ƿ�
		return 2;

	else // A, B ��ȣ�� ���� ��
	{
		BI_Get_Word_Length(&len_x, x); //len_x = x->wordlen
		BI_Get_Word_Length(&len_y, y); //len_y = y->wordlen

		if ((*x)->sign == 0) // A, B ��ȣ�� ����� �� (��ȣ�� ���� �����Ƿ�, if ���� ���� �ϳ��� ��)
		{
			if (len_x > len_y) // x�� ���̰� y���� ���
				return 1;	   // x�� ��ٴ� 1�� ���
			else if (len_x < len_y) // y�� ���̰� x���� ���
				return 2;			// y�� ��ٴ� 2 ���
			else // x�� ���� = y�� ����
			{
				for (i = len_x - 1; i >= 0; i--) //(int)len_x --> len_x�� �ٲ�.20.11.23.sy // �� ��
				{
					if ((*x)->a[i] > (*y)->a[i])
						return 1;
					else if ((*x)->a[i] < (*y)->a[i])
						return 2;
				}

				return 0;
			}
		}

		else // A, B ��ȣ�� ������ ��
		{
			// ����� ���ʹ� �ݴ밡 �ǵ��� return
			if (len_x > len_y)
				return 2;
			else if (len_x < len_y)
				return 1;
			else
			{
				for (i = (int)len_x - 1; i >= 0; i--)
				{
					if ((*x)->a[i] > (*y)->a[i])
						return 1;
					else if ((*x)->a[i] < (*y)->a[i])
						return 2;
				}

				return 0;
			}
		}
	}
}

/**
 * @brief Left shift
 * @details big integer << len
 * @param bigint* x shift ��ų bigint �� ������ ����
 * @param int len shift ��ų ��Ʈ ��
 * @return SUCCESS
 * @throws
	ERROR ����ü ���Ҵ� ��
	ERROR realloc ���� ��
 */
int BI_Left_Shift(bigint* x, int len) // len: �̵��� ��Ʈ ��
{
	int length = 0;
	int add_len = 0;
	int q = 0;
	int new_wordlen = 0;
	int i = 0;
	int wn = 0;
	int count = 0;
	int r = 0;

	if (x == NULL)
		return ERROR;

	word* temp = NULL;
	bigint* cp = NULL;
	BI_Assign(&cp, x); // bigint x�� bigint cp�� ����

	length = x->wordlen;
	wn = WORD_BIT_LEN * length;

	if ((len % WORD_BIT_LEN) == 0) // �̵��� ��Ʈ ���� WORD_BIT_LEN�� ����̸�
		add_len = len / WORD_BIT_LEN; // �̵��� ��Ʈ ���� WORD_BIT_LEN�� ���� ��ŭ ���� ���� �߰�
	else
		add_len = (len / WORD_BIT_LEN) + 1; // �̵��� ��Ʈ ���� WORD_BIT_LEN�� ���� �� + 1��ŭ ���� ���� �߰�

	new_wordlen = add_len + x->wordlen; // �߰��� ���� ���� + ���� ���� ����

#if OS == LINUX
	temp = (word*)realloc(x->a, sizeof(word) * new_wordlen); // new_wordlen��ŭ bigint ����ü ���Ҵ�
#elif OS == WINDOW
	temp = (word*)_recalloc(x->a, new_wordlen, sizeof(word));
	// _recalloc�� window������ ���ư��� �Լ� (visual studio���� realloc ���� warning�� ��)
#endif

	if (temp != NULL)
		x->a = temp;
	else
		return ERROR;

	x->wordlen = new_wordlen; // ���Ҵ��� ����ü ���� ����
	memset(&x->a[length], 0x00, add_len); // �߰��� ���� ���̸�ŭ 0���� �ʱ�ȭ

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
		for (i = 0; i <= q; i++)
			x->a[i] = 0; // 0��° �迭 ~ (a - 1)��° �迭 0���� �ʱ�ȭ

		x->a[q] = cp->a[0] << r; // q��° �迭�� ���� �迭 �� 0��° �迭�� r��ŭ �������� �̵��� �� ����

		for (i = 1; i <= cp->wordlen - 1; i++) // 1��° �迭 ~ �迭�� �� �ٷ� �� = (Aj << r) || (Aj-1 >> (WORD_BIT_LEN - r))
			x->a[i + q] = (cp->a[i] << r) | (cp->a[i - 1] >> (WORD_BIT_LEN - r));

		x->a[x->wordlen - 1] = (cp->a[cp->wordlen - 1] >> (WORD_BIT_LEN - r)); // ������ �迭 = An-1 >> (WORD_BIT_LEN - r)
	}

	BI_Delete(&cp);
	BI_Refine(x);

	return SUCCESS;
}

/**
 * @brief Right shift
 * @details big integer >> len
 * @param bigint* x shift ��ų bigint �� ������ ����
 * @param int len shift ��ų ��Ʈ ��
 * @return SUCCESS
 * @throws
	ERROR ����ü ���Ҵ� ��
 */
int BI_Right_Shift(bigint* x, int len)
{
	int i = 0;
	int wn = 0;
	int count = 0;
	int r = 0;
	int q = 0;

	if (x == NULL)
		return ERROR;

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
	else
	{
		BI_Delete(&cp);
		BI_Refine(x);

		return ERROR;
	}

	BI_Delete(&cp);
	BI_Refine(x);

	return SUCCESS;
}

/**
 * @brief Reduction
 * @details big integer mod r
 * @param bigint* x modular ��ų bigint �� ���������� ����
 * @param int r ��ⷯ�� ���ϴ� ��
 * @return SUCCESS
 * @throws ERROR ����ü ���Ҵ� ��
 */
int BI_Reduction(bigint** x, int r)
{
	int i = 0;
	int wn = 0;
	int count = 0;
	int size = 0;

	if (*x == NULL)
		return ERROR;

	size = (*x)->wordlen;
	wn = WORD_BIT_LEN * size;

	if (r >= wn)
	{
		return SUCCESS;
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
		
		r = r % WORD_BIT_LEN;
		(*x)->a[i] = (*x)->a[i] & (((word)1 << r) - 1);
		
		for (i = count + 1; i < size; i++) // �ش� ������ �������� �ֻ��� ������� 0���� ����
			(*x)->a[i] = 0;
	}

	BI_Refine(*x);

	return SUCCESS;
}

/**
 * @brief Compare and return longer word length
 * @details
	�� bigint�� ���忭 ���̸� ���Ͽ� �� �� ���忭�� ���� ��ȯ
 * @param bigint* A ���� ���� �񱳸� ������ bigint �� ������ ����
 * @param bigint* B ���� ���� �񱳸� ������ bigint �� ������ ����
 * @return int MAX(WordLen(A), WordLen(B)) �� �� ���忭 ����
 * @throws ERROR A(B)�� word length�� ����� �ƴ� ���
 */
int Compare_WordLen(bigint* A, bigint* B) // return wordlen ū ������
{
	int A_Len, B_Len;

	BI_Get_Word_Length(&A_Len, &A); // A�� ���忭 ����
	BI_Get_Word_Length(&B_Len, &B); // B�� ���忭 ����

	if (A_Len <= 0)
		return ERROR;
	if (B_Len <= 0)
		return ERROR;

	if (A_Len > B_Len) // A�� ���忭�� ���̰� �� ���
		return A_Len; // A�� ���忭 ���� return
	else // B�� ���忭�� ���̰� �� ���
		return B_Len; // B�� ���忭 ���� return
}