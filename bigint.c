#include "config.h"
#include "bigint.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define   zerorize

// Chpater 2.1
void array_init(word* a, int len)
{
	memset(a, 0, sizeof(word) * len); // 배열의 길이만큼 초기화
}

/*
	Delete BigInt function

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
void BI_Delete(bigint** x)
{
	if (*x == NULL) // bigint 구조체의 주소가 NULL 값이면 삭제할 필요 없기 때문에 return
		return;

#ifdef zerorize
	array_init((*x)->a, (*x)->wordlen); // 배열 초기화
#endif 
	free((*x)->a); // bigint 구조체 안의 word 배열 free
	free(*x); // bigint 구조체 free
	*x = NULL; // bigint 구조체의 주소값 NULL로 초기화
}

/*
	Create BigInt function

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
void BI_New(bigint** x, int wordlen)
{
	if (*x != NULL) //   x의 주소가 NULL이 아니면
		BI_Delete(x); // BI_Delete()로 초기화

	*x = (bigint*)malloc(sizeof(bigint)); // bigint *x 할당
	if (*x == NULL)
		return;
	(*x)->sign = NON_NEGATIVE; // 양수이고
	(*x)->wordlen = wordlen;   // 2번째 매개변수와 같은 길이인
	(*x)->a = (word*)calloc(wordlen, sizeof(word)); // bigint x 내부의 배열 a 할당
}

// Chapter 2.2 Show BigInt
/*
	Set BigInt by Array

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
void BI_Set_By_Array(bigint** x, int sign, word* a, int wordlen)
{
	int i;

	(*x)->sign = sign; // bigint 구조체의 부호 설정
	(*x)->wordlen = wordlen; // bigint 구조체의 워드열 길이 설정

	for (i = 0; i < wordlen; i++)
		(*x)->a[i] = a[i]; // 워드 단위 배열 a에 있는 값 bigint 구조체 내부의 워드 단위 배열에 대입
}

/*
	Set BigInt by String

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
void BI_Set_By_String(bigint** x, int sign, char* str, word base, int size)
{
	char* hex;
	int i, j, k;

	hex = (char*)malloc(size * sizeof(word)); // 입력받은 string 값을 hex 값으로 변환시킬 때 저장할 곳
	if (hex == NULL)
		return;
	Ascii_To_Hex(str, hex); // 입력받은 string 값 hex로 변환

	if (base == 2) // 2진수로 받은 것 (11 01101010 10101111 -> 3 byte 방에 a[2] a[1] a[0])
	{
		if ((strlen(str) % WORD_BIT_LEN) != 0) // 입력받은 string 길이가 WORD_BIT_LEN의 배수가 아니면
		{
			for (i = 0; i < (int)(strlen(str) / WORD_BIT_LEN); i++) // 입력받은 string 길이를 WORD_BIT_LEN으로 나눈 몫만큼
			{
				for (j = 0; j < WORD_BIT_LEN; j++)
					(*x)->a[i] |= (hex[strlen(str) - 1 - (WORD_BIT_LEN * i) - j] << j);
				// string을 hex로 변환한 배열 끝부터 (little endian) 하나씩 순서대로 bigint 구조체 배열에 넣어 주기
			}

			for (k = 0; k < (int)(strlen(str) % WORD_BIT_LEN); k++) // 입력받은 string 길이를 WORD_BIT_LEN으로 나눈 나머지만큼
				(*x)->a[(strlen(str) / WORD_BIT_LEN)] |= (hex[k] << ((strlen(str) % WORD_BIT_LEN) - k - 1));
			// string을 hex로 변환한 처음 배열에 있는 것 bigint 구조체 마지막 배열에 넣어 주기 (예시에서는 11) 
		}

		else // 입력받은 string 길이가 WORD_BIT_LEN의 배수이면
		{
			for (i = 0; i < (int)(strlen(str) / WORD_BIT_LEN); i++)
			{
				for (j = 0; j < WORD_BIT_LEN; j++)
					(*x)->a[i] |= (hex[strlen(str) - 1 - WORD_BIT_LEN * i - j] << j);
				// string을 hex로 변환한 배열 끝부터 (little endian) 하나씩 순서대로 bigint 구조체 배열에 넣어 주기
			}
		}
	}

	else if (base == 10)
		return; // 추후에 구현할 것

	else if (base == 16) // "123456789" -> {0x89, 0x67, 0x45, 0x23, 0x01} / {0x6789, 0x2345, 0x0001}
	{
		// '0x89' 이런 식으로 숫자 2개가 1 byte이므로 WORD_BIT_LEN이 8 -> 4, 32 -> 8, 64 -> 16개의 숫자니까 WORD_BIT_LEN / 4
		if ((strlen(str)) % (WORD_BIT_LEN / 4) != 0)
		{
			for (int i = 0; i < (int)(strlen(str) / (WORD_BIT_LEN / 4)); i++)
			{
				for (int j = 0; j < WORD_BIT_LEN / 4; j++)
					(*x)->a[i] |= hex[strlen(str) - 1 - (WORD_BIT_LEN / 4) * i - j] << (4 * j);
				// string을 hex로 변환한 배열 끝부터 (little endian) 하나씩 순서대로 bigint 구조체 배열에 넣어 주기
			}

			for (int k = 0; k < (int)(strlen(str) % (WORD_BIT_LEN / 4)); k++)
				(*x)->a[strlen(str) / (WORD_BIT_LEN / 4)] |= (hex[(strlen(str) % (WORD_BIT_LEN / 4)) - 1 + k] << (4 * k));
			// string을 hex로 변환한 처음 배열에 있는 것 bigint 구조체 마지막 배열에 넣어 주기 (예시에서는 1)
		}

		else
		{
			for (int i = 0; i < (int)(strlen(str) / (WORD_BIT_LEN / 4)); i++)
			{
				for (int j = 0; j < WORD_BIT_LEN / 4; j++)
					(*x)->a[i] |= hex[strlen(str) - 1 - (WORD_BIT_LEN / 4) * i - j] << (4 * j);
				// string을 hex로 변환한 배열 끝부터 (little endian) 하나씩 순서대로 bigint 구조체 배열에 넣어 주기
			}
		}
	}

}

// 입력받은 string -> hex로 변환
void Ascii_To_Hex(char* str, char* hex)
{
	int len = 0;
	len = strlen(str);

	for (int i = 0; i < len; i++)
	{
		if (str[i] < 0x40)
			hex[i] = str[i] - 0x30; // 0 ~ 9는 -0x30
		else if (str[i] < 0x47)
			hex[i] = str[i] - 0x31; // a ~ f는 -0x31
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

	if (base == 10) // 10진수일 때
	{
		printf("%d", x->a[0]);
		for (i = 1; i < x->wordlen; i++)
		{
			printf("%d * 2^%d", x->a[i], i * WORD_BIT_LEN); // 10진수 출력을 어떻게 해야할지 몰라서
		}
	}

	else if (base == 16) // 16진수일 때
	{
		if (x->sign == 1)
			printf("-"); // test vector 찍어볼 때 -가 들어가야 정확히 나오기 때문에 음수일 때는 - 찍도록
		printf("0x"); // hex일 때는 0x
		for (i = x->wordlen - 1; i >= 0; i--)
		{
			if (WORD_BIT_LEN == 32)
				for (j = 0; j < 4; j++) // word 길이가 32이면, 4bytes 이므로 1byte씩 찍도록
					printf("%02x", ((x->a[i]) >> (32 - (j + 1) * 8)) & 255); // 한 바이트씩 찍도록 0xff(255)로 마스킹

			else if (WORD_BIT_LEN == 64)
				for (j = 0; j < 8; j++) // word 길이가 64이면, 8bytes 이므로 1byte씩 찍도록
					printf("%02x", (x->a[i]) >> (64 - (j + 1) * 8) & 255); // 한 바이트씩 찍도록 0xff(255)로 마스킹

			else
				printf("%02x", (x->a[i]));
		}
	}

	else if (base == 2) // 2진수일 때
	{
		printf("0b");
		for (i = x->wordlen - 1; i >= 0; i--) // 최상위비트부터 출력하도록
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
		if (x->a[new_wordlen - 1] != 0) // 0이 아닌 수를 찾으면 끝내기
			break;
		new_wordlen--; // new_wordlen = 0이 아닌 수들로만 구성된 word의 길이
	}

	if (x->wordlen != new_wordlen)
	{
		x->wordlen = new_wordlen;
		temp = (word*)realloc(x->a, sizeof(word) * new_wordlen); // new_wordlen만큼의 워드형 배열 realloc

		if (temp != NULL)
			x->a = temp;
		else
			return;
	}

	if ((x->wordlen == 1) && (x->a[0] == 0x0))
		x->sign = NON_NEGATIVE; // 부호 결정
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
	int size = 0; //(추가) x->wordlen을 집어넣을 변수 size
	size = x->wordlen;
	if (*y != NULL) // 새롭게 생성할 y가 NULL이 아니면
		BI_Delete(y); // 초기화 시키기
	BI_New(y, x->wordlen); // x의 길이와 같은 y를 생성
	(*y)->sign = x->sign; // x의 부호와도 같게 만들기
	for (i = 0; i < size; i++)
		(*y)->a[i] = x->a[i]; // x의 값이 y에 다 들어가도록
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
	BI_New(x, wordlen); // bigint 구조체 생성
	(*x)->sign = sign; // 부호 결정
	Array_Rand((*x)->a, wordlen); // random으로 배열 설정

	BI_Refine(*x); // 앞쪽 0으로 채워진 부분 자르기
}

void Array_Rand(word* dst, int wordlen)
{
	unsigned char* p = (unsigned char*)dst; // rand() 함수의 출력값이 15bit이므로 워드 단위가 아닌 한 바이트 단위로 배열에 랜덤값 설정
	int cnt = wordlen * sizeof(word);
	while (cnt > 0)
	{
		*p = rand() & 0xff; // cnt만큼 random한 값 생성해서 배열에 저장
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
	*len = (*x)->wordlen; // Big Integer x의 wordlen를 대입
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
	i = (x->wordlen) - 1; // 변수 i에는 x의 wordlen - 1 값을 대입

	for (j = WORD_BIT_LEN - 1; j >= 0; j--) // 최상위 워드에 최상위 비트에 값이 차있는게 아니므로, 최상위 워드에 비트가 존재하는 곳 찾기
	{ // 반복문을 통해 최상위 워드의 최상위 비트부터 차례대로 값이 들어가있는지 확인
		k = 1;
		k = k << j;
		if ((x->a[i] & k) != 0)
			break; // 값이 들어가 있으면( != 0) break;
	}
	j += i * WORD_BIT_LEN + 1; // 최상위 워드의 몇번째 비트의 길이인지 출력하도록 j에 대입
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
	int r = 0; // 서우 추가
	//printf("j_th bit : ");
	if (j >= (x->wordlen) * WORD_BIT_LEN) // x의 bit len보다 길면 에러
		return ERROR;
	else
	{
		k = j / WORD_BIT_LEN; // 변수 k는 j번째 비트가 위치한 워드의 위치(몇 번째 워드)
		r = j % WORD_BIT_LEN; // x->a[k]에서 r만큼 왼쪽으로 이동해야 전체에서 j번째 위치 (서우 추가)
		i = 1;
		//i = i << j; // 1을 j번 left shift. ex) 1을 3만큼 left shift --> 1000
		i = i << r; // 서우 추가

		if (i == (x->a[k] & i)) // shift한 i와 j번째 비트가 위치한 워드와 &연산을 통해 j번째 비트의 값을 알아내기
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
	if ((x->sign) == NON_NEGATIVE) // bigint 구조체의 부호가 NON_NEGATIVE면 NON_NEGATIVE return
		return NON_NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint 구조체의 부호가 NEGATIVE면 NEGATIVE return
		return NEGATIVE;
	else // 둘 다 아닐 때 ERROR return
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
	if ((x->sign) == NON_NEGATIVE) // bigint 구조체의 부호가 NON_NEGATIVE면 부호를 NEGATIVE로 변경
		x->sign = NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint 구조체의 부호가 NEGATIVE면 부호를 NON_NEGATIVE로 변정
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
	BI_New(x, 1); // 길이가 1인 bigint x 생성
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x00; // 0을 만들어야하므로
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

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 1) // 부호가 음수이거나, LSB가 1이 아니면 FALSE
		return -1; // False

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen만큼 하나씩 이동해 가면서 LSB가 1이고 배열의 나머지 값이 0인지 확인
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

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 0) // 부호가 음수이고 LSB가 0이 아니면 FALSE
		return -1; // False

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen만큼 하나씩 이동해 가면서 배열의 값이 전부 0인지 확인
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

	if ((*x)->sign < (*y)->sign) // A가 양수, B가 음수면 당연히 A가 크므로
		return 1;
	else if ((*x)->sign > (*y)->sign) // A가 음수, B가 양수면 당연히 B가 크므로
		return -1;
	else // A, B 부호가 같을 때
	{
		BI_Get_Word_Length(&len_x, x); //len_x = x->wordlen
		BI_Get_Word_Length(&len_y, y); //len_y = y->wordlen
		if ((*x)->sign == 0) // A, B 부호가 양수일 때 (부호가 서로 같으므로, if 문을 통해 하나만 비교)
		{
			if (len_x > len_y) // x의 길이가 y보다 길면
				return 1;	   // x가 길다는 1을 출력
			else if (len_x < len_y) // y의 길이가 x보다 길면
				return -1;			// y가 길다는 -1 출력
			else // x의 길이 = y의 길이
			{
				for (i = len_x - 1; i >= 0; i--) //(int)len_x --> len_x로 바꿈.20.11.23.sy // 값 비교
				{
					if ((*x)->a[i] > (*y)->a[i])
						return 1;
					else if ((*x)->a[i] < (*y)->a[i])
						return -1;
				}
				return 0;
			}
		}
		else // A, B 부호가 음수일 때
		{
			// 양수일 때와는 반대가 되도록 return
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
void Left_Shift(bigint* x, int len) // len: 이동할 비트 수
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
	BI_Assign(&cp, x); // bigint x를 bigint cp에 복사

	wn = WORD_BIT_LEN * x->wordlen;

	if ((len % WORD_BIT_LEN) == 0) // 이동할 비트 수가 WORD_BIT_LEN의 배수이면
		add_len = len / WORD_BIT_LEN; // 이동할 비트 수를 WORD_BIT_LEN로 나눈 몫만큼 워드 길이 추가
	else
		add_len = (len / WORD_BIT_LEN) + 1; // 이동할 비트 수를 WORD_BIT_LEN로 나눈 몫 + 1만큼 워드 길이 추가

	new_wordlen = add_len + x->wordlen; // 추가될 워드 길이 + 원래 워드 길이

	temp = (word*)realloc(x->a, sizeof(word) * new_wordlen); // new_wordlen만큼 bigint 구조체 재할당
	if (temp != NULL)
		x->a = temp;

	x->wordlen = new_wordlen; // 재할당한 구조체 길이 설정

	for (i = 0; i < add_len; i++)
		x->a[length + i] = 0; // 추가된 워드 길이만큼 0으로 초기화

	q = len / WORD_BIT_LEN; // 이동할 비트 수를 WORD_BIT_LEN으로 나눈 몫
	r = len % WORD_BIT_LEN; // 이동할 비트 수를 WORD_BIT_LEN으로 나눈 나머지

	if (r == 0) // 이동할 비트 수가 WORD_BIT_LEN으로 나누어 떨어지면
	{
		for (i = length - 1; i >= 0; i--)
			x->a[i + add_len] = x->a[i]; // 기존의 워드열 add_len만큼 왼쪽으로 이동
		for (i = 0; i < add_len; i++)
			x->a[i] = 0; // 0번째 배열 ~ (add_len - 1)번째 배열 0으로 설정
	}

	else if (r != 0) // 이동할 비트 수가 WORD_BIT_LEN으로 나누어 떨어지지 않으면
	{
		for (i = 0; i <= q; i++)
			x->a[i] = 0; // 0번째 배열 ~ (a - 1)번째 배열 0으로 초기화

		x->a[q] = cp->a[0] << r; // q번째 배열에 원래 배열 중 0번째 배열을 r만큼 왼쪽으로 이동한 것 대입

		for (i = 1; i <= cp->wordlen - 1; i++) // 1번째 배열 ~ 배열의 끝 바로 앞 = (Aj << r) || (Aj-1 >> (WORD_BIT_LEN - r))
			x->a[i + q] = (cp->a[i] << r) | (cp->a[i - 1] >> (WORD_BIT_LEN - r));

		x->a[x->wordlen - 1] = cp->a[cp->wordlen - 1] >> (WORD_BIT_LEN - r); // 마지막 배열 = An-1 >> (WORD_BIT_LEN - r)
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

	if (len >= wn) // 오른쪽으로 시프트시킬 길이(len)가 배열의 길이(wn)보다 크면 전부 0으로 설정
	{
		for (i = 0; i < x->wordlen; i++)
			x->a[i] = 0;
	}

	else if ((len < wn) && (len % WORD_BIT_LEN == 0)) // 오른쪽으로 시프트시킬 길이가 배열의 길이보다 작고 WORD_BIT_LEN의 배수면
	{
		count = len / WORD_BIT_LEN; // 오른쪽으로 시프트시킬 워드 길이
		for (i = 0; i < x->wordlen - count; i++) // 이동할 워드 길이만큼 이동
			x->a[i] = x->a[i + count]; // 
		for (i = x->wordlen - count; i < x->wordlen; i++) // 나머지 0으로 설정
			x->a[i] = 0;
	}

	else if ((len < wn) && (len % WORD_BIT_LEN != 0)) // 오른쪽으로 시프트시킬 길이가 배열의 길이보다 작고 WORD_BIT_LEN의 배수가 아니면
	{
		for (i = q; i < x->wordlen - 1; i++)
			x->a[i] = (cp->a[i + 1] << ((WORD_BIT_LEN - r)) | (cp->a[i] >> r)); // 이동할 워드 길이만큼 이동

		x->a[x->wordlen - 1] = cp->a[cp->wordlen - 1] >> r; // 나머지만큼 이동
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
		count = r / WORD_BIT_LEN; // 남을 워드 길이 알아내기

		for (i = count; i < size; i++) // 해당 워드부터 최상위 워드까지 0으로 변경
			(*x)->a[i] = 0;
	}
	else
	{
		count = r / WORD_BIT_LEN;
		/*for (i = 0; i < count; i++)
			(*x)->a[i] = (*x)->a[i] & word_mask;*/ //(추가)주석처리해도 될 것 같아서 함
		r = r % WORD_BIT_LEN;
		(*x)->a[i] = (*x)->a[i] & (((word)1 << r) - 1);
		for (i = count + 1; i < size; i++) // 해당 워드의 다음부터 최상위 워드까지 0으로 변경
			(*x)->a[i] = 0;
	}

	BI_Refine(*x); // 추가
}

