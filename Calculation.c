﻿#include "config.h"
#include "BasicOperation.h"
#include "Calculation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define   zerorize

/* PROTO TYPE */
int ADD_ABc(bigint** C, bigint** A, bigint** B, int c, int i); // Single Addition include carry
int ADDC(bigint** C, bigint** A, bigint** B, int sign); // Addition Core
int ADDC_AAB(bigint** C, bigint** A, bigint** B, int sign); // Addition Core function (A = A + B)
int ADD_AAB(bigint** C, bigint** A, bigint** B); // Addition (A = A + B)
int SUBC(bigint** c, bigint** a, bigint** b); // Subtraction Core
int MUL_Word(word* c, word* a, word* b); // Single Multiplication
int Single_Squaring(bigint* C, bigint* A); // Single Squaring
int SQUC(bigint** C, bigint* A); // Squaring Core
int ADDC_DIV(bigint** C, bigint** A, bigint** B, int sign); // Addition Core which is used in Division
int ADD_DIV(bigint** C, bigint** A, bigint** B); // Addition which is used in Division
int DIVC(bigint** Q, bigint** R, bigint* A, bigint* B); // Division Core
int DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B); // Division Core Core
int DIVCC_n_m(bigint** Q, bigint* A, bigint* B, int m); // Division Core Core (when n = m)
int DIVCC_n_m1(bigint** Q, bigint* A, bigint* B, int m); // Division Core Core (when n = m + 1)
int EXP_Montgomery_MUL(bigint** T, bigint* X, bigint* N); // Only Exponentiation function used Montgomery Ladder (MUL ver)
int EXP_Montgomery_ADD(bigint** T, bigint* X, bigint* N); // Only Exponentiation function used Montgomery Ladder (ADD ver)
int EXP_LR_MUL(bigint** T, bigint* X, bigint* N); // Only Exponentiation function used Left to Right (MUL ver)
int EXP_LR_ADD(bigint** T, bigint* X, bigint* N); // Only Exponentiation function used Left to Right (ADD ver)
int EXP_RL_MUL(bigint** T, bigint* X, bigint* N); // Only Exponentiation function used Right to Left (MUL ver)
int EXP_RL_ADD(bigint** T, bigint* X, bigint* N); // Only Exponentiation function used Right to Left (ADD ver)

/**
 * @brief Addition (캐리를 포함한 단일 덧셈)
 * @details

	@n [pseudo code]
	@n Input  : C, A, B, c, i
	@n Output : carry (0 or 1)
	@n 1 : carry <- 0
	@n 2 : C <- A + B mod(2^word)
	@n 3 : if C < A then
	@n 4 :		carry <- 1
	@n 5 :	end if
	@n 6 : C <- C + c mod(2^word)
	@n 7 : if C < c then
	@n 8 :		carry = carry + 1
	@n 9 : end if
	@n 10 : return carry

 * @param bigint** C 덧셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param int c 이전 연산에서 받아 온 캐리 (0 또는 1)
 * @param int i 덧셈을 수행할 배열의 index

 * @return carry 캐리 여부 (0 또는 1)
 * @throws ERROR 배열의 index가 음수인 경우
 */
int ADD_ABc(bigint** C, bigint** A, bigint** B, int c, int i)
{
	int carry = 0; // [line 1]
	word temp = 0;

	if (i < 0) // 예외 처리
		return ERROR;

	temp = (*A)->a[i];
	(*C)->a[i] = (*A)->a[i] + (*B)->a[i]; // [line 2] A, B 각각의 i번째 값 더해서 C의 i번째 배열에 대입

	if ((*C)->a[i] < temp) // [line 3] 원래의 A의 i번째 값이 앞에서 계산한 결과보다 작으면 캐리가 발생한 것
		carry = 1; // [line 4]

	(*C)->a[i] = (*C)->a[i] + c; // [line 6] 인자로 받아온 캐리 값(c) 더해 주기

	if ((*C)->a[i] < (unsigned int)c) // [line 7] 앞의 결과가 c보다 작으면 캐리가 발생한 것
		carry = carry + 1; // [line 8]

	return carry; // [line 10]
}

/**
 * @brief Addition Core (WordLen(A) >= WordLen(B), Sign(A) = Sign(B))
 * @details

	@n [pseudo code]
	@n Input  : A, B, sign
	@n Output : C = A + B
	@n 1 : B[j] <- 0 for j = m, m + 1, ... , n - 1
	@n 2 : c <- 0
	@n 3 : for j = 0 to n - 1 do
	@n 4 :	c, C[j] <- ADD_ABc(A[j], B[j], c)
	@n 5 : end for
	@n 6 : C[n] <- c
	@n 7 : if C[n] = 1 then
	@n 8 :	C <- (-1)^sign * sum(C[j] * W^j) for j = 0 to n
	@n 9 : else
	@n 10 :	C <- (-1)^sign * sum(C[j] * W^j) for j = 0 to n - 1
	@n 11 : end if

 * @param bigint** C 덧셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param int sign 덧셈 결과 C의 부호
 * @return SUCCESS
 * @throws
	ERROR A(B)의 word length가 양수가 아닌 경우
	ERROR bigint 구조체 내부에 있는 배열 재할당 실패 시
 */
int ADDC(bigint** C, bigint** A, bigint** B, int sign)
{
	int A_Len;
	int B_Len;
	int i;
	int carry;

	BI_Get_Word_Length(&A_Len, A);
	BI_Get_Word_Length(&B_Len, B);

	if (A_Len <= 0)
		return ERROR;
	if (B_Len <= 0)
		return ERROR;

	word* temp;

	// A의 워드열 길이가 B의 워드열 길이보다 큰 경우이므로 B의 워드열 길이를 A의 워드열 길이만큼 늘려서 realloc
	(*B)->wordlen = A_Len;
	temp = (word*)realloc((*B)->a, sizeof(word) * (*B)->wordlen);
	
	if (temp != NULL)
		(*B)->a = temp;
	else
		return ERROR;

	memset(&(*B)->a[B_Len], 0x00, (A_Len - B_Len) * sizeof(word)); // [line 1] 늘어난 길이만큼 0으로 초기화

	carry = 0; // [line 2]

	for (i = 0; i < A_Len; i++) // [line 3]
		carry = ADD_ABc(C, A, B, carry, i); // [line 4] 단일 덧셈 A의 워드열 길이만큼 실행

	if (carry == 1) // [line 7] 마지막 carry가 1이면
		(*C)->a[A_Len] = 1; // [line 8] 결과값의 MSB에 1 설정

	BI_Refine(*C);

	// [line 8, 10] 부호 설정
	if (sign == 0)
		(*C)->sign = 0;
	else
		(*C)->sign = 1;

	return SUCCESS;
}

/**
 * @brief Addition Core (WordLen(A) >= WordLen(B), Sign(A) = Sign(B), A = A + B)
 * @details
	@n ADDC 함수와 동일하게 동작
	@n 덧셈을 수행한 결과를 다시 A에 저장 (A <- A + B)
 * @param bigint** C 덧셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param int sign 덧셈 결과 C의 부호
 * @return SUCCESS
 * @throws
	ERROR A(B)의 word length가 양수가 아닌 경우
	ERROR bigint 구조체 내부에 있는 배열 재할당 실패 시
 */
int ADDC_AAB(bigint** C, bigint** A, bigint** B, int sign)
{
	int A_Len;
	int B_Len;
	int i;
	int carry;

	BI_Get_Word_Length(&A_Len, A);
	BI_Get_Word_Length(&B_Len, B);

	if (A_Len <= 0)
		return ERROR;
	if (B_Len <= 0)
		return ERROR;

	word* temp;

	(*B)->wordlen = A_Len;
	temp = (word*)realloc((*B)->a, sizeof(word) * (*B)->wordlen);
	if (temp != NULL)
		(*B)->a = temp;
	else
		return ERROR;

	memset(&(*B)->a[B_Len], 0x00, (A_Len - B_Len) * sizeof(word));

	carry = 0;

	for (i = 0; i < A_Len; i++)
		carry = ADD_ABc(C, A, B, carry, i);

	if (carry == 1)
		(*C)->a[A_Len] = 1;

	if (sign == 0)
		(*C)->sign = 0;
	else
		(*C)->sign = 1;

	return SUCCESS;
}

/**
 * @brief Addition
 * @details

	 @n [pseudo code]
	 @n Input  : A, B
	 @n Output : C = A + B
	 @n 1 : if A = 0 then
	 @n 2 :	C <- B
	 @n 3 : end if
	 @n 4 : if B = 0 then
	 @n 5 :	C <- A
	 @n 6 : end if
	 @n 7 : if A > 0 and B < 0 then
	 @n 8 :	C <- SUB(A, |B|)
	 @n 9 : end if
	@n 10 : if A < 0 and B > 0 then
	@n 11 :	C <- SUB(B, |A|)
	@n 12 : end if
	@n 13 : if WordLen(A) >= WordLen(B) then
	@n 14 :	C <- ADDC(A, B)
	@n 15 : else
	@n 16 :	C <- ADDC(B, A)
	@n 17 : end if

 * @param bigint** C 덧셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 덧셈을 수행할 bigint 형 더블포인터 변수
 * @return SUCCESS 성공 시
 * @throws
	ERROR A(B)의 bigint 구조체 미할당 시
	ERROR A(B)의 부호 가져오기 실패 시
 */
int ADD(bigint** C, bigint** A, bigint** B)
{
	int A_Len = 0;
	int B_Len = 0;
	int A_sign;
	int B_sign;
	int i;
	int re;

	if ((*A) == NULL)
		return ERROR;
	if ((*B) == NULL)
		return ERROR;

	A_sign = BI_Get_Sign(*A);
	B_sign = BI_Get_Sign(*B);

	if (A_sign == ERROR)
		return ERROR;
	if (B_sign == ERROR)
		return ERROR;

	BI_Get_Word_Length(&A_Len, A);
	BI_Get_Word_Length(&B_Len, B);

	if (BI_Is_Zero(A) == 0) // [line 1] A is zero
	{
		// [line 2] A + B에서 A가 0이면 B return
		(*C)->sign = (*B)->sign;
		(*C)->wordlen = (*B)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*B)->a[i];

		return SUCCESS;
	}

	if (BI_Is_Zero(B) == 0) // [line 4] B is zero
	{
		// [line 5] A + B에서 B가 0이면 A return
		(*C)->sign = (*A)->sign;
		(*C)->wordlen = (*A)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*A)->a[i];

		return SUCCESS;
	}

	if ((A_sign == NON_NEGATIVE) && (B_sign == NEGATIVE)) // [line 7] A가 양수, B가 음수이면
	{
		bigint* temp = NULL;
		BI_Assign(&temp, *B); // temp = B

		BI_Flip_Sign(temp); // temp = |B|
		re = SUB(C, *A, temp); // [line 8] C = A - |B|
		if (re == ERROR)
			return ERROR;
		
		BI_Refine(*C);
		BI_Delete(&temp);

		return SUCCESS;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE)) // [line 10] A가 음수, B가 양수이면
	{
		bigint* temp = NULL;
		BI_Assign(&temp, *A); // temp = A

		BI_Flip_Sign(temp); // temp = |A|
		re = SUB(C, *B, temp); // [line 11] C = B - |A|
		if (re == ERROR)
			return ERROR;
		
		BI_Refine(*C);
		BI_Delete(&temp);

		return SUCCESS;
	}

	// A, B가 동일한 부호일 때
	if (A_Len >= B_Len) // [line 13] A의 워드열의 길이가 B의 워드열의 길이보다 길 때
	{
		re = ADDC(C, A, B, A_sign); // [line 14] 덧셈 실행
		if (re == ERROR)
			return ERROR;

		return SUCCESS;
	}
	else // [line 15] B의 워드열의 길이가 A의 워드열의 길이보다 길 때
	{
		re = ADDC(C, B, A, A_sign); // [line 16] 덧셈 실행
		if (re == ERROR)
			return ERROR;

		return SUCCESS;
	}
}

/**
 * @brief Addition (A = A + B)
 * @details
	@n ADD 함수와 동일하게 동작
	@n 덧셈을 수행한 결과를 다시 A에 저장 (A <- A + B)
 * @param bigint** C 덧셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 덧셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 덧셈을 수행할 bigint 형 더블포인터 변수
 * @return SUCCESS 성공 시
 * @throws
	ERROR A(B)의 bigint 구조체 미할당 시
	ERROR A(B)의 부호 가져오기 실패 시
 */
int ADD_AAB(bigint** C, bigint** A, bigint** B) // A = A + B
{
	int A_Len = 0;
	int B_Len = 0;
	int A_sign;
	int B_sign;
	int re;
	int i;

	if (*A == NULL)
		return ERROR;
	if (*B == NULL)
		return ERROR;

	A_sign = BI_Get_Sign(*A);
	B_sign = BI_Get_Sign(*B);

	if (A_sign == ERROR)
		return ERROR;
	if (B_sign == ERROR)
		return ERROR;;

	BI_Get_Word_Length(&A_Len, A);
	BI_Get_Word_Length(&B_Len, B);

	if (BI_Is_Zero(A) == 0) // A is zero
	{
		(*C)->sign = (*B)->sign;
		(*C)->wordlen = (*B)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*B)->a[i];

		return SUCCESS;
	}

	if (BI_Is_Zero(B) == 0) // B is zero
	{
		(*C)->sign = (*A)->sign;
		(*C)->wordlen = (*A)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*A)->a[i];

		return SUCCESS;
	}

	if ((A_sign == NON_NEGATIVE) && (B_sign == NEGATIVE))
	{
		bigint* temp = NULL;
		BI_Assign(&temp, *B);

		BI_Flip_Sign(temp);
		re = SUB(C, *A, temp); // SUB 함수
		if (re == ERROR)
			return ERROR;

		BI_Delete(&temp);

		return SUCCESS;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
	{
		bigint* temp = NULL;
		BI_Assign(&temp, *A);

		BI_Flip_Sign(temp);
		re = SUB(C, *B, temp); // SUB 함수
		if (re == ERROR)
			return ERROR;

		BI_Delete(&temp);

		return SUCCESS;
	}

	// A, B가 동일한 부호일 때
	if (A_Len >= B_Len)
	{
		re = ADDC_AAB(C, A, B, A_sign);
		if (re == ERROR)
			return ERROR;

		return SUCCESS;
	}
	else
	{
		re = ADDC_AAB(C, B, A, A_sign);
		if (re == ERROR)
			return ERROR;

		return SUCCESS;
	}
}

