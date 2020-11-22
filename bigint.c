#include "bigint.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define   zerorize

// Chpater 2.1
void array_init(word* a, int len)
{
	memset(a, 0, sizeof(word) * len - 1); // 배열의 길이만큼 초기화
}

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

void BI_New(bigint** x, int wordlen)
{
	if (*x != NULL) //   x의 주소가 NULL이 아니면
		BI_Delete(x); // BI_Delete()로 초기화

	*x = (bigint*)malloc(sizeof(bigint));
	(*x)->sign = NON_NEGATIVE; // 양수이고
	(*x)->wordlen = wordlen;   // 2번째 매개변수와 같은 길이인
	(*x)->a = (word*)calloc(wordlen, sizeof(word)); // bigint x 생성
}

// Chapter 2.2 Show BigInt
void BI_Set_By_Array(bigint** x, int sign, word* a, int wordlen)
{
	int i;

	(*x)->sign = sign; // bigint 구조체의 부호 설정
	(*x)->wordlen = wordlen; // bigint 구조체의 워드열 길이 설정

	for (i = 0; i < wordlen; i++)
		(*x)->a[i] = a[i]; // 워드 단위 배열 a에 있는 값 bigint 구조체 내부의 워드 단위 배열에 대입
}

void BI_Set_By_String(bigint** x, int sign, char* str, word base, int size)
{
	char* hex;
	int i, j, k;

	hex = (char*)malloc(size * sizeof(word)); // 입력받은 string 값을 hex 값으로 변환시킬 때 저장할 곳
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
void bi_refine(bigint* x)
{
	word* temp;

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
	}

	if ((x->wordlen == 1) && (x->a[0] == 0x0))
		x->sign = NON_NEGATIVE; // 부호 결정
}

// Chapter 2.4 Assign BigInt
void Assign_BI(bigint** y, bigint* x)
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
void bi_gen_rand(bigint** x, int sign, int wordlen)
{
	BI_New(x, wordlen); // bigint 구조체 생성
	(*x)->sign = sign; // 부호 결정
	array_rand((*x)->a, wordlen); // random으로 배열 설정

	bi_refine(*x); // 앞쪽 0으로 채워진 부분 자르기
}

void array_rand(word* dst, int wordlen)
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
void Get_Word_Length(int* len, bigint** x)
{
	*len = (*x)->wordlen; // Big Integer x의 wordlen를 대입
}

void Bit_Length(int* len, bigint* x)
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
	j += i * WORD_BIT_LEN; // 최상위 워드의 몇번째 비트의 길이인지 출력하도록 j에 대입
	*len = j;
}

void j_th_Bit_of_BI(int j, bigint* x)
{
	char* z = NULL;
	unsigned long long i = 0;
	int k = 0;
	printf("j_th bit : ");
	if (j >= (x->wordlen) * WORD_BIT_LEN) // x의 bit len보다 길면 에러
		return;
	else 
	{
		k = j / WORD_BIT_LEN; // 변수 k는 j번째 비트가 위치한 워드의 위치(몇 번째 워드)
		i = 1;
		i = i << j; // 1을 j번 left shift. ex) 1을 3만큼 left shift --> 1000

		if (i == (x->a[k] & i)) // shift한 i와 j번째 비트가 위치한 워드와 &연산을 통해 j번째 비트의 값을 알아내기
			printf("1");
		else
			printf("0");
	}
	printf("\n");
}

// Chapter 2.7 /* negative: 1, non-negative: 0 */
int Get_Sign(bigint* x)
{
	if ((x->sign) == NON_NEGATIVE) // bigint 구조체의 부호가 NON_NEGATIVE면 NON_NEGATIVE return
		return NON_NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint 구조체의 부호가 NEGATIVE면 NEGATIVE return
		return NEGATIVE;
	else // 둘 다 아닐 때 ERROR return
		return ERROR;
}