//Chapter 3 Addition

/**
 * @brief Addition (캐리를 포함한 단일 덧셈)
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

 * @param bigint** C 덧셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param unsigned int c 이전 연산에서 받아 온 캐리 (0 또는 1)
 * @param int i 덧셈을 수행할 배열의 index

 * @return carry 캐리 여부 (0 또는 1)
 */
unsigned int ADD_ABc(bigint** C, bigint** A, bigint** B, unsigned int c, int i)
{
	int carry = 0;
	word temp = 0; //word* --> word로 바꿈
	temp = (*A)->a[i];
	(*C)->a[i] = (*A)->a[i] + (*B)->a[i]; // A, B 각각의 i번째 값 더해서 C의 i번째 배열에 대입

	if ((*C)->a[i] < temp) // 원래의 A의 i번째 값이 앞에서 계산한 결과보다 작으면 캐리가 발생한 것
		carry = 1;

	(*C)->a[i] = (*C)->a[i] + c; // 인자로 받아온 캐리 값(c) 더해 주기

	if ((*C)->a[i] < c) // 앞의 결과가 c보다 작으면 캐리가 발생한 것
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

 * @param bigint** C 덧셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param int sign 덧셈 결과 C의 부호
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

	// A의 워드열 길이가 B의 워드열 길이보다 큰 경우이므로 B의 워드열 길이를 A의 워드열 길이만큼 늘려서 realloc
	(*B)->wordlen = A_Len;
	temp = (word*)realloc((*B)->a, sizeof(word) * A_Len);
	if (temp != NULL)
		(*B)->a = temp;

	for (i = B_Len; i < A_Len; i++)
		(*B)->a[i] = 0; // 늘어난 길이만큼 0으로 초기화

	carry = 0;

	for (i = 0; i < A_Len; i++)
		carry = ADD_ABc(C, A, B, carry, i); // 단일 덧셈 A의 워드열 길이만큼 실행

	if (carry == 1) // 마지막 carry가 1이면
		(*C)->a[A_Len] = 1; // 결과값의 MSB에 1 설정
//	else // 마지막 carry가 0이면
//		(*C)->a[A_Len] = 0; // 결과값의 MSB에 0 설정

	BI_Refine(*C);

	// 부호 설정
	if (sign == 0)
		(*C)->sign = 0;
	else
		(*C)->sign = 1;
}

/**
 * @brief Addition Core (WordLen(A) >= WordLen(B), Sign(A) = Sign(B), A = A + B)
 * @details
	ADDC 함수와 동일하게 동작
	덧셈을 수행한 결과를 다시 A에 저장 (A <- A + B)
 * @param bigint** C 덧셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param int sign 덧셈 결과 C의 부호
 */