/**
* @brief Subtraction
* @details
	@n [pseudo code]
	@n Input  : C, A, B
	@n Output : C = A - B

	@n 1 : if A = 0 then
	@n 2 :		return -B
	@n 3 :	end if
	@n 4 :	if B = 0 then
	@n 5 :		return A
	@n 6 :	end if
	@n 7 :	if A >= B > 0 then
	@n 8 :		return SUBC(A, B)
	@n 9 :	else
	@n 10:		return -SUBC(B, A)
	@n 11:	end if
	@n 12:	if 0 > A >= B then
	@n 13:		return SUBC(|B|, |A|)
	@n 14:	else
	@n 15:		return -SUBC(|A|, |B|)
	@n 16:	end if
	@n 17: 	if A > 0 and B < 0 then
	@n 18:		return ADD(A, |B|)
	@n 19:	else
	@n 20:		return -ADD(|A|, B)
	@n 21:	end if

* @param bigint** C 뺄셈 연산의(A - B) 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint** A 뺄셈 연산의(A - B) A에 해당하는 bigint 형 포인터 변수
* @param bigint** B 뺄셈 연산의(A - B) B에 해당하는 bigint 형 포인터 변수
* @return SUCCESS 성공 시
* @throws
	ERROR A, B, C의 bigint 구조체 미할당 시
	ERROR A, B, C의 word length가 양수가 아닌 경우
*/
int SUB(bigint** C, bigint* A, bigint* B)
{
	unsigned int borrow = 0;
	int len = 0;
	int i = 0;
	int result;
	int ret = 0;

	if ((*C == NULL) | (B == NULL) | (A == NULL))
		return ERROR;

	BI_Get_Word_Length(&len, &A);   // A->wordlen을 len에 대입
	BI_Get_Word_Length(&borrow, &B);// B->wordlen을 borrow에 대입

	if (len < 0)
		return ERROR;
	if (borrow < 0)
		return ERROR;

	result = BI_Compare(&A, &B);

	if (BI_Is_Zero(&A) == 0) // [line 1] 0-B --> -B
	{
		BI_Assign(C, B); // [line 2] B의 값 그대로

		if (BI_Is_Zero(&B) == 0) // 0 - 0인 경우
			return SUCCESS;

		(*C)->sign = NEGATIVE; // [line 2] 부호만 반대로

		return SUCCESS; // memory leackege X
	}

	if (BI_Is_Zero(&B) == 0) // [line 4] A - 0 --> A
	{
		if (&A == C) // 1st arg == 2nd arg(AAB)인 경우
			return SUCCESS; // 이미 1st arg = 2nd arg 이므로 return으로 종료.
		BI_Assign(C, A); // [line 5] 1st arg != 2nd arg(AAB)인 경우 C에 A를 assign
		return SUCCESS;
	}

	if ((A->sign ^ B->sign) == 0) // A, B 부호가 같을 때
	{
		if ((A->sign & B->sign) == 0) // [line 7] A, B의 부호가 모두 양수일 때
		{
			if (result == 2) // [line 9] A, B를 비교해서 A < B일 때. (BI_Compare(A, B)의 return : 2)
			{
				ret = SUBC(C, &B, &A); // [line 10] B - A 를 하고
				if (ret == -1)
					return ERROR;
				
				BI_Flip_Sign(*C); // [line 10] 뺄셈 연산이 종료되었으므로 원래대로 부호 바꿔주기

				return SUCCESS;
			}
			else if (result == 0) // A = B일 때, C의 값은 0이 되어야한다.
			{
				for (i = 0; i < (*C)->wordlen; i++) // C의 길이만큼
					(*C)->a[i] = 0;  // 0을 대입
				
				(*C)->sign = 0; // C의 부호도 NON-NEGATIVE로 변경

				return SUCCESS;
			}
			else if (result == 1)// [line 7] A, B 를 비교해서 A >= B일 때. BI_Compare(A, B)'s return : 0, 1
			{
				ret = SUBC(C, &A, &B); // [line 8] A - B 연산
				if (ret == -1)
					return ERROR;
				
				return SUCCESS;
			}
			else
				return ERROR;
		}

		else // A, B의 부호가 모두 음수일 때
		{
			BI_Flip_Sign(A); // A의 부호가 음수이므로 부호 바꿔주기
			BI_Flip_Sign(B); // B의 부호가 음수이므로 부호 바꿔주기
			
			result = BI_Compare(&A, &B);
			if (result == 2) // [line 12]
			{
				ret = SUBC(C, &B, &A); // [line 13]
				if (ret == -1)
					return ERROR;
				
				BI_Flip_Sign(A); // 뺄셈 연산이 종료되었으므로 원래대로 부호 원위치
				BI_Flip_Sign(B); // 뺄셈 연산이 종료되었으므로 원래대로 부호 원위치

				return SUCCESS;
			}
			else if ((result == 0) | (result == 1)) // [line 14]
			{
				ret = SUBC(C, &A, &B); // [line 15]
				if (ret == -1)
					return ERROR;
				
				BI_Flip_Sign(*C); // [line 15]에서 결과의 부호 바꿔주기
				BI_Flip_Sign(A); // 뺄셈 연산이 종료되었으므로 원래대로 부호 원위치
				BI_Flip_Sign(B); // 뺄셈 연산이 종료되었으므로 원래대로 부호 원위치

				return SUCCESS;
			}
			else
				return ERROR;
		}
	}

	else // A,B 부호가 다를 때
	{
		if (A->sign == 0) // [line 17] A가 양수, B가 음수
		{
			BI_Flip_Sign(B); // B의 부호를 바꿔주고
			ret = ADD(C, &A, &B);  // [line 18] ADD 연산
			if (ret == -1)
				return ERROR;
			
			BI_Flip_Sign(B); // 부호 원위치

			return SUCCESS;
		}
		else // [line 19]
		{
			BI_Flip_Sign(A); // A => |A|
			ret = ADD(C, &A, &B); // [line 20]
			if (ret == -1)
				return ERROR;

			BI_Flip_Sign(A); // 부호 원위치
			BI_Flip_Sign(*C); // 부호 원위치

			return SUCCESS;
		}
	}
}

/**
* @brief Subtraction Core
* @details
	@n [pseudo code]
	@n Input  : C, A, B
	@n Output : C = A - B

	@n 1 : B_{j} <- 0 for j =  m downto n-1 do
	@n 2 :	b <- 0
	@n 3 :	for j = 0 to n - 1 do
	@n 4 :		C <- A - b (mod W)
	@n 5 :		if A < b then
	@n 6 :			b <- 1
	@n 7 :		else
	@n 8 :			b <- 0
	@n 9 :		end if
	@n 10:		if C < B then
	@n 11:			b <- b + 1
	@n 12:		end if
	@n 13:		C <- C - b
	@n 14:	end for
	@n 15: l <- min{j : C_{n-1} = C_{n-2} = ... = C_{j} = 0}
	@n 16: C <- len(l)

* @param bigint** C 단일 뺄셈 연산의(A - B) 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint** A 단일 뺄셈 연산의(A - B) A에 해당하는 bigint 형 포인터 변수
* @param bigint** B 단일 뺄셈 연산의(A - B) B에 해당하는 bigint 형 포인터 변수
* @return SUCCESS 성공 시
* @throws
	ERROR A, B, C 의 bigint 구조체 미할당 시
	ERROR A, B, C의 word length가 양수가 아닌 경우
*/
int SUBC(bigint** C, bigint** A, bigint** B)
{
	int len, i = 0;
	int result = 0;
	unsigned int borrow = 0;

	bigint* temp = NULL; // A와 b의 길이가 다를 때 -> bigint** b의 길이를 바꿀 수 없으므로 temp를 만들어줌
	bigint* temp3 = NULL; // C랑 A랑 같을 때를 대비하여

	if ((*C == NULL) | (B == NULL) | (A == NULL))
		return ERROR;

	BI_Get_Word_Length(&len, A); // b보다 큰 A의 길이를 구하자	
	BI_New(&temp, len);  // A의 워드 길이와 같게 temp 를 생성

	result = BI_Compare(A, B);
	if ((result == 0) | (result == 1))
		(*C)->sign = 0; // 매개변수 C에 이미 부호가 들어가있을 때 바꿔주는 게 없어서
	
	if (result == 2)
		(*C)->sign = 1;
	
	if ((*C)->wordlen < len) // Binary Long Division에서 C의 길이가 1이고, A, B의 길이가 2일 때가 있어서. //A = 0x40bd
	{
		bigint* temp2 = NULL;
		BI_New(&temp2, len); // C보다 더 큰 길이인 len을 가지는 temp2를 생성
		
		for (i = 0; i < (*C)->wordlen; i++) // C->wordle의 길이만큼만 temp2에 대입. 안채워진 부분은 0으로 그대로
			temp2->a[i] = (*C)->a[i];
		
		BI_Assign(C, temp2);
		BI_Delete(&temp2);
	}
	
	for (i = 0; i < (*B)->wordlen; i++)//
		temp->a[i] = (*B)->a[i]; // b와 같은 값을 가지고 있어야하고, 더 길게 생성됐을 때는 0이 들어가있어야함.
		// A가 b보다 길 때 b의 길이를 맞춰 줘야 하는데 b를 건들이면 b가 바뀌기 때문에 temp를 이용

	BI_Assign(&temp3, *A); // 이후 A와 동일하게
	
	for (i = 0; i < len; i++) // [line 3]
	{
		(*C)->a[i] = temp3->a[i] - borrow; // [line 4] (*C)->a[i] = (*A)->a[i] - (borrow); // A - b의 값을 C 에 대입 // 처음 borrow값은 초기화된 0으로 들어옴
		(*C)->a[i] = (*C)->a[i] & word_mask; // mod 2 ^ (WORD_BIT_LEN)
		
		if (temp3->a[i] < borrow) // [line 5] if ((*A)->a[i] < borrow) // borrow가 생길 때
			borrow = 1;// [line 6] *borrow = 1;
		else // [line 7] borrow 안될 때
		{
			borrow = 0;// [line 8] *borrow = 0;
			if ((*C)->a[i] < temp->a[i]) // [line 10]
				borrow += 1;// [line 11] borrow = borrow + 1;
		}
		
		(*C)->a[i] -= temp->a[i]; // [line 13] temp에 넣어놓은 b와 뺄셈 연산
		(*C)->a[i] = (*C)->a[i] & word_mask; // mod 2 ^ (WORD_BIT_LEN)
	}

	BI_Delete(&temp3);
	BI_Delete(&temp);
	BI_Refine(*C); // [line 15, 16]

	return SUCCESS;
}

/**
 * @brief Multiplication
 * @details
	@n flag에 따라 곱셈 연산 수행(Karatsuba multiplication / textbook multiplication)
	@n Input  : A
	@n Output : C = A * B
 * @param bigint* C 곱셈 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* A 곱셈 연산을 수행할 bigint 형 포인터 변수
 * @param bigint* B 곱셈 연산을 수행할 bigint 형 포인터 변수
 * @return SUCCESS
 * @throws
	ERROR flag 미지정 시
	ERROR A(B)의 bigint 구조체 미할당 시
	ERROR A(B)의 길이가 양수가 아닌 경우
 */
int Multiplication(bigint** C, bigint* A, bigint* B)
{
	int A_Len = 0;
	int B_Len = 0;
	int C_Len = 0;
	int ret;

	if ((B == NULL) | (A == NULL))
		return ERROR;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A의 워드열 길이
	BI_Get_Word_Length(&B_Len, &B); // B_Len = B의 워드열 길이

	if (A_Len < 0)
		return ERROR;
	if (B_Len < 0)
		return ERROR;

	if (FLAG_MUL == TEXTBOOK) // textbook multiplication
	{
		C_Len = A_Len + B_Len; // C의 길이 정해 준 뒤 bigint 생성
		BI_New(C, C_Len); // bigint 생성
		ret = MUL_Multi(C, A, B);
		
		if (ret == ERROR)
			return ERROR;
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

		ret = MUL_Karatsuba(C, A, B);
		if (ret == ERROR)
			return ERROR;
		BI_Refine(*C);

		if ((BI_Is_Zero(&A) == 0) | (BI_Is_Zero(&B) == 0)) // 피연산자 둘 중 하나라도 0이면 부호 결정하지 않고 return
			return SUCCESS;

		if ((sign_A ^ sign_B) == 1) // C의 부호 결정
			(*C)->sign = NEGATIVE;
		else
			(*C)->sign = NON_NEGATIVE;
	}
	else
	{
		printf("INVALID FLAG!!!\n");
		return ERROR;
	}
		

	return SUCCESS;
}

/**
* @brief Multiplication in Word
* @details

	@n [pseudo code]
	@n Input  : C, A, B

	@n 1 : A1, A0 <- A >> w/2, A mod 2^(w/2)
	@n 2 : A1, A0 <- A >> w/2, A mod 2^(w/2)
	@n 3 : MUL1, MUL0 <- A1 * B1, A0 * B0
	@n 4 : SUM0, SUM1 <- A1 * B0, A0 * B1
	@n 5 : SUM1 = SUM1 + SUM0 (mod W)
	@n 6 : if (SUM1 < SUM0) then
	@n 7 :		carry1 <- 1
	@n 8 : SUM2 <- SUM1 << (w/2)
	@n 9 : SUM1 <- SUM1 >> (w/2)
	@n 10: MUL0 <- MUL0 + SUM2 (mod W)
	@n 11: if (MUL0 < SUM2)
	@n 12:		carry0 <- 1
	@n 13: MUL1 <- MUL1 + SUM1 + carry0 + (carry1 << (w/2)) (mod W)
	@n 14: C = (00.. || MUL1 || MUL0 || ..00)

* @param word* C 단일 워드 곱셈 연산의 결과를 저장할 word 형 포인터 변수
* @param word* A 단일 워드 곱셈 연산의 곱하는 수인 word 형 포인터 변수
* @param word* B 단일 워드 곱셈 연산의 곱하는 수인 word 형 포인터 변수
* @return SUCCESS 성공 시
* @throws ERROR A, B, C가 미할당 시
*/
int MUL_Word(word* C, word* A, word* B) // 단일 워드 곱셈
{
	int carry0 = 0; // 워드간의 덧셈을 통해 생성되는 carry를 담을 int 형 변수 carry0
	int carry1 = 0; // 워드간의 덧셈을 통해 생성되는 carry를 담을 int 형 변수 carry1

	word sum0 = 0; // 워드간의 덧셈을 담을 word형 변수 sum0
	word sum1 = 0; // 워드간의 덧셈을 담을 word형 변수 sum1
	word sum2 = 0; // 워드간의 덧셈을 담을 word형 변수 sum2
	word mul0 = 0; // 단일 워드 곱셈 결과 중 하위 워드를 담을 word형 변수 mul0
	word mul1 = 0; // 단일 워드 곱셈 결과 중 상위 워드를 담을 word형 변수 mul1
	word A1 = 0; // A의 최상위비트부터 중간비트까지 담을 word형 변수 A1
	word B1 = 0; // B의 최상위비트부터 중간비트까지 담을 word형 변수 B1
	word A0 = 0; // A의 중간비트부터 최하위 비트까지 담을 word형 변수 A0
	word B0 = 0; // B의 중간비트부터 최하위 비트까지 담을 word형 변수 B0

	if ((C == NULL) | (B == NULL) | (A == NULL))
		return ERROR;

	A1 = ((*A) >> (WORD_BIT_LEN >> 1)); // A의 최상위비트부터 중간비트까지
	B1 = ((*B) >> (WORD_BIT_LEN >> 1)); // B의 최상위비트부터 중간비트까지
	A0 = (*A) & (((word)1 << (WORD_BIT_LEN >> 1)) - 1); // A의 중간비트부터 최하위 비트까지
	B0 = (*B) & (((word)1 << (WORD_BIT_LEN >> 1)) - 1); // B의 중간비트부터 최하위 비트까지

	mul1 = A1 * B1; //  [line 3] A1과 B1을 곱해 mul1에 대입
	mul0 = A0 * B0; //  [line 3] A0와 B0를 곱해 mul0에 대입
	sum0 = A1 * B0; //  [line 4] A1과 B0를 곱해 sum0에 대입
	sum1 = A0 * B1; //	[line 4] A0와 B1을 곱해 sum1에 대입
	sum1 += sum0; //	[line 5] sum1에 sum0를 덧셈
	
	if (sum1 < sum0) // [line 6] carry 발생했으면,
		carry1 = 1; // [line 7] carr1가 1이 되도록 대입
	
	sum2 = (sum1 & (((word)1 << (WORD_BIT_LEN >> 1)) - 1));  // sum1의 뒷부분(중간비트 ~ 최하위비트)을 만들어서 sum2에 대입
	sum2 = sum2 << (WORD_BIT_LEN >> 1); // [line 8] sum1의 뒷부분(중간비트 ~ 최하위비트)는 mul0와 더할 때 자리를 맞춰줘야하므로
	sum1 = sum1 >> (WORD_BIT_LEN >> 1); // [line 9] sum1의 앞부분(최상위비트 ~ 중간비트)을 만들어주기
	mul0 = sum2 + mul0; // [line 10] 곱셈 결과의 단일 워드 중 하위 워드 부분 덧셈 연산
	
	if (mul0 < sum2) // [line 11] 덧셈 연산에서 carry가 발생했으면,
		carry0 = 1; // [line 12] carry1 = 1
	
	mul1 = sum1 + mul1 + carry0 + ((word)carry1 << (WORD_BIT_LEN >> 1)); // [line 13] 곱셈 결과의 단일 워드 중 상위 워드 부분 덧셈 연산

	*C = mul0; // [line 14] 곱셈 결과 후의 단일 워드 중 하위에 대입
	*(C + 1) = mul1; // [line 14] 곱셈 결과 후의 단일 워드 중 상위에 대입

	return SUCCESS;
}

