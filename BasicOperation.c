#include "config.h"
#include "BasicOperation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Initialize Array
 * @details 배열을 입력 길이만큼 0으로 초기화
 * @param wordt* a 초기화시킬 배열의 word 형 포인터 변수
 * @param int len 초기화시킬 배열의 길이
 * @return SUCCESS
 */
int array_init(word* a, int len)
{
	memset(a, 0, sizeof(word) * len); // 배열의 길이만큼 초기화
	return SUCCESS;
}

/**
 * @brief Create Bigint
 * @details bigint 구조체 및 구조체 멤버 생성
 * @param bigint** x 생성할 bigint 형 더블포인터 변수
 * @param int wordlen 생성할 구조체 내부 배열의 워드열 길이
 * @return SUCCESS
 * @throws
	ERROR bigint 구조체 할당 실패 시
	ERROR bigint 구조체 내부 배열 할당 실패 시
 */
int BI_New(bigint** x, int wordlen)
{
	if (*x != NULL) //   x의 주소가 NULL이 아니면
		BI_Delete(x); // BI_Delete()로 초기화

	*x = (bigint*)malloc(sizeof(bigint)); // bigint *x 할당
	if (*x == NULL)
		return ERROR;

	(*x)->sign = NON_NEGATIVE; // 양수이고
	(*x)->wordlen = wordlen;   // 2번째 매개변수와 같은 길이인
	(*x)->a = (word*)calloc(wordlen, sizeof(word)); // bigint x 내부의 배열 a 할당
	
	if (x == NULL)
		return ERROR;

	return SUCCESS;
}

/**
 * @brief Delete Bigint
 * @details bigint 구조체 및 구조체 내부 배열 삭제
 * @param bigint** x 삭제할 bigint 형 더블포인터 변수
 * @return SUCCESS
 */
int BI_Delete(bigint** x)
{
	if (*x == NULL) // bigint 구조체의 주소가 NULL 값이면 삭제할 필요 없기 때문에 return
		return SUCCESS;

#ifdef zerorize
	array_init((*x)->a, (*x)->wordlen); // 배열 초기화
#endif 
	free((*x)->a); // bigint 구조체 안의 word 배열 free
	free(*x); // bigint 구조체 free
	*x = NULL; // bigint 구조체의 주소값 NULL로 초기화

	return SUCCESS;
}

/**
 * @brief Set Bigint by Array
 * @details 입력받은 배열로 big integer 설정
 * @param bigint** x 설정할 bigint 형 더블포인터 변수
 * @param int sign 설정할 bigint의 부호
 * @param word* a 배열의 word 형 포인터 변수
 * @param int wordlen 생성할 구조체 내부 배열의 워드열 길이
 * @return SUCCESS
 * @throws ERROR 설정할 bigint 구조체 미할당 시
 */
int BI_Set_By_Array(bigint** x, int sign, word* a, int wordlen)
{
	int i;
	if (*x == NULL)
		return ERROR;

	(*x)->sign = sign; // bigint 구조체의 부호 설정
	(*x)->wordlen = wordlen; // bigint 구조체의 워드열 길이 설정

	for (i = 0; i < wordlen; i++)
		(*x)->a[i] = a[i]; // 워드 단위 배열 a에 있는 값 bigint 구조체 내부의 워드 단위 배열에 대입

	return SUCCESS;
}