void Flip_Sign(bigint* x)
{
	if ((x->sign) == NON_NEGATIVE) // bigint 구조체의 부호가 NON_NEGATIVE면 부호를 NEGATIVE로 변경
		x->sign = NEGATIVE;
	else if ((x->sign) == NEGATIVE) // bigint 구조체의 부호가 NEGATIVE면 부호를 NON_NEGATIVE로 변정
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
	BI_New(x, 1); // 길이가 1인 bigint x 생성
	(*x)->sign = NON_NEGATIVE; 
	(*x)->a[0] = 0x00; // 0을 만들어야하므로
}

int Is_One(bigint** x)
{
	int i = 0;

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 1) // 부호가 음수이거나, LSB가 1이 아니면 FALSE
		return -1; // False

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen만큼 하나씩 이동해 가면서 LSB가 1이고 배열의 나머지 값이 0인지 확인
	{
		if ((*x)->a[i] != 0)
			return -1;
	}
	return 0;
}

int Is_Zero(bigint** x)
{
	int i = 0;

	if ((*x)->sign == NEGATIVE || (*x)->a[0] != 0) // 부호가 음수이고 LSB가 0이 아니면 FALSE
		return -1; // False

	for (i = (*x)->wordlen - 1; i > 0; i--) // wordlen만큼 하나씩 이동해 가면서 배열의 값이 전부 0인지 확인
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

	if ((*x)->sign < (*y)->sign) // A가 양수, B가 음수면 당연히 A가 크므로
		return 1; 
	else if ((*x)->sign > (*y)->sign) // A가 음수, B가 양수면 당연히 B가 크므로
		return -1;
	else // A, B 부호가 같을 때
	{
		Get_Word_Length(&len_x, x); //len_x = x->wordlen
		Get_Word_Length(&len_y, y); //len_y = y->wordlen
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
	word* temp;

	length = x->wordlen;

	bigint* cp = NULL;
	Assign_BI(&cp, x); // bigint x를 bigint cp에 복사

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
		for (i = 0; i < q; i++)
			x->a[i] = 0; // 0번째 배열 ~ (a - 1)번째 배열 0으로 초기화

		x->a[q] = cp->a[0] << r; // q번째 배열에 원래 배열 중 0번째 배열을 r만큼 왼쪽으로 이동한 것 대입

		for (i = 1; i < x->wordlen - 1; i++) // 1번째 배열 ~ 배열의 끝 바로 앞 = (Aj << r) || (Aj-1 >> (WORD_BIT_LEN - r))
			x->a[i + q] = (cp->a[i] << r) | (cp->a[i - 1] >> (WORD_BIT_LEN - r));

		x->a[x->wordlen - 1] = cp->a[cp->wordlen - 1] >> (WORD_BIT_LEN - r); // 마지막 배열 = An-1 >> (WORD_BIT_LEN - r)
	}

	bi_refine(x);
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

	bi_refine(x);
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

	bi_refine(*x); // 추가
}

//Chapter 3 Addition
// 캐리 포함한 단일 덧셈
int ADD_ABc(bigint** C, bigint** A, bigint** B, int c, int i)
{
	int carry = 0;
	word* temp = 0;
	temp = (*A)->a[i];
	(*C)->a[i] = (*A)->a[i] + (*B)->a[i]; // A, B 각각의 i번째 값 더해서 C의 i번째 배열에 대입

	if ((*C)->a[i] < temp) // 원래의 A의 i번째 값이 앞에서 계산한 결과보다 작으면 캐리가 발생한 것
		carry = 1;

	(*C)->a[i] = (*C)->a[i] + c; // 인자로 받아온 캐리 값(c) 더해 주기

	if ((*C)->a[i] < c) // 앞의 결과가 c보다 작으면 캐리가 발생한 것
		carry = carry + 1;

	return carry;
}