/**
* @brief Multliplication in Multi Word
* @details

	@n [pseudo code]
	@n Input  : C, A, B

	@n 1 : if A = 0 or B = 0 then
	@n 2 :		C <- 0;
	@n 3 : end if
	@n 4 : if A = 1
	@n 5 :		if Sign(A) is NON_NEGATIVE
	@n 6 :			return B
	@n 7 :		else
	@n 8 :			return -B
	@n 10: end if
	@n 11: if B = 1
	@n 12:		if sign(B) is NON_NEGATIVE
	@n 13:			return A
	@n 14:		else
	@n 15:			return -A
	@n 16: end if
	@n 17: for j = 0 to n - 1 do // MULC(|A|, |B|)
	@n 18:		for i = 0 to m - 1 do
	@n 19:			Temp <- A_{j} * B_{i}
	@n 20:			Temp <- Temp << w(i+j)
	@n 21:			C <- ADDC(C, Temp)
	@n 22:		end for
	@n 23: end for
	@n 24: Sign(C) = Sign(A) ^ Sign(B)

* @param bigint** C 다중 워드 곱셈 연산의 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint* A 다중 워드 곱셈 연산의 곱하는 수인 bigint 형 포인터 변수
* @param bigint* B 다중 워드 곱셈 연산의 곱하는 수인 bigint 형 포인터 변수
* @return SUCCESS 성공 시
* @throws
	ERROR
	ERROR A, B, C의 부호 가져오기 실패 시
	ERROR A, B, C의 word length가 양수가 아닌 경우
*/
int MUL_Multi(bigint** C, bigint* A, bigint* B)
{
	int i, j = 0;
	int size_a, size_b, size_c = 0;
	int sign_a, sign_b = 0;
	int ret = 0;

	sign_a = BI_Get_Sign(A);
	sign_b = BI_Get_Sign(B);

	if (sign_a == ERROR)
		return ERROR;
	if (sign_b == ERROR)
		return ERROR;

	if ((BI_Is_Zero(&A) == 0) | (BI_Is_Zero(&B) == 0)) // [line 1] 피연산자 중 하나라도 0이면,
	{
		BI_Set_Zero(C); // [line 2] 곱셈 결과는 0이므로, 연산 진행하지 않고 0 출력 후
		return SUCCESS; // return; 처리
	}

	if (BI_Is_One(&A) == 0) // [line 4]
	{
		BI_Assign(C, B); // C <- 1 * B 이므로 C에 B assign
		if (sign_a == 0) // [line 5] 1이 양수인 +1이면,
			(*C)->sign = sign_b; // [line 6] C의 부호가 B의 부호와 동일하게
		else // [line 7] 1이 음수인 -1이면, 
			(*C)->sign = !sign_b; // [line 8] C의 부호가 B의 부호와 반대로
		
		return SUCCESS;
	}

	if (BI_Is_One(&B) == 0) // [line 11]
	{
		BI_Assign(C, A); // C <- A * 1 이므로 C에 B assign
		if (sign_a == 0) // [line 12] 1이 양수인 +1이면,
			(*C)->sign = sign_a; // [line 13] C의 부호가 B의 부호와 동일하게
		else // [line 14] 1이 음수인 -1이면, 
			(*C)->sign = !sign_a; // [line 15] C의 부호가 B의 부호와 반대로
		
		return SUCCESS;
	}

	BI_Get_Word_Length(&size_a, &A); // A의 word 길이를 size_a에 대입
	BI_Get_Word_Length(&size_b, &B); // B의 word 길이를 size_b에 대입

	if ((size_a < 0) | (size_b < 0))
		return ERROR;

	bigint* Temp = NULL; // 단일 워드 곱셈 연산의 결과를 저장해 C와 덧셈 연산을 진행할 big integer형 포인터
	
	BI_Get_Word_Length(&size_c, C); // C의 word 길이를 size_r에 대입
	BI_New(&Temp, size_c); // C와 덧셈 연산을 진행해야하므로 C와 동일한 wordlen으로 생성

	for (i = 0; i < B->wordlen; i++) // [line 17]
	{
		for (j = 0; j < A->wordlen; j++) // [line 18]
		{
			ret = MUL_Word(&Temp->a[i + j], &A->a[j], &B->a[i]); // [line 19, 20] A의 단일워드와 B의 단일워드 연산 후 Temp의 단일 워드에 대입
			if (ret == -1)
			{
				BI_Delete(&Temp);
				return ERROR;
			}
				
			ret = ADDC_AAB(C, C, &Temp, 0); // [line 21] 단일워드 곱셈한 Temp와 C를 덧셈연산 진행
			if (ret == -1)
			{
				BI_Delete(&Temp);
				return ERROR;
			}
			
			Temp->a[i + j] = 0; // 곱셈 연산에 사용된 워드 부분 초기화
			Temp->a[i + j + 1] = 0; // 위와 동일
		}
	}
	
	BI_Delete(&Temp); // 할당된 Temp를 delete.
	
	(*C)->sign = A->sign ^ B->sign; // 결과값 C의 부호를 결정 [line 24]
	BI_Refine(*C); // Refine 시켜주기
	
	return SUCCESS;
}



/**
 * @brief Karatsuba Multiplication
 * @details

	 @n [pseudo code]
	 @n Input  : A, B
	 @n Output : C = A * B
	  @n 1 : procedure MUL_Karatsuba(A, B)
	  @n 2 :	if flag >= min(WordLen(A), WordLen(B)) then
	  @n 3 :		C <- MUL(A, B)
	  @n 4 :	end if
	  @n 5 :	l <- (max(WordLen(A), WordLen(B)) + 1) >> 1
	  @n 6 :	A1, A0 <- A >> lw, A mod 2^lw
	  @n 7 :	B1, B0 <- B >> lw, B mod 2^lw
	  @n 8 :	T1, T0 <- MUL_Karatsuba(A1, B1), MUL_Karatsuba(A0, B0)
	  @n 9 :	R <- (T1 << 2lw) + T0
	 @n 10 :	S1, S0 <- SUB(A0, A1), SUB(B1, B0)
	 @n 11 :	S <- (-1)^(Sign(S1)^Sign(S2)) * MUL_Karatsuba(|S1|, |S0|)
	 @n 12 :	S <- ADD(S, T1)
	 @n 13 :	S <- ADD(S, T0)
	 @n 14 :	S << lw
	 @n 15 :	R <- ADD(R, S)
	 @n 16 :	C <- R
	 @n 17 : end procedure

 * @param bigint** C 카라추바 곱셈 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint** A 카라추바 곱셈을 수행할 bigint 형 더블포인터 변수
 * @param bigint** B 카라추바 곱셈을 수행할 bigint 형 더블포인터 변수
 * @return SUCCESS
 * @throws
	ERROR A(B)의 word length 가져오기 실패 시
	ERROR S0(S1)의 부호 가져오기 실패 시
 */
int MUL_Karatsuba(bigint** C, bigint* A, bigint* B)
{
	int i = 0;
	int len, A_Len, B_Len;
	int S0_sign, S1_sign;
	int ret;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A의 워드열 길이
	BI_Get_Word_Length(&B_Len, &B); // B_Len = B의 워드열 길이

	// 예외 처리
	if (A_Len < 0)
		return ERROR;
	if (B_Len < 0)
		return ERROR;

	if (FLAG >= MIN(A_Len, B_Len)) // [line 2] A의 워드열의 길이와 B의 워드열의 길이 중 더 작은 것이 flag보다 작으면
	{
		ret = MUL_Multi(C, A, B); // [line 3] textbook multiplication 실행
		if (ret == ERROR)
			return ERROR;
		
		return 0;
	}

	len = (MAX(A_Len, B_Len) + 1) >> 1; // [line 5]

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

	BI_Right_Shift(A1, len * WORD_BIT_LEN); // [line 6] A1 = A >> len word (A를 len 워드만큼 오른쪽으로 이동)
	BI_Reduction(&A0, len * WORD_BIT_LEN); // [line 6] A0 = A mod (2^(len * wordlen)) (A를 len 워드만큼 modular 연산 수행)
	BI_Right_Shift(B1, len * WORD_BIT_LEN); // [line 7] B1 = A >> len word (A를 len 워드만큼 오른쪽으로 이동)
	BI_Reduction(&B0, len * WORD_BIT_LEN); // [line 7] B0 = A mod (2^(len * wordlen)) (A를 len 워드만큼 modular 연산 수행)

	BI_New(&T0, len * 2);
	BI_New(&T1, len * 2);
	BI_New(&S0, Compare_WordLen(B0, B1)); // S0 = B1 - B0이니까 B1과 B0 중 더 큰 워드 길이만큼 bigint 생성
	BI_New(&S1, Compare_WordLen(A0, A1)); // S1 = A0 - A1이니까 A0과 A1 중 더 큰 워드 길이만큼 bigint 생성
	BI_New(&R, len * 4);
	BI_New(&S, len * 2);

	ret = MUL_Karatsuba(&T1, A1, B1); // [line 8] T1 = A1 * B1
	if (ret == ERROR)
	{
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

		return ERROR;
	}
	
	ret = MUL_Karatsuba(&T0, A0, B0); // [line 8] T0 = A0 * B0
	if (ret == ERROR)
	{
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

		return ERROR;
	}

	BI_Refine(T1);
	BI_Refine(T0);

	bigint* T1_tmp = NULL;
	BI_Assign(&T1_tmp, T1);
	BI_Left_Shift(T1_tmp, 2 * WORD_BIT_LEN * len); // T1_tmp = T1을 2 * len 워드만큼 오른쪽으로 이동

	// [line 9] R = T1 || T0
	for (i = 0; i < T1->wordlen; i++)
		R->a[len * 2 + i] = T1_tmp->a[T1_tmp->wordlen - T1->wordlen + i];
	for (i = 0; i < T0->wordlen; i++)
		R->a[i] = T0->a[i];

	BI_Refine(R);

	ret = SUB(&S1, A0, A1); // [line 10] S1 = A0 - A1
	if (ret == ERROR)
	{
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

		BI_Delete(&T1_tmp);

		return ERROR;
	}

	ret = SUB(&S0, B1, B0); // [line 10] S0 = B1 - B0
	if (ret == ERROR)
	{
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

		BI_Delete(&T1_tmp);

		return ERROR;
	}


	S0_sign = BI_Get_Sign(S0);
	S1_sign = BI_Get_Sign(S1);

	// 예외 처리
	if ((S0_sign == ERROR) | (S1_sign == ERROR))
	{
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

		BI_Delete(&T1_tmp);

		return ERROR;
	}

	// S0, S1은 음수일 수 있으므로 절댓값 취해 주기
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

	ret = MUL_Karatsuba(&S, S1, S0); // [line 11] S = S1 * S0
	if (ret == ERROR)
	{
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

		BI_Delete(&T1_tmp);

		return ERROR;
	}

	BI_Refine(S);

	// S 부호 정해 주기
	if (S1_sign ^ S0_sign)
		S->sign = NEGATIVE;
	else
		S->sign = NON_NEGATIVE;

	BI_New(&ADD_result1, len * 2 + 1);
	BI_New(&ADD_result2, len * 2 + 1);

	ret = ADD(&ADD_result1, &S, &T1); // [line 12] ADD_result1 = S + T1
	if (ret == ERROR)
	{
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

		BI_Delete(&T1_tmp);

		BI_Delete(&ADD_result1);

		return ERROR;
	}
	
	ret = ADD(&ADD_result2, &ADD_result1, &T0); // [line 13] ADD_result2 = ADD_result1 + T0 = S + T1 + T0
	if (ret == ERROR)
	{
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

		BI_Delete(&T1_tmp);

		BI_Delete(&ADD_result1);
		BI_Delete(&ADD_result2);

		return ERROR;
	}
	
	BI_Left_Shift(ADD_result2, len * WORD_BIT_LEN); // [line 14] ADD_result2 << len 워드
	
	ret = ADD_AAB(C, &R, &ADD_result2); // [line 15] C = R + ADD_result2
	if (ret == ERROR)
	{
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

		BI_Delete(&T1_tmp);

		BI_Delete(&ADD_result1);
		BI_Delete(&ADD_result2);

		return ERROR;
	}

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

	return SUCCESS;
}

/**
 * @brief Squaring
 * @details
	@n flag에 따라 제곱 연산 수행(Karatsuba sqauring / textbook sqauring)
	@n Input  : A
	@n Output : C = A ^ 2
 * @param bigint** C 제곱 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* A 제곱 연산을 수행할 bigint 형 포인터 변수
 * @return SUCCESS
 * @throws
	ERROR A의 bigint 구조체 미할당 시
	ERROR A의 word length가 양수가 아닌 경우
	ERROR flag 미지정 시
 */
int Squaring(bigint** C, bigint* A)
{
	int A_Len = 0;
	int ret;

	// 예외 처리
	if (A == NULL)
		return ERROR;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A의 워드열 길이

	// 예외 처리
	if (A_Len <= 0)
		return ERROR;

	if (FLAG_SQU == TEXTBOOK) // textbook sqauring
	{
		ret = SQU(C, A);
		if (ret == ERROR)
			return ERROR;
		
		BI_Refine(*C);
	}
	else if (FLAG_SQU == KARATSUBA) // karatsuba sqauring
	{
		int C_Len = (A_Len + 1) >> 1;
		C_Len = 4 * C_Len + 1;

		BI_New(C, C_Len);

		ret = SQUC_Karatsuba(C, A);
		if (ret == ERROR)
			return ERROR;
		
		BI_Refine(*C);
	}
	else // 예외 처리
	{
		printf("INVALID FLAG!!!\n");
		return ERROR;
	}

	return SUCCESS;
}

/**
 * @brief Single Squaring
 * @details

	 @n [pseudo code]
	 @n Input  : A
	 @n Output : C = A ^ 2
	  @n 1 : A1, A0 <- |A| >> w/2, |A| mod 2^(w/2)
	  @n 2 : C1, C0 <- A1^2, A0^2
	  @n 3 : C <- (C1 << w) + C0
	  @n 4 : T <- A0 * A1
	  @n 5 : T <- T << (w/2 + 1)
	  @n 6 : C <- C + T

 * @param bigint* C 제곱 연산을 수행한 결과를 저장할 bigint 형 포인터 변수
 * @param bigint* A 제곱 연산을 수행할 bigint 형 포인터 변수

 * @return SUCCESS
 * @throws
	ERROR A의 부호 가져오기 실패 시
 */