/**
 * @brief Set Bigint by String
 * @details 입력받은 문자열로 big integer 설정
 * @param bigint** x 설정할 bigint 형 더블포인터 변수
 * @param int sign 설정할 bigint의 부호
 * @param char* str 문자열의 char 형 포인터 변수
 * @param word base 설정할 진수의 base
 * @return SUCCESS
 * @throws
	ERROR 설정할 bigint 구조체 미할당 시
	ERROR 문자열을 변환한 hex 값 배열 할당 실패 시
	ERROR hex 값으로 변환 실패 시
	ERROR base가 2, 16이 아닌 경우
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

	hex = (char*)calloc(sizeof(word), len); // 입력받은 string 값을 hex 값으로 변환시킬 때 저장할 곳
	if (hex == NULL)
		return ERROR;

	re = Ascii_To_Hex(str, hex); // 입력받은 string 값 hex로 변환
	if (re == ERROR)
		return ERROR;

	if (base == 2) // 2진수로 받은 것 (11 01101010 10101111 -> 3 byte 방에 a[2] a[1] a[0])
	{
		if ((len % WORD_BIT_LEN) != 0) // 입력받은 string 길이가 WORD_BIT_LEN의 배수가 아니면
		{
			for (i = 0; i < len / WORD_BIT_LEN; i++) // 입력받은 string 길이를 WORD_BIT_LEN으로 나눈 몫만큼
			{
				for (j = 0; j < WORD_BIT_LEN; j++)
					(*x)->a[i] |= (hex[len - 1 - (WORD_BIT_LEN * i) - j] << j);
				// string을 hex로 변환한 배열 끝부터 (little endian) 하나씩 순서대로 bigint 구조체 배열에 넣어 주기
			}

			for (k = 0; k < len % WORD_BIT_LEN; k++) // 입력받은 string 길이를 WORD_BIT_LEN으로 나눈 나머지만큼
				(*x)->a[len / WORD_BIT_LEN] |= (hex[k] << ((len % WORD_BIT_LEN) - k - 1));
			// string을 hex로 변환한 처음 배열에 있는 것 bigint 구조체 마지막 배열에 넣어 주기 (예시에서는 11) 
		}

		else // 입력받은 string 길이가 WORD_BIT_LEN의 배수이면
		{
			for (i = 0; i < len / WORD_BIT_LEN; i++)
			{
				for (j = 0; j < WORD_BIT_LEN; j++)
					(*x)->a[i] |= (hex[len - 1 - WORD_BIT_LEN * i - j] << j);
				// string을 hex로 변환한 배열 끝부터 (little endian) 하나씩 순서대로 bigint 구조체 배열에 넣어 주기
			}
		}
	}

	else if (base == 16) // "123456789" -> {0x89, 0x67, 0x45, 0x23, 0x01} / {0x6789, 0x2345, 0x0001}
	{
		// '0x89' 이런 식으로 숫자 2개가 1 byte이므로 WORD_BIT_LEN이 8 -> 4, 32 -> 8, 64 -> 16개의 숫자니까 WORD_BIT_LEN / 4
		if (len % (WORD_BIT_LEN / 4) != 0)
		{
			for (i = 0; i < len / (WORD_BIT_LEN / 4); i++)
			{
				for (j = 0; j < WORD_BIT_LEN / 4; j++)
					(*x)->a[i] |= hex[len - 1 - (WORD_BIT_LEN / 4) * i - j] << (4 * j);
				// string을 hex로 변환한 배열 끝부터 (little endian) 하나씩 순서대로 bigint 구조체 배열에 넣어 주기
			}

			for (k = 0; k < len % (WORD_BIT_LEN / 4); k++)
				(*x)->a[len / (WORD_BIT_LEN / 4)] |= (hex[(len % (WORD_BIT_LEN / 4)) - 1 + k] << (4 * k));
			// string을 hex로 변환한 처음 배열에 있는 것 bigint 구조체 마지막 배열에 넣어 주기 (예시에서는 1)
		}

		else
		{
			for (i = 0; i < (len / (WORD_BIT_LEN / 4)); i++)
			{
				for (j = 0; j < WORD_BIT_LEN / 4; j++)
					(*x)->a[i] |= hex[len - 1 - (WORD_BIT_LEN / 4) * i - j] << (4 * j);
				// string을 hex로 변환한 배열 끝부터 (little endian) 하나씩 순서대로 bigint 구조체 배열에 넣어 주기
			}
		}
	}

	return SUCCESS;
}

/**
 * @brief Transform String to Hex
 * @details 입력받은 문자열을 hex 값으로 변환
 * @param char* str 문자열을 가리키는 char 형 포인터 변수
 * @param char* hex 변환된 hex 값을 저장할 char 형 포인터 변수
 * @return SUCCESS
 * @throws ERROR 0~9, A~F가 아닌 값이 들어왔을 경우
 */