// WordLen(A) >= WordLen(B)
void ADDC(bigint** C, bigint** A, bigint** B, int sign)
{
	int A_Len;
	int B_Len;
	int i;
	int carry;

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
	else // 마지막 carry가 0이면
		(*C)->a[A_Len] = 0; // 결과값의 MSB에 0 설정

	bi_refine(*C);

	// 부호 설정
	if (sign == 0)
		(*C)->sign = 0;
	else
		(*C)->sign = 1;
}

// ADD 함수에서 A = A + B인 경우만 별도로 구현
void ADDC_AAB(bigint** C, bigint** A, bigint** B, int sign)
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
		(*C)->a[A_Len] = 1;;

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
		// A + B에서 A가 0이면 B return
		(*C)->sign = (*B)->sign;
		(*C)->wordlen = (*B)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*B)->a[i];

		return;
	}

	if (Is_Zero(B) == 0) // B is zero
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
		Assign_BI(&temp, *B); // temp = B

		Flip_Sign(temp); // temp = |B|
		SUB(C, *A, temp); // C = A - |B|
		bi_refine(*C);

		BI_Delete(&temp);

		return;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE)) // A가 음수, B가 양수이면
	{
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		Assign_BI(&temp, *A); // temp = A

		Flip_Sign(temp); // temp = |A|
		SUB(C, *B, temp); // C = B - |A|
		bi_refine(*C);

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

// ADD 함수에서 A = A + B인 경우만 별도로 구현
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
		SUB(C, *A, temp); // SUB 함수

		BI_Delete(&temp);

		return;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
	{
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		Assign_BI(&temp, *A);

		Flip_Sign(temp);
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

int Compare_WordLen(bigint* A, bigint* B) // return wordlen 큰 사이즈
{
	int A_Len, B_Len;

	Get_Word_Length(&A_Len, &A); // A의 워드열 길이
	Get_Word_Length(&B_Len, &B); // B의 워드열 길이

	if (A_Len > B_Len) // A의 워드열의 길이가 더 길면
		return A_Len; // A의 워드열 길이 return
	else // B의 워드열의 길이가 더 길면
		return B_Len; // B의 워드열 길이 return
}

void SUB(bigint** C, bigint* A, bigint* B)
{
	int borrow = 0;
	int len = 0;
	bigint* d = NULL;

	Get_Word_Length(&len, &A);   // A->wordlen을 len에 대입
	Get_Word_Length(&borrow, &B);// B->wordlen을 borrow에 대입 
	
	if (Is_Zero(&A) == 0) // 0-B --> -B
	{
		Assign_BI(C, B); // B의 값 그대로
		(*C)->sign = NEGATIVE; // 부호만 반대로

		return; // memory leackege X
	} 

	if (Is_Zero(&B) == 0) // A-0 --> A
	{
		Assign_BI(C, A); // A의 값 그대로

		return;// memory leackege X
	} 


	if ((A->sign ^ B->sign) == 0) // A, B 부호가 같을 때
	{
		if ((A->sign & B->sign) == 0) // A, B의 부호가 모두 양수일 때
		{
			if (Compare_BI(&A, &B) < 0) // A, B를 비교해서 A < B일 때. (Compare_BI(A, B)의 return : -1)
			{
				SUBC_BI(&borrow, C, &B, &A); // B - A 를 하고
				Flip_Sign(*C); // 부호를 바꿔주기

				return;// memory leACkege X
			}
			else if (Compare_BI(&A, &B) == 0)
			{
				bi_refine(*C); // A와 B가 동일하면 0이 나오므로, 기존에 생성한 C가 그대로 나오면 됨. 그래서 바로 return
				return;// memory leACkege X
			}
			else // A, B 를 비교해서 A >= B일 때. Compare_BI(A, B)'s return : 0, 1
			{
				SUBC_BI(&borrow, C, &A, &B); // A - B 연산

				return;// memory leACkege X
			}
		}

		else // A, B의 부호가 모두 음수일 때
		{
			Flip_Sign(A); // A의 부호가 음수이므로 부호 바꿔주기
			Flip_Sign(B); // B의 부호가 음수이므로 부호 바꿔주기
			if (Compare_BI(&A, &B) < 0)
			{
				SUBC_BI(&borrow, C, &B, &A);
				Flip_Sign(A); // 부호 원위치
				Flip_Sign(B); // 부호 원위치

				return;// memory leACkege X
			}
			else
			{
				SUBC_BI(&borrow, C, &A, &B);
				Flip_Sign(*C);
				Flip_Sign(A); // 부호 원위치
				Flip_Sign(B); // 부호 원위치

				return;// memory leACkege X
			}
		}
	}

	else // A,B 부호가 다를 때
	{
		if (A->sign == 0) // A가 양수, B가 음수
		{
			Flip_Sign(B); // B의 부호를 바꿔주고
			ADD(C, &A, &B);  // ADD 연산
			Flip_Sign(B); // 부호 원위치

			return;
		}
		else
		{
			Flip_Sign(A); //
			ADD(C, &A, &B);

			Flip_Sign(A); // 부호 원위치
			Flip_Sign(*C); // 부호 원위치

			return;
		}
	}
}

bigint* SUBC_BI(int* borrow, bigint** C, bigint** A, bigint** B)
{
	int len, i = 0;
	bigint* temp = NULL; // A와 b의 길이가 다를 때 -> bigint** b의 길이를 바꿀 수 없으므로 temp를 만들어줌

	Get_Word_Length(&len, A); // b보다 큰 A의 길이를 구하자
	BI_New(&temp, len);  // A의 워드 길이와 같게 temp 를 생성
	for (i = 0; i < (*B)->wordlen; i++)
		temp->a[i] = (*B)->a[i]; // b와 같은 값을 가지고 있어야하고, 더 길게 생성됐을 때는 0이 들어가있어야함.
	// A가 b보다 길 때 b의 길이를 맞춰줘야하는데 b를 건들이면 b가 바뀌기 때문에 temp를 이용

	for (i = 0; i < len; i++)
	{
		if (i == 0)
			*borrow = 0;

		(*C)->a[i] = (*A)->a[i] - (*borrow); // A - b의 값을 C 에 대입
		(*C)->a[i] = (*C)->a[i] & word_mask; // mod 2 ^ (WORD_BIT_LEN)
		if ((*A)->a[i] < *borrow) // borrow 될 때
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

	BI_Delete(&temp);
	bi_refine(*C);
}

void MUL_Test(word* C, word* A, word* B) // 단일 워드 곱셈
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
	A0 = (*A) & (word)((1 << (WORD_BIT_LEN >> 1)) - 1); // A의 중간비트부터 최하위 비트까지
	B0 = (*B) & (word)((1 << (WORD_BIT_LEN >> 1)) - 1); // B의 중간비트부터 최하위 비트까지

	mul1 = A1 * B1;
	mul0 = A0 * B0;
	sum1 = A1 * B0;
	sum1 += A0 * B1;
	if (sum1 < A0 * B1)
		carry1 += 1;
	sum2 = (sum1 & (word)((1 << (WORD_BIT_LEN >> 1)) - 1));
	sum2 = sum2 << (WORD_BIT_LEN >> 1); //sum1의 뒷부분
	sum1 = sum1 >> (WORD_BIT_LEN >> 1); // sum1의 앞부분
	mul0 = sum2 + mul0;
	if (mul0 < sum2)
		carry0 += 1;
	mul1 = sum1 + mul1 + carry0 + (carry1 << (WORD_BIT_LEN >> 1));

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

	BI_New(&D, (*result)->wordlen); // 단일 워드 곱셈 연산의 결과를 저장할 big integer d

	for (i = 0; i < B->wordlen; i++)
	{
		for (j = 0; j < A->wordlen; j++)
		{
			MUL_Test(&D->a[i + j], &A->a[j], &B->a[i]); // A의 단일워드와 B의 단일워드 연산 후 D의 단일 워드에 대입
			ADDC_AAB(result, result, &D, 0); 
			D->a[i + j] = 0;
			D->a[i + j + 1] = 0;
		}
	}
	BI_Delete(&D);
	(*result)->sign = A->sign ^ B->sign;
	bi_refine(*result);
}

void Karatsuba(bigint** C, bigint* A, bigint* B)
{
	int i, j = 0;
	int len, A_Len, B_Len;
	int S0_sign, S1_sign;
	int flag = 3;

	Get_Word_Length(&A_Len, &A); // A_Len = A의 워드열 길이
	Get_Word_Length(&B_Len, &B); // B_Len = B의 워드열 길이

	if (flag >= MIN(A_Len, B_Len)) // A의 워드열의 길이와 B의 워드열의 길이 중 더 작은 것이 flag보다 작으면
	{
		MUL_MUL(C, A, B); // textbook multiplication 실행
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

	Right_Shift(A1, len * WORD_BIT_LEN); // A1 = A >> len word (A를 len 워드만큼 오른쪽으로 이동)
	Reduction_BI(&A0, len * WORD_BIT_LEN); // A0 = A mod (2^(len * wordlen)) (A를 len 워드만큼 modular 연산 수행)
	Right_Shift(B1, len * WORD_BIT_LEN); // B1 = A >> len word (A를 len 워드만큼 오른쪽으로 이동)
	Reduction_BI(&B0, len * WORD_BIT_LEN); // B0 = A mod (2^(len * wordlen)) (A를 len 워드만큼 modular 연산 수행)

	BI_New(&T0, len * 2);
	BI_New(&T1, len * 2);
	BI_New(&S0, Compare_WordLen(B0, B1)); // S0 = B1 - B0이니까 B1과 B0 중 더 큰 워드 길이만큼 bigint 생성
	BI_New(&S1, Compare_WordLen(A0, A1)); // S1 = A0 - A1이니까 A0과 A1 중 더 큰 워드 길이만큼 bigint 생성
	BI_New(&R, len * 4);
	BI_New(&S, len * 2);

	Karatsuba(&T1, A1, B1); // T1 = A1 * B1
	Karatsuba(&T0, A0, B0); // T0 = A0 * B0

	bi_refine(T1);
	bi_refine(T0);

	bigint* T1_tmp = NULL;
	Assign_BI(&T1_tmp, T1);
	Left_Shift(T1_tmp, 2 * WORD_BIT_LEN * len); // T1_tmp = T1을 2 * len 워드만큼 오른쪽으로 이동

	// R = T1 || T0
	for (i = 0; i < T1->wordlen; i++)
		R->a[len * 2 + i] = T1_tmp->a[T1_tmp->wordlen - T1->wordlen + i];
	for (i = 0; i < T0->wordlen; i++)
		R->a[i] = T0->a[i];

	bi_refine(R);

	SUB(&S1, A0, A1); // S1 = A0 - A1
	SUB(&S0, B1, B0); // S0 = B1 - B0

	S0_sign = Get_Sign(S0);
	S1_sign = Get_Sign(S1);

	//S0, S1은 음수일 수 있으므로 절댓값 취해 주기
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
	bi_refine(S);

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
}

void Single_Squaring(bigint* C, bigint* A)
{
	int i;

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

	// A1 = |A| >> WORD_BIT_LEN / 2, A0 = |A| mod (WORD_BIT_LEN / 2) 를 위한 |A| 처리
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

void Multi_Squaring(bigint* C, bigint* A)
{
	int i, j, k;
	int wordlen_A;

	bigint* C1 = NULL;
	bigint* C2 = NULL;
	bigint* T1 = NULL;
	bigint* T2 = NULL;
	bigint* A_i = NULL;
	bigint* A_j = NULL;

	BI_New(&T1, 2);
	BI_New(&T2, 2);
	BI_New(&A_i, 1);
	BI_New(&A_j, 1);

	A_i->sign = A->sign;
	A_j->sign = A->sign;

	Get_Word_Length(&wordlen_A, &A);

	BI_New(&C1, 2 * wordlen_A);
	BI_New(&C2, 2 * wordlen_A);

	for (j = 0; j < wordlen_A; j++)
	{
		A_j->a[0] = A->a[j];
		Single_Squaring(T1, A_j);
		Left_Shift(T1, (int)(2 * j * WORD_BIT_LEN)); // T1 = T1 << ((2 * j) * WORD_BIT_LEN)

		// C1 = T1 || C1
		if (j == 0)
		{
			for (k = 0; k < T1->wordlen; k++)
				C1->a[k] = T1->a[k]; // j = 0일 때는 C1 = T1 || C1 = T1 (C1이 처음에 0이니까)
		}
		else
		{
			for (k = 0; k < 2; k++)
				C1->a[2 * j + k] = T1->a[2 * j + k]; // 그 다음부터는 T1이 2 워드 단위로 왼쪽으로 시프트 연산한 결과이므로 2 * j번째부터 넣어 주기
		}

		//array_init(T1->a, T1->wordlen);

		for (i = j + 1; i < wordlen_A; i++)
		{
			A_i->a[0] = A->a[i];
			MUL_MUL(&T2, A_j, A_i);
			Left_Shift(T2, (int)((i + j) * WORD_BIT_LEN));

			ADD_AAB(&C2, &C2, &T2);
			array_init(T2->a, T2->wordlen);
		}
	}
	Left_Shift(C2, 1);
	ADD(&C, &C1, &C2);

	BI_Delete(&C1);
	BI_Delete(&C2);
	BI_Delete(&T1);
	BI_Delete(&T2);
	BI_Delete(&A_i);
	BI_Delete(&A_j);
}

void Binary_Long_Div(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int i = 0;
	int temp = 0;
	int size = 0;
	int len = 0;
	int result = 0;
	bigint* T = NULL;
	bigint* U = NULL;
	BI_New(&T, 1);
	BI_Set_Zero(Q);
	BI_Set_Zero(R);

	Get_Word_Length(&size, &A); // size == bigint A's word length.

	size = size * WORD_BIT_LEN; // 변수 size 는 A의 총 비트 길이
	for (i = size - 1; i >= 0; i--) // 최상위 비트부터
	{
		len = (int)(i / WORD_BIT_LEN); // len에는 지금 조사하고 있는 비트의 워드 위치
		temp = A->a[len]; // 현재 연산 중인 워드의 값을 temp에 대입
		len = i % WORD_BIT_LEN; // 해당 워드에 몇 번째 비트인지
		temp = temp >> len; // 해당 비트를 left shift해서 첫번째 비트에 놓이도록
		temp = temp & 1; // 1과 & 해서 
		T->a[0] = temp; // 해당 비트의 값을 대입 // j_th_Bit_Length 바꿔서 하기

		Left_Shift(*R, 1); // 2R
		ADD_DIV(R, R, &T); // 2R + a{j}
		result = Compare_BI(&B, R); // B >= R
		if (result == -1) // B >= R인지 비교
		{
			len = (int)(i / WORD_BIT_LEN) + 1;
			BI_New(&U, len);

			U->a[len - 1] = 1 << (i % WORD_BIT_LEN);
			ADD_DIV(Q, Q, &U); // Q + 2 ^ j
			SUB(R, *R, B); // R - B
			bi_refine(*R);
			BI_Delete(&U);

		}
	}
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

	bi_refine(*B);
	//bi_refine(*C);

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
			BI_New(A, B_Len);
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
		SUB(C, *A, temp); // SUB 함수
		//bi_refine(C);

		BI_Delete(&temp);

		return;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
	{
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		Assign_BI(&temp, *A);

		Flip_Sign(temp);
		SUB(C, *B, temp); // SUB 함수
		//bi_refine(C);

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