void ADDC_AAB(bigint** C, bigint** A, bigint** B, int sign)
{
	int A_Len;
	int B_Len;
	int i;
	int carry;

	BI_Get_Word_Length(&A_Len, A);
	BI_Get_Word_Length(&B_Len, B);

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

/**
 * @brief Addition
 * @details

	 [pseudo code]
	 Input  : C, A, B
	 1 : if A = 0 then
	 2 :	return B
	 3 : end if
	 4 : if B = 0 then
	 5 :	return A
	 6 : end if
	 7 : if A > 0 and B < 0 then
	 8 :	return SUB(A, |B|)
	 9 : end if
	10 : if A < 0 and B > 0 then
	11 :	return SUB(B, |A|)
	12 : end if
	13 : if WordLen(A) >= WordLen(B) then
	14 :	return ADDC(A, B)
	15 : else
	16 :	return ADDC(B, A)
	17 : end if

 * @param bigint** C 덧셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 덧셈을 수행할 bigint 형 더블포인터 변수
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
		// A + B에서 A가 0이면 B return
		(*C)->sign = (*B)->sign;
		(*C)->wordlen = (*B)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*B)->a[i];

		return;
	}

	if (BI_Is_Zero(B) == 0) // B is zero
	{
		// A + B에서 B가 0이면 A return
		(*C)->sign = (*A)->sign;
		(*C)->wordlen = (*A)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*A)->a[i];

		return;
	}

	if ((A_sign == NON_NEGATIVE) && (B_sign == NEGATIVE)) // A가 양수, B가 음수이면
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

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE)) // A가 음수, B가 양수이면
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

	// A, B가 동일한 부호일 때
	if (A_Len >= B_Len) // A의 워드열의 길이가 B의 워드열의 길이보다 길 때
	{
		ADDC(C, A, B, A_sign); // 덧셈 실행
		return;
	}
	else // B의 워드열의 길이가 A의 워드열의 길이보다 길 때
	{
		ADDC(C, B, A, A_sign); // 덧셈 실행
		return;
	}

}

/**
 * @brief Addition (A = A + B)
 * @details
	ADD 함수와 동일하게 동작
	덧셈을 수행한 결과를 다시 A에 저장 (A <- A + B)
 * @param bigint** C 덧셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 덧셈을 수행할 bigint 형 더블포인터 변수
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
		SUB(C, *A, temp); // SUB 함수

		BI_Delete(&temp);

		return;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
	{
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		BI_Assign(&temp, *A);

		BI_Flip_Sign(temp);
		SUB(C, *B, temp); // SUB 함수

		BI_Delete(&temp);

		return;
	}

	// A, B가 동일한 부호일 때
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

/**
 * @brief Compare and return longer word length
 * @details
	두 bigint의 워드열 길이를 비교하여 더 긴 워드열의 길이 반환
 * @param bigint* A 워드 길이 비교를 수행할 bigint 형 포인터 변수
 * @param bigint* B 워드 길이 비교를 수행할 bigint 형 포인터 변수
 * @return MAX(WordLen(A), WordLen(B)) (더 긴 워드열 길이)
 */
int Compare_WordLen(bigint* A, bigint* B) // return wordlen 큰 사이즈
{
	int A_Len, B_Len;

	BI_Get_Word_Length(&A_Len, &A); // A의 워드열 길이
	BI_Get_Word_Length(&B_Len, &B); // B의 워드열 길이

	if (A_Len > B_Len) // A의 워드열의 길이가 더 길면
		return A_Len; // A의 워드열 길이 return
	else // B의 워드열의 길이가 더 길면
		return B_Len; // B의 워드열 길이 return
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

* @param bigint** C 뺄셈 연산의(A - B) 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint** A 뺄셈 연산의(A - B) A에 해당하는 bigint 형 포인터 변수
* @param bigint** B 뺄셈 연산의(A - B) B에 해당하는 bigint 형 포인터 변수
*/
void SUB(bigint** C, bigint* A, bigint* B)
{
	unsigned int borrow = 0;
	int len = 0;
	int i = 0;
	bigint* d = NULL;

	BI_Get_Word_Length(&len, &A);   // A->wordlen을 len에 대입
	BI_Get_Word_Length(&borrow, &B);// B->wordlen을 borrow에 대입 

	if (BI_Is_Zero(&A) == 0) // 0-B --> -B
	{
		BI_Assign(C, B); // B의 값 그대로
		(*C)->sign = NEGATIVE; // 부호만 반대로

		return; // memory leackege X
	}

	if (BI_Is_Zero(&B) == 0) // A - 0 --> A
	{
		if (&A == C) // 1st arg == 2nd arg(AAB)인 경우
			return; // 이미 1st arg = 2nd arg 이므로 return으로 종료.
		BI_Assign(C, A); // 1st arg != 2nd arg(AAB)인 경우 C에 A를 assign
		return;
		//BI_Assign(C, A); // A의 값 그대로
		//if (A == C)
		//	BI_New(A, borrow);
		//(*C)->sign = A->sign;
		//(*C)->wordlen = A->wordlen;
		//for (i = 0; i < (*C)->wordlen; i++)
		//	(*C)->a[i] = A->a[i];
		//return;// memory leackege X
	}


	if ((A->sign ^ B->sign) == 0) // A, B 부호가 같을 때
	{
		if ((A->sign & B->sign) == 0) // A, B의 부호가 모두 양수일 때
		{
			if (BI_Compare(&A, &B) < 0) // A, B를 비교해서 A < B일 때. (BI_Compare(A, B)의 return : -1)
			{
				SUBC(&borrow, C, &B, &A); // B - A 를 하고
				BI_Flip_Sign(*C); // 뺄셈 연산이 종료되었으므로 원래대로 부호 바꿔주기

				return;
			}
			else if (BI_Compare(&A, &B) == 0) // A = B일 때, C의 값은 0이 되어야한다.
			{
				// BI_Set_Zero(C); 로 바꾸는게 어떨까?
				for (i = 0; i < (*C)->wordlen; i++) // C의 길이만큼
					(*C)->a[i] = 0;  // 0을 대입
				(*C)->sign = 0; // C의 부호도 NON-NEGATIVE로 변경

				return;
			}
			else // A, B 를 비교해서 A >= B일 때. BI_Compare(A, B)'s return : 0, 1
			{
				SUBC(&borrow, C, &A, &B); // A - B 연산

				return;
			}
		}

		else // A, B의 부호가 모두 음수일 때
		{
			BI_Flip_Sign(A); // A의 부호가 음수이므로 부호 바꿔주기
			BI_Flip_Sign(B); // B의 부호가 음수이므로 부호 바꿔주기
			if (BI_Compare(&A, &B) < 0)
			{
				SUBC(&borrow, C, &B, &A);
				BI_Flip_Sign(A); // 뺄셈 연산이 종료되었으므로 원래대로 부호 원위치
				BI_Flip_Sign(B); // 뺄셈 연산이 종료되었으므로 원래대로 부호 원위치

				return;// memory leACkege X
			}
			else
			{
				SUBC(&borrow, C, &A, &B);
				BI_Flip_Sign(*C);
				BI_Flip_Sign(A); // 부호 원위치
				BI_Flip_Sign(B); // 부호 원위치

				return;// memory leACkege X
			}
		}
	}

	else // A,B 부호가 다를 때
	{
		if (A->sign == 0) // A가 양수, B가 음수
		{
			BI_Flip_Sign(B); // B의 부호를 바꿔주고
			ADD(C, &A, &B);  // ADD 연산
			BI_Flip_Sign(B); // 부호 원위치

			return;
		}
		else
		{
			BI_Flip_Sign(A); //
			ADD(C, &A, &B);

			BI_Flip_Sign(A); // 부호 원위치
			BI_Flip_Sign(*C); // 부호 원위치

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

* @param unsigned int* borrow 단일 뺄셈 연산에서 return 되는 borrow에 해당하는 unsigned int 형 포인터 변수
* @param bigint** C 단일 뺄셈 연산의(A - B) 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint** A 단일 뺄셈 연산의(A - B) A에 해당하는 bigint 형 포인터 변수
* @param bigint** B 단일 뺄셈 연산의(A - B) B에 해당하는 bigint 형 포인터 변수
*/
void SUBC(unsigned int* borrow, bigint** C, bigint** A, bigint** B)
{
	int len, i = 0;
	int result = 0;
	bigint* temp = NULL; // A와 b의 길이가 다를 때 -> bigint** b의 길이를 바꿀 수 없으므로 temp를 만들어줌
	bigint* temp3 = NULL; // C랑 A랑 같을 때를 대비하여
	BI_Get_Word_Length(&len, A); // b보다 큰 A의 길이를 구하자
	BI_New(&temp, len);  // A의 워드 길이와 같게 temp 를 생성
	result = BI_Compare(A, B);
	if (result >= 0)
		(*C)->sign = 0; // 매개변수 C에 이미 부호가 들어가있을 때 바꿔주는 게 없어서
	if (result < 0)
		(*C)->sign = 1;
	if ((*C)->wordlen < len) // Binary Long Division에서 C의 길이가 1이고, A, B의 길이가 2일 때가 있어서. //A = 0x40bd
	{
		bigint* temp2 = NULL;
		BI_New(&temp2, len);
		for (i = 0; i < (*C)->wordlen; i++)
			temp2->a[i] = (*C)->a[i];
		BI_Assign(C, temp2);
		BI_Delete(&temp2);
	}
	for (i = 0; i < (*B)->wordlen; i++)
		temp->a[i] = (*B)->a[i]; // b와 같은 값을 가지고 있어야하고, 더 길게 생성됐을 때는 0이 들어가있어야함.
	// A가 b보다 길 때 b의 길이를 맞춰줘야하는데 b를 건들이면 b가 바뀌기 때문에 temp를 이용

	for (i = 0; i < len; i++)
	{

		BI_New(&temp3, len); // A의 wordlen과 같은 len의 길이로 temp3 생성
		BI_Assign(&temp3, *A); // 이후 A와 동일하게
		if (i == 0)
			*borrow = 0;

		(*C)->a[i] = temp3->a[i] - (*borrow);//(*C)->a[i] = (*A)->a[i] - (*borrow); // A - b의 값을 C 에 대입
		(*C)->a[i] = (*C)->a[i] & word_mask; // mod 2 ^ (WORD_BIT_LEN)
		if (temp3->a[i] < *borrow)//if ((*A)->a[i] < *borrow) // borrow 될 때
			*borrow = 1;
		else // borrow 안될 때
		{
			*borrow = 0;
			if ((*C)->a[i] < temp->a[i])
				*borrow = *borrow + 1;
		}
		(*C)->a[i] -= temp->a[i]; // temp에 넣어놓은 b와 뺄셈 연산
		(*C)->a[i] = (*C)->a[i] & word_mask; // mod 2 ^ (WORD_BIT_LEN)
	}
	BI_Delete(&temp3);
	BI_Delete(&temp);
	BI_Refine(*C);
}

/**
 * @brief Multiplication
 * @details
	flag에 따라 곱셈 연산 수행(Karatsuba multiplication / textbook multiplication)
	C = A * B
 * @param bigint* C 곱셈 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* A 곱셈 연산을 수행할 bigint 형 포인터 변수
 * @param bigint* B 곱셈 연산을 수행할 bigint 형 포인터 변수
 * @return 0 (성공)
 * @throws
	flag 미지정 시 error
	A, B의 부호 가져오기 실패 시 error
 */
int Multiplication(bigint** C, bigint* A, bigint* B)
{
	int A_Len = 0;
	int B_Len = 0;
	int C_Len = 0;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A의 워드열 길이
	BI_Get_Word_Length(&B_Len, &B); // B_Len = B의 워드열 길이

	if (FLAG_MUL == TEXTBOOK) // textbook multiplication
	{
		C_Len = A_Len + B_Len; // C의 길이 정해 준 뒤 bigint 생성
		BI_New(C, C_Len); // bigint 생성
		MUL_Multi(C, A, B);
	}
	else if (FLAG_MUL == KARATSUBA) // karatsuba multiplication
	{
		C_Len = (MAX(A->wordlen, B->wordlen) + 1) >> 1;
		C_Len = 4 * C_Len + 1; // C의 길이 정해 준 뒤
		BI_New(C, C_Len); // bigint 생성

		int sign_A = BI_Get_Sign(A); // sign_A = A의 부호
		int sign_B = BI_Get_Sign(B); // sign_B = B의 부호

		if (sign_A == ERROR || sign_B == ERROR) // 부호 가져오기 실패 시 에러 반한
		{
			printf("get sign error\n");
			return ERROR;
		}

		// karatsuba 인자는 양수값이므로 절댓값 취해 주기
		if (sign_A == NEGATIVE)
			BI_Flip_Sign(A);

		if (sign_B == NEGATIVE)
			BI_Flip_Sign(B);

		MUL_Karatsuba(C, A, B);
		BI_Refine(C);

		if ((sign_A ^ sign_B) == 1) // C의 부호 결정
			(*C)->sign = NEGATIVE;
		else
			(*C)->sign = NON_NEGATIVE;
	}
	else
		return ERROR;

	return 0;
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

* @param word* C 단일 워드 곱셈 연산의 결과를 저장할 word 형 포인터 변수
* @param word* A 단일 워드 곱셈 연산의 곱하는 수인 word 형 포인터 변수
* @param word* B 단일 워드 곱셈 연산의 곱하는 수인 word 형 포인터 변수
*/
void MUL_Word(word* C, word* A, word* B) // 단일 워드 곱셈
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

	A1 = ((*A) >> (WORD_BIT_LEN >> 1)); // A의 최상위비트부터 중간비트까지
	B1 = ((*B) >> (WORD_BIT_LEN >> 1)); // B의 최상위비트부터 중간비트까지
	A0 = (*A) & (((word)1 << (WORD_BIT_LEN >> 1)) - 1); // A의 중간비트부터 최하위 비트까지
	B0 = (*B) & (((word)1 << (WORD_BIT_LEN >> 1)) - 1); // B의 중간비트부터 최하위 비트까지

	mul1 = A1 * B1;
	mul0 = A0 * B0;
	sum1 = A1 * B0;
	sum1 += A0 * B1;
	if (sum1 < A0 * B1)
		carry1 += 1;
	sum2 = (sum1 & (((word)1 << (WORD_BIT_LEN >> 1)) - 1));
	sum2 = sum2 << (WORD_BIT_LEN >> 1); //sum1의 뒷부분
	sum1 = sum1 >> (WORD_BIT_LEN >> 1); // sum1의 앞부분
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

* @param bigint** result 다중 워드 곱셈 연산의 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint* A 다중 워드 곱셈 연산의 곱하는 수인 bigint 형 포인터 변수
* @param bigint* B 다중 워드 곱셈 연산의 곱하는 수인 bigint 형 포인터 변수
*/
void MUL_Multi(bigint** result, bigint* A, bigint* B)
{
	int i, j, len = 0;
	int size_a, size_b = 0;

	BI_Get_Word_Length(&size_a, &A);
	BI_Get_Word_Length(&size_b, &B);

	bigint* D = NULL;
	bigint* result2 = NULL;

	BI_New(&D, (*result)->wordlen); // 단일 워드 곱셈 연산의 결과를 저장할 big integer d

	for (i = 0; i < B->wordlen; i++)
	{
		for (j = 0; j < A->wordlen; j++)
		{
			MUL_Word(&D->a[i + j], &A->a[j], &B->a[i]); // A의 단일워드와 B의 단일워드 연산 후 D의 단일 워드에 대입
			ADDC_AAB(result, result, &D, 0);
			D->a[i + j] = 0;
			D->a[i + j + 1] = 0;
		}
	}
	BI_Delete(&D);
	(*result)->sign = A->sign ^ B->sign;
	BI_Refine(*result);
}

/**
 * @brief Karatsuba Multiplication
 * @details

	 [pseudo code]
	 Input  : C, A, B
	 1 : procedure MUL_Karatsuba(A, B)
	 2 : if flag >= min(WordLen(A), WordLen(B)) then

 * @param bigint** C 카라추바 곱셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 카라추바 곱셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 카라추바 곱셈을 수행할 bigint 형 더블포인터 변수
 */
void MUL_Karatsuba(bigint** C, bigint* A, bigint* B)
{
	int i, j = 0;
	int len, A_Len, B_Len;
	int S0_sign, S1_sign;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A의 워드열 길이
	BI_Get_Word_Length(&B_Len, &B); // B_Len = B의 워드열 길이

	if (FLAG >= MIN(A_Len, B_Len)) // A의 워드열의 길이와 B의 워드열의 길이 중 더 작은 것이 flag보다 작으면
	{
		MUL_Multi(C, A, B); // textbook multiplication 실행
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

	Right_Shift(A1, len * WORD_BIT_LEN); // A1 = A >> len word (A를 len 워드만큼 오른쪽으로 이동)
	BI_Reduction(&A0, len * WORD_BIT_LEN); // A0 = A mod (2^(len * wordlen)) (A를 len 워드만큼 modular 연산 수행)
	Right_Shift(B1, len * WORD_BIT_LEN); // B1 = A >> len word (A를 len 워드만큼 오른쪽으로 이동)
	BI_Reduction(&B0, len * WORD_BIT_LEN); // B0 = A mod (2^(len * wordlen)) (A를 len 워드만큼 modular 연산 수행)

	BI_New(&T0, len * 2);
	BI_New(&T1, len * 2);
	BI_New(&S0, Compare_WordLen(B0, B1)); // S0 = B1 - B0이니까 B1과 B0 중 더 큰 워드 길이만큼 bigint 생성
	BI_New(&S1, Compare_WordLen(A0, A1)); // S1 = A0 - A1이니까 A0과 A1 중 더 큰 워드 길이만큼 bigint 생성
	BI_New(&R, len * 4);
	BI_New(&S, len * 2);

	MUL_Karatsuba(&T1, A1, B1); // T1 = A1 * B1
	MUL_Karatsuba(&T0, A0, B0); // T0 = A0 * B0

	BI_Refine(T1);
	BI_Refine(T0);

	bigint* T1_tmp = NULL;
	BI_Assign(&T1_tmp, T1);
	Left_Shift(T1_tmp, 2 * WORD_BIT_LEN * len); // T1_tmp = T1을 2 * len 워드만큼 오른쪽으로 이동

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

	//S0, S1은 음수일 수 있으므로 절댓값 취해 주기
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

	MUL_Karatsuba(&S, S1, S0); // S = S1 * S0
	BI_Refine(S);

	// S 부호 정해 주기
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
	Left_Shift(ADD_result2, len * WORD_BIT_LEN); // ADD_result2 << len 워드
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

/**
 * @brief Squaring
 * @details
	flag에 따라 제곱 연산 수행(Karatsuba sqauring / textbook sqauring)
	C = A * A
 * @param bigint** C 제곱 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* A 제곱 연산을 수행할 bigint 형 포인터 변수
 * @return 0 (성공)
 * @throws
	flag 미지정 시 error
 */
int Squaring(bigint** C, bigint* A)
{
	int A_Len = 0;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A의 워드열 길이

	if (FLAG_SQU == TEXTBOOK) // textbook sqauring
	{
		SQU(C, A);
		BI_Refine(*C);
	}
	else if (FLAG_SQU == KARATSUBA) // karatsuba sqauring
	{
		int C_Len = (A_Len + 1) >> 1;
		C_Len = 4 * C_Len + 1;

		BI_New(C, C_Len);

		SQUC_Karatsuba(C, A);
		BI_Refine(*C);
	}
	else
		return ERROR;

	return 0;
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

	// A1 = |A| >> WORD_BIT_LEN / 2, A0 = |A| mod (WORD_BIT_LEN / 2) 를 위한 |A| 처리
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

	BI_New(C, wordlen_A * 2 + 1);
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
				C1->a[k] = temp1->a[k]; // j = 0일 때는 C1 = T1 || C1 = T1 (C1이 처음에 0이니까)
		}
		else
		{
			for (k = 0; k < (temp1->wordlen - (2 * j)); k++)
			{
				//if(temp1->wordlen < C1->wordlen)
				C1->a[2 * j + k] = temp1->a[2 * j + k]; // 그 다음부터는 T1이 2 워드 단위로 왼쪽으로 시프트 연산한 결과이므로 2 * j번째부터 넣어 주기

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
			temp = (word*)realloc(C2->a, sizeof(word) * len); // new_wordlen만큼 bigint 구조체 재할당
			if (temp != NULL)
				C2->a = temp;

			for (m = c2_len; m < len; m++)
				C2->a[m] = 0; // 늘어난 길이만큼 0으로 초기화

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
	int flag0; // A가 0인지 판별하는 flag
	int flag1; // A가 1인지 판별하는 flag
	int flag2 = -1; // A가 음수일 때 -1인지 판별하는 flag
	int sign;

	bigint* temp = NULL;

	BI_Assign(&temp, A);

	sign = BI_Get_Sign(A);
	if (sign == NEGATIVE) // A의 부호가 음수인 경우
	{
		BI_Flip_Sign(temp); // 부호 바꾼 뒤
		flag2 = BI_Is_One(&temp); // 1인지 판별 (즉, -1인지 판별)
	}

	flag0 = BI_Is_Zero(&A); // A가 0인지 판별
	flag1 = BI_Is_One(&A); // A가 1인지 판별

	if (flag0 == 0) // A = 0인 경우, A^2 = 0 return
	{
		BI_Assign(C, A);
		BI_Delete(&temp);
		return;
	}
	if (flag1 == 0) // A = 1인 경우, A^2 = 1 return
	{
		BI_Assign(C, A);
		BI_Delete(&temp);
		return;
	}
	if (flag2 == 0) // A = -1인 경우, A^2 = 1 return
	{
		BI_Assign(C, temp);
		BI_Delete(&temp);
		return;
	}

	BI_Delete(&temp);
	BI_New(C, A->wordlen * 2 + 1);
	SQUC(C, A); // A = 0. -1, 1이 아닌 경우 SQUC 실행

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
void SQUC_Karatsuba(bigint** C, bigint* A)
{
	int i;
	int len, A_Len;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A의 워드열 길이

	if (FLAG >= A_Len) // A의 워드열의 길이와 B의 워드열의 길이 중 더 작은 것이 flag보다 작으면
	{
		SQUC(C, A); // textbook squaring 실행
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
	Left_Shift(T1_tmp, 2 * WORD_BIT_LEN * len); // T1_tmp = T1을 2 * len 워드만큼 오른쪽으로 이동

	// R = T1 || T0
	for (i = 0; i < T1->wordlen; i++)
		R->a[len * 2 + i] = T1_tmp->a[T1_tmp->wordlen - T1->wordlen + i];
	for (i = 0; i < T0->wordlen; i++)
		R->a[i] = T0->a[i];

	BI_Refine(R);

	MUL_Karatsuba(&S, A1, A0); // S = A1 * A0
	BI_Refine(S);

	Left_Shift(S, len * WORD_BIT_LEN + 1); // S << len * WORD_BIT_LEN + 1 비트만큼 왼쪽으로 이동
	ADDC(C, &R, &S, R->sign); // C = R + S

	BI_Delete(&A0);
	BI_Delete(&A1);
	BI_Delete(&T0);
	BI_Delete(&T1);
	BI_Delete(&R);
	BI_Delete(&S);
	BI_Delete(&T1_tmp);
}


/**
 * @brief Division
 * @details
	flag에 따라 나눗셈 연산 수행(binary long division / multi-precision long division)
	A = B * Q + R
 * @param bigint** Q 나눗셈 연산을 수행한 결과 중 몫을 저장할 bigint 형 더블포인터 변수
 * @param bigint** R 나눗셈 연산을 수행한 결과 중 나머지 저장할 bigint 형 더블포인터 변수
 * @param bigint* A 나눗셈 연산에서 나누려는 수인 bigint 형 포인터 변수
 * @param bigint* B 나누는 수인 bigint 형 포인터 변수
 * @return 0 (성공)
 * @throws
	flag 미지정 시 error
	B > A일 경우 error
 */
int Division(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int size = 0;
	size = BI_Compare(&A, &B); // A와 B의 크기 비교

	if (size == -1)
	{
		//printf("print(\"B is larger than A.\")\n");
		BI_Set_Zero(Q);
		BI_Assign(R, A);
		return ERROR;
	}

	if (FLAG_DIV == BINARY_LONG) // binary long division
		Binary_Long_Div(Q, R, A, B);
	else if (FLAG_DIV == MULTI_PRE) // multi-precision long division
		DIV(Q, R, A, B);
	else
		return ERROR;

	return 0;
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
* @param Q Binary Long Divsion 연산의 몫에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param R Binary Long Divsion 연산의 나머지에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param A Binary Long Divsion 연산의 나누려는 수인 bigint 형 포인터 변수
* @param B Binary Long Divsion 연산의 나누는 수인 bigint 형 포인터 변수
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

	size = size * WORD_BIT_LEN; // 변수 size 는 A의 총 비트 길이
	for (i = size - 1; i >= 0; i--) // 최상위 비트부터
	{
		len = (int)(i / WORD_BIT_LEN); // len에는 지금 조사하고 있는 비트의 워드 위치
		temp = A->a[len]; // 현재 연산 중인 워드의 값을 temp에 대입
		len = i % WORD_BIT_LEN; // 해당 워드에 몇 번째 비트인지
		temp = temp >> len; // 해당 비트를 left shift해서 첫번째 비트에 놓이도록
		temp = temp & 1; // 1과 & 해서 
		T->a[0] = temp; // 해당 비트의 값을 대입 // j_th_BI_Bit_Length 바꿔서 하기

		Left_Shift(*R, 1); // 2R
		ADD_DIV(R, R, &T); // 2R + a{j}
		result = BI_Compare(&B, R); // R >= B --> 0, -1
		if (result < 1) // R >= B인지 비교
		{
			len = (int)(i / WORD_BIT_LEN) + 1;
			BI_New(&U, len);

			U->a[len - 1] = (word)1 << (i % WORD_BIT_LEN);
			ADD_DIV(Q, Q, &U); // Q + 2 ^ j
			SUB(R, *R, B); // R - B
			BI_Delete(&U); // 덧셈해준 빅넘버 U(2 ^ j)는 delete 해준다.

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
			BI_New(C, B_Len); // Binary Long Divison에서 필요

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
		SUB(C, *A, temp); // SUB 함수
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
		SUB(C, *B, temp); // SUB 함수
		//BI_Refine(C);

		BI_Delete(&temp);

		return;
	}

	// A, B가 동일한 부호일 때
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
* @param Q Multi Long Divsion 연산의 몫에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param R Multi Long Divsion 연산의 나머지에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param A Multi Long Divsion 연산의 나누려는 수인 bigint 형 포인터 변수
* @param B Multi Long Divsion 연산의 나누는 수인 bigint 형 포인터 변수
*/
void DIV(bigint** Q, bigint** R, bigint* A, bigint* B) // Long Division Algorithm(Multi-precision version)에서 DIV(A, B) fucntion.
{
	int i = 0; // for 반복문에서 사용할 변수 i.
	int len = 0; // 변수 len은 빅넘버 A의 size를 담을 변수
	int result = 0; // BI_Compare(&A, &B)의 결과를 담을 변수
	bigint* Temp = NULL; // A의 j번째 워드( A_{j} )를 담을 빅넘버 Temp 초기화
	bigint* Word = NULL; // Q의 i번째 워드( Q_{i} )를 담을 빅넘버 Word 초기화

	BI_Set_Zero(R); // 길이가 1이고 값이 0인 빅넘버 *R 생성
	//BI_New(&Word, 1); // 길이가 1이고 값이 0인 빅넘버 Word 생성. BI_New(&Word, 1)도 가능. //1201

	BI_Get_Word_Length(&len, &A);
	BI_New(Q, len);
	if (B->sign & 1) // [line 1]
		return; // return INVALID.
	result = BI_Compare(&A, &B);
	if (result < 0) // [line 4] B > A 일 경우, Compare(&A, &B)의 return : -1
	{
		BI_Assign(R, A);
		return;
	}

	for (i = len - 1; i >= 0; i--) // line 8.
	{
		BI_Set_Zero(&Word); // 길이가 1이고 값이 0인 빅넘버 Word 생성. BI_New(&Word, 1)도 가능. //1201
		BI_New(&Temp, 1); // A의 j번째 워드(A_{j})를 담는 빅넘버 Temp를 워드 길이가 1로 생성. 
		Temp->a[0] = A->a[i]; // Temp에 A_{j}를 대입.
		Left_Shift(*R, WORD_BIT_LEN); // R을 왼쪽으로 W만큼 shift ==> R * W
		ADD_DIV(R, R, &Temp); // [line 9] R * W에 A_{j}를 덧셈 연산 수행. 
		DIVC(&Word, R, *R, B); //빅넘버 Word는 Q_{i}를 저장하는 변수. 
		BI_Delete(&Temp); // 반복문 내에서 생성한 빅넘버 Temp delete.
		(*Q)->a[i] = Word->a[0]; // [line 10] Q에 Q_{i}를 대입. 
		BI_Delete(&Word); // 빅넘버 Word delete
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
	4 :	Compute k∈Z+ such that 2^k B_{m-1}∈[2^(w-1), 2^w)
	5 :	A', B' <- 2^k * A, 2^k * B
	6 :	Q', R' <- DIVCC(A', B')
	7 : Q, R <- Q', 2^(-k) * R'
	8 : return (Q, R)

*/
void DIVC(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int result = 0; // A, B 대소비교값을 담을 변수 result
	int i = 0; // 반복문에 사용할 변수 i
	int k = 0; // line 4와 관련된 변수 k
	int len = 0; // B의 wordlen을 저장해줄 변수 len
	bigint* AP = NULL; // A를 left shift 해서 보관할 빅넘버 A'(AP, A Prime)
	bigint* BP = NULL; // B를 left shift 해서 보관할 빅넘버 B'(BP, B Prime)

	result = BI_Compare(&A, &B);
	if (result == -1) // A < B 보다 클 때
	{
		return;
		// return (Q, R) = (0, A)을 해줘야하는데
		// DIV에서의 2번째 arg와 3번째 arg를 같은 R로 대입했으므로
		// R은 A가 되었고, Q는 DIV 함수내에서 0이다. 
	}
	BI_Assign(&AP, A); // 빅넘버 A' 에 빅넘버 A assing
	BI_Assign(&BP, B); // 빅넘버 B' 에 빅넘버 B assing
	// line 4. Compute k.
	BI_Get_Word_Length(&len, &B);
	bigint* comp = NULL; // k를 compute하기 위한 빅넘버 comp 선언 및 초기화
	BI_New(&comp, 1); // 빅넘버 comp의 길이 1로 생성
	comp->a[0] = B->a[len - 1]; //comp에 B_{m - 1} 대입
	while (1)
	{
		Left_Shift(comp, 1); // comp를 left shift ==> comp * 2
		k++; // comp * 2^k 하도록.
		if (comp->wordlen == 2) // 2^w을 넘어가면,
		{
			k--; // 넘어갔으니 하나 줄여주고
			break; // break;
		}
	}
	Left_Shift(AP, k); // AP = AP << k 
	Left_Shift(BP, k); // BP = BP << k //line 5. 
	DIVCC(Q, R, AP, BP); // [line 6]
	Right_Shift(*R, k); // [line 7] (Q' == Q)
	BI_Delete(&AP); // 선언해준 빅넘버 AP delete.
	BI_Delete(&BP); // 선언해준 빅넘버 BP delete.
	BI_Delete(&comp); // 선언해준 빅넘버 comp delete.
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
	int n = 0; // 변수 n은 A의 size
	int m = 0; // 변수 m은 B의 size
	word A_msw = 0; // 빅넘버 A의 최상위 비트를 저장할 워드단위 변수
	word B_msw = 0; // 빅넘버 B의 최상위 비트를 저장할 워드단위 변수
	// (msw == Most Significant Word)
	bigint* BQ = NULL; // 빅넘버 B와 빅넘버 Q를 곱한 값을 저장할 빅넘버 BQ를 선언 및 초기화.
	bigint* one = NULL; // 워드길이가 1이고 값이 1로 사용할 빅넘버 one를 선언 및 초기화.

	BI_Set_One(&one); // 값이 1이고 워드길이가 1이 되도록 BI_Set_One() 사용
	BI_Get_Word_Length(&n, &A); // 변수 n에 A의 size 대입
	BI_Get_Word_Length(&m, &B); // 변수 m에 B의 size 대입


	A_msw = A->a[n - 1]; // 빅넘버 A의 최상위 워드를 A_msw에 대입
	B_msw = B->a[m - 1]; // 빅넘버 B의 최상위 워드를 B_msw에 대입

	if (n == m) // DIVCC 에서 if(n == m) 일 때 // [line 1]
	{
		DIVCC_n_m(Q, A, B, m); // [line 2]
	}
	else if (n == m + 1) // [line 4] DIVCC 에서 if(n == m + 1) 일 때 
	{
		if (A_msw == B_msw) // line 5.
			(*Q)->a[0] = word_mask; // Q에 W-1을 대입해야하므로 word_mask 대입.
		else // [line 7]
			DIVCC_n_m1(Q, A, B, m); // [line 8]
	}
	BI_New(&BQ, n + m); // 빅넘버 B와 빅넘버 Q를 곱한 값을 저장할 빅넘버 BQ를 B와 Q의 곱셈연산이 가능한 길이인 n + m 로 생성.
	MUL_Multi(&BQ, *Q, B); // B와 Q를 곱셈 연산해 빅넘버 BQ에 대입 (이 때 Karatsuba로 변경도 가능 --> 속도 비교 가능!!)
	SUB(R, A, BQ);// R = A - B * Q // [line 11]
	while ((*R)->sign & 1) // [line 12] R의 부호가 음수일 때 1 & 1 = 1 
	{
		SUB(Q, *Q, one); // Q = Q - 1.
		ADD_AAB(R, R, &B); // R = R + B. 1st arg, 2nd arg가 같으므로 ADD_AAB 함수 사용
	}
	BI_Delete(&one); // 선언해준 빅넘버 one delete.
	BI_Delete(&BQ); // 선언해준 빅넘버 BQ delete.

}

/*
	Long Division Algorithm Core's Core's condition (if n == m) (Multi-precision version)

	[pseudo code]
	Input  :
	Output :
	1 : Q <- A_{m-1} / B_{m-1}
	2 : Q <- LowerBound(Q)
*/
void DIVCC_n_m(bigint** Q, bigint* A, bigint* B, int m) // DIVCC 에서 if(n == m) 일 때
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
void DIVCC_n_m1(bigint** Q, bigint* A, bigint* B, int m) // DIVCC 에서 if(n == m + 1) 일 때
{
	bigint* Temp1 = NULL; // A_{m} * W + A_{m - 1}을 대입할 빅넘버 Temp1 선언 및 초기화
	bigint* Temp2 = NULL; // B_{m}을 대입할 빅넘버 Temp2 선언 및 초기화
	bigint* Trash = NULL; // 나눗셈 함수에서 나머지 매개변수로 쓰일 빅넘버 Trash 선언 및 초기화

	BI_New(&Temp1, 2); // 빅넘버 Temp1의 길이는 2로 생성
	BI_New(&Temp2, 1); // 빅넘버 Temp2의 길이는 1로 생성
	BI_New(&Trash, 1); // 빅넘버 Trash의 길이는 1로 생성

	Temp1->a[1] = A->a[m]; // A_{m}을 대입 --> A_{m} * W
	Temp1->a[0] = A->a[m - 1]; // A_{m - 1}을 대입
	Temp2->a[0] = B->a[m - 1]; // B_{m - 1}을 대입

	Binary_Long_Div(Q, &Trash, Temp1, Temp2); // DIVCC()의 line 8

	BI_Delete(&Temp1); // 선언해준 빅넘버 Temp1 delete.
	BI_Delete(&Temp2); // 선언해준 빅넘버 Temp2 delete.
	BI_Delete(&Trash); // 선언해준 빅넘버 Trash delete.
}

/**
 * @brief Modular exponentiation of multiplication
 * @details
	flag에 따라 모듈러 지수승 연산 수행(Left to Right / Right to Left / Montgomery ladder)
	C = A ^ N (mod M)
 * @param bigint** C 모듈러 지수승 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* A 모듈러 지수승 연산을 수행할 bigint 형 포인터 변수
 * @param bigint* N 모듈러 지수승 연산에서의 지수를 가리키는 bigint 형 포인터 변수
 * @param bigint* M 모듈러 지수승 연산에서의 모듈러를 수행할 bigint 형 포인터 변수
 * @return 0 (성공)
 * @throws
	flag 미지정 시 error
 */
int Modular_Exponentiation_MUL(bigint** C, bigint* A, bigint* N, bigint* M)
{
	if (FLAG_EXP == LTOR) // Left to Right
		MOD_EXP_LR_MUL(C, A, N, M);
	else if (FLAG_EXP == RTOL) // Right to Left
		MOD_EXP_RL_MUL(C, A, N, M);
	else if (FLAG_EXP == MONTGOMERY) // Montgomery ladder
	{
		MOD_EXP_Montgomery_MUL(C, A, N, M);
		BI_Refine(*C);
	}
	else
		return ERROR;

	return 0;
}

/**
 * @brief Modular exponentiation of addition
 * @details
	flag에 따라 모듈러 곱셈 연산 수행(Left to Right / Right to Left / Montgomery ladder)
	C = A * N (mod M)
 * @param bigint** C 모듈러 곱셈 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* A 모듈러 곱셈 연산을 수행할 bigint 형 포인터 변수
 * @param bigint* N 모듈러 곱셈 연산에서의 지수를 가리키는 bigint 형 포인터 변수
 * @param bigint* M 모듈러 곱셈 연산에서의 모듈러를 수행할 bigint 형 포인터 변수
 * @return 0 (성공)
 * @throws
	flag 미지정 시 error
 */
int Modular_Exponentiation_ADD(bigint** C, bigint* A, bigint* N, bigint* M)
{
	if (FLAG_EXP == LTOR) // Left to Right
		MOD_EXP_LR_ADD(C, A, N, M);
	else if (FLAG_EXP == RTOL) // Right to Left
		MOD_EXP_RL_ADD(C, A, N, M);
	else if (FLAG_EXP == MONTGOMERY) // Montgomery ladder
	{
		MOD_EXP_Montgomery_ADD(C, A, N, M);
		BI_Refine(*C);
	}
	else
		return ERROR;

	return 0;
}

// C = A ^ N
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
void EXP_Montgomery_MUL(bigint** C, bigint* A, bigint* N)
{
	int i, bit;
	int N_bit_len;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0를 저장할 임시 변수
	bigint* temp1 = NULL; // t1을 저장할 임시 변수

	if (N->sign == NEGATIVE) // 예외 처리 (지수는 양수만)
		return;

	BI_Bit_Length(&N_bit_len, N); // N의 비트열 길이 -> N_bit_len

	BI_Set_One(&t0); // t0 = 1
	BI_Assign(&t1, A); // t1 = A

	for (i = N_bit_len - 1; i >= 0; i--)
	{
		bit = BI_j_th_Bit_of_BI(i, N); // N의 i번째 비트가 1인지 0인지 판별

		if (bit == 1) // N의 i번째 비트가 1일 경우
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

			BI_New(&temp0, t0_len + t1_len); // temp0(t0) = t0 * t1을 저장하기 위한 bigint 생성
			//BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2을 저장하기 위한 bigint 생성

			MUL_Multi(&temp0, t0, t1); // temp0 = t0 * t1
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 갱신)
			BI_Delete(&temp0);

			SQU(&temp1, t1); // temp1 = t1^2
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 갱신)
			BI_Delete(&temp1);

		}
		else if (bit == 0) // N의 i번째 비트가 0일 경우
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

			//BI_New(&temp0, t0_len * 2 + 1); // temp0(t0) = t0^2을 저장하기 위한 bigint 생성
			BI_New(&temp1, t0_len + t1_len);  // temp1(t1) = t0 * t1을 저장하기 위한 bigint 생성

			MUL_Multi(&temp1, t0, t1); // temp1 = t0 * t1
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 갱신)
			BI_Delete(&temp1);


			SQU(&temp0, t0); // temp0 = t0^2
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 갱신)
			BI_Delete(&temp0);
		}
		else // N의 i번째 비트가 1도 0도 아닌 경우
			return; // 예외 처리
	}

	BI_Assign(C, t0); // 마지막 t0 값 C에 할당

	BI_Delete(&t0);
	BI_Delete(&t1);
}

// C = N * A
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
void EXP_Montgomery_ADD(bigint** C, bigint* A, bigint* N)
{
	int i, bit;
	int N_bit_len;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0를 저장할 임시 변수
	bigint* temp1 = NULL; // t1을 저장할 임시 변수

	if (N->sign == NEGATIVE) // 예외 처리 (양수만)
		return;

	BI_Bit_Length(&N_bit_len, N); // N의 비트열 길이 -> N_bit_len

	BI_Set_Zero(&t0); // t0 = 0
	BI_Assign(&t1, A); // t1 = A

	for (i = N_bit_len - 1; i >= 0; i--)
	{
		bit = BI_j_th_Bit_of_BI(i, N); // N의 i번째 비트가 1인지 0인지 판별

		if (bit == 1) // N의 i번째 비트가 1일 경우
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 + t1을 저장하기 위한 bigint 생성

			ADD(&temp0, &t0, &t1); // temp0 = t0 + t1
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 갱신)
			BI_Delete(&temp0);

			Left_Shift(t1, 1); // t1 = t1 << 2

		}
		else if (bit == 0) // N의 i번째 비트가 0일 경우
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

			BI_New(&temp1, MAX(t0_len, t1_len) + 1);  // temp1(t1) = t0 + t1을 저장하기 위한 bigint 생성

			ADD(&temp1, &t0, &t1); // temp1 = t0 + t1
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 갱신)
			BI_Delete(&temp1);

			Left_Shift(t0, 1); // t0 = t0 << 2
		}
		else // N의 i번째 비트가 1도 0도 아닌 경우
			return; // 예외 처리
	}

	BI_Assign(C, t0); // 마지막 t0 값 C에 할당

	BI_Delete(&t0);
	BI_Delete(&t1);
}

// C = A^N (mod M)
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
void MOD_EXP_Montgomery_MUL(bigint** C, bigint* A, bigint* N, bigint* M)
{
	int i, bit;
	int N_bit_len;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0를 저장할 임시 변수
	bigint* temp1 = NULL; // t1을 저장할 임시 변수

	bigint* Q = NULL;
	bigint* R = NULL;

	if (N->sign == NEGATIVE) // 예외 처리
		return;
	if (M->sign == NEGATIVE)
		return;
	if (A->sign == NEGATIVE)
		return;

	BI_Bit_Length(&N_bit_len, N); // N의 비트열 길이 -> N_bit_len

	BI_Set_One(&t0); // t0 = 1
	BI_Assign(&t1, A); // t1 = A

	for (i = N_bit_len - 1; i >= 0; i--)
	{
		bit = BI_j_th_Bit_of_BI(i, N); // N의 i번째 비트가 1인지 0인지 판별

		if (bit == 1) // N의 i번째 비트가 1일 경우
		{
			//BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
			//BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

			//BI_New(&temp0, t0_len + t1_len); // temp0(t0) = t0 * t1을 저장하기 위한 bigint 생성
			//BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2을 저장하기 위한 bigint 생성

			Multiplication(&temp0, t0, t1); // temp0 = t0 * t1
			Division(&Q, &R, temp0, M); // R = temp0 (mod M)
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 갱신)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);

			Squaring(&temp1, t1); // temp1 = t1^2
			Division(&Q, &R, temp1, M); // R = temp1 (mod M)
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 갱신)
			BI_Delete(&temp1);
			BI_Delete(&Q);
			BI_Delete(&R);

		}
		else if (bit == 0) // N의 i번째 비트가 0일 경우
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

			//BI_New(&temp0, t0_len * 2 + 1); // temp0(t0) = t0^2을 저장하기 위한 bigint 생성
			//BI_New(&temp1, t0_len + t1_len);  // temp1(t1) = t0 * t1을 저장하기 위한 bigint 생성

			Multiplication(&temp1, t0, t1); // temp1 = t0 * t1
			Division(&Q, &R, temp1, M); // R = temp1 (mod M)
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 갱신)
			BI_Delete(&temp1);
			BI_Delete(&Q);
			BI_Delete(&R);

			Squaring(&temp0, t0); // temp0 = t0^2
			Division(&Q, &R, temp0, M); // R = temp0 (mod M)
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 갱신)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);
		}
		else // N의 i번째 비트가 1도 0도 아닌 경우
			return; // 예외 처리
	}

	BI_Assign(C, t0); // 마지막 t0 값 C에 할당

	BI_Delete(&t0);
	BI_Delete(&t1);
}