int Ascii_To_Hex(char* str, char* hex)
{
	int len = 0;
	int i;

	len = strlen(str);

	for (i = 0; i < len; i++)
	{
		if (str[i] > 0x29 && str[i] < 0x40) // 0x30~0x39 -> 0~9
			hex[i] = str[i] - 0x30; // 0 ~ 9는 -0x30
		else if (str[i] > 0x40 && str[i] < 0x47) // 0x41~0x46 -> A~F
			hex[i] = str[i] - 0x31; // a ~ f는 -0x31
		else
			return ERROR;
	}

	return SUCCESS;
}

/**
 * @brief Show Big Integer
 * @details 입력받은 문자열을 hex 값으로 변환
 * @param bigint* x 출력할 값을 가리키는 bigint 형 포인터 변수
 * @param int base 출력하고 싶은 형태(진수)
 * @return SUCCESS
 * @throws
	ERROR 출력할 bigint 구조체 미할당 시
	ERROR base가 2, 16이 아닌 경우
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
	return SUCCESS;
}

/**
 * @brief Refine Big Integer (Remove Last Zero Words)
 * @details 구조체 재할당 (구조체 내부 배열에서 MSB에서 0으로 채워진 워드열 제거)
 * @param bigint* x 재할당할 bigint 형 포인터 변수
 * @return SUCCESS
 * @throws
	ERROR 재설정할 bigint 구조체 미할당 시
	ERROR 구조체 재할당 실패 시
 */
int BI_Refine(bigint* x)
{
	word* temp = NULL;

	if (x == NULL)
		return ERROR;

	int new_wordlen = x->wordlen;

	while (new_wordlen > 1) // (x의 워드열 길이-  1)번만큼 반복
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
			return ERROR;
	}

	if (BI_Is_Zero(&x) == 1) // 0인 경우
		x->sign = NON_NEGATIVE; // 부호 결정

	return SUCCESS;
}

/**
 * @brief Assign Big Integer
 * @details bigint 구조체 복사
 * @param bigint** x 복사한 값을 저장할 bigint 형 더블포인터 변수
 * @param bigint* y 복사할 값을 가리키는 bigint 형 포인터 변수
 * @return SUCCESS
 */
int BI_Assign(bigint** y, bigint* x)
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

	return SUCCESS;
}

/**
 * @brief Generate Random Number Big Integer
 * @details 랜덤한 big integer 생성
 * @param bigint** x 생성한 랜덤값을 저장할 bigint 형 더블포인터 변수
 * @param int sign 생성할 랜덤값의 부호
 * @param int wordlen 생성할 랜덤값의 길이
 * @return SUCCESS
 * @throws
	ERROR 랜덤값 생성 실패 시
	ERROR Refine 실패 시
 */
int BI_Gen_Rand(bigint** x, int sign, int wordlen)
{
	int re;

	BI_New(x, wordlen); // bigint 구조체 생성
	(*x)->sign = sign; // 부호 결정
	
	re = Array_Rand((*x)->a, wordlen); // random으로 배열 설정
	if (re == ERROR)
		return ERROR;

	re = BI_Refine(*x); // 앞쪽 0으로 채워진 부분 자르기
	if (re == ERROR)
		return ERROR;

	return SUCCESS;
}

/**
 * @brief Generate Random number in Array
 * @details 랜덤값 생셩하여 배열에 저장
 * @param word* dst 생성한 랜덤값을 저장할 포인터 변수
 * @param int wordlen 생성할 랜덤값의 길이
 * @return SUCCESS
 * @throws
	ERROR dst 미할당 시
 */
int Array_Rand(word* dst, int wordlen)
{
	if (dst == NULL)
		return ERROR;

	unsigned char* p = (unsigned char*)dst; // rand() 함수의 출력값이 15bit이므로 워드 단위가 아닌 한 바이트 단위로 배열에 랜덤값 설정
	int cnt = wordlen * sizeof(word);
	
	while (cnt > 0)
	{
		*p = rand() & 0xff; // cnt만큼 random한 값 생성해서 배열에 저장
		p++;
		cnt--;
	}

	return SUCCESS;
}

/**
 * @brief Get Word Length
 * @details 구조체 bigint의 워드열의 길이
 * @param int* len 워드열의 길이를 저장할 int 형 포인터 변수
 * @param bigint** x 워드열의 길이를 구할 bigint 형 더블포인터 변수
 * @return SUCCESS
 * @throws ERROR 구조체 미할당 시
 */