int Single_Squaring(bigint* C, bigint* A)
{
	int A_Sign = 0;
	int ret;

	A_Sign = BI_Get_Sign(A);
	if (A_Sign == ERROR) // 예외 처리
		return ERROR;

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
	if (A_Sign == NEGATIVE)
	{
		BI_Flip_Sign(A1);
		BI_Flip_Sign(A0);
	}

	BI_Right_Shift(A1, (int)(WORD_BIT_LEN / 2)); // [line 1] A1 = |A| >> WORD_BIT_LEN / 2
	BI_Reduction(&A0, (int)(WORD_BIT_LEN / 2)); // [line 1] A0 = |A| mod (WORD_BIT_LEN / 2)

	C1->a[0] = A1->a[0] * A1->a[0]; // [line 2] C1 = A1^2
	C0->a[0] = A0->a[0] * A0->a[0]; // [line 2] C0 = A0^2

	// [line 3] C = (C1 << WORD_BIT_LEN) + C0
	C->a[1] = C1->a[0];
	C->a[0] = C0->a[0];

	ret = Multiplication(&T, A0, A1); // [line 4] T = A0 * A1
	if (ret == ERROR)
	{
		BI_Delete(&A1);
		BI_Delete(&A0);
		BI_Delete(&C1);
		BI_Delete(&C0);
		BI_Delete(&T);
		
		return ERROR;
	}

	BI_Left_Shift(T, (WORD_BIT_LEN / 2) + 1); // [line 5] T = T << ((WORD_BIT_LEN / 2) + 1)

	ret = ADD_AAB(&C, &C, &T); // [line 6] C = C + T
	if (ret == ERROR)
	{
		BI_Delete(&A1);
		BI_Delete(&A0);
		BI_Delete(&C1);
		BI_Delete(&C0);
		BI_Delete(&T);
		
		return ERROR;
	}	

	BI_Delete(&A1);
	BI_Delete(&A0);
	BI_Delete(&C1);
	BI_Delete(&C0);
	BI_Delete(&T);

	return SUCCESS;
}

/**
 * @brief Squaring Core
 * @details

	 @n [pseudo code]
	 @n Input  : A
	 @n Output : C = A ^ 2
	  @n 1 : C1, C2 <- 0
	  @n 2 : for j = 0 to WordLen(A) - 1 do
	  @n 3 :	T1 <- A[j] * A[j]
	  @n 4 :	T1 <- T1 << 2jw
	  @n 5 :	C1 <- T1 + C1
	  @n 6 :	 for i = j + 1 to WordLen(A) - 1 do
	  @n 7 :		T2 = A[j] * A[i]
	  @n 8 :		T2 <- T2 << (i + j)w
	  @n 9 :		C2 <- ADD(C2, T2)
	 @n 10 :	end for
	 @n 11 : end for
	@n  12 : C2 <- C2 << 1
	@n  13 : C <- ADD(C1, C2)

 * @param bigint** C 제곱 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* A 제곱 연산을 수행할 bigint 형 포인터 변수
 * @return SUCCESS
 * @throws
	ERROR A의 word length가 양수가 아닌 경우
	ERROR bigint 구조체 내부에 있는 배열 재할당 실패 시
 */
int SQUC(bigint** C, bigint* A)
{
	int i, j, k, m;
	int wordlen_A;
	int len, c2_len;
	int ret;

	BI_Get_Word_Length(&wordlen_A, &A);
	if (wordlen_A <= 0)
		return ERROR;

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

	BI_New(C, wordlen_A * 2 + 1);
	BI_New(&C1, 2 * wordlen_A); // [line 1] 크기만 크게 잡고 0으로 설정
	BI_New(&C2, 2 * wordlen_A); // [line 1] 크기만 크게 잡고 0으로 설정

	for (j = 0; j < wordlen_A; j++)
	{
		BI_New(&T1, 2);
		A_j->a[0] = A->a[j];
		
		ret = Single_Squaring(T1, A_j); // [line 3]
		if (ret == ERROR)
		{
			BI_Delete(&A_i);
			BI_Delete(&A_j);
			BI_Delete(&C1);
			BI_Delete(&C2);
			BI_Delete(&T1);

			return ERROR;
		}
			
		BI_Assign(&temp1, T1);
		BI_Left_Shift(temp1, (int)(2 * j * WORD_BIT_LEN)); // [line 4] T1 = T1 << ((2 * j) * WORD_BIT_LEN)

		// [line 5] C1 = T1 || C1
		if (j == 0)
		{
			for (k = 0; k < temp1->wordlen; k++)
				C1->a[k] = temp1->a[k]; // j = 0일 때는 C1 = T1 || C1 = T1 (C1이 처음에 0이니까)
		}
		else
		{
			for (k = 0; k < (temp1->wordlen - (2 * j)); k++)
			{
				C1->a[2 * j + k] = temp1->a[2 * j + k]; // 그 다음부터는 T1이 2 워드 단위로 왼쪽으로 시프트 연산한 결과이므로 2 * j번째부터 넣어 주기

			}
		}

		BI_Delete(&T1);
		BI_Delete(&temp1);

		for (i = j + 1; i < wordlen_A; i++) // [line 6]
		{
			BI_New(&T2, 2);
			A_i->a[0] = A->a[i];
			
			ret = MUL_Multi(&T2, A_j, A_i); // [line 7]
			if (ret == ERROR)
			{
				BI_Delete(&A_i);
				BI_Delete(&A_j);
				BI_Delete(&C1);
				BI_Delete(&C2);
				BI_Delete(&T2);

				return ERROR;
			}
				
			BI_Assign(&temp2, T2);
			BI_Left_Shift(temp2, (int)((i + j) * WORD_BIT_LEN)); // [line 8]

			c2_len = C2->wordlen;

			len = MAX(C2->wordlen, temp2->wordlen) + 1; // C2 = C2 + temp2를 하기 위해 C2와 temp2 중 더 긴 워드열의 길이 구하기
			C2->wordlen = len;
			temp = (word*)realloc(C2->a, sizeof(word) * len); // new_wordlen만큼 bigint 구조체 재할당
			
			if (temp != NULL)
				C2->a = temp;
			else
			{
				BI_Delete(&A_i);
				BI_Delete(&A_j);
				BI_Delete(&C1);
				BI_Delete(&C2);
				BI_Delete(&T2);
				BI_Delete(&temp2);

				return ERROR;
			}

			for (m = c2_len; m < len; m++)
				C2->a[m] = 0; // 늘어난 길이만큼 0으로 초기화

			ret = ADD_AAB(&C2, &C2, &temp2); // [line 9]
			if (ret == ERROR)
			{
				BI_Delete(&A_i);
				BI_Delete(&A_j);
				BI_Delete(&C1);
				BI_Delete(&C2);
				BI_Delete(&T2);
				BI_Delete(&temp2);

				return ERROR;
			}
				
			BI_Refine(C2);

			array_init(T2->a, T2->wordlen);
			BI_Delete(&temp2);
			BI_Delete(&T2);
		}
	}

	BI_Left_Shift(C2, 1); // [line 12] C2 = C2 << 1
	
	ret = ADD(C, &C1, &C2); // [line 13] C = C1 + C2
	if (ret == ERROR)
	{
		BI_Delete(&A_i);
		BI_Delete(&A_j);
		BI_Delete(&C1);
		BI_Delete(&C2);

		return ERROR;
	}
		
	BI_Delete(&C1);
	BI_Delete(&C2);
	BI_Delete(&A_i);
	BI_Delete(&A_j);

	return SUCCESS;
}

/**
 * @brief Squaring
 * @details

	 @n [pseudo code]
	 @n Input  : A
	 @n Output : C = A ^ 2
	 @n  1 : if A = 0 or A = 1 or A = -1 then
	 @n  2 :	C <- |A|
	 @n  3 : end if
	 @n  4 : C <- SQUC(A)

 * @param bigint** C 제곱 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* A 제곱 연산을 수행할 bigint 형 포인터 변수

 * @return SUCCESS
 * @throws
	ERROR A의 부호 가져오기 실패 시
 */
int SQU(bigint** C, bigint* A)
{
	int flag0; // A가 0인지 판별하는 flag
	int flag1; // A가 1인지 판별하는 flag
	int flag2 = -1; // A가 음수일 때 -1인지 판별하는 flag
	int sign;
	int ret;

	bigint* temp = NULL;

	BI_Assign(&temp, A);

	sign = BI_Get_Sign(A);
	if (sign == ERROR)
	{
		BI_Delete(&temp);
		return ERROR;
	}

	if (sign == NEGATIVE) // A의 부호가 음수인 경우
	{
		BI_Flip_Sign(temp); // 부호 바꾼 뒤
		flag2 = BI_Is_One(&temp); // 1인지 판별 (즉, -1인지 판별)
	}

	flag0 = BI_Is_Zero(&A); // A가 0인지 판별
	flag1 = BI_Is_One(&A); // A가 1인지 판별

	if (flag0 == 0) // [line 1] A = 0인 경우, A^2 = 0 return
	{
		BI_Assign(C, A); // [line 2]
		BI_Delete(&temp);

		return SUCCESS;
	}
	if (flag1 == 0) // [line 1] A = 1인 경우, A^2 = 1 return
	{
		BI_Assign(C, A); // [line 2]
		BI_Delete(&temp);

		return SUCCESS;
	}
	if (flag2 == 0) // [line 1] A = -1인 경우, A^2 = 1 return
	{
		BI_Assign(C, temp); // [line 2]
		BI_Delete(&temp);

		return SUCCESS;
	}

	BI_Delete(&temp);
	BI_New(C, A->wordlen * 2 + 1);
	
	ret = SQUC(C, A); // A = 0. -1, 1이 아닌 경우 SQUC 실행
	if (ret == ERROR)
		return ERROR;

	return SUCCESS;
}

/**
 * @brief Karatsuba Squaring
 * @details

	 @n [pseudo code]
	 @n Input  : A
	 @n Output : C = A ^ 2
	 @n  1 : procedure SQUC_Karatsuba(A, B)
	 @n  2 :	if flag >= WordLen(A) then
	 @n  3 :		C <- SQUC(A)
	  @n 4 :	end if
	  @n 5 :	l <- (WordLen(A) + 1) >> 1
	  @n 6 :	A1, A0 <- |A| >> lw, |A| mod 2^lw
	  @n 7 :	T1, T0 <- SQUC_Karatsuba(A1), SQUC_Karatsuba(A0)
	  @n 8 :	R <- (T1 << 2lw) + T0
	  @n 9 :	S1 <- MUL_Karatsuba(A1, A0)
	 @n 10 :	S <- S << (lw + 1)
	 @n 11 :	R <- ADDC(R, S)
	 @n 12 :	C <- R
	 @n 13 : end procedure

 * @param bigint** C 제곱 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* A 제곱 연산을 수행할 bigint 형 포인터 변수

 * @return SUCCESS
 * @throws
	ERROR A의 word length가 양수가 아닌 경우
 */
int SQUC_Karatsuba(bigint** C, bigint* A)
{
	int i;
	int len, A_Len;
	int ret;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A의 워드열 길이
	if (A_Len <= 0)
		return ERROR;

	if (FLAG >= A_Len) // [line 2] A의 워드열의 길이와 B의 워드열의 길이 중 더 작은 것이 flag보다 작으면
	{
		ret = SQUC(C, A); // [line 3] textbook squaring 실행
		if (ret == ERROR)
			return ERROR;

		return SUCCESS;
	}

	len = (A_Len + 1) >> 1; // [line 5]

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

	BI_Right_Shift(A1, len * (int)(WORD_BIT_LEN)); // [line 6] A1 = |A| >> WORD_BIT_LEN / 2
	BI_Reduction(&A0, len * (int)(WORD_BIT_LEN)); // [line 6] A0 = |A| mod (WORD_BIT_LEN / 2)

	ret = SQUC_Karatsuba(&T1, A1); // [line 7]
	if (ret == ERROR)
	{
		BI_Delete(&A0);
		BI_Delete(&A1);
		BI_Delete(&T0);
		BI_Delete(&T1);
		BI_Delete(&R);
		BI_Delete(&S);

		return ERROR;
	}
	
	ret = SQUC_Karatsuba(&T0, A0); // [line 7]
	if (ret == ERROR)
	{
		BI_Delete(&A0);
		BI_Delete(&A1);
		BI_Delete(&T0);
		BI_Delete(&T1);
		BI_Delete(&R);
		BI_Delete(&S);

		return ERROR;
	}

	BI_Refine(T1);
	BI_Refine(T0);

	bigint* T1_tmp = NULL;
	BI_Assign(&T1_tmp, T1);
	BI_Left_Shift(T1_tmp, 2 * WORD_BIT_LEN * len); // T1_tmp = T1을 2 * len 워드만큼 오른쪽으로 이동

	// [line 8] R = T1 || T0
	for (i = 0; i < T1->wordlen; i++)
		R->a[len * 2 + i] = T1_tmp->a[T1_tmp->wordlen - T1->wordlen + i];
	for (i = 0; i < T0->wordlen; i++)
		R->a[i] = T0->a[i];

	BI_Refine(R);

	ret = MUL_Karatsuba(&S, A1, A0); // [line 9] S = A1 * A0
	if (ret == ERROR)
	{
		BI_Delete(&A0);
		BI_Delete(&A1);
		BI_Delete(&T0);
		BI_Delete(&T1);
		BI_Delete(&R);
		BI_Delete(&S);
		BI_Delete(&T1_tmp);

		return ERROR;
	}

	BI_Refine(S);
	BI_Left_Shift(S, len * WORD_BIT_LEN + 1); // [line 10] S << len * WORD_BIT_LEN + 1 비트만큼 왼쪽으로 이동
	
	ret = ADDC(C, &R, &S, R->sign); // [line 11] C = R + S
	if (ret == ERROR)
	{
		BI_Delete(&A0);
		BI_Delete(&A1);
		BI_Delete(&T0);
		BI_Delete(&T1);
		BI_Delete(&R);
		BI_Delete(&S);
		BI_Delete(&T1_tmp);

		return ERROR;
	}

	BI_Delete(&A0);
	BI_Delete(&A1);
	BI_Delete(&T0);
	BI_Delete(&T1);
	BI_Delete(&R);
	BI_Delete(&S);
	BI_Delete(&T1_tmp);

	return SUCCESS;
}

/**
 * @brief Division
 * @details
	@n flag에 따라 나눗셈 연산 수행(binary long division / multi-precision long division)
	@n Input  : A, B
	@n Output : Q, R (A = B * Q + R)
 * @param bigint** Q 나눗셈 연산을 수행한 결과 중 몫을 저장할 bigint 형 더블포인터 변수
 * @param bigint** R 나눗셈 연산을 수행한 결과 중 나머지 저장할 bigint 형 더블포인터 변수
 * @param bigint* A 나눗셈 연산에서 나누려는 수인 bigint 형 포인터 변수
 * @param bigint* B 나누는 수인 bigint 형 포인터 변수
 * @return SUCCESS
 * @throws
	ERROR A(B)의 bigint 구조체 미할당 시
	ERROR flag 미지정 시
 */