// C = N * A (mod M)
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
void MOD_EXP_Montgomery_ADD(bigint** C, bigint* A, bigint* N, bigint* M)
{
	int i, bit;
	int N_bit_len;
	int t0_len = 0;
	int t1_len = 0;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0를 저장할 임시 변수
	bigint* temp1 = NULL; // t1을 저장할 임시 변수

	bigint* Q = NULL;
	bigint* R = NULL;

	if (N->sign == NEGATIVE) // 예외 처리
		return;
	if (M->sign == NEGATIVE)
		return;
	if (A->sign == NEGATIVE)
		return;

	BI_Bit_Length(&N_bit_len, N); // N의 비트열 길이 -> N_bit_len

	BI_Set_Zero(&t0); // t0 = 0
	BI_Assign(&t1, A); // t1 = A

	for (i = N_bit_len - 1; i >= 0; i--)
	{
		bit = BI_j_th_Bit_of_BI(i, N); // N의 i번째 비트가 1인지 0인지 판별

		if (bit == 1) // N의 i번째 비트가 1일 경우
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 + t1을 저장하기 위한 bigint 생성

			ADD(&temp0, &t0, &t1); // temp0 = t0 + t1
			Division(&Q, &R, temp0, M); // R = temp0 (mod M)
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 갱신)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);

			Left_Shift(t1, 1); // t1 = t1 << 2
			Division(&Q, &R, t1, M); // t1 = t1 (mod M)
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 갱신)
			BI_Delete(&Q);
			BI_Delete(&R);
		}
		else if (bit == 0) // N의 i번째 비트가 0일 경우
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

			BI_New(&temp1, MAX(t0_len, t1_len) + 1);  // temp1(t1) = t0 + t1을 저장하기 위한 bigint 생성

			ADD(&temp1, &t0, &t1); // temp1 = t0 + t1
			Division(&Q, &R, temp1, M); // R = temp1 (mod M)
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 갱신)
			BI_Delete(&temp1);
			BI_Delete(&Q);
			BI_Delete(&R);

			Left_Shift(t0, 1); // t0 = t0 << 2
			Division(&Q, &R, t0, M); // t0 = t0 (mod M)
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 갱신)
			BI_Delete(&Q);
			BI_Delete(&R);
		}
		else // N의 i번째 비트가 1도 0도 아닌 경우
			return; // 예외 처리
	}

	BI_Assign(C, t0); // 마지막 t0 값 C에 할당

	BI_Delete(&t0);
	BI_Delete(&t1);
}