int BI_Get_Word_Length(int* len, bigint** x)
{
	if (*x == NULL)
		return ERROR;

	*len = (*x)->wordlen; // Big Integer x의 wordlen를 대입
	return SUCCESS;
}

/**
 * @brief Get Bit Length
 * @details 구조체 bigint의 비트의 길이
 * @param int* len 비트열의 길이를 저장할 int 형 포인터 변수
 * @param bigint** x 비트열의 길이를 구할 bigint 형 더블포인터 변수
 * @return SUCCESS
 * @throws ERROR 구조체 미할당 시
 */
int BI_Get_Bit_Length(int* len, bigint* x)
{
	int i = 0;
	int j = 0;
	int k = 0;

	if (x == NULL)
		return ERROR;

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

	return SUCCESS;
}

/**
 * @brief Get j-th bit of Big Integer
 * @details j번째 비트 판별 (0 또는 1)
 * @param int* len 비트열의 길이를 저장할 int 형 포인터 변수
 * @param bigint** x 비트열의 길이를 구할 bigint 형 더블포인터 변수
 * @return j-th bit of Big Integer (0 또는 1)
 * @throws
	ERROR 구조체 미할당 시
	ERROR j가 x의 big length보다 클 때
 */
int BI_Get_j_th_Bit_of_BI(int j, bigint* x)
{
	unsigned long long i = 0;
	int k = 0;
	int r = 0;

	if (x == NULL)
		return ERROR;

	if (j >= (x->wordlen) * WORD_BIT_LEN) // x의 bit len보다 크면 에러
		return ERROR;

	k = j / WORD_BIT_LEN; // 변수 k는 j번째 비트가 위치한 워드의 위치(몇 번째 워드)
	r = j % WORD_BIT_LEN; // x->a[k]에서 r만큼 왼쪽으로 이동해야 전체에서 j번째 위치
	i = 1;
	i = i << r;

	if (i == (x->a[k] & i)) // shift한 i와 j번째 비트가 위치한 워드와 &연산을 통해 j번째 비트의 값을 알아내기
		return 1;
	else
		return 0;
}

/**
 * @brief Get Sign of Big Integer
 * @details big integer의 부호 판별
 * @param bigint* x 부호를 판별할 bigint 형 포인터 변수
 * @return sign (NEGATIVE / NON_NEGATIVE)
 * @throws
	ERROR 구조체 미할당 시
	ERROR x의 부호가 NON_NEGATIVE도 NEGATIVE도 아닐 경우
 */
int BI_Get_Sign(bigint* x)
{
	if (x == NULL)
		return ERROR;
	
	if ((x->sign) == NON_NEGATIVE) // bigint 구조체의 부호가 NON_NEGATIVE면 NON_NEGATIVE return
		return NON_NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint 구조체의 부호가 NEGATIVE면 NEGATIVE return
		return NEGATIVE;
	else // 둘 다 아닐 때 ERROR return
		return ERROR;
}

/**
 * @brief Flip sign of Big Integer
 * @details big integer의 부호를 바꿔 주는 함수
 * @param bigint* x 부호를 바꿀 bigint 형 포인터 변수
 * @return SUCCESS
 * @throws
	ERROR 구조체 미할당 시
	ERROR x의 부호가 NON_NEGATIVE도 NEGATIVE도 아닐 경우
 */
int BI_Flip_Sign(bigint* x)
{
	if (x == NULL)
		return ERROR;

	if ((x->sign) == NON_NEGATIVE) // bigint 구조체의 부호가 NON_NEGATIVE면 부호를 NEGATIVE로 변경
		x->sign = NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint 구조체의 부호가 NEGATIVE면 부호를 NON_NEGATIVE로 변정
		x->sign = NON_NEGATIVE;
	else
		return ERROR;

	return SUCCESS;
}

/**
 * @brief Set Big Integer to 1
 * @details big integer를 1로 설정
 * @param bigint** x 1로 설정할 bigint 형 더블포인터 변수
 * @return SUCCESS
 */