int Division(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int size = 0;
	int ret;

	// 예외 처리
	if ((A == NULL) | (B == NULL))
		return ERROR;

	if (BI_Is_Zero(&B) == 0) // 나누는 수가 0 이므로
	{
		printf("INVALID: Cannot divide with 0\n");
		return ERROR;
	}

	if ((A->sign == NEGATIVE) | (B->sign == NEGATIVE)) // A, B > 0 의 조건을 만족하지 않을 때
	{
		printf("INVALID: Enter only positive values\n");
		return ERROR;
	}

	size = BI_Compare(&A, &B); // A와 B의 크기 비교

	if (size == ERROR)
		return ERROR;

	if (size == 2)
	{
		BI_Set_Zero(Q);
		BI_Assign(R, A);
		return SUCCESS;
	}

	if (FLAG_DIV == BINARY_LONG) // binary long division
	{
		ret = Binary_Long_Div(Q, R, A, B);
		if (ret == ERROR)
			return ERROR;
	}
	else if (FLAG_DIV == MULTI_PRE) // multi-precision long division
	{
		ret = DIV(Q, R, A, B);
		if (ret == ERROR)
			return ERROR;
	}
	else
	{
		printf("INVALID FLAG!\n");
		return ERROR;
	}

	return SUCCESS;
}


/**
* @brief Binary_Long_Division Algorithm
* @details
	@n [pseudo code]
	@n Input  : Q, R, A, B

	@n 1 : (Q, R) <- (0, 0)
	@n 2 : for j = n -1 downto 0 do
	@n 3 :		R <- 2 * R + a_{j}
	@n 4 :		if R >= B then
	@n 5 :			Q <- Q + 2^j
	@n 6 :			R <- R - B
	@n 7 :		end if
	@n 8 :	end for
	@n 9 :	return (Q, R)

* @param bigint** Q Binary Long Divsion 연산의 몫에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint* R Binary Long Divsion 연산의 나머지에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint* A Binary Long Divsion 연산의 나누려는 수인 bigint 형 포인터 변수
* @param bigint* B Binary Long Divsion 연산의 나누는 수인 bigint 형 포인터 변수
*/
int Binary_Long_Div(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int i = 0;
	int size = 0;
	int len = 0;
	int result = 0;
	int ret = 0;
	word temp = 0;

	bigint* T = NULL;
	bigint* U = NULL;

	BI_New(&T, 1);
	BI_Set_Zero(Q); // [line 1]
	BI_Set_Zero(R); // [line 1]

	BI_Get_Word_Length(&size, &A); // size == bigint A's word length.
	size = size * WORD_BIT_LEN; // 변수 size 는 A의 총 비트 길이
	
	for (i = size - 1; i >= 0; i--) // [line 2] 최상위 비트부터
	{
		len = (int)(i / WORD_BIT_LEN); // len에는 지금 조사하고 있는 비트의 워드 위치
		temp = A->a[len]; // 현재 연산 중인 워드의 값을 temp에 대입
		len = i % WORD_BIT_LEN; // 해당 워드에 몇 번째 비트인지
		temp = temp >> len; // 해당 비트를 BI_Left shift해서 첫번째 비트에 놓이도록
		temp = temp & 1; // 1과 & 해서 
		T->a[0] = temp; // 해당 비트의 값을 대입 // j_th_BI_Get_Bit_Length 바꿔서 하기

		BI_Left_Shift(*R, 1); // [line 3] 2R
		
		ret = ADD_DIV(R, R, &T); // [line 3] 2R + a{j}
		if (ret == -1)
		{
			BI_Delete(&T);
			return ERROR;
		}
		
		result = BI_Compare(&B, R); // R = B -> 0, R > B -> -1이므로, result 에는 0, -1이 들어가야한다. 
		if ((result == 0) | (result == 2)) // [line 3] R >= B인지 비교
		{
			len = (int)(i / WORD_BIT_LEN) + 1;
			BI_New(&U, len);

			U->a[len - 1] = (word)1 << (i % WORD_BIT_LEN); // make 2 ^ j
			
			ret = ADD_DIV(Q, Q, &U); // [line 5] Q <- Q + 2 ^ j
			if (ret == -1)
			{
				BI_Delete(&T);
				BI_Delete(&U);
				
				return ERROR;
			}
			
			ret = SUB(R, *R, B); // [line 6] R <- R - B 
			if (ret == -1)
			{
				BI_Delete(&T);
				BI_Delete(&U);
				
				return ERROR;
			}
			
			BI_Delete(&U); // 덧셈해준 빅넘버 U(2 ^ j)는 delete 해준다.
		}
	}
	
	BI_Refine(*Q); // Q를 refine 시켜주기
	BI_Delete(&T); // 할당한 빅넘버 T delete.
	
	return SUCCESS;
}

int ADDC_DIV(bigint** C, bigint** A, bigint** B, int sign)
{
	int A_Len;
	int B_Len;
	int i;
	int carry;

	A_Len = (*A)->wordlen;
	B_Len = (*B)->wordlen;

	word* temp;

	(*B)->wordlen = A_Len;
	temp = (word*)realloc((*B)->a, sizeof(word) * (*B)->wordlen);
	if (temp != NULL)
		(*B)->a = temp;

	memset(&(*B)->a[B_Len], 0x00, (A_Len - B_Len) * sizeof(word));

	carry = 0;

	for (i = 0; i < A_Len; i++)
		carry = ADD_ABc(C, A, B, carry, i);

	if (carry == 1)
		(*C)->a[A_Len] = 1;

	BI_Refine(*B);

	if (sign == 0)
		(*C)->sign = 0;
	else
		(*C)->sign = 1;

	return SUCCESS;
}

int ADD_DIV(bigint** C, bigint** A, bigint** B)
{
	int A_Len = 0;
	int B_Len = 0;
	int A_sign;
	int B_sign;
	int i;
	int ret = 0;

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

		return SUCCESS;
	}

	if (BI_Is_Zero(B) == 0) // B is zero
	{
		(*C)->sign = (*A)->sign;
		(*C)->wordlen = (*A)->wordlen;
		
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*A)->a[i];

		return SUCCESS;
	}

	if ((A_sign == NON_NEGATIVE) && (B_sign == NEGATIVE))
	{
		bigint* temp = NULL;
		BI_New(&temp, B_Len);
		BI_Assign(&temp, *B);

		BI_Flip_Sign(temp);
		ret = SUB(C, *A, temp); // SUB 함수
		if (ret == -1)
			return ERROR;

		BI_Delete(&temp);

		return SUCCESS;
	}

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
	{
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		BI_Assign(&temp, *A);

		BI_Flip_Sign(temp);
		ret = SUB(C, *B, temp); // SUB 함수
		if (ret == -1)
			return ERROR;

		BI_Delete(&temp);

		return SUCCESS;
	}

	// A, B가 동일한 부호일 때
	if (A_Len >= B_Len)
	{
		ret = ADDC_DIV(C, A, B, A_sign);
		if (ret == -1)
			return ERROR;
		
		return SUCCESS;
	}
	else
	{
		ret = ADDC_DIV(C, B, A, A_sign);
		if (ret == -1)
			return ERROR;
		
		return SUCCESS;
	}
}

/**
* @brief Long Division Algorithm (Multi-precision version)
* @details

	@n [pseudo code]
	@n Input  : Q, R, A, B

	@n 1 : if B <= 0 then
	@n 2 :		return INVAILD
	@n 3 : end if
	@n 4 : if A < B then
	@n 5 :		return (0, A)
	@n 6 : end if
	@n 7 : (Q, R) <- (0, 0)
	@n 8 : for n-1 downtown 0 to
	@n 9 :		R <- RW + A_{i}
	@n 10:		(Q_{i}, R) <- DIVC(R, B)
	@n 11: end for

* @param bigint** Q Multi Long Divsion 연산의 몫에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint* R Multi Long Divsion 연산의 나머지에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint* A Multi Long Divsion 연산의 나누려는 수인 bigint 형 포인터 변수
* @param bigint* B Multi Long Divsion 연산의 나누는 수인 bigint 형 포인터 변수
*/
int DIV(bigint** Q, bigint** R, bigint* A, bigint* B) // Long Division Algorithm(Multi-precision version)에서 DIV(A, B) fucntion.
{
	int i = 0; // for 반복문에서 사용할 변수 i.
	int len = 0; // 변수 len은 빅넘버 A의 size를 담을 변수
	int result = 0; // BI_Compare(&A, &B)의 결과를 담을 변수
	
	bigint* Temp = NULL; // A의 j번째 워드( A_{j} )를 담을 빅넘버 Temp 초기화
	bigint* Word = NULL; // Q의 i번째 워드( Q_{i} )를 담을 빅넘버 Word 초기화

	BI_Set_Zero(R); // 길이가 1이고 값이 0인 빅넘버 *R 생성
	BI_Get_Word_Length(&len, &A);
	BI_New(Q, len);
	
	if (B->sign & 1) // [line 1]
		return ERROR; // return INVALID.
	
	result = BI_Compare(&A, &B);
	if (result == 2) // [line 4] B > A 일 경우, Compare(&A, &B)의 return : 2
	{
		BI_Assign(R, A);
		return SUCCESS;
	}

	for (i = len - 1; i >= 0; i--) // line 8.
	{
		BI_Set_Zero(&Word); // 길이가 1이고 값이 0인 빅넘버 Word 생성. BI_New(&Word, 1)도 가능.
		BI_New(&Temp, 1); // A의 j번째 워드(A_{j})를 담는 빅넘버 Temp를 워드 길이가 1로 생성. 
		
		Temp->a[0] = A->a[i]; // Temp에 A_{j}를 대입.
		BI_Left_Shift(*R, WORD_BIT_LEN); // R을 왼쪽으로 W만큼 shift ==> R * W
		
		ADD_DIV(R, R, &Temp); // [line 9] R * W에 A_{j}를 덧셈 연산 수행. 
		DIVC(&Word, R, *R, B); //빅넘버 Word는 Q_{i}를 저장하는 변수. 
		BI_Delete(&Temp); // 반복문 내에서 생성한 빅넘버 Temp delete.
		
		(*Q)->a[i] = Word->a[0]; // [line 10] Q에 Q_{i}를 대입. 
		BI_Delete(&Word); // 빅넘버 Word delete
	}

	BI_Refine(*Q);

	return SUCCESS;
}

/*
* @breif Long Division Algorithm Core function (Multi-precision version)
* @details
	@n [pseudo code]
	@n Input  : Q, R, A, B

	@n 1 : if A < B then
	@n 2 :		return (0, A)
	@n 3 :	end if
	@n 4 :	Compute k∈Z+ such that 2^k B_{m-1}∈[2^(w-1), 2^w)
	@n 5 :	A', B' <- 2^k * A, 2^k * B
	@n 6 :	Q', R' <- DIVCC(A', B')
	@n 7 : Q, R <- Q', 2^(-k) * R'
	@n 8 : return (Q, R)

* @param bigint** Q DIV( )의 몫에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint* R DIV( )의 나머지에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint* A DIV( )의 나누려는 수인 bigint 형 포인터 변수
* @param bigint* B DIV( )의 나누는 수인 bigint 형 포인터 변수

*/
int DIVC(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int result = 0; // A, B 대소비교값을 담을 변수 result
	int k = 0; // line 4와 관련된 변수 k
	int len = 0; // B의 wordlen을 저장해줄 변수 len
	
	bigint* AP = NULL; // A를 BI_Left shift 해서 보관할 빅넘버 A'(AP, A Prime)
	bigint* BP = NULL; // B를 BI_Left shift 해서 보관할 빅넘버 B'(BP, B Prime)

	result = BI_Compare(&A, &B);
	if (result == 2) // A < B 보다 클 때
		return SUCCESS;

	BI_Assign(&AP, A); // 빅넘버 A' 에 빅넘버 A assing
	BI_Assign(&BP, B); // 빅넘버 B' 에 빅넘버 B assing

	// line 4. Compute k.
	BI_Get_Word_Length(&len, &B);
	bigint* comp = NULL; // k를 compute하기 위한 빅넘버 comp 선언 및 초기화
	BI_New(&comp, 1); // 빅넘버 comp의 길이 1로 생성
	comp->a[0] = B->a[len - 1]; //comp에 B_{m - 1} 대입

	while (1)
	{
		BI_Left_Shift(comp, 1); // comp를 BI_Left shift ==> comp * 2
		k++; // comp * 2^k 하도록.
		
		if (comp->wordlen == 2) // 2^w을 넘어가면,
		{
			k--; // 넘어갔으니 하나 줄여주고
			break; // break;
		}
	}

	BI_Left_Shift(AP, k); // AP = AP << k 
	BI_Left_Shift(BP, k); // BP = BP << k //line 5. 
	DIVCC(Q, R, AP, BP); // [line 6]
	BI_Right_Shift(*R, k); // [line 7] (Q' == Q)
	
	BI_Delete(&AP); // 선언해준 빅넘버 AP delete.
	BI_Delete(&BP); // 선언해준 빅넘버 BP delete.
	BI_Delete(&comp); // 선언해준 빅넘버 comp delete.

	return SUCCESS;
}

/*
* @brief Long Division Algorithm Core's Core function (Multi-precision version)
* @details
	@n [pseudo code]
	@n Input  : Q, R, A, B

	@n 1 : if n == m then    <-- DIVCC_n_m( )
	@n 2 :		Q <- Lower Bound(A_{m-1} / B_{m-1})
	@n 3 : end if
	@n 4 : if n == m + 1 then <-- DIVCC_n_m1( )
	@n 5 :		if A_{m} = B_{m-1} then
	@n 6 :			Q <- W - 1
	@n 7 :		else
	@n 8 :			Q <- Lower Bound(A_{m} * W + A_{m-1} / B_{m-1})
	@n 9 :		end if
	@n 10:	end if
	@n 11: R <- A - BQ
	@n 12:	while R < 0 do
	@n 13:		(Q, R) <- (Q - 1, R + B)
	@n 14:	end while
	@n 15:	return (Q, R)

* @param bigint** Q DIVC( )의 몫에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint* R DIVC( )의 나머지에 대한 결과를 저장할 bigint 형 더블 포인터 변수
* @param bigint* A DIVC( )의 나누려는 수인 bigint 형 포인터 변수
* @param bigint* B DIVC( )의 나누는 수인 bigint 형 포인터 변수
*/
int DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B) // 7.2.3 DIVCC(A, B)
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
		DIVCC_n_m(Q, A, B, m); // [line 2]

	else if (n == m + 1) // [line 4] DIVCC 에서 if(n == m + 1) 일 때 
	{
		if (A_msw == B_msw) // line 5.
			(*Q)->a[0] = word_mask; // Q에 W-1을 대입해야하므로 word_mask 대입.
		else // [line 7]
			DIVCC_n_m1(Q, A, B, m); // [line 8]
	}

	BI_New(&BQ, n + m); // 빅넘버 B와 빅넘버 Q를 곱한 값을 저장할 빅넘버 BQ를 B와 Q의 곱셈연산이 가능한 길이인 n + m 로 생성.
	Multiplication(&BQ, *Q, B); // B와 Q를 곱셈 연산해 빅넘버 BQ에 대입
	SUB(R, A, BQ);// R = A - B * Q // [line 11]

	while ((*R)->sign & 1) // [line 12] R의 부호가 음수일 때 1 & 1 = 1  ( Q, Q - 1, Q - 2)
	{
		SUB(Q, *Q, one); // Q = Q - 1.
		ADD_AAB(R, R, &B); // R = R + B. 1st arg, 2nd arg가 같으므로 ADD_AAB 함수 사용
	}

	BI_Delete(&one); // 선언해준 빅넘버 one delete.
	BI_Delete(&BQ); // 선언해준 빅넘버 BQ delete.

	return SUCCESS;
}