/**
* @brief [Exponentiation] Left to Right MUL function
* @details

	[pseudo code]
	Input  : T, X, N, M

	1 : t <- 1
	2 : for i <- l - 1 downto 0 do
	3 :		t <- t^2 ( ^ 2 : SQU )
	4 :		t <- t * X^(N_{i}) ( * : MUL_Multi() )
	5 :	end for
	6 : T <- t

* @param T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
*/
void EXP_LR_MUL(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // 반복문에 사용될 변수 i
	int len = 0; // X의 비트 길이를 담을 변수 len
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
		BI_Get_Word_Length(&t0_len, &t0); // t1의 워드열 길이 -> t1_len
		//BI_New(&temp1, 2 * t0_len + 1); // temp1(t1) = t1^2을 저장하기 위한 bigint 생성
		Squaring(&temp1, t0);
		BI_Delete(&t0);

		BI_New(&temp0, 2 * t0_len + x_len + 1); // temp1(t1) = t1^2을 저장하기 위한 bigint 생성
		result = BI_j_th_Bit_of_BI(i, N);

		if (result == 1)
			Multiplication(&temp0, temp1, X);

		else
			BI_Assign(&temp0, temp1);

		BI_Delete(&temp1);
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
	//BI_Delete(&temp0);
	//BI_Delete(&temp1);
}