int BI_Set_One(bigint** x)
{
	BI_New(x, 1);
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x1; // 첫번째 배열에만 1 대입 (1을 만들어야 하므로)

	return SUCCESS;
}

/**
 * @brief Set Big Integer to 0
 * @details big integer를 0으로 설정
 * @param bigint** x 0으로 설정할 bigint 형 더블포인터 변수
 * @return SUCCESS
 */
int BI_Set_Zero(bigint** x)
{
	BI_New(x, 1); // 길이가 1인 bigint x 생성
	(*x)->sign = NON_NEGATIVE;
	(*x)->a[0] = 0x00; // 첫번째 배열에만 대입 (0을 만들어야 하므로)

	return SUCCESS;
}

/**
 * @brief Determine Big Integer is 1
 * @details big integer가 1인지 판별
 * @param bigint** x 판별할 bigint 형 더블포인터 변수
 * @return TRUE or FALSE
 * @throws
	ERROR 구조체 미할당 시
 */
int BI_Is_One(bigint** x)
{
	int i = 0;

	if (*x == NULL)
		return ERROR;

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 1) // 부호가 음수이거나, LSB가 1이 아니면 FALSE
		return FALSE;

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen만큼 하나씩 이동해 가면서 LSB가 1이고 배열의 나머지 값이 0인지 확인
	{
		if ((*x)->a[i] != 0)
			return FALSE;
	}
	
	return TRUE;
}

/**
 * @brief Determine Big Integer is 0
 * @details big integer가 0인지 판별
 * @param bigint** x 판별할 bigint 형 더블포인터 변수
 * @return TRUE or FALSE
 * @throws
	ERROR 구조체 미할당 시
 */
int BI_Is_Zero(bigint** x)
{
	int i = 0;

	if (*x == NULL)
		return ERROR;

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 0) // 부호가 음수이고 LSB가 0이 아니면 FALSE
		return FALSE;

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen만큼 하나씩 이동해 가면서 배열의 값이 전부 0인지 확인
	{
		if ((*x)->a[i] != 0)
			return FALSE;
	}
	
	return TRUE;
}

/**
 * @brief Compare BigInt x with BigInt y
 * @details
	big integer x와 y의 크기 비교
	x > y : 1
	x = y : 0
	x < y : 2
 * @param bigint** x 비교할 bigint 형 더블포인터 변수
 * @param bigint** y 비교할 bigint 형 더블포인터 변수
 * @return 0 or 1 or 2
 * @throws
	ERROR 구조체 x(y) 미할당 시
 */