/*
* @brief Long Division Algorithm Core's Core's condition (if n == m) (Multi-precision version)
* @details

	@n [pseudo code]
	@n Input  : Q, A, B, m

	@n 1 : Q <- A_{m-1} / B_{m-1}
	@n 2 : Q <- LowerBound(Q)

* @param bigint** Q DIVCC()에서 Q hat인 bigint 형 더블 포인터 변수
* @param bigint* A DIVCC()에서 나눠지는 수인 A의 bigint 형 포인터 변수
* @param bigint* B DIVCC()에서 나누려는 수인 B의 bigint 형 포인터 변수
* @param int m DIVCC()에서 B의 wordlen을 나타내는 int형 변수
*/
int DIVCC_n_m(bigint** Q, bigint* A, bigint* B, int m) // DIVCC 에서 if(n == m) 일 때
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

	return SUCCESS;
}

/*
	Long Division Algorithm Core's Core's condition (if n == m + 1) (Multi-precision version)

	[pseudo code]
	Input  : Q, A, B, m

	1 : Q <- A_{m} * W + A_{m-1} / B_{m-1}
	2 : Q <- LowerBound(Q)

* @param bigint** Q DIVCC()에서 Q hat인 bigint 형 더블 포인터 변수
* @param bigint* A DIVCC()에서 나눠지는 수인 A의 bigint 형 포인터 변수
* @param bigint* B DIVCC()에서 나누려는 수인 B의 bigint 형 포인터 변수
* @param int m DIVCC()에서 B의 wordlen을 나타내는 int형 변수
*/
int DIVCC_n_m1(bigint** Q, bigint* A, bigint* B, int m) // DIVCC 에서 if(n == m + 1) 일 때
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

	return SUCCESS;
}

/**
 * @brief Modular exponentiation of multiplication
 * @details
	@n flag에 따라 모듈러 지수승 연산 수행(BI_Left to BI_Right / BI_Right to BI_Left / Montgomery Ladder)
	@n Input  : X, N, M
	@n Output : T = X ^ N (mod M)
 * @param bigint** T 모듈러 지수승 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* X 모듈러 지수승 연산을 수행할 bigint 형 포인터 변수
 * @param bigint* N 모듈러 지수승 연산에서의 지수를 가리키는 bigint 형 포인터 변수
 * @param bigint* M 모듈러 지수승 연산에서의 모듈러를 수행할 bigint 형 포인터 변수
 * @return SUCCESS
 * @throws
	ERROR X(N, M) bigint 구조체 미할당 시
	ERROR flag 미지정 시
	ERROR 피연산자가 음수인 경우
	ERROR 지수가 음수인 경우
	ERROR 모듈러 취하는 수가 양수가 아닌 경우
 */
int Modular_Exponentiation_MUL(bigint** T, bigint* X, bigint* N, bigint* M)
{
	if ((X == NULL) | (N == NULL) | (M == NULL))
		return ERROR;

	if ((N->sign == NEGATIVE) | (X->sign == NEGATIVE) | (M->sign == NEGATIVE))
	{
		printf("INVALID: Enter only positive values\n");
		return ERROR;
	}

	if (BI_Is_Zero(&M) == 0) // mod 0일 때
	{
		printf("INVALID: Cannot divide with 0\n");
		return ERROR;
	}

	if (BI_Is_One(&M) == 0) // mod 1 일 때
	{
		BI_Set_Zero(T);
		return SUCCESS;
	}

	if (BI_Is_Zero(&X) == 0) // T <- 0 ^ N (mod M) = 0
	{
		BI_Set_Zero(T);
		return SUCCESS;
	}

	if (BI_Is_One(&X) == 0) // T <- 1 ^ N (mod M) = 1
	{
		BI_Set_One(T);
		return SUCCESS;
	}

	if (BI_Is_Zero(&N) == 0) // T <- A ^ 0 (mod M) = 1
	{
		BI_Set_One(T);
		return SUCCESS;
	}

	if (FLAG_EXP == LTOR) // BI_Left to BI_Right
		MOD_EXP_LR_MUL(T, X, N, M);

	else if (FLAG_EXP == RTOL) // BI_Right to BI_Left
		MOD_EXP_RL_MUL(T, X, N, M);

	else if (FLAG_EXP == MONTGOMERY) // Montgomery Ladder
	{
		MOD_EXP_Montgomery_MUL(T, X, N, M);
		BI_Refine(*T);
	}

	else
		return ERROR;

	return SUCCESS;
}

/**
 * @brief Modular exponentiation of addition
 * @details
	@n flag에 따라 모듈러 exponentiation 덧셈 연산 수행(BI_Left to BI_Right / BI_Right to BI_Left / Montgomery Ladder)
	@n Input  : X, N, M
	@n Output : T = X * N (mod M)
 * @param bigint** T 모듈러 exponentiation 덧셈 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* X 모듈러 exponentiation 덧셈 연산을 수행할 bigint 형 포인터 변수
 * @param bigint* N 모듈러 exponentiation 덧셈 연산에서의 지수(덧셈할 횟수)를 가리키는 bigint 형 포인터 변수
 * @param bigint* M 모듈러 exponentiation 덧셈 연산에서의 모듈러를 수행할 bigint 형 포인터 변수
 * @return SUCCESS
 * @throws
	ERROR flag 미지정 시
	ERROR X(N, M) bigint 구조체 미할당 시
	ERROR 피연산자가 음수인 경우
	ERROR 덧셈하는 횟수가 음수인 경우 (N < 0)
	ERROR 모듈러 취하는 수가 양수가 아닌 경우
 */
int Modular_Exponentiation_ADD(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int ret;

	if ((X == NULL) | (N == NULL) | (M == NULL))
		return ERROR;

	if ((N->sign == NEGATIVE) | (X->sign == NEGATIVE) | (M->sign == NEGATIVE))
	{
		printf("INVALID: Enter only positive values\n");
		return ERROR;
	}

	if (BI_Is_Zero(&M) == 0) // mod 0일 때
	{
		printf("INVALID: Cannot divide with 0\n");
		return ERROR;
	}

	if (BI_Is_One(&M) == 0) // mod 1 일 때
	{
		BI_Set_Zero(T);
		return SUCCESS;
	}

	if ((BI_Is_Zero(&N) == 0) | (BI_Is_Zero(&X) == 0)) // T <- 0 * N (mod M) = X * 0 (mod M) = 0
	{
		BI_Set_Zero(T);
		return SUCCESS;
	}

	if (FLAG_EXP == LTOR) // BI_Left to BI_Right
	{
		ret = MOD_EXP_LR_ADD(T, X, N, M);
		if (ret == ERROR)
			return ERROR;
	}
	else if (FLAG_EXP == RTOL) // BI_Right to BI_Left
	{
		ret = MOD_EXP_RL_ADD(T, X, N, M);
		if (ret == ERROR)
			return ERROR;
	}
	else if (FLAG_EXP == MONTGOMERY) // Montgomery Ladder
	{
		ret = MOD_EXP_Montgomery_ADD(T, X, N, M);
		if (ret == ERROR)
			return ERROR;

		BI_Refine(*T);
	}
	else
		return ERROR;

	return SUCCESS;
}

/**
 * @brief Exponentiation of multiplication used Montgomery Ladder
 * @details
	@n [pseudo code]
	@n Input  : X, N
	@n Output : T = X ^ N

	1 : t0, t1 <- 1, X
	2 : for i <- BitLen(N) - 1 downto 0 do
	3 :		t(1-(N_i)) <- t0 * t1
	4 :		t(N_i) <- t(N_i) ^ 2
	5 :	end for
	6  T <- t0

 * @param bigint** T 지수승 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* X 지수승 연산을 수행할 bigint 형 포인터 변수
 * @param bigint* N 지수승 연산에서의 지수를 가리키는 bigint 형 포인터 변수
 * @return SUCCESS
 * @throws
	ERROR 지수가 음수인 경우
	ERROR N의 i번째 비트가 1도 0도 아닌 경우
 */
int EXP_Montgomery_MUL(bigint** T, bigint* X, bigint* N)
{
	int i, bit;
	int N_bit_len;
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0를 저장할 임시 변수
	bigint* temp1 = NULL; // t1을 저장할 임시 변수

	if (N->sign == NEGATIVE) // 예외 처리 (지수는 양수만)
		return ERROR;

	BI_Get_Bit_Length(&N_bit_len, N); // N의 비트열 길이 -> N_bit_len

	BI_Set_One(&t0); // [line 1] t0 = 1
	BI_Assign(&t1, X); // [line 1] t1 = A

	for (i = N_bit_len - 1; i >= 0; i--) // [line 2]
	{
		bit = BI_Get_j_th_Bit_of_BI(i, N); // N의 i번째 비트가 1인지 0인지 판별

		if (bit == 1) // N의 i번째 비트가 1일 경우
		{
			// [line 3]
			ret = Multiplication(&temp0, t0, t1); // temp0 = t0 * t1
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR;
			}
			
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 갱신)
			BI_Delete(&temp0);

			// [line 4]
			ret = Squaring(&temp1, t1); // temp1 = t1^2
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR;
			}
			
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 갱신)
			BI_Delete(&temp1);

		}

		else if (bit == 0) // N의 i번째 비트가 0일 경우
		{
			// [line 3]
			ret = Multiplication(&temp1, t0, t1); // temp1 = t0 * t1
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR;
			}
			
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 갱신)
			BI_Delete(&temp1);

			// [line 4]
			ret = Squaring(&temp0, t0); // temp0 = t0^2
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR;
			}
			
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 갱신)
			BI_Delete(&temp0);
		}

		else // N의 i번째 비트가 1도 0도 아닌 경우
		{
			BI_Delete(&t0);
			BI_Delete(&t1);

			return ERROR; // 예외 처리
		}
	}

	BI_Assign(T, t0); // 마지막 t0 값 T에 할당

	BI_Delete(&t0);
	BI_Delete(&t1);

	return SUCCESS;
}

/**
 * @brief Exponentiation of addition used Montgomery Ladder
 * @details
	@n [pseudo code]
	@n Input  : X, N
	@n Output : T = X ^ N

	@n 1 : t0, t1 <- 0, X
	@n 2 : for i <- BitLen(N) - 1 downto 0 do
	@n 3 :		t(1-(N_i)) <- t0 + t1
	@n 4 :		t(N_i) <- 2 * t(N_i)
	@n 5 :	end for
	@n 6  T <- t0

 * @param bigint** T exponentiation 덧셈 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* X exponentiation 덧셈 연산을 수행할 bigint 형 포인터 변수
 * @param bigint* N exponentiation 덧셈 연산에서의 지수(덧셈할 횟수)를 가리키는 bigint 형 포인터 변수
 * @return SUCCESS
 * @throws
	ERROR 더하는 횟수가 음수인 경우
	ERROR N의 i번째 비트가 1도 0도 아닌 경우
 */
int EXP_Montgomery_ADD(bigint** T, bigint* X, bigint* N)
{
	int i, bit;
	int N_bit_len;
	int t0_len = 0;
	int t1_len = 0;
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0를 저장할 임시 변수
	bigint* temp1 = NULL; // t1을 저장할 임시 변수

	if (N->sign == NEGATIVE) // 예외 처리 (양수만)
		return ERROR;

	BI_Get_Bit_Length(&N_bit_len, N); // N의 비트열 길이 -> N_bit_len

	// [line 1]
	BI_Set_Zero(&t0); // t0 = 0
	BI_Assign(&t1, X); // t1 = A

	for (i = N_bit_len - 1; i >= 0; i--) // [line 2]
	{
		bit = BI_Get_j_th_Bit_of_BI(i, N); // N의 i번째 비트가 1인지 0인지 판별

		if (bit == 1) // N의 i번째 비트가 1일 경우
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 + t1을 저장하기 위한 bigint 생성

			// [line 3]
			ret = ADD(&temp0, &t0, &t1); // temp0 = t0 + t1
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);
				BI_Delete(&temp0);

				return ERROR; // 예외 처리
			}
			
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 갱신)
			BI_Delete(&temp0);

			BI_Left_Shift(t1, 1); // [line 4] t1 = t1 << 2
		}
		else if (bit == 0) // N의 i번째 비트가 0일 경우
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

			BI_New(&temp1, MAX(t0_len, t1_len) + 1);  // temp1(t1) = t0 + t1을 저장하기 위한 bigint 생성

			// [line 3]
			ret = ADD(&temp1, &t0, &t1); // temp1 = t0 + t1
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);
				BI_Delete(&temp1);

				return ERROR; // 예외 처리
			}
			
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 갱신)
			BI_Delete(&temp1);

			BI_Left_Shift(t0, 1); // [line 4] t0 = t0 << 2
		}
		else // N의 i번째 비트가 1도 0도 아닌 경우
		{
			BI_Delete(&t0);
			BI_Delete(&t1);

			return ERROR; // 예외 처리
		}
	}

	BI_Assign(T, t0); // 마지막 t0 값 T에 할당

	BI_Delete(&t0);
	BI_Delete(&t1);

	return SUCCESS;
}

/**
 * @brief Modular Exponentiation of multiplication used Montgomery Ladder
 * @details
	@n [pseudo code]
	@n Input  : X, N
	@n Output : T = X ^ N (mod M)

	@n 1 : t0, t1 <- 1, X
	@n 2 : for i <- BitLen(N) - 1 downto 0 do
	@n 3 :		t(1-(N_i)) <- t0 * t1 (mod M)
	@n 4 :		t(N_i) <- t(N_i) ^ 2 (mod M)
	@n 5 :	end for
	@n 6  T <- t0

 * @param bigint** T 모듈러 지수승 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* X 모듈러 지수승 연산을 수행할 bigint 형 포인터 변수
 * @param bigint* N 모듈러 지수승 연산에서의 지수를 가리키는 bigint 형 포인터 변수
 * @param bigint* M 모듈러 지수승 연산에서의 모듈러를 수행할 bigint 형 포인터 변수
 * @return SUCCESS 성공 시
 * @throws
	ERROR 지수가 음수인 경우
	ERROR 밑이 음수인 경우
	ERROR 모듈러 취하는 수가 음수인 경우
	ERROR N의 i번째 비트가 1도 0도 아닌 경우
 */
int MOD_EXP_Montgomery_MUL(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i, bit;
	int N_bit_len;
	int t0_len = 0;
	int t1_len = 0;
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0를 저장할 임시 변수
	bigint* temp1 = NULL; // t1을 저장할 임시 변수

	bigint* Q = NULL;
	bigint* R = NULL;

	if (N->sign == NEGATIVE) // 예외 처리
		return ERROR;
	if (M->sign == NEGATIVE)
		return ERROR;
	if (X->sign == NEGATIVE)
		return ERROR;

	BI_Get_Bit_Length(&N_bit_len, N); // N의 비트열 길이 -> N_bit_len

	// [line 1]
	BI_Set_One(&t0); // t0 = 1
	BI_Assign(&t1, X); // t1 = X

	for (i = N_bit_len - 1; i >= 0; i--) // [line 2]
	{
		bit = BI_Get_j_th_Bit_of_BI(i, N); // N의 i번째 비트가 1인지 0인지 판별

		if (bit == 1) // N의 i번째 비트가 1일 경우
		{
			// [line 3]
			ret = Multiplication(&temp0, t0, t1); // temp0 = t0 * t1
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}
			
			ret = Division(&Q, &R, temp0, M); // R = temp0 (mod M)
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);
				BI_Delete(&temp0);

				return ERROR; // 예외 처리
			}
			
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 갱신)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);

			// [line 4]
			ret = Squaring(&temp1, t1); // temp1 = t1^2
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}
			
			ret = Division(&Q, &R, temp1, M); // R = temp1 (mod M)
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);
				BI_Delete(&temp1);

				return ERROR; // 예외 처리
			}
			
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

			// [line 3]
			ret = Multiplication(&temp1, t0, t1); // temp1 = t0 * t1
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}

			ret = Division(&Q, &R, temp1, M); // R = temp1 (mod M)
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);
				BI_Delete(&temp1);

				return ERROR; // 예외 처리
			}

			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 갱신)
			BI_Delete(&temp1);
			BI_Delete(&Q);
			BI_Delete(&R);

			// [line 4]
			ret = Squaring(&temp0, t0); // temp0 = t0^2
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}

			ret = Division(&Q, &R, temp0, M); // R = temp0 (mod M)
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);
				BI_Delete(&temp0);

				return ERROR; // 예외 처리
			}

			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 갱신)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);
		}
		else // N의 i번째 비트가 1도 0도 아닌 경우
		{
			BI_Delete(&t0);
			BI_Delete(&t1);

			return ERROR; // 예외 처리
		}
	}

	BI_Assign(T, t0); // 마지막 t0 값 T에 할당

	BI_Delete(&t0);
	BI_Delete(&t1);

	return SUCCESS;
}