/**
* @brief [Exponentiation] Left to Right ADD function
* @details

	[pseudo code]
	Input  : T, X, N, M

	1 : t <- 0
	2 : for i <- l - 1 downto 0 do
	3 :		t <-  2 * t 
	4 :		t <- t + N_{i} * X 
	5 :	end for
	6 : T <- t

* @param T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
*/
void EXP_LR_ADD(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // 반복문에 사용될 변수 i
	int len = 0; // X의 비트 길이를 담을 변수 len
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
		BI_Get_Word_Length(&t0_len, &t0); // t1의 워드열 길이 -> t1_len
		BI_Get_Word_Length(&x_len, &X); // t1의 워드열 길이 -> t1_len
		BI_New(&temp0, t0_len + x_len); // temp1(t1) = t1^2을 저장하기 위한 bigint 생성
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

/**
* @brief [Exponentiation] Right to Left MUL function
* details

	[pseudo code]
	Input : T, X, N, M
	1 : (t0, t1) <- (1, X)
	2 :	for i <- 0 to l - 1 do
	3 :		t0 <- t0 * t1^(N_{i}) ( * : MUL_Multi )
	4 :		t1 <- t1 ^ 2( ^ : SQU )
	5 :	end for
	6 :	T <- t

* @param T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
*/
void EXP_RL_MUL(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // 반복문에 사용될 변수 i
	int len = 0; // X의 비트 길이를 담을 변수 len
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
	for(i = 0; i <= len - 1; i++)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1의 워드열 길이 -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

		BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2을 저장하기 위한 bigint 생성

		result = BI_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			//BI_New(&temp0, t0_len + t1_len); // temp0(t0) = t0 * t1을 저장하기 위한 bigint 생성
			Multiplication(&temp0, t0, t1);
		}
		else
		{
			BI_Assign(&temp0, t0);
			BI_Delete(&t0);
		}
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);

		Squaring(&temp1, t1);
		BI_Delete(&t1);
		BI_Assign(&t1, temp1);
		BI_Delete(&temp1);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
	BI_Delete(&t1);
	BI_Refine(*T);
}