int BI_Compare(bigint** x, bigint** y) // return : 1(x > y), 0(x == y), 2(x < y) 
{
	int i = 0;
	int len_x, len_y = 0;

	if ((*x == NULL) | (*y == NULL))
		return ERROR;

	if ((*x)->sign < (*y)->sign) // A가 양수, B가 음수면 당연히 A가 크므로
		return 1;

	else if ((*x)->sign > (*y)->sign) // A가 음수, B가 양수면 당연히 B가 크므로
		return 2;

	else // A, B 부호가 같을 때
	{
		BI_Get_Word_Length(&len_x, x); //len_x = x->wordlen
		BI_Get_Word_Length(&len_y, y); //len_y = y->wordlen

		if ((*x)->sign == 0) // A, B 부호가 양수일 때 (부호가 서로 같으므로, if 문을 통해 하나만 비교)
		{
			if (len_x > len_y) // x의 길이가 y보다 길면
				return 1;	   // x가 길다는 1을 출력
			else if (len_x < len_y) // y의 길이가 x보다 길면
				return 2;			// y가 길다는 2 출력
			else // x의 길이 = y의 길이
			{
				for (i = len_x - 1; i >= 0; i--) //(int)len_x --> len_x로 바꿈.20.11.23.sy // 값 비교
				{
					if ((*x)->a[i] > (*y)->a[i])
						return 1;
					else if ((*x)->a[i] < (*y)->a[i])
						return 2;
				}

				return 0;
			}
		}

		else // A, B 부호가 음수일 때
		{
			// 양수일 때와는 반대가 되도록 return
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
 * @param bigint* x shift 시킬 bigint 형 포인터 변수
 * @param int len shift 시킬 비트 수
 * @return SUCCESS
 * @throws
	ERROR 구조체 미할당 시
	ERROR realloc 에러 시
 */
int BI_Left_Shift(bigint* x, int len) // len: 이동할 비트 수
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
	BI_Assign(&cp, x); // bigint x를 bigint cp에 복사

	length = x->wordlen;
	wn = WORD_BIT_LEN * length;

	if ((len % WORD_BIT_LEN) == 0) // 이동할 비트 수가 WORD_BIT_LEN의 배수이면
		add_len = len / WORD_BIT_LEN; // 이동할 비트 수를 WORD_BIT_LEN로 나눈 몫만큼 워드 길이 추가
	else
		add_len = (len / WORD_BIT_LEN) + 1; // 이동할 비트 수를 WORD_BIT_LEN로 나눈 몫 + 1만큼 워드 길이 추가

	new_wordlen = add_len + x->wordlen; // 추가될 워드 길이 + 원래 워드 길이

#if OS == LINUX
	temp = (word*)realloc(x->a, sizeof(word) * new_wordlen); // new_wordlen만큼 bigint 구조체 재할당
#elif OS == WINDOW
	temp = (word*)_recalloc(x->a, new_wordlen, sizeof(word));
	// _recalloc은 window에서만 돌아가는 함수 (visual studio에서 realloc 쓰면 warning이 뜸)
#endif

	if (temp != NULL)
		x->a = temp;
	else
		return ERROR;

	x->wordlen = new_wordlen; // 재할당한 구조체 길이 설정
	memset(&x->a[length], 0x00, add_len); // 추가된 워드 길이만큼 0으로 초기화

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

		x->a[x->wordlen - 1] = (cp->a[cp->wordlen - 1] >> (WORD_BIT_LEN - r)); // 마지막 배열 = An-1 >> (WORD_BIT_LEN - r)
	}

	BI_Delete(&cp);
	BI_Refine(x);

	return SUCCESS;
}

/**
 * @brief Right shift
 * @details big integer >> len
 * @param bigint* x shift 시킬 bigint 형 포인터 변수
 * @param int len shift 시킬 비트 수
 * @return SUCCESS
 * @throws
	ERROR 구조체 미할당 시
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
 * @param bigint* x modular 시킬 bigint 형 더블포인터 변수
 * @param int r 모듈러를 취하는 수
 * @return SUCCESS
 * @throws ERROR 구조체 미할당 시
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
		count = r / WORD_BIT_LEN; // 남을 워드 길이 알아내기

		for (i = count; i < size; i++) // 해당 워드부터 최상위 워드까지 0으로 변경
			(*x)->a[i] = 0;
	}
	else
	{
		count = r / WORD_BIT_LEN;
		
		r = r % WORD_BIT_LEN;
		(*x)->a[i] = (*x)->a[i] & (((word)1 << r) - 1);
		
		for (i = count + 1; i < size; i++) // 해당 워드의 다음부터 최상위 워드까지 0으로 변경
			(*x)->a[i] = 0;
	}

	BI_Refine(*x);

	return SUCCESS;
}

/**
 * @brief Compare and return longer word length
 * @details
	두 bigint의 워드열 길이를 비교하여 더 긴 워드열의 길이 반환
 * @param bigint* A 워드 길이 비교를 수행할 bigint 형 포인터 변수
 * @param bigint* B 워드 길이 비교를 수행할 bigint 형 포인터 변수
 * @return int MAX(WordLen(A), WordLen(B)) 더 긴 워드열 길이
 * @throws ERROR A(B)의 word length가 양수가 아닌 경우
 */
int Compare_WordLen(bigint* A, bigint* B) // return wordlen 큰 사이즈
{
	int A_Len, B_Len;

	BI_Get_Word_Length(&A_Len, &A); // A의 워드열 길이
	BI_Get_Word_Length(&B_Len, &B); // B의 워드열 길이

	if (A_Len <= 0)
		return ERROR;
	if (B_Len <= 0)
		return ERROR;

	if (A_Len > B_Len) // A의 워드열의 길이가 더 길면
		return A_Len; // A의 워드열 길이 return
	else // B의 워드열의 길이가 더 길면
		return B_Len; // B의 워드열 길이 return
}