/**
 * @brief Modular Exponentiation of addition used Montgomery Ladder
 * @details
	@n [pseudo code]
	@n Input  : X, N
	@n Output : T = X ^ N (mod M)

	@n 1 : t0, t1 <- 0, X
	@n 2 : for i <- BitLen(N) - 1 downto 0 do
	@n 3 :		t(1-(N_i)) <- t0 + t1 (mod M)
	@n 4 :		t(N_i) <- 2 * t(N_i) (mod M)
	@n 5 :	end for
	@n 6  T <- t0

 * @param bigint** T 모듈러 지수 덧셈 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
 * @param bigint* X 모듈러 지수 덧셈 연산을 수행할 bigint 형 포인터 변수
 * @param bigint* N 모듈러 지수 덧셈 연산에서의 지수(덧셈할 횟수)를 가리키는 bigint 형 포인터 변수
 * @param bigint* M 모듈러 지수 연산에서의 모듈러를 수행할 bigint 형 포인터 변수
 * @return SUCCESS 성공 시
 * @throws
	ERROR 더하는 횟수가 음수인 경우
	ERROR 더하는 대상이 음수인 경우
	ERROR 모듈러 취하는 수가 음수인 경우
	ERROR N의 i번째 비트가 1도 0도 아닌 경우
 */
int MOD_EXP_Montgomery_ADD(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i, bit;
	int N_bit_len;
	int t0_len = 0;
	int t1_len = 0;
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0를 저장할 임시 변수
	bigint* temp1 = NULL; // t1을 저장할 임시 변수

	bigint* Q = NULL;
	bigint* R = NULL;

	if (N->sign == NEGATIVE) // 예외 처리
		return ERROR;
	if (M->sign == NEGATIVE)
		return ERROR;
	if (X->sign == NEGATIVE)
		return ERROR;

	BI_Get_Bit_Length(&N_bit_len, N); // N의 비트열 길이 -> N_bit_len

	// [line 1]
	BI_Set_Zero(&t0); // t0 = 0
	BI_Assign(&t1, X); // t1 = X

	for (i = N_bit_len - 1; i >= 0; i--) // [line 2]
	{
		bit = BI_Get_j_th_Bit_of_BI(i, N); // N의 i번째 비트가 1인지 0인지 판별

		if (bit == 1) // N의 i번째 비트가 1일 경우
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 + t1을 저장하기 위한 bigint 생성

			// [line 3]
			ret = ADD(&temp0, &t0, &t1); // temp0 = t0 + t1
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}
			
			ret = Division(&Q, &R, temp0, M); // R = temp0 (mod M)
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);
				BI_Delete(&temp0);

				return ERROR; // 예외 처리
			}
			
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 갱신)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);

			// [line 4]
			BI_Left_Shift(t1, 1); // t1 = t1 << 2
			ret = Division(&Q, &R, t1, M); // t1 = t1 (mod M)
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}
			
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

			// [line 3]
			ret = ADD(&temp1, &t0, &t1); // temp1 = t0 + t1
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}
			
			ret = Division(&Q, &R, temp1, M); // R = temp1 (mod M)
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);
				BI_Delete(&temp1);

				return ERROR; // 예외 처리
			}
			
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 갱신)
			BI_Delete(&temp1);
			BI_Delete(&Q);
			BI_Delete(&R);

			// [line 4]
			BI_Left_Shift(t0, 1); // t0 = t0 << 2
			ret = Division(&Q, &R, t0, M); // t0 = t0 (mod M)
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}
			
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 갱신)
			BI_Delete(&Q);
			BI_Delete(&R);
		}
		else // N의 i번째 비트가 1도 0도 아닌 경우
		{
			BI_Delete(&t0);
			BI_Delete(&t1);

			return ERROR; // 예외 처리
		}
	}

	BI_Assign(T, t0); // 마지막 t0 값 T에 할당

	BI_Delete(&t0);
	BI_Delete(&t1);

	return SUCCESS;
}

/**
* @brief [Exponentiation] BI_Left to BI_Right MUL function
* @details

	@n [pseudo code]
	@n Input  : T, X, N, M

	@n 1 : t <- 1
	@n 2 : for i <- l - 1 downto 0 do
	@n 3 :		t <- t^2 ( ^ 2 : SQU )
	@n 4 :		t <- t * X^(N_{i}) ( * : MUL_Multi() )
	@n 5 :	end for
	@n 6 : T <- t

* @param bigint** T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param bigint* X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param bigint* N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
* @return SUCCESS 성공 시
*/
int EXP_LR_MUL(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // 반복문에 사용될 int형 변수 i
	int len = 0; // 지수승 N의 비트 길이를 담을 int형 변수 len
	int result = 0; // 해당 비트 값의 결과를 return 받는 int형 변수 result
	int x_len = 0; // X의 워드열 길이를 대입할 int형 변수 x_len
	int t0_len = 0; // t0의 워드열 길이를 대입할 int형 변수 t0_len
	int ret;

	bigint* t0 = NULL;
	bigint* temp1 = NULL;

	BI_Set_One(&t0);  // [line 1] t0를 1로 선언

	BI_Get_Bit_Length(&len, N);  // 지수승(N)에 대한 비트 길이 -> len 대입
	BI_Get_Word_Length(&x_len, &X);

	for (i = len - 1; i >= 0; i--) // [line 2]
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1의 워드열 길이 -> t1_len
		
		ret = Squaring(&temp1, t0); // [line 3] temp1 = t0^2 연산
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			return ERROR; // 예외 처리
		}
		
		BI_Delete(&t0); // t0를 Delete
		result = BI_Get_j_th_Bit_of_BI(i, N); // 반복문에서 사용 중인 변수 i를 이용해 N의 해당 비트가 1인지 0인지 조사

		if (result == 1) // 해당 비트가 1이면,
		{
			ret = Multiplication(&t0, temp1, X); // [line 4] t1^2 * X (곱셈)연산 후 temp0에 대입
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&temp1);

				return ERROR; // 예외 처리
			}
		}
		else // 해당 비트가 0이면, 곱셈을 진행하지 않아도 되므로
			BI_Assign(&t0, temp1); // [line 4] temp0에 t0 assign --> 반복문에서 t0로 연산을 진행할 수 있도록

		BI_Delete(&temp1); // temp1를 delete.
	} // [line 5]

	BI_Assign(T, t0); // [line 6] 반복문이 끝난 결과인 t0를 T에 assign
	BI_Delete(&t0); // t0를 delete.

	return SUCCESS;
}

/**
* @brief [Exponentiation] BI_Left to BI_Right ADD function
* @details

	@n [pseudo code]
	@n Input  : T, X, N, M

	@n 1 : t <- 0
	@n 2 : for i <- l - 1 downto 0 do
	@n 3 :		t <-  2 * t
	@n 4 :		t <- t + N_{i} * X
	@n 5 :	end for
	@n 6 : T <- t

* @param bigint** T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param bigint* X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param bigint* N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
* @return SUCCESS 성공 시
 */
int EXP_LR_ADD(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // 반복문에 사용될 변수 i
	int len = 0; // X의 비트 길이를 담을 변수 len
	int result = 0;
	int x_len = 0;
	int t0_len = 0;
	int ret;

	bigint* t0 = NULL;
	bigint* temp0 = NULL;

	BI_Set_Zero(&t0); // [line 1]
	BI_Get_Bit_Length(&len, N);

	for (i = len - 1; i >= 0; i--) // [line 2]
	{
		BI_Left_Shift(t0, 1); // [line 3]
		BI_Get_Word_Length(&t0_len, &t0); // t1의 워드열 길이 -> t1_len
		BI_Get_Word_Length(&x_len, &X); // t1의 워드열 길이 -> t1_len
		BI_New(&temp0, t0_len + x_len); // temp1(t1) = t1^2을 저장하기 위한 bigint 생성
		
		result = BI_Get_j_th_Bit_of_BI(i, N);
		if (result == 1) // [line 4]
		{
			ret = ADD(&temp0, &t0, &X);
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				return ERROR; // 예외 처리
			}
		}
		else // [line 4]
			BI_Assign(&temp0, t0);

		BI_Delete(&t0);
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);
	} // [line 5]

	BI_Assign(T, t0); // [line 6]
	BI_Delete(&t0);

	return SUCCESS;
}

/**
* @brief [Exponentiation] BI_Right to BI_Left MUL function
* details

	@n [pseudo code]
	@n Input : T, X, N, M
	@n 1 : (t0, t1) <- (1, X)
	@n 2 :	for i <- 0 to l - 1 do
	@n 3 :		t0 <- t0 * t1^(N_{i}) ( * : optional )
	@n 4 :		t1 <- t1 ^ 2( ^ 2 : optional )
	@n 5 :	end for
	@n 6 :	T <- t

* @param bigint** T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param bigint* X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param bigint* N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
* @return SUCCESS 성공 시
*/
int EXP_RL_MUL(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // 반복문에 사용될 변수 i
	int len = 0; // X의 비트 길이를 담을 변수 len
	int result = 0;
	int t0_len = 0;
	int t1_len = 0;
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;

	BI_Set_One(&t0); // [line 1]
	BI_Assign(&t1, X); // [line 1]

	BI_Get_Bit_Length(&len, N);

	if (len == 1)
	{
		BI_Assign(T, X);
		BI_Delete(&t0);
		BI_Delete(&t1);
		
		return SUCCESS;
	}

	for (i = 0; i <= len - 1; i++) // [line 2]
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1의 워드열 길이 -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

		BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2을 저장하기 위한 bigint 생성

		result = BI_Get_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			ret = Multiplication(&temp0, t0, t1); // [line 3]
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}
		}
		else
		{
			BI_Assign(&temp0, t0);
			BI_Delete(&t0);
		}
		
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);

		ret = Squaring(&temp1, t1); //  [line 4]
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			BI_Delete(&t1);

			return ERROR; // 예외 처리
		}
		
		BI_Delete(&t1);
		BI_Assign(&t1, temp1);
		BI_Delete(&temp1);
	} // [line 5]
	
	BI_Assign(T, t0);
	BI_Delete(&t0);
	BI_Delete(&t1);
	BI_Refine(*T);

	return SUCCESS;
}

/**
* @brief [Exponentiation] BI_Right to BI_Left ADD function
* @details

	@n [pseudo code]
	@n Input : T, X, N, M
	@n 1 : (t0, t1) <- (0, X)
	@n 2 :	for i <- 0 to l - 1 do
	@n 3 :		t0 <- t0 + N_{i} * t1
	@n 4 :		t1 <- 2 * t1
	@n 5 :	end for
	@n 6 :	T <- t

* @param bigint** T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param bigint* X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param bigint* N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
*/
int EXP_RL_ADD(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // 반복문에 사용될 변수 i
	int len = 0; // X의 비트 길이를 담을 변수 len
	int result = 0;
	int t0_len = 0;
	int t1_len = 0;
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;

	BI_Set_Zero(&t0); // [line 1]
	BI_Assign(&t1, X); // [line 1]

	BI_Get_Bit_Length(&len, N);
	if (len == 1)
	{
		BI_Assign(T, X);
		BI_Delete(&t0);
		BI_Delete(&t1);

		return SUCCESS;
	}

	for (i = 0; i <= len - 1; i++) //  [line 2]
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1의 워드열 길이 -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

		result = BI_Get_j_th_Bit_of_BI(i, N);

		if (result == 1) // [line 3] 
		{
			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 * t1을 저장하기 위한 bigint 생성
			
			ret = ADD(&temp0, &t0, &t1);
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}
		}
		else // [line 3]
		{
			BI_Assign(&temp0, t0);
			BI_Delete(&t0);
		}

		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);

		BI_Left_Shift(t1, 1); // [line 4]
	} // [line 5]

	BI_Assign(T, t0); // [line 6]
	BI_Delete(&t0);
	BI_Delete(&t1);
	BI_Refine(*T);

	return SUCCESS;
}

/**
* @brief [Modular Exponentiation] BI_Left to BI_Right MUL function
* @details

	@n [pseudo code]
	@n Input  : T, X, N, M

	@n 1 : t <- 1
	@n 2 : for i <- l - 1 downto 0 do
	@n 3 :		t <- t^2 mod M ( ^ 2 : optional , mod : optional)
	@n 4 :		t <- t * X^(N_{i}) mod M ( * : option, mod : optional)
	@n 5 :	end for
	@n 6 : T <- t

* @param bigint** T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param bigint* X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param bigint* N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
* @param bigint* M Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
* @return SUCCESS 성공 시
*/
int MOD_EXP_LR_MUL(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // 반복문에 사용될 int형 변수 i
	int len = 0; // 지수승 N의 비트 길이를 담을 int형 변수 len
	int result = 0; // 해당 비트 값의 결과를 return 받는 int형 변수 result
	int x_len = 0; // X의 워드열 길이를 대입할 int형 변수 x_len
	int ret;

	bigint* t0 = NULL; // T0
	bigint* temp0 = NULL; //
	bigint* temp1 = NULL; //
	bigint* trash_q = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 몫 매개변수로 들어갈 bigint 포인터형 변수 
	bigint* trash_r = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 나머지 매개변수로 들어갈 bigint 포인터형 변수

	BI_Set_One(&t0); //  [line 1] t0를 1로 선언
	BI_Get_Bit_Length(&len, N); // 지수승(N)에 대한 비트 길이 -> len 대입

	for (i = len - 1; i >= 0; i--) //  [line 2] 지수승(N)의 최상위 비트부터 최하위 비트까지 반복문
	{
		ret = Squaring(&temp1, t0); //  [line 3] temp1 = t0^2 연산
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			return ERROR; // 예외 처리
		}
		
		BI_Delete(&t0); // t0를 Delete
		
		ret = Division(&trash_q, &trash_r, temp1, M); //  [line 3] temp1(=t0^2)에 대하여 M으로 모듈러 연산
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			BI_Delete(&temp1);

			return ERROR; // 예외 처리
		}
		
		BI_Assign(&temp1, trash_r); // 나머지만 관심 있으므로 나머지를 temp1에 Assign
		BI_Delete(&trash_r); // temp1에 같은 값을 대입했으므로 delete
		BI_Delete(&trash_q); //trash_q로 바꿀까

		BI_Get_Word_Length(&x_len, &X); // X의 워드열 길이 -> x_len
		
		result = BI_Get_j_th_Bit_of_BI(i, N); // 반복문에서 사용 중인 변수 i를 이용해 N의 해당 비트가 1인지 0인지 조사
		if (result == 1) // [line 4] 해당 비트가 1이면,
		{
			ret = Multiplication(&temp0, temp1, X); // [line 4] t1^2 * X (곱셈)연산 후 temp0에 대입
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&temp1);

				return ERROR; // 예외 처리
			}
			
			ret = Division(&trash_q, &trash_r, temp0, M); // [line 4] 곱셈 연산한 temp0에 대하여 M으로 모듈러 연산 진행
			if (ret == ERROR)
			{
				BI_Delete(&temp0);
				BI_Delete(&temp1);

				return ERROR; // 예외 처리
			};
			
			BI_Assign(&t0, trash_r); // 나머지만 관심 있으므로 나머지를 temp0에 assign. --> 반복문에서 t0로 연산을 진행할 수 있도록
			BI_Delete(&trash_r); // temp0에 같은 값을 대입했으므로 delete
			BI_Delete(&trash_q); // Binary Long_Div()에서 사용한 몫도 필요없으므로 delete
		}

		else // 해당 비트가 0이면, 곱셈을 진행하지 않아도 되므로
			BI_Assign(&t0, temp1); // temp0에 t0 assign --> 반복문에서 t0로 연산을 진행할 수 있도록

		BI_Delete(&temp1); // temp1를 delete.
		BI_Delete(&temp0); // temp0를 delete.
	} // [line 5]

	BI_Assign(T, t0); // [line 6] 반복문이 끝난 결과인 t0를 T에 assign
	BI_Delete(&t0); // t0를 delete.

	return SUCCESS;
}