/**
* @brief [Exponentiation] Right to Left ADD function
* @details

	[pseudo code]
	Input : T, X, N, M
	1 : (t0, t1) <- (0, X)
	2 :	for i <- 0 to l - 1 do
	3 :		t0 <- t0 + N_{i} * t1
	4 :		t1 <- 2 * t1 
	5 :	end for
	6 :	T <- t

* @param T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
*/
void EXP_RL_ADD(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // 반복문에 사용될 변수 i
	int len = 0; // X의 비트 길이를 담을 변수 len
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
	for(i = 0; i <= len - 1; i++)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1의 워드열 길이 -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

		result = BI_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 * t1을 저장하기 위한 bigint 생성
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

/**
* @brief [Modular Exponentiation] Left to Right MUL function (used Binary Long Division)
* @details

	[pseudo code]
	Input  : T, X, N, M

	1 : t <- 1
	2 : for i <- l - 1 downto 0 do
	3 :		t <- t^2 mod M ( ^ 2 : SQU , mod : Binary_Long_Div)
	4 :		t <- t * X^(N_{i}) mod M ( * : MUL_Multi(), mod : Binary_Long_Div)
	5 :	end for
	6 : T <- t
	
* @param T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
* @param M Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
*/
void MOD_EXP_LR_MUL(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // 반복문에 사용될 int형 변수 i
	int len = 0; // 지수승 N의 비트 길이를 담을 int형 변수 len
	int result = 0; // 해당 비트 값의 결과를 return 받는 int형 변수 result
	int x_len = 0; // X의 워드열 길이를 대입할 int형 변수 x_len
	//int t0_len = 0; // t0의 워드열 길이를 대입할 int형 변수 t0_len

	bigint* t0 = NULL; // T0
	bigint* temp0 = NULL; //
	bigint* temp1 = NULL; //
	bigint* trash_q = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 몫 매개변수로 들어갈 bigint 포인터형 변수 
	bigint* trash_r = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 나머지 매개변수로 들어갈 bigint 포인터형 변수
	
	BI_Set_One(&t0); // t0를 1로 선언

	BI_Bit_Length(&len, N); // 지수승(N)에 대한 비트 길이 -> len 대입

	for (i = len - 1; i >= 0; i--) // 지수승(N)의 최상위 비트부터 최하위 비트까지 반복문
	{
		//BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
		//BI_New(&temp1, 2 * t0_len + 1); // temp1 = t0^2 을 저장하기 위한 bigint 생성
		Squaring(&temp1, t0); // temp1 = t0^2 연산
		BI_Delete(&t0); // t0를 Delete
		Division(&trash_q, &trash_r, temp1, M); // temp1(=t0^2)에 대하여 M으로 모듈러 연산
		BI_Assign(&temp1, trash_r); // 나머지만 관심 있으므로 나머지를 temp1에 Assign
		BI_Delete(&trash_r); // temp1에 같은 값을 대입했으므로 delete
		BI_Delete(&trash_q); //trash_q로 바꿀까

		BI_Get_Word_Length(&x_len, &X); // X의 워드열 길이 -> x_len
		//BI_New(&temp0, 2 * t0_len + x_len + 1); // temp0 = t1^2 * X 를 저장하기 위한 bigint 생성
		result = BI_j_th_Bit_of_BI(i, N); // 반복문에서 사용 중인 변수 i를 이용해 N의 해당 비트가 1인지 0인지 조사
		if (result == 1) // 해당 비트가 1이면,
		{
			Multiplication(&temp0, temp1, X); //t1^2 * X (곱셈)연산 후 temp0에 대입
			Division(&trash_q, &trash_r, temp0, M); // 곱셈 연산한 temp0에 대하여 M으로 모듈러 연산 진행
			BI_Assign(&t0, trash_r); // 나머지만 관심 있으므로 나머지를 temp0에 assign. --> 반복문에서 t0로 연산을 진행할 수 있도록
			BI_Delete(&trash_r); // temp0에 같은 값을 대입했으므로 delete
			BI_Delete(&trash_q); // Binary Long_Div()에서 사용한 몫도 필요없으므로 delete
		}

		else // 해당 비트가 0이면, 곱셈을 진행하지 않아도 되므로
			BI_Assign(&t0, temp1); // temp0에 t0 assign --> 반복문에서 t0로 연산을 진행할 수 있도록

		BI_Delete(&temp1); // temp1를 delete.
		BI_Delete(&temp0); // temp0를 delete.
	}
	BI_Assign(T, t0); // 반복문이 끝난 결과인 t0를 T에 assign
	BI_Delete(&t0); // t0를 delete.
}

/**
* @brief [Modular Exponentiation] Left to Right ADD function (used Binary Long Division)
* @details

	[pseudo code]
	Input  : T, X, N, M
	
	1 : t <- 0
	2 : for i <- l - 1 downto 0 do
	3 :		t <-  2 * t mod M ( mod : Binary_Long_Div )
	4 :		t <- t + N_{i} * X mod M ( mod : Binary_Long_Div )
	5 :	end for
	6 : T <- t

* @param T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
* @param M Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
*/
void MOD_EXP_LR_ADD(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // 반복문에 사용될 int형 변수 i
	int len = 0; // 지수승 N의 비트 길이를 담을 int형 변수 len
	int result = 0; // 해당 비트 값의 결과를 return 받는 int형 변수 result
	int x_len = 0; // X의 워드열 길이를 대입할 int형 변수 x_len
	int t0_len = 0; // 나머지 워드열 길이를 대입할 int형 변수 t0_len

	bigint* t0 = NULL;
	bigint* temp0 = NULL;
	bigint* trash_q = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 몫 매개변수로 들어갈 bigint 포인터형 변수
	bigint* trash_r = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 나머지 매개변수로 들어갈 bigint 포인터형 변수
	BI_Set_Zero(&t0); // t0를 0으로 선언

	BI_Bit_Length(&len, N); // 지수승(N)에 대한 비트 길이 -> len 대입

	for (i = len - 1; i >= 0; i--)
	{
		Left_Shift(t0, 1); // t0 <- 2 * t0
		Division(&trash_q, &trash_r, t0, M); // 2 * t0에 대해 Binary_Long_Div에 의해 MOD 연산된 값 -> 2 * t0 (mod M) == trash_r
		BI_Get_Word_Length(&t0_len, &trash_r); // 나머지의 워드열 길이 -> t0_len
		BI_Get_Word_Length(&x_len, &X); // X의 워드열 길이 -> x_len
		BI_New(&temp0, MAX(t0_len, x_len)); // trash_r(= 2 * t0 (mod M))과 X의 덧셈결과를 담을 bigint temp0 생성
		result = BI_j_th_Bit_of_BI(i, N); // 반복문에서 사용 중인 변수 i를 이용해 N의 해당 비트가 1인지 0인지 조사

		if (result == 1)// 해당 비트가 1이면(If result is 1),
		{
			ADD(&temp0, &trash_r, &X); // [line 4] trash_r + X 의 값을 temp0에 대입 
			Division(&trash_q, &trash_r, temp0, M); // temp0의 modular 값을 구해서 trash_r에 다시 대입
			BI_Assign(&t0, trash_r);  // 나머지만 관심 있으므로 나머지를 temp0에 assign. --> 반복문에서 다시 돌 때 t0로 연산을 진행할 수 있도록
		}
		else // 해당 비트가 0이면(If result is 0),
		{
			BI_Assign(&t0, trash_r); // 나머지만 관심 있으므로 나머지를 temp0에 assign. --> 반복문에서 다시 돌 때 t0로 연산을 진행할 수 있도록 
		}
		
		BI_Delete(&temp0); // temp0를 delete.
		BI_Delete(&trash_q); // trash_q를 delete.
		BI_Delete(&trash_r); // trash_r를 delete.
	}
	BI_Assign(T, t0); // 반복문이 끝난 최종 결과인 t0를 T에 assign
	BI_Delete(&t0); // t0를 delete.
} 

/**
* @brief [Modular Exponentiation] Right to Left MUL function (used Binary Long Division)
* details

	[pseudo code]
	Input : T, X, N, M
	1 : (t0, t1) <- (1, X)
	2 :	for i <- 0 to l - 1 do
	3 :		t0 <- t0 * t1^(N_{i}) mod M ( * : MUL_Multi, mod : Binary_Long_Div )
	4 :		t1 <- t1 ^ 2 mod M ( ^ : SQU, mod : Binary_Long_Div )
	5 :	end for
	6 :	T <- t

* @param T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
* @param M Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
*/
void MOD_EXP_RL_MUL(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // 반복문에 사용될 int형 변수 i
	int len = 0; // 지수승 N의 비트 길이를 담을 int형 변수 len
	int result = 0; // 해당 비트 값의 결과를 return 받는 int형 변수 result
	int t0_len = 0; // t0의 워드열 길이를 대입할 int형 변수 t0_len
	int t1_len = 0; // t1의 워드열 길이를 대입할 int형 변수 t1_len

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 몫 매개변수로 들어갈 bigint 포인터형 변수
	bigint* trash_r = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 나머지 매개변수로 들어갈 bigint 포인터형 변수

	BI_Set_One(&t0); // t0를 1로 선언
	BI_Assign(&t1, X); // t1에 X를 assign

	BI_Bit_Length(&len, N);  // 지수승(N)에 대한 비트 길이 -> len 대입
	
	if ((len == 1) & (N->a[0] == 1)) // X ^ 1일 때
	{
		BI_Assign(T, X);  // 이 경우에서는 T의 값이 X이므로 assign
		Division(&trash_q, &trash_r, *T, M); // X mod M 연산 후 trash_r에 대입
		BI_Assign(T, trash_r); // X mod M 값인 trash_r을 T에 assign
		BI_Delete(&trash_q); // 할당한 trash_q delete
		BI_Delete(&trash_r); // 할당한 trash_r delete
		BI_Delete(&t0); // 할당한 t0 delete
		BI_Delete(&t1); // 할당한 t1 delete
		return;
	}

	//if (len == 1 & N->a[0] == 0) // X ^ 0일 때 예외처리 추가
	//{
	//	
	//}
	for (i = 0; i <= len - 1; i++) // 지수승(N)의 최하위 비트부터 최상위 비트까지 반복문
	{
		BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
		BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len
		
		BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2을 저장하기 위한 bigint 생성

		result = BI_j_th_Bit_of_BI(i, N);  // 반복문에서 사용 중인 변수 i를 이용해 N의 해당 비트가 1인지 0인지 조사

		if (result == 1)// 해당 비트가 1이면,
		{
			//BI_New(&temp0, t0_len + t1_len); // temp0(t0) = t0 * t1을 저장하기 위한 bigint 생성
			Multiplication(&temp0, t0, t1); // temp0에 t0 * t1 을 연산
		}
		else // 해당 비트가 0이면,
		{
			BI_Assign(&temp0, t0); // t0에 1을 곱하는 것이므로, temp0에 t0을 assign --> 반복문에서 t0로 연산을 진행할 수 있도록
		}
		Division(&trash_q, &trash_r, temp0, M); // 연산된 temp0(t0)에 Binary_Long_Div()를 이용해 modular M 연산 후 결과값을 trash_r에 대입
		BI_Assign(&t0, trash_r);
		BI_Delete(&temp0); // 할당한 temp0 delete
		BI_Delete(&trash_q); // 연산을 통해 할당한 trash_q delete
		BI_Delete(&trash_r); // 연산을 통해 할당한 trash_r delete

		Squaring(&temp1, t1);  // temp1(t1) <- t1 * t1
		Division(&trash_q, &trash_r, temp1, M); // temp1( = t1 * t1)에 Binary_Long_Div()를 이용해 modular M 연산 후 결과값을 trash_r에 대입
		BI_Assign(&t1, trash_r); // t1 * t1 (mod M) 값인 trash_r을 다시 t1에 assign --> 반복문에서 t1로 연산을 진행할 수 있도록
		BI_Delete(&trash_q); // 연산을 통해 할당한 trash_q delete
		BI_Delete(&trash_r); // 연산을 통해 할당한 trash_r delete
		BI_Delete(&temp1); // 할당한 temp0 delete
	}
	BI_Assign(T, t0); // 반복문이 끝난 결과인 t0를 T에 assign
	BI_Delete(&t0); // 연산을 통해 할당한 t0 delete
	BI_Delete(&t1); // 연산을 통해 할당한 t1 delete
	BI_Refine(*T); // 결과값인 T를 refine 해주기
}

/**
* @brief [Modular Exponentiation] Right to Left ADD function (used Binary Long Division)
* @details

	[pseudo code]
	Input : T, X, N, M
	1 : (t0, t1) <- (0, X)
	2 :	for i <- 0 to l - 1 do
	3 :		t0 <- t0 + N_{i} * t1 mod M ( mod : Binary_Long_Div )
	4 :		t1 <- 2 * t1 mod M ( mod : Binary_Long_Div )
	5 :	end for
	6 :	T <- t

* @param T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
* @param M Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
*/
void MOD_EXP_RL_ADD(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // 반복문에 사용될 int형 변수 i
	int len = 0; // 지수승 N의 비트 길이를 담을 int형 변수 len
	int result = 0; // 해당 비트 값의 결과를 return 받는 int형 변수 result
	int t0_len = 0; // t0의 워드열 길이를 대입할 int형 변수 t0_len
	int t1_len = 0; // t1의 워드열 길이를 대입할 int형 변수 t1_len

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 몫 매개변수로 들어갈 bigint 포인터형 변수
	bigint* trash_r = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 나머지 매개변수로 들어갈 bigint 포인터형 변수

	BI_Set_Zero(&t0); // t0를 0으로 선언
	BI_Assign(&t1, X); // t1에 X를 assign
	
	BI_Bit_Length(&len, N); // 지수승(N)에 대한 비트 길이 -> len 대입
	
	if ((len == 1) & (N->a[0] == 1)) // 1 * X일 때
	{
		BI_Assign(T, X);  // 이 경우에서는 T의 값이 X이므로 assign
		Division(&trash_q, &trash_r, *T, M); // X mod M 연산 후 trash_r에 대입
		BI_Assign(T, trash_r); // X mod M 값인 trash_r을 T에 assign
		BI_Delete(&trash_q); // 할당한 trash_q delete
		BI_Delete(&trash_r); // 할당한 trash_r delete
		BI_Delete(&t0); // 할당한 t0 delete
		BI_Delete(&t1); // 할당한 t1 delete
		return;
	}
	//if (len == 1 & N->a[0] == 0) // 0 * X일 때 예외처리 추가
	//{
	
	//	return;
	//}
	for(i = 0; i <= len - 1; i++) // 지수승(N)의 최하위 비트부터 최상위 비트까지 반복문
	{ 
		BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
		BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

		result = BI_j_th_Bit_of_BI(i, N);  // 반복문에서 사용 중인 변수 i를 이용해 N의 해당 비트가 1인지 0인지 조사

		if (result == 1) // 해당 비트가 1이면,
		{
			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 + t1을 저장하기 위한 bigint 생성
			ADD(&temp0, &t0, &t1); // temp0에 t0 + t1 연산
		}
		else // 해당 비트가 0이면,
		{
			BI_Assign(&temp0, t0); // t0에 0을 더하는 것이므로, temp0에 t0을 assign.
		}
		Division(&trash_q, &trash_r, temp0, M); // 연산된 temp0(t0)에 modular M 연산 후 결과값을 trash_r에 대입
		BI_Delete(&temp0); // 할당한 temp0 delete
		BI_Assign(&t0, trash_r); // t0 + result * t1 (mod M) 값인 trash_r을 t0에 assign --> 반복문에서 t0로 연산을 진행할 수 있도록
		BI_Delete(&trash_q); // 연산을 통해 할당한 trash_q delete
		BI_Delete(&trash_r); // 연산을 통해 할당한 trash_r delete
		
		Left_Shift(t1, 1); // t1 <- 2 * t1
		Division(&trash_q, &trash_r, t1, M); // 2 * t1에 modular M 연산 후 결과값을 trash_r에 대입
		BI_Assign(&t1, trash_r); // 2 * t1 (mod M) 값인 trash_r을 다시 t1에 assign --> 반복문에서 t1로 연산을 진행할 수 있도록
		BI_Delete(&trash_q); // 연산을 통해 할당한 trash_q delete
		BI_Delete(&trash_r); // 연산을 통해 할당한 trash_r delete
	}
	BI_Assign(T, t0); // 반복문이 끝난 결과인 t0를 T에 assign
	BI_Delete(&t0); // 연산을 통해 할당한 t0 delete
	BI_Delete(&t1); // 연산을 통해 할당한 t1 delete
	BI_Refine(*T); // 결과값인 T를 refine 해주기
}