/**
* @brief [Modular Exponentiation] BI_Left to BI_Right ADD function
* @details

	@n [pseudo code]
	@n Input  : T, X, N, M

	@n 1 : t <- 0
	@n 2 : for i <- l - 1 downto 0 do
	@n 3 :		t <-  2 * t mod M ( mod : optional )
	@n 4 :		t <- t + N_{i} * X mod M ( mod : optional )
	@n 5 :	end for
	@n 6 : T <- t

* @param bigint** T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param bigint* X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param bigint* N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
* @param bigint* M Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
* @return SUCCESS 성공 시
*/
int MOD_EXP_LR_ADD(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // 반복문에 사용될 int형 변수 i
	int len = 0; // 지수승 N의 비트 길이를 담을 int형 변수 len
	int result = 0; // 해당 비트 값의 결과를 return 받는 int형 변수 result
	int x_len = 0; // X의 워드열 길이를 대입할 int형 변수 x_len
	int t0_len = 0; // 나머지 워드열 길이를 대입할 int형 변수 t0_len
	int ret;

	bigint* t0 = NULL;
	bigint* temp0 = NULL;
	bigint* trash_q = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 몫 매개변수로 들어갈 bigint 포인터형 변수
	bigint* trash_r = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 나머지 매개변수로 들어갈 bigint 포인터형 변수
	
	BI_Set_Zero(&t0); // [line 1] t0를 0으로 선언
	BI_Get_Bit_Length(&len, N); // 지수승(N)에 대한 비트 길이 -> len 대입

	for (i = len - 1; i >= 0; i--) // [line 2] 
	{
		BI_Left_Shift(t0, 1); // t0 <- 2 * t0 // [line 3]
		
		ret = Division(&trash_q, &trash_r, t0, M); // [line 3] 2 * t0에 대해 Binary_Long_Div에 의해 MOD 연산된 값 -> 2 * t0 (mod M) == trash_r
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			return ERROR; // 예외 처리
		}
		
		BI_Get_Word_Length(&t0_len, &trash_r); // 나머지의 워드열 길이 -> t0_len
		BI_Get_Word_Length(&x_len, &X); // X의 워드열 길이 -> x_len
		BI_New(&temp0, MAX(t0_len, x_len)); // trash_r(= 2 * t0 (mod M))과 X의 덧셈결과를 담을 bigint temp0 생성
		
		result = BI_Get_j_th_Bit_of_BI(i, N); // [line 4] 반복문에서 사용 중인 변수 i를 이용해 N의 해당 비트가 1인지 0인지 조사

		if (result == 1)// [line 4] 해당 비트가 1이면(If result is 1),
		{
			ret = ADD(&temp0, &trash_r, &X); // [line 4] trash_r + X 의 값을 temp0에 대입 
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&temp0);

				return ERROR; // 예외 처리
			}
			
			ret = Division(&trash_q, &trash_r, temp0, M); // temp0의 modular 값을 구해서 trash_r에 다시 대입
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&temp0);

				return ERROR; // 예외 처리
			}
			
			BI_Assign(&t0, trash_r);  // 나머지만 관심 있으므로 나머지를 temp0에 assign. --> 반복문에서 다시 돌 때 t0로 연산을 진행할 수 있도록
		}
		else // [line 4] 해당 비트가 0이면(If result is 0),
			BI_Assign(&t0, trash_r); // [line 4] 나머지만 관심 있으므로 나머지를 temp0에 assign. --> 반복문에서 다시 돌 때 t0로 연산을 진행할 수 있도록 

		BI_Delete(&temp0); // temp0를 delete.
		BI_Delete(&trash_q); // trash_q를 delete.
		BI_Delete(&trash_r); // trash_r를 delete.
	} // [line 5]

	BI_Assign(T, t0); // [line 6] 반복문이 끝난 최종 결과인 t0를 T에 assign
	BI_Delete(&t0); // t0를 delete.

	return SUCCESS;
}

/**
* @brief [Modular Exponentiation] BI_Right to BI_Left MUL function
* details

	@n [pseudo code]
	@n Input : T, X, N, M
	@n 1 : (t0, t1) <- (1, X)
	@n 2 :	for i <- 0 to l - 1 do
	@n 3 :		t0 <- t0 * t1^(N_{i}) mod M ( * :optional, mod : optional )
	@n 4 :		t1 <- t1 ^ 2 mod M ( ^ : optional, mod :optional )
	@n 5 :	end for
	@n 6 :	T <- t

* @param bigint** T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param bigint* X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param bigint* N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
* @param bigint* M Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
* @return SUCCESS 성공 시
*/
int MOD_EXP_RL_MUL(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // 반복문에 사용될 int형 변수 i
	int len = 0; // 지수승 N의 비트 길이를 담을 int형 변수 len
	int result = 0; // 해당 비트 값의 결과를 return 받는 int형 변수 result
	int t0_len = 0; // t0의 워드열 길이를 대입할 int형 변수 t0_len
	int t1_len = 0; // t1의 워드열 길이를 대입할 int형 변수 t1_len
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 몫 매개변수로 들어갈 bigint 포인터형 변수
	bigint* trash_r = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 나머지 매개변수로 들어갈 bigint 포인터형 변수

	BI_Set_One(&t0); // [line 1] t0를 1로 선언
	BI_Assign(&t1, X); // [line 1] t1에 X를 assign

	BI_Get_Bit_Length(&len, N);  // 지수승(N)에 대한 비트 길이 -> len 대입

	if ((len == 1) & (N->a[0] == 1)) // X ^ 1일 때
	{
		BI_Assign(T, X);  // 이 경우에서는 T의 값이 X이므로 assign
		
		ret = Division(&trash_q, &trash_r, *T, M); // X mod M 연산 후 trash_r에 대입
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			BI_Delete(&t1);

			return ERROR; // 예외 처리
		}
		BI_Assign(T, trash_r); // X mod M 값인 trash_r을 T에 assign
		BI_Delete(&trash_q); // 할당한 trash_q delete
		BI_Delete(&trash_r); // 할당한 trash_r delete
		BI_Delete(&t0); // 할당한 t0 delete
		BI_Delete(&t1); // 할당한 t1 delete
		
		return SUCCESS;
	}

	for (i = 0; i <= len - 1; i++) // [line 2] 지수승(N)의 최하위 비트부터 최상위 비트까지 반복문
	{
		BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
		BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

		BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2을 저장하기 위한 bigint 생성

		result = BI_Get_j_th_Bit_of_BI(i, N);  // [line 3] 반복문에서 사용 중인 변수 i를 이용해 N의 해당 비트가 1인지 0인지 조사

		if (result == 1)// [line 3] 해당 비트가 1이면,
		{
			ret = Multiplication(&temp0, t0, t1); // [line 3] temp0에 t0 * t1 을 연산
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}
		}
		else // [line 3] 해당 비트가 0이면,
			BI_Assign(&temp0, t0); // [line 3] t0에 1을 곱하는 것이므로, temp0에 t0을 assign --> 반복문에서 t0로 연산을 진행할 수 있도록

		ret = Division(&trash_q, &trash_r, temp0, M); // [line 3] 연산된 temp0(t0)에 Binary_Long_Div()를 이용해 modular M 연산 후 결과값을 trash_r에 대입
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			BI_Delete(&t1);
			BI_Delete(&temp0);

			return ERROR; // 예외 처리
		}

		BI_Assign(&t0, trash_r);
		BI_Delete(&temp0); // 할당한 temp0 delete
		BI_Delete(&trash_q); // 연산을 통해 할당한 trash_q delete
		BI_Delete(&trash_r); // 연산을 통해 할당한 trash_r delete

		ret = Squaring(&temp1, t1);  // [line 4] temp1(t1) <- t1 * t1
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			BI_Delete(&t1);

			return ERROR; // 예외 처리
		}
		
		ret = Division(&trash_q, &trash_r, temp1, M); // [line 4] temp1( = t1 * t1)에 Binary_Long_Div()를 이용해 modular M 연산 후 결과값을 trash_r에 대입
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			BI_Delete(&t1);
			BI_Delete(&temp1);

			return ERROR; // 예외 처리
		}
		
		BI_Assign(&t1, trash_r); // t1 * t1 (mod M) 값인 trash_r을 다시 t1에 assign --> 반복문에서 t1로 연산을 진행할 수 있도록
		BI_Delete(&trash_q); // 연산을 통해 할당한 trash_q delete
		BI_Delete(&trash_r); // 연산을 통해 할당한 trash_r delete
		BI_Delete(&temp1); // 할당한 temp0 delete
	} // [line 5]

	BI_Assign(T, t0); // [line 6] 반복문이 끝난 결과인 t0를 T에 assign
	BI_Delete(&t0); // 연산을 통해 할당한 t0 delete
	BI_Delete(&t1); // 연산을 통해 할당한 t1 delete
	BI_Refine(*T); // 결과값인 T를 refine 해주기

	return SUCCESS;
}

/**
* @brief [Modular Exponentiation] BI_Right to BI_Left ADD function (used Binary Long Division)
* @details

	@n [pseudo code]
	@n Input : T, X, N, M
	@n 1 : (t0, t1) <- (0, X)
	@n 2 :	for i <- 0 to l - 1 do
	@n 3 :		t0 <- t0 + N_{i} * t1 mod M ( mod : optional )
	@n 4 :		t1 <- 2 * t1 mod M ( mod : optional )
	@n 5 :	end for
	@n 6 :	T <- t

* @param bigint** T Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
* @param bigint* X Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
* @param bigint* N Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
* @param bigint* M Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
* @return SUCCESS 성공 시
*/
int MOD_EXP_RL_ADD(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // 반복문에 사용될 int형 변수 i
	int len = 0; // 지수승 N의 비트 길이를 담을 int형 변수 len
	int result = 0; // 해당 비트 값의 결과를 return 받는 int형 변수 result
	int t0_len = 0; // t0의 워드열 길이를 대입할 int형 변수 t0_len
	int t1_len = 0; // t1의 워드열 길이를 대입할 int형 변수 t1_len
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 몫 매개변수로 들어갈 bigint 포인터형 변수
	bigint* trash_r = NULL; // Modular 연산을 위해 사용되는 Binary_Long_Div()의 나머지 매개변수로 들어갈 bigint 포인터형 변수

	BI_Set_Zero(&t0); // [line 1] t0를 0으로 선언
	BI_Assign(&t1, X); // [line 1] t1에 X를 assign

	BI_Get_Bit_Length(&len, N); // 지수승(N)에 대한 비트 길이 -> len 대입

	if ((len == 1) & (N->a[0] == 1)) // 1 * X일 때
	{
		BI_Assign(T, X);  // 이 경우에서는 T의 값이 X이므로 assign
		
		ret = Division(&trash_q, &trash_r, *T, M); // X mod M 연산 후 trash_r에 대입
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			BI_Delete(&t1);

			return ERROR; // 예외 처리
		}

		BI_Assign(T, trash_r); // X mod M 값인 trash_r을 T에 assign
		BI_Delete(&trash_q); // 할당한 trash_q delete
		BI_Delete(&trash_r); // 할당한 trash_r delete
		BI_Delete(&t0); // 할당한 t0 delete
		BI_Delete(&t1); // 할당한 t1 delete
		
		return SUCCESS;
	}

	for (i = 0; i <= len - 1; i++) // [line 2] 지수승(N)의 최하위 비트부터 최상위 비트까지 반복문
	{
		BI_Get_Word_Length(&t0_len, &t0); // t0의 워드열 길이 -> t0_len
		BI_Get_Word_Length(&t1_len, &t1); // t1의 워드열 길이 -> t1_len

		result = BI_Get_j_th_Bit_of_BI(i, N);  // [line 3] 반복문에서 사용 중인 변수 i를 이용해 N의 해당 비트가 1인지 0인지 조사

		if (result == 1) // [line 3] 해당 비트가 1이면,
		{
			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 + t1을 저장하기 위한 bigint 생성
			
			ret = ADD(&temp0, &t0, &t1); // [line 3] temp0에 t0 + t1 연산
			if (ret == ERROR)
			{
				BI_Delete(&t0);
				BI_Delete(&t1);

				return ERROR; // 예외 처리
			}
		}
		else // [line 3] 해당 비트가 0이면,
			BI_Assign(&temp0, t0); // [line 3] t0에 0을 더하는 것이므로, temp0에 t0을 assign.
		
		ret = Division(&trash_q, &trash_r, temp0, M); // [line 3] 연산된 temp0(t0)에 modular M 연산 후 결과값을 trash_r에 대입
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			BI_Delete(&t1);
			BI_Delete(&temp0);

			return ERROR; // 예외 처리
		}
		
		BI_Delete(&temp0); // 할당한 temp0 delete
		BI_Assign(&t0, trash_r); // t0 + result * t1 (mod M) 값인 trash_r을 t0에 assign --> 반복문에서 t0로 연산을 진행할 수 있도록
		BI_Delete(&trash_q); // 연산을 통해 할당한 trash_q delete
		BI_Delete(&trash_r); // 연산을 통해 할당한 trash_r delete

		BI_Left_Shift(t1, 1); // [line 4] t1 <- 2 * t1
		
		ret = Division(&trash_q, &trash_r, t1, M); // [line 4] 2 * t1에 modular M 연산 후 결과값을 trash_r에 대입
		if (ret == ERROR)
		{
			BI_Delete(&t0);
			BI_Delete(&t1);

			return ERROR; // 예외 처리
		}
		
		BI_Assign(&t1, trash_r); // 2 * t1 (mod M) 값인 trash_r을 다시 t1에 assign --> 반복문에서 t1로 연산을 진행할 수 있도록
		BI_Delete(&trash_q); // 연산을 통해 할당한 trash_q delete
		BI_Delete(&trash_r); // 연산을 통해 할당한 trash_r delete
	} // [line 5]
	
	BI_Assign(T, t0); // [line 6] 반복문이 끝난 결과인 t0를 T에 assign
	BI_Delete(&t0); // 연산을 통해 할당한 t0 delete
	BI_Delete(&t1); // 연산을 통해 할당한 t1 delete
	BI_Refine(*T); // 결과값인 T를 refine 해주기

	return SUCCESS;
}
