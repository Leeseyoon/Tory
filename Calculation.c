#include "config.h"
#include "BasicOperation.h"
#include "Calculation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define   zerorize

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

 * @param bigint** C ���� ����� ������ bigint �� ���������� ����
 * @param bigint** A ������ ������ bigint �� ���������� ����
 * @param bigint** B ������ ������ bigint �� ���������� ����
 * @param unsigned int c ���� ���꿡�� �޾� �� ĳ�� (0 �Ǵ� 1)
 * @param int i ������ ������ �迭�� index

 * @return carry ĳ�� ���� (0 �Ǵ� 1)
 * @throws ERROR �迭�� index�� ������ ���
 */
unsigned int ADD_ABc(bigint** C, bigint** A, bigint** B, unsigned int c, int i)
{
	int carry = 0; // [line 1]
	word temp = 0;

	if (i < 0) // ���� ó��
		return ERROR;

	temp = (*A)->a[i];
	(*C)->a[i] = (*A)->a[i] + (*B)->a[i]; // [line 2] A, B ������ i��° �� ���ؼ� C�� i��° �迭�� ����

	if ((*C)->a[i] < temp) // [line 3] ������ A�� i��° ���� �տ��� ����� ������� ������ ĳ���� �߻��� ��
		carry = 1; // [line 4]

	(*C)->a[i] = (*C)->a[i] + c; // [line 6] ���ڷ� �޾ƿ� ĳ�� ��(c) ���� �ֱ�

	if ((*C)->a[i] < c) // [line 7] ���� ����� c���� ������ ĳ���� �߻��� ��
		carry = carry + 1; // [line 8]

	return carry; // [line 10]
}

/**
 * @brief Addition Core (WordLen(A) >= WordLen(B), Sign(A) = Sign(B))
 * @details

	 [pseudo code]
	 Input  : A, B, sign
	 Output : C = A + B
	 1 : B[j] <- 0 for j = m, m + 1, ... , n - 1
	 2 : c <- 0
	 3 : for j = 0 to n - 1 do
	 4 :	c, C[j] <- ADD_ABc(A[j], B[j], c)
	 5 : end for
	 6 : C[n] <- c
	 7 : if C[n] = 1 then
	 8 :	C <- (-1)^sign * sum(C[j] * W^j) for j = 0 to n
	 9 : else
	10 :	C <- (-1)^sign * sum(C[j] * W^j) for j = 0 to n - 1
	11 : end if

 * @param bigint** C ���� ����� ������ bigint �� ���������� ����
 * @param bigint** A ������ ������ bigint �� ���������� ����
 * @param bigint** B ������ ������ bigint �� ���������� ����
 * @param int sign ���� ��� C�� ��ȣ
 * @return SUCCESS
 * @throws
	ERROR A(B)�� word length�� ����� �ƴ� ���
	ERROR bigint ����ü ���ο� �ִ� �迭 ���Ҵ� ���� ��
 */
int ADDC(bigint** C, bigint** A, bigint** B, int sign)
{
	int A_Len;
	int B_Len;
	int i;
	unsigned int carry;

	BI_Get_Word_Length(&A_Len, A);
	BI_Get_Word_Length(&B_Len, B);

	if (A_Len <= 0)
		return ERROR;
	if (B_Len <= 0)
		return ERROR;

	word* temp;

	// A�� ���忭 ���̰� B�� ���忭 ���̺��� ū ����̹Ƿ� B�� ���忭 ���̸� A�� ���忭 ���̸�ŭ �÷��� realloc
	(*B)->wordlen = A_Len;
	temp = (word*)realloc((*B)->a, sizeof(word) * A_Len);
	if (temp != NULL)
		(*B)->a = temp;
	else
		return ERROR;

	memset(&(*B)->a[B_Len], 0x00, A_Len - B_Len); // [line 1] �þ ���̸�ŭ 0���� �ʱ�ȭ

	carry = 0; // [line 2]

	for (i = 0; i < A_Len; i++) // [line 3]
		carry = ADD_ABc(C, A, B, carry, i); // [line 4] ���� ���� A�� ���忭 ���̸�ŭ ����

	if (carry == 1) // [line 7] ������ carry�� 1�̸�
		(*C)->a[A_Len] = 1; // [line 8] ������� MSB�� 1 ����
//	else // ������ carry�� 0�̸�
//		(*C)->a[A_Len] = 0; // ������� MSB�� 0 ����

	BI_Refine(*C);

	// [line 8, 10] ��ȣ ����
	if (sign == 0)
		(*C)->sign = 0;
	else
		(*C)->sign = 1;

	return SUCCESS;
}

/**
 * @brief Addition Core (WordLen(A) >= WordLen(B), Sign(A) = Sign(B), A = A + B)
 * @details
	ADDC �Լ��� �����ϰ� ����
	������ ������ ����� �ٽ� A�� ���� (A <- A + B)
 * @param bigint** C ���� ����� ������ bigint �� ���������� ����
 * @param bigint** A ������ ������ bigint �� ���������� ����
 * @param bigint** B ������ ������ bigint �� ���������� ����
 * @param int sign ���� ��� C�� ��ȣ
 * @return SUCCESS
 * @throws
	ERROR A(B)�� word length�� ����� �ƴ� ���
	ERROR bigint ����ü ���ο� �ִ� �迭 ���Ҵ� ���� ��
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
	temp = (word*)realloc((*B)->a, sizeof(word) * A_Len);
	if (temp != NULL)
		(*B)->a = temp;
	else
		return ERROR;

	memset(&(*B)->a[B_Len], 0x00, A_Len - B_Len);

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

	 [pseudo code]
	 Input  : A, B
	 Output : C = A + B
	 1 : if A = 0 then
	 2 :	C <- B
	 3 : end if
	 4 : if B = 0 then
	 5 :	C <- A
	 6 : end if
	 7 : if A > 0 and B < 0 then
	 8 :	C <- SUB(A, |B|)
	 9 : end if
	10 : if A < 0 and B > 0 then
	11 :	C <- SUB(B, |A|)
	12 : end if
	13 : if WordLen(A) >= WordLen(B) then
	14 :	C <- ADDC(A, B)
	15 : else
	16 :	C <- ADDC(B, A)
	17 : end if

 * @param bigint** C ���� ����� ������ bigint �� ���������� ����
 * @param bigint** A ������ ������ bigint �� ���������� ����
 * @param bigint** B ������ ������ bigint �� ���������� ����
 * @return SUCCESS ���� ��
 * @throws
	ERROR A(B)�� bigint ����ü ���Ҵ� ��
	ERROR A(B)�� ��ȣ �������� ���� ��
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
		// [line 2] A + B���� A�� 0�̸� B return
		(*C)->sign = (*B)->sign;
		(*C)->wordlen = (*B)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*B)->a[i];

		return SUCCESS;
	}

	if (BI_Is_Zero(B) == 0) // [line 4] B is zero
	{
		// [line 5] A + B���� B�� 0�̸� A return
		(*C)->sign = (*A)->sign;
		(*C)->wordlen = (*A)->wordlen;
		for (i = 0; i < (*C)->wordlen; i++)
			(*C)->a[i] = (*A)->a[i];

		return SUCCESS;
	}

	if ((A_sign == NON_NEGATIVE) && (B_sign == NEGATIVE)) // [line 7] A�� ���, B�� �����̸�
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

	if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE)) // [line 10] A�� ����, B�� ����̸�
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

	// A, B�� ������ ��ȣ�� ��
	if (A_Len >= B_Len) // [line 13] A�� ���忭�� ���̰� B�� ���忭�� ���̺��� �� ��
	{
		re = ADDC(C, A, B, A_sign); // [line 14] ���� ����
		if (re == ERROR)
			return ERROR;

		return SUCCESS;
	}
	else // [line 15] B�� ���忭�� ���̰� A�� ���忭�� ���̺��� �� ��
	{
		re = ADDC(C, B, A, A_sign); // [line 16] ���� ����
		if (re == ERROR)
			return ERROR;

		return SUCCESS;
	}

}

/**
 * @brief Addition (A = A + B)
 * @details
	ADD �Լ��� �����ϰ� ����
	������ ������ ����� �ٽ� A�� ���� (A <- A + B)
 * @param bigint** C ���� ����� ������ bigint �� ���������� ����
 * @param bigint** A ������ ������ bigint �� ���������� ����
 * @param bigint** B ������ ������ bigint �� ���������� ����
 * @return SUCCESS ���� ��
 * @throws
	ERROR A(B)�� bigint ����ü ���Ҵ� ��
	ERROR A(B)�� ��ȣ �������� ���� ��
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
		re = SUB(C, *A, temp); // SUB �Լ�
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
		re = SUB(C, *B, temp); // SUB �Լ�
		if (re == ERROR)
			return ERROR;

		BI_Delete(&temp);

		return SUCCESS;
	}

	// A, B�� ������ ��ȣ�� ��
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

/**
* @brief Subtraction
* @details
	[pseudo code]
	Input  : C, A, B
	Output : C = A - B

	1 : if A = 0 then
	2 :		return -B
	3 :	end if
	4 :	if B = 0 then
	5 :		return A
	6 :	end if
	7 :	if A >= B > 0 then
	8 :		return SUBC(A, B)
	9 :	else
	10:		return -SUBC(B, A)
	11:	end if
	12:	if 0 > A >= B then
	13:		return SUBC(|B|, |A|)
	14:	else
	15:		return -SUBC(|A|, |B|)
	16:	end if
	17: 	if A > 0 and B < 0 then
	18:		return ADD(A, |B|)
	19:	else
	20:		return -ADD(|A|, B)
	21:	end if

* @param bigint** C ���� ������(A - B) ����� ������ bigint �� ���� ������ ����
* @param bigint** A ���� ������(A - B) A�� �ش��ϴ� bigint �� ������ ����
* @param bigint** B ���� ������(A - B) B�� �ش��ϴ� bigint �� ������ ����
* @return SUCCESS ���� ��
* @throws
	ERROR A, B, C �� bigint ����ü ���Ҵ� ��
	ERROR A, B, C�� word length�� ����� �ƴ� ���
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

	BI_Get_Word_Length(&len, &A);   // A->wordlen�� len�� ����
	BI_Get_Word_Length(&borrow, &B);// B->wordlen�� borrow�� ����

	if (len < 0)
		return ERROR;
	if (borrow < 0)
		return ERROR;

	result = BI_Compare(&A, &B);

	if (BI_Is_Zero(&A) == 0) // [line 1] 0-B --> -B
	{
		BI_Assign(C, B); // [line 2] B�� �� �״��
		(*C)->sign = NEGATIVE; // [line 2] ��ȣ�� �ݴ��

		return SUCCESS; // memory leackege X
	}

	if (BI_Is_Zero(&B) == 0) // [line 4] A - 0 --> A
	{
		if (&A == C) // 1st arg == 2nd arg(AAB)�� ���
			return SUCCESS; // �̹� 1st arg = 2nd arg �̹Ƿ� return���� ����.
		BI_Assign(C, A); // [line 5] 1st arg != 2nd arg(AAB)�� ��� C�� A�� assign
		return SUCCESS;
	}


	if ((A->sign ^ B->sign) == 0) // A, B ��ȣ�� ���� ��
	{
		if ((A->sign & B->sign) == 0) // [line 7] A, B�� ��ȣ�� ��� ����� ��
		{
			if (result == 2) // [line 9] A, B�� ���ؼ� A < B�� ��. (BI_Compare(A, B)�� return : 2)
			{
				ret = SUBC(C, &B, &A); // [line 10] B - A �� �ϰ�
				if (ret == -1)
					return ERROR;
				BI_Flip_Sign(*C); // [line 10] ���� ������ ����Ǿ����Ƿ� ������� ��ȣ �ٲ��ֱ�

				return SUCCESS;
			}
			else if (result == 0) // A = B�� ��, C�� ���� 0�� �Ǿ���Ѵ�.
			{
				// BI_Set_Zero(C); �� �ٲٴ°� ���?
				for (i = 0; i < (*C)->wordlen; i++) // C�� ���̸�ŭ
					(*C)->a[i] = 0;  // 0�� ����
				(*C)->sign = 0; // C�� ��ȣ�� NON-NEGATIVE�� ����

				return SUCCESS;
			}
			else if (result == 1)// [line 7] A, B �� ���ؼ� A >= B�� ��. BI_Compare(A, B)'s return : 0, 1
			{
				ret = SUBC(C, &A, &B); // [line 8] A - B ����
				if (ret == -1)
					return ERROR;
				return SUCCESS;
			}
			else
				return ERROR;
		}

		else // A, B�� ��ȣ�� ��� ������ ��
		{
			BI_Flip_Sign(A); // A�� ��ȣ�� �����̹Ƿ� ��ȣ �ٲ��ֱ�
			BI_Flip_Sign(B); // B�� ��ȣ�� �����̹Ƿ� ��ȣ �ٲ��ֱ�
			if (result == 2) // [line 12]
			{
				ret = SUBC(C, &B, &A); // [line 13]
				if (ret == -1)
					return ERROR;
				BI_Flip_Sign(A); // ���� ������ ����Ǿ����Ƿ� ������� ��ȣ ����ġ
				BI_Flip_Sign(B); // ���� ������ ����Ǿ����Ƿ� ������� ��ȣ ����ġ

				return SUCCESS;
			}
			else if ((result == 0) | (result == 1)) // [line 14]
			{
				ret = SUBC(C, &A, &B); // [line 15]
				if (ret == -1)
					return ERROR;
				BI_Flip_Sign(*C); // [line 15]���� ����� ��ȣ �ٲ��ֱ�
				BI_Flip_Sign(A); // ���� ������ ����Ǿ����Ƿ� ������� ��ȣ ����ġ
				BI_Flip_Sign(B); // ���� ������ ����Ǿ����Ƿ� ������� ��ȣ ����ġ

				return SUCCESS;
			}
			else
				return ERROR;
		}
	}

	else // A,B ��ȣ�� �ٸ� ��
	{
		if (A->sign == 0) // [line 17] A�� ���, B�� ����
		{
			BI_Flip_Sign(B); // B�� ��ȣ�� �ٲ��ְ�
			ret = ADD(C, &A, &B);  // [line 18] ADD ����
			if (ret == -1)
				return ERROR;
			BI_Flip_Sign(B); // ��ȣ ����ġ

			return SUCCESS;
		}
		else // [line 19]
		{
			BI_Flip_Sign(A); // A => |A|
			ret = ADD(C, &A, &B); // [line 20]
			if (ret == -1)
				return ERROR;

			BI_Flip_Sign(A); // ��ȣ ����ġ
			BI_Flip_Sign(*C); // ��ȣ ����ġ

			return SUCCESS;
		}
	}
}

/**
* @brief Subtraction Core
* @details
	[pseudo code]
	Input  : C, A, B
	Output : C = A - B

	1 : B_{j} <- 0 for j =  m downto n-1 do
	2 :	b <- 0
	3 :	for j = 0 to n - 1 do
	4 :		C <- A - b (mod W)
	5 :		if A < b then
	6 :			b <- 1
	7 :		else
	8 :			b <- 0
	9 :		end if
	10:		if C < B then
	11:			b <- b + 1
	12:		end if
	13:		C <- C - b
	14:	end for
	15: l <- min{j : C_{n-1} = C_{n-2} = ... = C_{j} = 0}
	16: C <- len(l)

* @param bigint** C ���� ���� ������(A - B) ����� ������ bigint �� ���� ������ ����
* @param bigint** A ���� ���� ������(A - B) A�� �ش��ϴ� bigint �� ������ ����
* @param bigint** B ���� ���� ������(A - B) B�� �ش��ϴ� bigint �� ������ ����
* @return SUCCESS ���� ��
* @throws
	ERROR A, B, C �� bigint ����ü ���Ҵ� ��
	ERROR A, B, C�� word length�� ����� �ƴ� ���
*/
int SUBC(bigint** C, bigint** A, bigint** B)
{
	int len, i = 0;
	int result = 0;
	unsigned int borrow = 0;
	bigint* temp = NULL; // A�� b�� ���̰� �ٸ� �� -> bigint** b�� ���̸� �ٲ� �� �����Ƿ� temp�� �������
	bigint* temp3 = NULL; // C�� A�� ���� ���� ����Ͽ�

	if ((*C == NULL) | (B == NULL) | (A == NULL))
		return ERROR;

	BI_Get_Word_Length(&len, A); // b���� ū A�� ���̸� ������
	BI_Get_Word_Length(&borrow, B); // b���� ū A�� ���̸� ������

	if (len > borrow)
	{
		BI_New(&temp, len);  // A�� ���� ���̿� ���� temp �� ����
		for (i = 0; i < (*B)->wordlen; i++)//
			temp->a[i] = (*B)->a[i]; // b�� ���� ���� ������ �־���ϰ�, �� ��� �������� ���� 0�� ���־����.
	}
	else
	{
		BI_New(&temp3, borrow);
		for (i = 0; i < (*A)->wordlen; i++)//
			temp3->a[i] = (*A)->a[i]; // b�� ���� ���� ������ �־���ϰ�, �� ��� �������� ���� 0�� ���־����.
		
	}
	borrow = 0;
	result = BI_Compare(A, B);
	if ((result == 0) | (result == 1))
		(*C)->sign = 0; // �Ű����� C�� �̹� ��ȣ�� ������ �� �ٲ��ִ� �� ���
	if (result == 2)
		(*C)->sign = 1;
	if ((*C)->wordlen < len) // Binary Long Division���� C�� ���̰� 1�̰�, A, B�� ���̰� 2�� ���� �־. //A = 0x40bd
	{
		bigint* temp2 = NULL;
		BI_New(&temp2, len); // C���� �� ū ������ len�� ������ temp2�� ����
		for (i = 0; i < (*C)->wordlen; i++) // C->wordle�� ���̸�ŭ�� temp2�� ����. ��ä���� �κ��� 0���� �״��
			temp2->a[i] = (*C)->a[i];
		BI_Assign(C, temp2);
		BI_Delete(&temp2);
	}
	//for (i = 0; i < (*B)->wordlen; i++)//
	//	temp->a[i] = (*B)->a[i]; // b�� ���� ���� ������ �־���ϰ�, �� ��� �������� ���� 0�� ���־����.
	// A�� b���� �� �� b�� ���̸� ���� ��� �ϴµ� b�� �ǵ��̸� b�� �ٲ�� ������ temp�� �̿�

	//BI_New(&temp3, len); // A�� wordlen�� ���� len�� ���̷� temp3 ����
	//BI_Assign(&temp3, *A); // ���� A�� �����ϰ�
	for (i = 0; i < len; i++) // [line 3]
	{
		(*C)->a[i] = temp3->a[i] - borrow; // [line 4] (*C)->a[i] = (*A)->a[i] - (borrow); // A - b�� ���� C �� ���� // ó�� borrow���� �ʱ�ȭ�� 0���� ����
		(*C)->a[i] = (*C)->a[i] & word_mask; // mod 2 ^ (WORD_BIT_LEN)
		if (temp3->a[i] < borrow) // [line 5] if ((*A)->a[i] < borrow) // borrow�� ���� ��
			borrow = 1;// [line 6] *borrow = 1;
		else // [line 7] borrow �ȵ� ��
		{
			borrow = 0;// [line 8] *borrow = 0;
			if ((*C)->a[i] < temp->a[i]) // [line 10]
				borrow += 1;// [line 11] borrow = borrow + 1;
		}
		(*C)->a[i] -= temp->a[i]; // [line 13] temp�� �־���� b�� ���� ����
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
	flag�� ���� ���� ���� ����(Karatsuba multiplication / textbook multiplication)
	Input  : A
	Output : C = A * B
 * @param bigint* C ���� ������ ������ ����� ������ bigint �� ���������� ����
 * @param bigint* A ���� ������ ������ bigint �� ������ ����
 * @param bigint* B ���� ������ ������ bigint �� ������ ����
 * @return SUCCESS
 * @throws
	ERROR flag ������ ��
	ERROR A(B)�� bigint ����ü ���Ҵ� ��
	ERROR A(B)�� ���̰� ����� �ƴ� ���
 */
int Multiplication(bigint** C, bigint* A, bigint* B)
{
	int A_Len = 0;
	int B_Len = 0;
	int C_Len = 0;
	int ret;

	if ((B == NULL) | (A == NULL))
		return ERROR;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A�� ���忭 ����
	BI_Get_Word_Length(&B_Len, &B); // B_Len = B�� ���忭 ����

	if (A_Len < 0)
		return ERROR;
	if (B_Len < 0)
		return ERROR;

	if (FLAG_MUL == TEXTBOOK) // textbook multiplication
	{
		C_Len = A_Len + B_Len; // C�� ���� ���� �� �� bigint ����
		BI_New(C, C_Len); // bigint ����
		ret = MUL_Multi(C, A, B);
		if (ret == ERROR)
			return ERROR;
	}
	else if (FLAG_MUL == KARATSUBA) // karatsuba multiplication
	{
		C_Len = (MAX(A->wordlen, B->wordlen) + 1) >> 1;
		C_Len = 4 * C_Len + 1; // C�� ���� ���� �� ��
		BI_New(C, C_Len); // bigint ����

		int sign_A = BI_Get_Sign(A); // sign_A = A�� ��ȣ
		int sign_B = BI_Get_Sign(B); // sign_B = B�� ��ȣ

		if (sign_A == ERROR || sign_B == ERROR) // ��ȣ �������� ���� �� ���� ����
		{
			printf("get sign error\n");
			return ERROR;
		}

		// karatsuba ���ڴ� ������̹Ƿ� ���� ���� �ֱ�
		if (sign_A == NEGATIVE)
			BI_Flip_Sign(A);

		if (sign_B == NEGATIVE)
			BI_Flip_Sign(B);

		ret = MUL_Karatsuba(C, A, B);
		if (ret == ERROR)
			return ERROR;
		BI_Refine(*C);

		if ((sign_A ^ sign_B) == 1) // C�� ��ȣ ����
			(*C)->sign = NEGATIVE;
		else
			(*C)->sign = NON_NEGATIVE;
	}
	else
		return ERROR;

	return SUCCESS;
}

/**
* @brief Multiplication in Word
* @details

	[pseudo code]
	Input  : C, A, B

	1 : A1, A0 <- A >> w/2, A mod 2^(w/2)
	2 : A1, A0 <- A >> w/2, A mod 2^(w/2)
	3 : MUL1, MUL0 <- A1 * B1, A0 * B0
	4 : SUM0, SUM1 <- A1 * B0, A0 * B1
	5 : SUM1 = SUM1 + SUM0 (mod W)
	6 : if (SUM1 < SUM0) then
	7 :		carry1 <- 1
	8 : SUM2 <- SUM1 << (w/2)
	9 : SUM1 <- SUM1 >> (w/2)
	10: MUL0 <- MUL0 + SUM2 (mod W)
	11: if (MUL0 < SUM2)
	12:		carry0 <- 1
	13: MUL1 <- MUL1 + SUM1 + carry0 + (carry1 << (w/2)) (mod W)
	14: C = (00.. || MUL1 || MUL0 || ..00)

* @param word* C ���� ���� ���� ������ ����� ������ word �� ������ ����
* @param word* A ���� ���� ���� ������ ���ϴ� ���� word �� ������ ����
* @param word* B ���� ���� ���� ������ ���ϴ� ���� word �� ������ ����
* @return SUCCESS ���� ��
* @throws ERROR A, B, C�� ���Ҵ� ��
*/
int MUL_Word(word* C, word* A, word* B) // ���� ���� ����
{
	int carry0 = 0; // ���尣�� ������ ���� �����Ǵ� carry�� ���� int �� ���� carry0
	int carry1 = 0; // ���尣�� ������ ���� �����Ǵ� carry�� ���� int �� ���� carry1

	word sum0 = 0; // ���尣�� ������ ���� word�� ���� sum0
	word sum1 = 0; // ���尣�� ������ ���� word�� ���� sum1
	word sum2 = 0; // ���尣�� ������ ���� word�� ���� sum2
	word mul0 = 0; // ���� ���� ���� ��� �� ���� ���带 ���� word�� ���� mul0
	word mul1 = 0; // ���� ���� ���� ��� �� ���� ���带 ���� word�� ���� mul1
	word A1 = 0; // A�� �ֻ�����Ʈ���� �߰���Ʈ���� ���� word�� ���� A1
	word B1 = 0; // B�� �ֻ�����Ʈ���� �߰���Ʈ���� ���� word�� ���� B1
	word A0 = 0; // A�� �߰���Ʈ���� ������ ��Ʈ���� ���� word�� ���� A0
	word B0 = 0; // B�� �߰���Ʈ���� ������ ��Ʈ���� ���� word�� ���� B0

	if ((C == NULL) | (B == NULL) | (A == NULL))
		return ERROR;

	A1 = ((*A) >> (WORD_BIT_LEN >> 1)); // A�� �ֻ�����Ʈ���� �߰���Ʈ����
	B1 = ((*B) >> (WORD_BIT_LEN >> 1)); // B�� �ֻ�����Ʈ���� �߰���Ʈ����
	A0 = (*A) & (((word)1 << (WORD_BIT_LEN >> 1)) - 1); // A�� �߰���Ʈ���� ������ ��Ʈ����
	B0 = (*B) & (((word)1 << (WORD_BIT_LEN >> 1)) - 1); // B�� �߰���Ʈ���� ������ ��Ʈ����

	mul1 = A1 * B1; //  [line 3] A1�� B1�� ���� mul1�� ����
	mul0 = A0 * B0; //  [line 3] A0�� B0�� ���� mul0�� ����
	sum0 = A1 * B0; //  [line 4] A1�� B0�� ���� sum0�� ����
	sum1 = A0 * B1; //	[line 4] A0�� B1�� ���� sum1�� ����
	sum1 += sum0; //	[line 5] sum1�� sum0�� ����
	if (sum1 < sum0) // [line 6] carry �߻�������,
		carry1 = 1; // [line 7] carr1�� 1�� �ǵ��� ����
	sum2 = (sum1 & (((word)1 << (WORD_BIT_LEN >> 1)) - 1));  // sum1�� �޺κ�(�߰���Ʈ ~ ��������Ʈ)�� ���� sum2�� ����
	sum2 = sum2 << (WORD_BIT_LEN >> 1); // [line 8] sum1�� �޺κ�(�߰���Ʈ ~ ��������Ʈ)�� mul0�� ���� �� �ڸ��� ��������ϹǷ�
	sum1 = sum1 >> (WORD_BIT_LEN >> 1); // [line 9] sum1�� �պκ�(�ֻ�����Ʈ ~ �߰���Ʈ)�� ������ֱ�
	mul0 = sum2 + mul0; // [line 10] ���� ����� ���� ���� �� ���� ���� �κ� ���� ����
	if (mul0 < sum2) // [line 11] ���� ���꿡�� carry�� �߻�������,
		carry0 = 1; // [line 12] carry1 = 1
	mul1 = sum1 + mul1 + carry0 + ((word)carry1 << (WORD_BIT_LEN >> 1)); // [line 13] ���� ����� ���� ���� �� ���� ���� �κ� ���� ����

	*C = mul0; // [line 14] ���� ��� ���� ���� ���� �� ������ ����
	*(C + 1) = mul1; // [line 14] ���� ��� ���� ���� ���� �� ������ ����

	return SUCCESS;
}

/**
* @brief Multliplication in Multi Word
* @details

	[pseudo code]
	Input  : C, A, B

	1 : if A = 0 or B = 0 then
	2 :		C <- 0;
	3 : end if
	4 : if A = 1
	5 :		if Sign(A) is NON_NEGATIVE
	6 :			return B
	7 :		else
	8 :			return -B
	10: end if
	11: if B = 1
	12:		if sign(B) is NON_NEGATIVE
	13:			return A
	14:		else
	15:			return -A
	16: end if
	17: for j = 0 to n - 1 do // MULC(|A|, |B|)
	18:		for i = 0 to m - 1 do
	19:			Temp <- A_{j} * B_{i}
	20:			Temp <- Temp << w(i+j)
	21:			C <- ADDC(C, Temp)
	22:		end for
	23: end for
	24: Sign(C) = Sign(A) ^ Sign(B)

* @param bigint** C ���� ���� ���� ������ ����� ������ bigint �� ���� ������ ����
* @param bigint* A ���� ���� ���� ������ ���ϴ� ���� bigint �� ������ ����
* @param bigint* B ���� ���� ���� ������ ���ϴ� ���� bigint �� ������ ����
* @return SUCCESS ���� ��
* @throws
	ERROR
	ERROR A, B, C�� ��ȣ �������� ���� ��
	ERROR A, B, C�� word length�� ����� �ƴ� ���
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

	if ((BI_Is_Zero(&A) & BI_Is_Zero(&B)) == 0) // [line 1] �ǿ������� �ϳ��� 0�̸�,
	{
		BI_Set_Zero(C); // [line 2] ���� ����� 0�̹Ƿ�, ���� �������� �ʰ� 0 ��� ��
		return SUCCESS; // return;ó��
	}

	if (BI_Is_One(&A) == 0) // [line 4]
	{
		BI_Assign(C, B); // C <- 1 * B �̹Ƿ� C�� B assign
		if (sign_a == 0) // [line 5] 1�� ����� +1�̸�,
			(*C)->sign = sign_b; // [line 6] C�� ��ȣ�� B�� ��ȣ�� �����ϰ�
		else // [line 7] 1�� ������ -1�̸�, 
			(*C)->sign = !sign_b; // [line 8] C�� ��ȣ�� B�� ��ȣ�� �ݴ��
		return SUCCESS;
	}

	if (BI_Is_One(&B) == 0) // [line 11]
	{
		BI_Assign(C, A); // C <- A * 1 �̹Ƿ� C�� B assign
		if (sign_a == 0) // [line 12] 1�� ����� +1�̸�,
			(*C)->sign = sign_a; // [line 13] C�� ��ȣ�� B�� ��ȣ�� �����ϰ�
		else // [line 14] 1�� ������ -1�̸�, 
			(*C)->sign = !sign_a; // [line 15] C�� ��ȣ�� B�� ��ȣ�� �ݴ��
		return SUCCESS;
	}

	BI_Get_Word_Length(&size_a, &A); // A�� word ���̸� size_a�� ����
	BI_Get_Word_Length(&size_b, &B); // B�� word ���̸� size_b�� ����

	if ((size_a < 0) | (size_b < 0))
		return ERROR;

	bigint* Temp = NULL; // ���� ���� ���� ������ ����� ������ C�� ���� ������ ������ big integer�� ������
	BI_Get_Word_Length(&size_c, C); // C�� word ���̸� size_r�� ����
	BI_New(&Temp, size_c); // C�� ���� ������ �����ؾ��ϹǷ� C�� ������ wordlen���� ����

	for (i = 0; i < B->wordlen; i++) // [line 17]
	{
		for (j = 0; j < A->wordlen; j++) // [line 18]
		{
			ret = MUL_Word(&Temp->a[i + j], &A->a[j], &B->a[i]); // [line 19, 20] A�� ���Ͽ���� B�� ���Ͽ��� ���� �� Temp�� ���� ���忡 ����
			if (ret == -1)
				return ERROR;
			ret = ADDC_AAB(C, C, &Temp, 0); // [line 21] ���Ͽ��� ������ Temp�� C�� �������� ����
			if (ret == -1)
				return ERROR;
			Temp->a[i + j] = 0; // ���� ���꿡 ���� ���� �κ� �ʱ�ȭ
			Temp->a[i + j + 1] = 0; // ���� ����
		}
	}
	BI_Delete(&Temp); // �Ҵ�� Temp�� delete.
	(*C)->sign = A->sign ^ B->sign; // ����� C�� ��ȣ�� ���� [line 24]
	BI_Refine(*C); // Refine �����ֱ�
	return SUCCESS;
}



/**
 * @brief Karatsuba Multiplication
 * @details

	 [pseudo code]
	 Input  : A, B
	 Output : C = A * B
	  1 : procedure MUL_Karatsuba(A, B)
	  2 :	if flag >= min(WordLen(A), WordLen(B)) then
	  3 :		C <- MUL(A, B)
	  4 :	end if
	  5 :	l <- (max(WordLen(A), WordLen(B)) + 1) >> 1
	  6 :	A1, A0 <- A >> lw, A mod 2^lw
	  7 :	B1, B0 <- B >> lw, B mod 2^lw
	  8 :	T1, T0 <- MUL_Karatsuba(A1, B1), MUL_Karatsuba(A0, B0)
	  9 :	R <- (T1 << 2lw) + T0
	 10 :	S1, S0 <- SUB(A0, A1), SUB(B1, B0)
	 11 :	S <- (-1)^(Sign(S1)^Sign(S2)) * MUL_Karatsuba(|S1|, |S0|)
	 12 :	S <- ADD(S, T1)
	 13 :	S <- ADD(S, T0)
	 14 :	S << lw
	 15 :	R <- ADD(R, S)
	 16 :	C <- R
	 17 : end procedure

 * @param bigint** C ī���߹� ���� ����� ������ bigint �� ���������� ����
 * @param bigint** A ī���߹� ������ ������ bigint �� ���������� ����
 * @param bigint** B ī���߹� ������ ������ bigint �� ���������� ����
 * @return SUCCESS
 * @throws
	ERROR A(B)�� word length �������� ���� ��
	ERROR S0(S1)�� ��ȣ �������� ���� ��
 */
int MUL_Karatsuba(bigint** C, bigint* A, bigint* B)
{
	int i = 0;
	int len, A_Len, B_Len;
	int S0_sign, S1_sign;
	int ret;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A�� ���忭 ����
	BI_Get_Word_Length(&B_Len, &B); // B_Len = B�� ���忭 ����

	// ���� ó��
	if (A_Len < 0)
		return ERROR;
	if (B_Len < 0)
		return ERROR;

	if (FLAG >= MIN(A_Len, B_Len)) // [line 2] A�� ���忭�� ���̿� B�� ���忭�� ���� �� �� ���� ���� flag���� ������
	{
		ret = MUL_Multi(C, A, B); // [line 3] textbook multiplication ����
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

	BI_Right_Shift(A1, len * WORD_BIT_LEN); // [line 6] A1 = A >> len word (A�� len ���常ŭ ���������� �̵�)
	BI_Reduction(&A0, len * WORD_BIT_LEN); // [line 6] A0 = A mod (2^(len * wordlen)) (A�� len ���常ŭ modular ���� ����)
	BI_Right_Shift(B1, len * WORD_BIT_LEN); // [line 7] B1 = A >> len word (A�� len ���常ŭ ���������� �̵�)
	BI_Reduction(&B0, len * WORD_BIT_LEN); // [line 7] B0 = A mod (2^(len * wordlen)) (A�� len ���常ŭ modular ���� ����)

	BI_New(&T0, len * 2);
	BI_New(&T1, len * 2);
	BI_New(&S0, Compare_WordLen(B0, B1)); // S0 = B1 - B0�̴ϱ� B1�� B0 �� �� ū ���� ���̸�ŭ bigint ����
	BI_New(&S1, Compare_WordLen(A0, A1)); // S1 = A0 - A1�̴ϱ� A0�� A1 �� �� ū ���� ���̸�ŭ bigint ����
	BI_New(&R, len * 4);
	BI_New(&S, len * 2);

	ret = MUL_Karatsuba(&T1, A1, B1); // [line 8] T1 = A1 * B1
	if (ret == ERROR)
		return ERROR;
	ret = MUL_Karatsuba(&T0, A0, B0); // [line 8] T0 = A0 * B0
	if (ret == ERROR)
		return ERROR;

	BI_Refine(T1);
	BI_Refine(T0);

	bigint* T1_tmp = NULL;
	BI_Assign(&T1_tmp, T1);
	BI_Left_Shift(T1_tmp, 2 * WORD_BIT_LEN * len); // T1_tmp = T1�� 2 * len ���常ŭ ���������� �̵�

	// [line 9] R = T1 || T0
	for (i = 0; i < T1->wordlen; i++)
		R->a[len * 2 + i] = T1_tmp->a[T1_tmp->wordlen - T1->wordlen + i];
	for (i = 0; i < T0->wordlen; i++)
		R->a[i] = T0->a[i];

	BI_Refine(R);

	ret = SUB(&S1, A0, A1); // [line 10] S1 = A0 - A1
	if (ret == ERROR)
		return ERROR;

	ret = SUB(&S0, B1, B0); // [line 10] S0 = B1 - B0
	if (ret == ERROR)
		return ERROR;

	S0_sign = BI_Get_Sign(S0);
	S1_sign = BI_Get_Sign(S1);

	// ���� ó��
	if (S0_sign == ERROR)
		return ERROR;
	if (S1_sign == ERROR)
		return ERROR;

	// S0, S1�� ������ �� �����Ƿ� ���� ���� �ֱ�
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
		return ERROR;
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

	ret = ADD(&ADD_result1, &S, &T1); // [line 12] ADD_result1 = S + T1
	if (ret == ERROR)
		return ERROR;
	ret = ADD(&ADD_result2, &ADD_result1, &T0); // [line 13] ADD_result2 = ADD_result1 + T0 = S + T1 + T0
	if (ret == ERROR)
		return ERROR;
	BI_Left_Shift(ADD_result2, len * WORD_BIT_LEN); // [line 14] ADD_result2 << len ����
	ret = ADD_AAB(C, &R, &ADD_result2); // [line 15] C = R + ADD_result2
	if (ret == ERROR)
		return ERROR;

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
	flag�� ���� ���� ���� ����(Karatsuba sqauring / textbook sqauring)
	Input  : A
	Output : C = A ^ 2
 * @param bigint** C ���� ������ ������ ����� ������ bigint �� ���������� ����
 * @param bigint* A ���� ������ ������ bigint �� ������ ����
 * @return SUCCESS
 * @throws
	ERROR A�� bigint ����ü ���Ҵ� ��
	ERROR A�� word length�� ����� �ƴ� ���
	ERROR flag ������ ��
 */
int Squaring(bigint** C, bigint* A)
{
	int A_Len = 0;
	int ret;

	// ���� ó��
	if (A == NULL)
		return ERROR;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A�� ���忭 ����

	// ���� ó��
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
	else // ���� ó��
		return ERROR;

	return SUCCESS;
}

/**
 * @brief Single Squaring
 * @details

	 [pseudo code]
	 Input  : A
	 Output : C = A ^ 2
	  1 : A1, A0 <- |A| >> w/2, |A| mod 2^(w/2)
	  2 : C1, C0 <- A1^2, A0^2
	  3 : C <- (C1 << w) + C0
	  4 : T <- A0 * A1
	  5 : T <- T << (w/2 + 1)
	  6 : C <- C + T

 * @param bigint* C ���� ������ ������ ����� ������ bigint �� ������ ����
 * @param bigint* A ���� ������ ������ bigint �� ������ ����

 * @return SUCCESS
 * @throws
	ERROR A�� ��ȣ �������� ���� ��
 */
int Single_Squaring(bigint* C, bigint* A)
{
	int A_Sign = 0;
	int ret;

	bigint* A1 = NULL;
	bigint* A0 = NULL;
	bigint* C1 = NULL;
	bigint* C0 = NULL;
	bigint* T = NULL;

	A_Sign = BI_Get_Sign(A);

	if (A_Sign == ERROR) // ���� ó��
		return ERROR;

	BI_Assign(&A1, A);
	BI_Assign(&A0, A);

	BI_New(&C1, 1);
	BI_New(&C0, 1);
	BI_New(&T, 2);

	// A1 = |A| >> WORD_BIT_LEN / 2, A0 = |A| mod (WORD_BIT_LEN / 2) �� ���� |A| ó��
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

	ret = MUL_Multi(&T, A0, A1); // [line 4] T = A0 * A1
	if (ret == ERROR)
		return ERROR;
	BI_Left_Shift(T, (WORD_BIT_LEN / 2) + 1); // [line 5] T = T << ((WORD_BIT_LEN / 2) + 1)

	ret = ADD_AAB(&C, &C, &T); // [line 6] C = C + T
	if (ret == ERROR)
		return ERROR;

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

	 [pseudo code]
	 Input  : A
	 Output : C = A ^ 2
	  1 : C1, C2 <- 0
	  2 : for j = 0 to WordLen(A) - 1 do
	  3 :	T1 <- A[j] * A[j]
	  4 :	T1 <- T1 << 2jw
	  5 :	C1 <- T1 + C1
	  6 :	 for i = j + 1 to WordLen(A) - 1 do
	  7 :		T2 = A[j] * A[i]
	  8 :		T2 <- T2 << (i + j)w
	  9 :		C2 <- ADD(C2, T2)
	 10 :	end for
	 11 : end for
	 12 : C2 <- C2 << 1
	 13 : C <- ADD(C1, C2)

 * @param bigint** C ���� ������ ������ ����� ������ bigint �� ���������� ����
 * @param bigint* A ���� ������ ������ bigint �� ������ ����

 * @return SUCCESS
 * @throws
	ERROR A�� word length�� ����� �ƴ� ���
	ERROR bigint ����ü ���ο� �ִ� �迭 ���Ҵ� ���� ��
 */
int SQUC(bigint** C, bigint* A)
{
	int i, j, k, m;
	int wordlen_A;
	int len, c2_len;
	int ret;

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
	if (wordlen_A <= 0)
		return ERROR;

	BI_New(C, wordlen_A * 2 + 1);
	BI_New(&C1, 2 * wordlen_A); // [line 1] ũ�⸸ ũ�� ��� 0���� ����
	BI_New(&C2, 2 * wordlen_A); // [line 1] ũ�⸸ ũ�� ��� 0���� ����

	for (j = 0; j < wordlen_A; j++)
	{
		BI_New(&T1, 2);
		A_j->a[0] = A->a[j];
		ret = Single_Squaring(T1, A_j); // [line 3]
		if (ret == ERROR)
			return ERROR;
		BI_Assign(&temp1, T1);
		BI_Left_Shift(temp1, (int)(2 * j * WORD_BIT_LEN)); // [line 4] T1 = T1 << ((2 * j) * WORD_BIT_LEN)

		// [line 5] C1 = T1 || C1
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

		for (i = j + 1; i < wordlen_A; i++) // [line 6]
		{
			BI_New(&T2, 2);
			A_i->a[0] = A->a[i];
			ret = MUL_Multi(&T2, A_j, A_i); // [line 7]
			if (ret == ERROR)
				return ERROR;
			BI_Assign(&temp2, T2);
			BI_Left_Shift(temp2, (int)((i + j) * WORD_BIT_LEN)); // [line 8]

			c2_len = C2->wordlen;

			len = MAX(C2->wordlen, temp2->wordlen) + 1; // C2 = C2 + temp2�� �ϱ� ���� C2�� temp2 �� �� �� ���忭�� ���� ���ϱ�
			C2->wordlen = len;
			temp = (word*)realloc(C2->a, sizeof(word) * len); // new_wordlen��ŭ bigint ����ü ���Ҵ�
			if (temp != NULL)
				C2->a = temp;
			else
				return ERROR;

			for (m = c2_len; m < len; m++)
				C2->a[m] = 0; // �þ ���̸�ŭ 0���� �ʱ�ȭ

			ret = ADD_AAB(&C2, &C2, &temp2); // [line 9]
			if (ret == ERROR)
				return ERROR;
			BI_Refine(C2);

			array_init(T2->a, T2->wordlen);
			BI_Delete(&temp2);
			BI_Delete(&T2);
		}
	}

	BI_Left_Shift(C2, 1); // [line 12] C2 = C2 << 1
	ret = ADD(C, &C1, &C2); // [line 13] C = C1 + C2
	if (ret == ERROR)
		return ERROR;

	BI_Delete(&C1);
	BI_Delete(&C2);
	BI_Delete(&A_i);
	BI_Delete(&A_j);
	BI_Delete(&temp1);
	BI_Delete(&temp2);

	return SUCCESS;
}

/**
 * @brief Squaring
 * @details

	 [pseudo code]
	 Input  : A
	 Output : C = A ^ 2
	  1 : if A = 0 or A = 1 or A = -1 then
	  2 :	C <- |A|
	  3 : end if
	  4 : C <- SQUC(A)

 * @param bigint** C ���� ������ ������ ����� ������ bigint �� ���������� ����
 * @param bigint* A ���� ������ ������ bigint �� ������ ����

 * @return SUCCESS
 * @throws
	ERROR A�� ��ȣ �������� ���� ��
 */
int SQU(bigint** C, bigint* A)
{
	int flag0; // A�� 0���� �Ǻ��ϴ� flag
	int flag1; // A�� 1���� �Ǻ��ϴ� flag
	int flag2 = -1; // A�� ������ �� -1���� �Ǻ��ϴ� flag
	int sign;
	int ret;

	bigint* temp = NULL;

	BI_Assign(&temp, A);

	sign = BI_Get_Sign(A);
	if (sign == ERROR)
		return ERROR;

	if (sign == NEGATIVE) // A�� ��ȣ�� ������ ���
	{
		BI_Flip_Sign(temp); // ��ȣ �ٲ� ��
		flag2 = BI_Is_One(&temp); // 1���� �Ǻ� (��, -1���� �Ǻ�)
	}

	flag0 = BI_Is_Zero(&A); // A�� 0���� �Ǻ�
	flag1 = BI_Is_One(&A); // A�� 1���� �Ǻ�

	if (flag0 == 0) // [line 1] A = 0�� ���, A^2 = 0 return
	{
		BI_Assign(C, A); // [line 2]
		BI_Delete(&temp);

		return SUCCESS;
	}
	if (flag1 == 0) // [line 1] A = 1�� ���, A^2 = 1 return
	{
		BI_Assign(C, A); // [line 2]
		BI_Delete(&temp);

		return SUCCESS;
	}
	if (flag2 == 0) // [line 1] A = -1�� ���, A^2 = 1 return
	{
		BI_Assign(C, temp); // [line 2]
		BI_Delete(&temp);

		return SUCCESS;
	}

	BI_Delete(&temp);
	BI_New(C, A->wordlen * 2 + 1);
	ret = SQUC(C, A); // A = 0. -1, 1�� �ƴ� ��� SQUC ����
	if (ret == ERROR)
		return ERROR;

	return SUCCESS;
}

/**
 * @brief Karatsuba Squaring
 * @details

	 [pseudo code]
	 Input  : A
	 Output : C = A ^ 2
	  1 : procedure SQUC_Karatsuba(A, B)
	  2 :	if flag >= WordLen(A) then
	  3 :		C <- SQUC(A)
	  4 :	end if
	  5 :	l <- (WordLen(A) + 1) >> 1
	  6 :	A1, A0 <- |A| >> lw, |A| mod 2^lw
	  7 :	T1, T0 <- SQUC_Karatsuba(A1), SQUC_Karatsuba(A0)
	  8 :	R <- (T1 << 2lw) + T0
	  9 :	S1 <- MUL_Karatsuba(A1, A0)
	 10 :	S <- S << (lw + 1)
	 11 :	R <- ADDC(R, S)
	 12 :	C <- R
	 13 : end procedure

 * @param bigint** C ���� ������ ������ ����� ������ bigint �� ���������� ����
 * @param bigint* A ���� ������ ������ bigint �� ������ ����

 * @return SUCCESS
 * @throws
	ERROR A�� word length�� ����� �ƴ� ���
 */
int SQUC_Karatsuba(bigint** C, bigint* A)
{
	int i;
	int len, A_Len;
	int ret;

	BI_Get_Word_Length(&A_Len, &A); // A_Len = A�� ���忭 ����
	if (A_Len <= 0)
		return ERROR;

	if (FLAG >= A_Len) // [line 2] A�� ���忭�� ���̿� B�� ���忭�� ���� �� �� ���� ���� flag���� ������
	{
		ret = SQUC(C, A); // [line 3] textbook squaring ����
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
		return ERROR;
	ret = SQUC_Karatsuba(&T0, A0); // [line 7]
	if (ret == ERROR)
		return ERROR;

	BI_Refine(T1);
	BI_Refine(T0);

	bigint* T1_tmp = NULL;
	BI_Assign(&T1_tmp, T1);
	BI_Left_Shift(T1_tmp, 2 * WORD_BIT_LEN * len); // T1_tmp = T1�� 2 * len ���常ŭ ���������� �̵�

	// [line 8] R = T1 || T0
	for (i = 0; i < T1->wordlen; i++)
		R->a[len * 2 + i] = T1_tmp->a[T1_tmp->wordlen - T1->wordlen + i];
	for (i = 0; i < T0->wordlen; i++)
		R->a[i] = T0->a[i];

	BI_Refine(R);

	ret = MUL_Karatsuba(&S, A1, A0); // [line 9] S = A1 * A0
	if (ret == ERROR)
		return ERROR;
	BI_Refine(S);

	BI_Left_Shift(S, len * WORD_BIT_LEN + 1); // [line 10] S << len * WORD_BIT_LEN + 1 ��Ʈ��ŭ �������� �̵�
	ret = ADDC(C, &R, &S, R->sign); // [line 11] C = R + S
	if (ret == ERROR)
		return ERROR;

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
	flag�� ���� ������ ���� ����(binary long division / multi-precision long division)
	Input  : A, B
	Output : Q, R (A = B * Q + R)
 * @param bigint** Q ������ ������ ������ ��� �� ���� ������ bigint �� ���������� ����
 * @param bigint** R ������ ������ ������ ��� �� ������ ������ bigint �� ���������� ����
 * @param bigint* A ������ ���꿡�� �������� ���� bigint �� ������ ����
 * @param bigint* B ������ ���� bigint �� ������ ����
 * @return SUCCESS
 * @throws
	ERROR flag ������ ��
	ERROR B > A�� ���
 */
int Division(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int size = 0;
	int ret;

	size = BI_Compare(&A, &B); // A�� B�� ũ�� ��

	if (size == ERROR)
		return ERROR;

	if (size == 2)
	{
		BI_Set_Zero(Q);
		BI_Assign(R, A);
		return ERROR;
	}

	if (BI_Is_Zero(&B) == 0) // ������ ���� 0 �̹Ƿ�
	{
		return ERROR;
	}

	if (A->sign | B->sign) // A, B > 0 �� ������ �������� ���� ��
	{
		return ERROR;
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
		return ERROR;

	return SUCCESS;
}


/**
* @brief Binary_Long_Division Algorithm
* @details
	[pseudo code]
	Input  : Q, R, A, B

	1 : (Q, R) <- (0, 0)
	2 : for j = n -1 downto 0 do
	3 :		R <- 2 * R + a_{j}
	4 :		if R >= B then
	5 :			Q <- Q + 2^j
	6 :			R <- R - B
	7 :		end if
	8 :	end for
	9 :	return (Q, R)

* @param bigint** Q Binary Long Divsion ������ �� ���� ����� ������ bigint �� ���� ������ ����
* @param bigint* R Binary Long Divsion ������ �������� ���� ����� ������ bigint �� ���� ������ ����
* @param bigint* A Binary Long Divsion ������ �������� ���� bigint �� ������ ����
* @param bigint* B Binary Long Divsion ������ ������ ���� bigint �� ������ ����
*/
int Binary_Long_Div(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int i = 0;
	word temp = 0;
	int size = 0;
	int len = 0;
	int result = 0;
	int ret = 0;

	bigint* T = NULL;
	bigint* U = NULL;
	BI_New(&T, 1);
	BI_Set_Zero(Q); // [line 1]
	BI_Set_Zero(R); // [line 1]

	BI_Get_Word_Length(&size, &A); // size == bigint A's word length.

	size = size * WORD_BIT_LEN; // ���� size �� A�� �� ��Ʈ ����
	for (i = size - 1; i >= 0; i--) // [line 2] �ֻ��� ��Ʈ����
	{
		len = (int)(i / WORD_BIT_LEN); // len���� ���� �����ϰ� �ִ� ��Ʈ�� ���� ��ġ
		temp = A->a[len]; // ���� ���� ���� ������ ���� temp�� ����
		len = i % WORD_BIT_LEN; // �ش� ���忡 �� ��° ��Ʈ����
		temp = temp >> len; // �ش� ��Ʈ�� BI_Left shift�ؼ� ù��° ��Ʈ�� ���̵���
		temp = temp & 1; // 1�� & �ؼ� 
		T->a[0] = temp; // �ش� ��Ʈ�� ���� ���� // j_th_BI_Get_Bit_Length �ٲ㼭 �ϱ�

		BI_Left_Shift(*R, 1); // [line 3] 2R
		ret = ADD_DIV(R, R, &T); // [line 3] 2R + a{j}
		if (ret == -1)
			return ERROR;
		result = BI_Compare(&B, R); // R = B -> 0, R > B -> -1�̹Ƿ�, result ���� 0, -1�� �����Ѵ�. 
		if ((result == 0) | (result == 2)) // [line 3] R >= B���� ��
		{
			len = (int)(i / WORD_BIT_LEN) + 1;
			BI_New(&U, len);

			U->a[len - 1] = (word)1 << (i % WORD_BIT_LEN); // make 2 ^ j
			ret = ADD_DIV(Q, Q, &U); // [line 5] Q <- Q + 2 ^ j
			if (ret == -1)
				return ERROR;
			ret = SUB(R, *R, B); // [line 6] R <- R - B 
			if (ret == -1)
				return ERROR;
			BI_Delete(&U); // �������� ��ѹ� U(2 ^ j)�� delete ���ش�.
		}
	}
	BI_Refine(*Q); // Q�� refine �����ֱ�
	BI_Delete(&T); // �Ҵ��� ��ѹ� T delete.
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
	temp = (word*)realloc((*B)->a, sizeof(word) * A_Len);
	if (temp != NULL)
		(*B)->a = temp;

	memset(&(*B)->a[B_Len], 0x00, A_Len - B_Len);

	carry = 0;

	for (i = 0; i < A_Len; i++)
		carry = ADD_ABc(C, A, B, carry, i);

	if (carry == 1)
		(*C)->a[A_Len] = 1;

	BI_Refine(*B);
	//BI_Refine(*C);

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
			BI_New(C, B_Len); // Binary Long Divison���� �ʿ�

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
		ret = SUB(C, *A, temp); // SUB �Լ�
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
		ret = SUB(C, *B, temp); // SUB �Լ�
		if (ret == -1)
			return ERROR;

		BI_Delete(&temp);

		return SUCCESS;
	}

	// A, B�� ������ ��ȣ�� ��
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

	[pseudo code]
	Input  : Q, R, A, B

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

* @param bigint** Q Multi Long Divsion ������ �� ���� ����� ������ bigint �� ���� ������ ����
* @param bigint* R Multi Long Divsion ������ �������� ���� ����� ������ bigint �� ���� ������ ����
* @param bigint* A Multi Long Divsion ������ �������� ���� bigint �� ������ ����
* @param bigint* B Multi Long Divsion ������ ������ ���� bigint �� ������ ����
*/
int DIV(bigint** Q, bigint** R, bigint* A, bigint* B) // Long Division Algorithm(Multi-precision version)���� DIV(A, B) fucntion.
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
		return ERROR; // return INVALID.
	result = BI_Compare(&A, &B);
	if (result == 2) // [line 4] B > A �� ���, Compare(&A, &B)�� return : 2
	{
		BI_Assign(R, A);
		return SUCCESS;
	}

	for (i = len - 1; i >= 0; i--) // line 8.
	{
		BI_Set_Zero(&Word); // ���̰� 1�̰� ���� 0�� ��ѹ� Word ����. BI_New(&Word, 1)�� ����.
		BI_New(&Temp, 1); // A�� j��° ����(A_{j})�� ��� ��ѹ� Temp�� ���� ���̰� 1�� ����. 
		Temp->a[0] = A->a[i]; // Temp�� A_{j}�� ����.
		BI_Left_Shift(*R, WORD_BIT_LEN); // R�� �������� W��ŭ shift ==> R * W
		ADD_DIV(R, R, &Temp); // [line 9] R * W�� A_{j}�� ���� ���� ����. 
		DIVC(&Word, R, *R, B); //��ѹ� Word�� Q_{i}�� �����ϴ� ����. 
		BI_Delete(&Temp); // �ݺ��� ������ ������ ��ѹ� Temp delete.
		(*Q)->a[i] = Word->a[0]; // [line 10] Q�� Q_{i}�� ����. 
		BI_Delete(&Word); // ��ѹ� Word delete
	}

	BI_Refine(*Q);

	return SUCCESS;
}

/*
	Long Division Algorithm Core function (Multi-precision version)

	[pseudo code]
	Input  : Q, R, A, B

	1 : if A < B then
	2 :		return (0, A)
	3 :	end if
	4 :	Compute k��Z+ such that 2^k B_{m-1}��[2^(w-1), 2^w)
	5 :	A', B' <- 2^k * A, 2^k * B
	6 :	Q', R' <- DIVCC(A', B')
	7 : Q, R <- Q', 2^(-k) * R'
	8 : return (Q, R)

* @param bigint** Q DIV( )�� �� ���� ����� ������ bigint �� ���� ������ ����
* @param bigint* R DIV( )�� �������� ���� ����� ������ bigint �� ���� ������ ����
* @param bigint* A DIV( )�� �������� ���� bigint �� ������ ����
* @param bigint* B DIV( )�� ������ ���� bigint �� ������ ����

*/
int DIVC(bigint** Q, bigint** R, bigint* A, bigint* B)
{
	int result = 0; // A, B ��Һ񱳰��� ���� ���� result
	//int i = 0; // �ݺ����� ����� ���� i
	int k = 0; // line 4�� ���õ� ���� k
	int len = 0; // B�� wordlen�� �������� ���� len
	bigint* AP = NULL; // A�� BI_Left shift �ؼ� ������ ��ѹ� A'(AP, A Prime)
	bigint* BP = NULL; // B�� BI_Left shift �ؼ� ������ ��ѹ� B'(BP, B Prime)

	result = BI_Compare(&A, &B);
	if (result == 2) // A < B ���� Ŭ ��
	{
		return SUCCESS;
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
		BI_Left_Shift(comp, 1); // comp�� BI_Left shift ==> comp * 2
		k++; // comp * 2^k �ϵ���.
		if (comp->wordlen == 2) // 2^w�� �Ѿ��,
		{
			k--; // �Ѿ���� �ϳ� �ٿ��ְ�
			break; // break;
		}
	}

	BI_Left_Shift(AP, k); // AP = AP << k 
	BI_Left_Shift(BP, k); // BP = BP << k //line 5. 
	DIVCC(Q, R, AP, BP); // [line 6]
	BI_Right_Shift(*R, k); // [line 7] (Q' == Q)
	BI_Delete(&AP); // �������� ��ѹ� AP delete.
	BI_Delete(&BP); // �������� ��ѹ� BP delete.
	BI_Delete(&comp); // �������� ��ѹ� comp delete.

	return SUCCESS;
}

/*
	Long Division Algorithm Core's Core function (Multi-precision version)

	[pseudo code]
	Input  : Q, R, A, B

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

* @param bigint** Q DIVC( )�� �� ���� ����� ������ bigint �� ���� ������ ����
* @param bigint* R DIVC( )�� �������� ���� ����� ������ bigint �� ���� ������ ����
* @param bigint* A DIVC( )�� �������� ���� bigint �� ������ ����
* @param bigint* B DIVC( )�� ������ ���� bigint �� ������ ����
*/
int DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B) // 7.2.3 DIVCC(A, B)
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
	while ((*R)->sign & 1) // [line 12] R�� ��ȣ�� ������ �� 1 & 1 = 1  ( Q, Q - 1, Q - 2)
	{
		SUB(Q, *Q, one); // Q = Q - 1.
		ADD_AAB(R, R, &B); // R = R + B. 1st arg, 2nd arg�� �����Ƿ� ADD_AAB �Լ� ���
	}
	BI_Delete(&one); // �������� ��ѹ� one delete.
	BI_Delete(&BQ); // �������� ��ѹ� BQ delete.

	return SUCCESS;
}

/*
	Long Division Algorithm Core's Core's condition (if n == m) (Multi-precision version)

	[pseudo code]
	Input  : Q, A, B, m

	1 : Q <- A_{m-1} / B_{m-1}
	2 : Q <- LowerBound(Q)

* @param bigint** Q DIVCC()���� Q hat�� bigint �� ���� ������ ����
* @param bigint* A DIVCC()���� �������� ���� A�� bigint �� ������ ����
* @param bigint* B DIVCC()���� �������� ���� B�� bigint �� ������ ����
* @param int m DIVCC()���� B�� wordlen�� ��Ÿ���� int�� ����
*/
int DIVCC_n_m(bigint** Q, bigint* A, bigint* B, int m) // DIVCC ���� if(n == m) �� ��
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

* @param bigint** Q DIVCC()���� Q hat�� bigint �� ���� ������ ����
* @param bigint* A DIVCC()���� �������� ���� A�� bigint �� ������ ����
* @param bigint* B DIVCC()���� �������� ���� B�� bigint �� ������ ����
* @param int m DIVCC()���� B�� wordlen�� ��Ÿ���� int�� ����
*/
int DIVCC_n_m1(bigint** Q, bigint* A, bigint* B, int m) // DIVCC ���� if(n == m + 1) �� ��
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

	return SUCCESS;
}
/**
 * @brief Modular exponentiation of multiplication
 * @details
	flag�� ���� ��ⷯ ������ ���� ����(BI_Left to BI_Right / BI_Right to BI_Left / Montgomery Ladder)
	Input  : X, N, M
	Output : T = X ^ N (mod M)
 * @param bigint** T ��ⷯ ������ ������ ������ ����� ������ bigint �� ���������� ����
 * @param bigint* X ��ⷯ ������ ������ ������ bigint �� ������ ����
 * @param bigint* N ��ⷯ ������ ���꿡���� ������ ����Ű�� bigint �� ������ ����
 * @param bigint* M ��ⷯ ������ ���꿡���� ��ⷯ�� ������ bigint �� ������ ����
 * @return SUCCESS
 * @throws
	ERROR flag ������ ��
	ERROR ������ ������ ���
	ERROR ��ⷯ ���ϴ� ���� 0�� ���
 */
int Modular_Exponentiation_MUL(bigint** T, bigint* X, bigint* N, bigint* M)
{
	if (N->sign == NEGATIVE) // ���� ó�� (������ �����)
		return ERROR;

	if (BI_Is_Zero(&M) == 0) // mod 0�� ��
		return ERROR;

	if (BI_Is_One(&M) == 0) // mod 1 �� ��
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
	flag�� ���� ��ⷯ exponentiation ���� ���� ����(BI_Left to BI_Right / BI_Right to BI_Left / Montgomery Ladder)
	Input  : X, N, M
	Output : T = X * N (mod M)
 * @param bigint** T ��ⷯ exponentiation ���� ������ ������ ����� ������ bigint �� ���������� ����
 * @param bigint* X ��ⷯ exponentiation ���� ������ ������ bigint �� ������ ����
 * @param bigint* N ��ⷯ exponentiation ���� ���꿡���� ����(������ Ƚ��)�� ����Ű�� bigint �� ������ ����
 * @param bigint* M ��ⷯ exponentiation ���� ���꿡���� ��ⷯ�� ������ bigint �� ������ ����
 * @return SUCCESS
 * @throws
	ERROR flag ������ ��
	ERROR �����ϴ� Ƚ���� ������ ��� (N < 0)
	ERROR ��ⷯ ���ϴ� ���� 0�� ���
 */
int Modular_Exponentiation_ADD(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int ret;

	if (N->sign == NEGATIVE)
		return ERROR;

	if (BI_Is_Zero(&M) == 0) // mod 0�� ��
		return ERROR;

	if (BI_Is_One(&M) == 0) // mod 1 �� ��
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
	[pseudo code]
	Input  : X, N
	Output : T = X ^ N

	1 : t0, t1 <- 1, X
	2 : for i <- BitLen(N) - 1 downto 0 do
	3 :		t(1-(N_i)) <- t0 * t1
	4 :		t(N_i) <- t(N_i) ^ 2
	5 :	end for
	6  T <- t0

 * @param bigint** T ������ ������ ������ ����� ������ bigint �� ���������� ����
 * @param bigint* X ������ ������ ������ bigint �� ������ ����
 * @param bigint* N ������ ���꿡���� ������ ����Ű�� bigint �� ������ ����
 * @return SUCCESS
 * @throws
	ERROR ������ ������ ���
	ERROR N�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
 */
int EXP_Montgomery_MUL(bigint** T, bigint* X, bigint* N)
{
	int i, bit;
	int N_bit_len;
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL; // t0�� ������ �ӽ� ����
	bigint* temp1 = NULL; // t1�� ������ �ӽ� ����

	if (N->sign == NEGATIVE) // ���� ó�� (������ �����)
		return ERROR;

	BI_Get_Bit_Length(&N_bit_len, N); // N�� ��Ʈ�� ���� -> N_bit_len

	BI_Set_One(&t0); // [line 1] t0 = 1
	BI_Assign(&t1, X); // [line 1] t1 = A

	for (i = N_bit_len - 1; i >= 0; i--) // [line 2]
	{
		bit = BI_Get_j_th_Bit_of_BI(i, N); // N�� i��° ��Ʈ�� 1���� 0���� �Ǻ�

		if (bit == 1) // N�� i��° ��Ʈ�� 1�� ���
		{
			// [line 3]
			ret = Multiplication(&temp0, t0, t1); // temp0 = t0 * t1
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 ����)
			BI_Delete(&temp0);

			// [line 4]
			ret = Squaring(&temp1, t1); // temp1 = t1^2
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 ����)
			BI_Delete(&temp1);

		}
		else if (bit == 0) // N�� i��° ��Ʈ�� 0�� ���
		{
			// [line 3]
			ret = Multiplication(&temp1, t0, t1); // temp1 = t0 * t1
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 ����)
			BI_Delete(&temp1);

			// [line 4]
			ret = Squaring(&temp0, t0); // temp0 = t0^2
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 ����)
			BI_Delete(&temp0);
		}
		else // N�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
			return ERROR; // ���� ó��
	}

	BI_Assign(T, t0); // ������ t0 �� T�� �Ҵ�

	BI_Delete(&t0);
	BI_Delete(&t1);

	return SUCCESS;
}

/**
 * @brief Exponentiation of addition used Montgomery Ladder
 * @details
	[pseudo code]
	Input  : X, N
	Output : T = X ^ N

	1 : t0, t1 <- 0, X
	2 : for i <- BitLen(N) - 1 downto 0 do
	3 :		t(1-(N_i)) <- t0 + t1
	4 :		t(N_i) <- 2 * t(N_i)
	5 :	end for
	6  T <- t0

 * @param bigint** T exponentiation ���� ������ ������ ����� ������ bigint �� ���������� ����
 * @param bigint* X exponentiation ���� ������ ������ bigint �� ������ ����
 * @param bigint* N exponentiation ���� ���꿡���� ����(������ Ƚ��)�� ����Ű�� bigint �� ������ ����
 * @return SUCCESS
 * @throws
	ERROR ���ϴ� Ƚ���� ������ ���
	ERROR N�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
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
	bigint* temp0 = NULL; // t0�� ������ �ӽ� ����
	bigint* temp1 = NULL; // t1�� ������ �ӽ� ����

	if (N->sign == NEGATIVE) // ���� ó�� (�����)
		return ERROR;

	BI_Get_Bit_Length(&N_bit_len, N); // N�� ��Ʈ�� ���� -> N_bit_len

	// [line 1]
	BI_Set_Zero(&t0); // t0 = 0
	BI_Assign(&t1, X); // t1 = A

	for (i = N_bit_len - 1; i >= 0; i--) // [line 2]
	{
		bit = BI_Get_j_th_Bit_of_BI(i, N); // N�� i��° ��Ʈ�� 1���� 0���� �Ǻ�

		if (bit == 1) // N�� i��° ��Ʈ�� 1�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 + t1�� �����ϱ� ���� bigint ����

			// [line 3]
			ret = ADD(&temp0, &t0, &t1); // temp0 = t0 + t1
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t0);
			BI_Assign(&t0, temp0); // t0 = temp0 (t0 ����)
			BI_Delete(&temp0);

			BI_Left_Shift(t1, 1); // [line 4] t1 = t1 << 2

		}
		else if (bit == 0) // N�� i��° ��Ʈ�� 0�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			BI_New(&temp1, MAX(t0_len, t1_len) + 1);  // temp1(t1) = t0 + t1�� �����ϱ� ���� bigint ����

			// [line 3]
			ret = ADD(&temp1, &t0, &t1); // temp1 = t0 + t1
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t1);
			BI_Assign(&t1, temp1); // t1 = temp1 (t1 ����)
			BI_Delete(&temp1);

			BI_Left_Shift(t0, 1); // [line 4] t0 = t0 << 2
		}
		else // N�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
			return ERROR; // ���� ó��
	}

	BI_Assign(T, t0); // ������ t0 �� T�� �Ҵ�

	BI_Delete(&t0);
	BI_Delete(&t1);

	return SUCCESS;
}

/**
 * @brief Modular Exponentiation of multiplication used Montgomery Ladder
 * @details
	[pseudo code]
	Input  : X, N
	Output : T = X ^ N (mod M)

	1 : t0, t1 <- 1, X
	2 : for i <- BitLen(N) - 1 downto 0 do
	3 :		t(1-(N_i)) <- t0 * t1 (mod M)
	4 :		t(N_i) <- t(N_i) ^ 2 (mod M)
	5 :	end for
	6  T <- t0

 * @param bigint** T ��ⷯ ������ ������ ������ ����� ������ bigint �� ���������� ����
 * @param bigint* X ��ⷯ ������ ������ ������ bigint �� ������ ����
 * @param bigint* N ��ⷯ ������ ���꿡���� ������ ����Ű�� bigint �� ������ ����
 * @param bigint* M ��ⷯ ������ ���꿡���� ��ⷯ�� ������ bigint �� ������ ����
 * @return SUCCESS ���� ��
 * @throws
	ERROR ������ ������ ���
	ERROR ���� ������ ���
	ERROR ��ⷯ ���ϴ� ���� ������ ���
	ERROR N�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
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
	bigint* temp0 = NULL; // t0�� ������ �ӽ� ����
	bigint* temp1 = NULL; // t1�� ������ �ӽ� ����

	bigint* Q = NULL;
	bigint* R = NULL;

	if (N->sign == NEGATIVE) // ���� ó��
		return ERROR;
	if (M->sign == NEGATIVE)
		return ERROR;
	if (X->sign == NEGATIVE)
		return ERROR;

	BI_Get_Bit_Length(&N_bit_len, N); // N�� ��Ʈ�� ���� -> N_bit_len

	// [line 1]
	BI_Set_One(&t0); // t0 = 1
	BI_Assign(&t1, X); // t1 = X

	for (i = N_bit_len - 1; i >= 0; i--) // [line 2]
	{
		bit = BI_Get_j_th_Bit_of_BI(i, N); // N�� i��° ��Ʈ�� 1���� 0���� �Ǻ�

		if (bit == 1) // N�� i��° ��Ʈ�� 1�� ���
		{
			// [line 3]
			ret = Multiplication(&temp0, t0, t1); // temp0 = t0 * t1
			if (ret == ERROR)
				return ERROR;
			ret = Division(&Q, &R, temp0, M); // R = temp0 (mod M)
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 ����)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);

			// [line 4]
			ret = Squaring(&temp1, t1); // temp1 = t1^2
			if (ret == ERROR)
				return ERROR;
			ret = Division(&Q, &R, temp1, M); // R = temp1 (mod M)
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 ����)
			BI_Delete(&temp1);
			BI_Delete(&Q);
			BI_Delete(&R);

		}
		else if (bit == 0) // N�� i��° ��Ʈ�� 0�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			// [line 3]
			ret = Multiplication(&temp1, t0, t1); // temp1 = t0 * t1
			if (ret == ERROR)
				return ERROR;
			ret = Division(&Q, &R, temp1, M); // R = temp1 (mod M)
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 ����)
			BI_Delete(&temp1);
			BI_Delete(&Q);
			BI_Delete(&R);

			// [line 4]
			ret = Squaring(&temp0, t0); // temp0 = t0^2
			if (ret == ERROR)
				return ERROR;
			ret = Division(&Q, &R, temp0, M); // R = temp0 (mod M)
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 ����)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);
		}
		else // N�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
			return ERROR; // ���� ó��
	}

	BI_Assign(T, t0); // ������ t0 �� T�� �Ҵ�

	BI_Delete(&t0);
	BI_Delete(&t1);

	return SUCCESS;
}


/**
 * @brief Modular Exponentiation of addition used Montgomery Ladder
 * @details
	[pseudo code]
	Input  : X, N
	Output : T = X ^ N (mod M)

	1 : t0, t1 <- 0, X
	2 : for i <- BitLen(N) - 1 downto 0 do
	3 :		t(1-(N_i)) <- t0 + t1 (mod M)
	4 :		t(N_i) <- 2 * t(N_i) (mod M)
	5 :	end for
	6  T <- t0

 * @param bigint** T ��ⷯ ���� ���� ������ ������ ����� ������ bigint �� ���������� ����
 * @param bigint* X ��ⷯ ���� ���� ������ ������ bigint �� ������ ����
 * @param bigint* N ��ⷯ ���� ���� ���꿡���� ����(������ Ƚ��)�� ����Ű�� bigint �� ������ ����
 * @param bigint* M ��ⷯ ���� ���꿡���� ��ⷯ�� ������ bigint �� ������ ����
 * @return SUCCESS ���� ��
 * @throws
	ERROR ���ϴ� Ƚ���� ������ ���
	ERROR ���ϴ� ����� ������ ���
	ERROR ��ⷯ ���ϴ� ���� ������ ���
	ERROR N�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
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
	bigint* temp0 = NULL; // t0�� ������ �ӽ� ����
	bigint* temp1 = NULL; // t1�� ������ �ӽ� ����

	bigint* Q = NULL;
	bigint* R = NULL;

	if (N->sign == NEGATIVE) // ���� ó��
		return ERROR;
	if (M->sign == NEGATIVE)
		return ERROR;
	if (X->sign == NEGATIVE)
		return ERROR;

	BI_Get_Bit_Length(&N_bit_len, N); // N�� ��Ʈ�� ���� -> N_bit_len

	// [line 1]
	BI_Set_Zero(&t0); // t0 = 0
	BI_Assign(&t1, X); // t1 = X

	for (i = N_bit_len - 1; i >= 0; i--) // [line 2]
	{
		bit = BI_Get_j_th_Bit_of_BI(i, N); // N�� i��° ��Ʈ�� 1���� 0���� �Ǻ�

		if (bit == 1) // N�� i��° ��Ʈ�� 1�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 + t1�� �����ϱ� ���� bigint ����

			// [line 3]
			ret = ADD(&temp0, &t0, &t1); // temp0 = t0 + t1
			if (ret == ERROR)
				return ERROR;
			ret = Division(&Q, &R, temp0, M); // R = temp0 (mod M)
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 ����)
			BI_Delete(&temp0);
			BI_Delete(&Q);
			BI_Delete(&R);

			// [line 4]
			BI_Left_Shift(t1, 1); // t1 = t1 << 2
			ret = Division(&Q, &R, t1, M); // t1 = t1 (mod M)
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 ����)
			BI_Delete(&Q);
			BI_Delete(&R);
		}
		else if (bit == 0) // N�� i��° ��Ʈ�� 0�� ���
		{
			BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
			BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

			BI_New(&temp1, MAX(t0_len, t1_len) + 1);  // temp1(t1) = t0 + t1�� �����ϱ� ���� bigint ����

			// [line 3]
			ret = ADD(&temp1, &t0, &t1); // temp1 = t0 + t1
			if (ret == ERROR)
				return ERROR;
			ret = Division(&Q, &R, temp1, M); // R = temp1 (mod M)
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t1);
			BI_Assign(&t1, R); // t1 = R (t1 ����)
			BI_Delete(&temp1);
			BI_Delete(&Q);
			BI_Delete(&R);

			// [line 4]
			BI_Left_Shift(t0, 1); // t0 = t0 << 2
			ret = Division(&Q, &R, t0, M); // t0 = t0 (mod M)
			if (ret == ERROR)
				return ERROR;
			BI_Delete(&t0);
			BI_Assign(&t0, R); // t0 = R (t0 ����)
			BI_Delete(&Q);
			BI_Delete(&R);
		}
		else // N�� i��° ��Ʈ�� 1�� 0�� �ƴ� ���
			return ERROR; // ���� ó��
	}

	BI_Assign(T, t0); // ������ t0 �� T�� �Ҵ�

	BI_Delete(&t0);
	BI_Delete(&t1);

	return SUCCESS;
}

/**
* @brief [Exponentiation] BI_Left to BI_Right MUL function
* @details

	[pseudo code]
	Input  : T, X, N, M

	1 : t <- 1
	2 : for i <- l - 1 downto 0 do
	3 :		t <- t^2 ( ^ 2 : SQU )
	4 :		t <- t * X^(N_{i}) ( * : MUL_Multi() )
	5 :	end for
	6 : T <- t

* @param bigint** T Modular ���� ���� ����� �ش��ϴ� bigint ���� �������� ����
* @param bigint* X Modular ���� ���꿡�� �ؿ� �ش��ϴ� bigint �������� ����
* @param bigint* N Modular ���� ���꿡�� ������ �ش��ϴ� bigint �������� ����
* @return SUCCESS ���� ��
*/
int EXP_LR_MUL(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // �ݺ����� ���� int�� ���� i
	int len = 0; // ������ N�� ��Ʈ ���̸� ���� int�� ���� len
	int result = 0; // �ش� ��Ʈ ���� ����� return �޴� int�� ���� result
	int x_len = 0; // X�� ���忭 ���̸� ������ int�� ���� x_len
	int t0_len = 0; // t0�� ���忭 ���̸� ������ int�� ���� t0_len
	int ret;

	bigint* t0 = NULL;
	bigint* temp1 = NULL;

	BI_Set_One(&t0);  // [line 1] t0�� 1�� ����

	BI_Get_Bit_Length(&len, N);  // ������(N)�� ���� ��Ʈ ���� -> len ����
	BI_Get_Word_Length(&x_len, &X);
	for (i = len - 1; i >= 0; i--)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		ret = Squaring(&temp1, t0); // temp1 = t0^2 ����
		if (ret == ERROR)
			return ERROR;
		BI_Delete(&t0); // t0�� Delete

		//BI_New(&temp0, 2 * t0_len + x_len + 1); // temp0 ���� temp1^2 * X �� �����ϱ� ���� bigint ����
		result = BI_Get_j_th_Bit_of_BI(i, N); // �ݺ������� ��� ���� ���� i�� �̿��� N�� �ش� ��Ʈ�� 1���� 0���� ����

		if (result == 1) // �ش� ��Ʈ�� 1�̸�,
		{
			ret = Multiplication(&t0, temp1, X); //t1^2 * X (����)���� �� temp0�� ����
			if (ret == ERROR)
				return ERROR;
		}

		else // �ش� ��Ʈ�� 0�̸�, ������ �������� �ʾƵ� �ǹǷ�
			BI_Assign(&t0, temp1); // temp0�� t0 assign --> �ݺ������� t0�� ������ ������ �� �ֵ���

		BI_Delete(&temp1); // temp1�� delete.
	}
	BI_Assign(T, t0); // �ݺ����� ���� ����� t0�� T�� assign
	BI_Delete(&t0); // t0�� delete.

	return SUCCESS;
}

/**
* @brief [Exponentiation] BI_Left to BI_Right ADD function
* @details

	[pseudo code]
	Input  : T, X, N, M

	1 : t <- 0
	2 : for i <- l - 1 downto 0 do
	3 :		t <-  2 * t
	4 :		t <- t + N_{i} * X
	5 :	end for
	6 : T <- t

* @param bigint** T Modular ���� ���� ����� �ش��ϴ� bigint ���� �������� ����
* @param bigint* X Modular ���� ���꿡�� �ؿ� �ش��ϴ� bigint �������� ����
* @param bigint* N Modular ���� ���꿡�� ������ �ش��ϴ� bigint �������� ����
* @return SUCCESS ���� ��
 */
int EXP_LR_ADD(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int x_len = 0;
	int t0_len = 0;
	int ret;

	bigint* t0 = NULL;
	bigint* temp0 = NULL;

	BI_Set_Zero(&t0);

	BI_Get_Bit_Length(&len, N);

	for (i = len - 1; i >= 0; i--)
	{
		BI_Left_Shift(t0, 1);
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&x_len, &X); // t1�� ���忭 ���� -> t1_len
		BI_New(&temp0, t0_len + x_len); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����
		result = BI_Get_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			ret = ADD(&temp0, &t0, &X);
			if (ret == ERROR)
				return ERROR;
		}
		else
			BI_Assign(&temp0, t0);

		BI_Delete(&t0);
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);

	return SUCCESS;
}

/**
* @brief [Exponentiation] BI_Right to BI_Left MUL function
* details

	[pseudo code]
	Input : T, X, N, M
	1 : (t0, t1) <- (1, X)
	2 :	for i <- 0 to l - 1 do
	3 :		t0 <- t0 * t1^(N_{i}) ( * : MUL_Multi )
	4 :		t1 <- t1 ^ 2( ^ : SQU )
	5 :	end for
	6 :	T <- t

* @param bigint** T Modular ���� ���� ����� �ش��ϴ� bigint ���� �������� ����
* @param bigint* X Modular ���� ���꿡�� �ؿ� �ش��ϴ� bigint �������� ����
* @param bigint* N Modular ���� ���꿡�� ������ �ش��ϴ� bigint �������� ����
* @return SUCCESS ���� ��
*/
int EXP_RL_MUL(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int t0_len = 0;
	int t1_len = 0;
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;

	BI_Set_One(&t0);
	BI_Assign(&t1, X); //line. 1. 

	BI_Get_Bit_Length(&len, N);

	if (len == 1)
	{
		BI_Assign(T, X);
		BI_Delete(&t0);
		BI_Delete(&t1);
		return SUCCESS;
	}
	for (i = 0; i <= len - 1; i++)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

		BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����

		result = BI_Get_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			ret = Multiplication(&temp0, t0, t1);
			if (ret == ERROR)
				return ERROR;
		}

		else
		{
			BI_Assign(&temp0, t0);
			BI_Delete(&t0);
		}
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);

		ret = Squaring(&temp1, t1);
		if (ret == ERROR)
			return ERROR;
		BI_Delete(&t1);
		BI_Assign(&t1, temp1);
		BI_Delete(&temp1);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
	BI_Delete(&t1);
	BI_Refine(*T);

	return SUCCESS;
}

/**
* @brief [Exponentiation] BI_Right to BI_Left ADD function
* @details

	[pseudo code]
	Input : T, X, N, M
	1 : (t0, t1) <- (0, X)
	2 :	for i <- 0 to l - 1 do
	3 :		t0 <- t0 + N_{i} * t1
	4 :		t1 <- 2 * t1
	5 :	end for
	6 :	T <- t

* @param bigint** T Modular ���� ���� ����� �ش��ϴ� bigint ���� �������� ����
* @param bigint* X Modular ���� ���꿡�� �ؿ� �ش��ϴ� bigint �������� ����
* @param bigint* N Modular ���� ���꿡�� ������ �ش��ϴ� bigint �������� ����
*/
int EXP_RL_ADD(bigint** T, bigint* X, bigint* N)
{
	int i = 0; // �ݺ����� ���� ���� i
	int len = 0; // X�� ��Ʈ ���̸� ���� ���� len
	int result = 0;
	int t0_len = 0;
	int t1_len = 0;
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	//bigint* temp1 = NULL;

	BI_Set_Zero(&t0);
	BI_Assign(&t1, X); //line. 1. 

	BI_Get_Bit_Length(&len, N);
	if (len == 1)
	{
		BI_Assign(T, X);
		BI_Delete(&t0);
		BI_Delete(&t1);
		return SUCCESS;
	}
	for (i = 0; i <= len - 1; i++)
	{
		BI_Get_Word_Length(&t0_len, &t0); // t1�� ���忭 ���� -> t1_len
		BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

		result = BI_Get_j_th_Bit_of_BI(i, N);

		if (result == 1)
		{
			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 * t1�� �����ϱ� ���� bigint ����
			ret = ADD(&temp0, &t0, &t1);
			if (ret == ERROR)
				return ERROR;
		}
		else
		{
			BI_Assign(&temp0, t0);
			BI_Delete(&t0);
		}
		BI_Assign(&t0, temp0);
		BI_Delete(&temp0);

		BI_Left_Shift(t1, 1);
	}
	BI_Assign(T, t0);
	BI_Delete(&t0);
	BI_Delete(&t1);
	BI_Refine(*T);

	return SUCCESS;
}

/**
* @brief [Modular Exponentiation] BI_Left to BI_Right MUL function
* @details

	[pseudo code]
	Input  : T, X, N, M

	1 : t <- 1
	2 : for i <- l - 1 downto 0 do
	3 :		t <- t^2 mod M ( ^ 2 : optional , mod : optional)
	4 :		t <- t * X^(N_{i}) mod M ( * : option, mod : optional)
	5 :	end for
	6 : T <- t

* @param bigint** T Modular ���� ���� ����� �ش��ϴ� bigint ���� �������� ����
* @param bigint* X Modular ���� ���꿡�� �ؿ� �ش��ϴ� bigint �������� ����
* @param bigint* N Modular ���� ���꿡�� ������ �ش��ϴ� bigint �������� ����
* @param bigint* M Modular ���� ���꿡�� X^N �� T�� �յ����ִ� bigint �������� ����
* @return SUCCESS ���� ��
*/
int MOD_EXP_LR_MUL(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� int�� ���� i
	int len = 0; // ������ N�� ��Ʈ ���̸� ���� int�� ���� len
	int result = 0; // �ش� ��Ʈ ���� ����� return �޴� int�� ���� result
	int x_len = 0; // X�� ���忭 ���̸� ������ int�� ���� x_len
	int ret;

	bigint* t0 = NULL; // T0
	bigint* temp0 = NULL; //
	bigint* temp1 = NULL; //
	bigint* trash_q = NULL; // Modular ������ ���� ���Ǵ� Binary_Long_Div()�� �� �Ű������� �� bigint �������� ���� 
	bigint* trash_r = NULL; // Modular ������ ���� ���Ǵ� Binary_Long_Div()�� ������ �Ű������� �� bigint �������� ����

	BI_Set_One(&t0); // t0�� 1�� ����

	BI_Get_Bit_Length(&len, N); // ������(N)�� ���� ��Ʈ ���� -> len ����

	for (i = len - 1; i >= 0; i--) // ������(N)�� �ֻ��� ��Ʈ���� ������ ��Ʈ���� �ݺ���
	{
		ret = Squaring(&temp1, t0); // temp1 = t0^2 ����
		if (ret == ERROR)
			return ERROR;
		BI_Delete(&t0); // t0�� Delete
		ret = Division(&trash_q, &trash_r, temp1, M); // temp1(=t0^2)�� ���Ͽ� M���� ��ⷯ ����
		if (ret == ERROR)
			return ERROR;
		BI_Assign(&temp1, trash_r); // �������� ���� �����Ƿ� �������� temp1�� Assign
		BI_Delete(&trash_r); // temp1�� ���� ���� ���������Ƿ� delete
		BI_Delete(&trash_q); //trash_q�� �ٲܱ�

		BI_Get_Word_Length(&x_len, &X); // X�� ���忭 ���� -> x_len
		result = BI_Get_j_th_Bit_of_BI(i, N); // �ݺ������� ��� ���� ���� i�� �̿��� N�� �ش� ��Ʈ�� 1���� 0���� ����
		if (result == 1) // �ش� ��Ʈ�� 1�̸�,
		{
			ret = Multiplication(&temp0, temp1, X); //t1^2 * X (����)���� �� temp0�� ����
			if (ret == ERROR)
				return ERROR;
			ret = Division(&trash_q, &trash_r, temp0, M); // ���� ������ temp0�� ���Ͽ� M���� ��ⷯ ���� ����
			if (ret == ERROR)
				return ERROR;
			BI_Assign(&t0, trash_r); // �������� ���� �����Ƿ� �������� temp0�� assign. --> �ݺ������� t0�� ������ ������ �� �ֵ���
			BI_Delete(&trash_r); // temp0�� ���� ���� ���������Ƿ� delete
			BI_Delete(&trash_q); // Binary Long_Div()���� ����� �� �ʿ�����Ƿ� delete
		}

		else // �ش� ��Ʈ�� 0�̸�, ������ �������� �ʾƵ� �ǹǷ�
			BI_Assign(&t0, temp1); // temp0�� t0 assign --> �ݺ������� t0�� ������ ������ �� �ֵ���

		BI_Delete(&temp1); // temp1�� delete.
		BI_Delete(&temp0); // temp0�� delete.
	}
	BI_Assign(T, t0); // �ݺ����� ���� ����� t0�� T�� assign
	BI_Delete(&t0); // t0�� delete.

	return SUCCESS;
}

/**
* @brief [Modular Exponentiation] BI_Left to BI_Right ADD function
* @details

	[pseudo code]
	Input  : T, X, N, M

	1 : t <- 0
	2 : for i <- l - 1 downto 0 do
	3 :		t <-  2 * t mod M ( mod : optional )
	4 :		t <- t + N_{i} * X mod M ( mod : optional )
	5 :	end for
	6 : T <- t

* @param bigint** T Modular ���� ���� ����� �ش��ϴ� bigint ���� �������� ����
* @param bigint* X Modular ���� ���꿡�� �ؿ� �ش��ϴ� bigint �������� ����
* @param bigint* N Modular ���� ���꿡�� ������ �ش��ϴ� bigint �������� ����
* @param bigint* M Modular ���� ���꿡�� X^N �� T�� �յ����ִ� bigint �������� ����
* @return SUCCESS ���� ��
*/
int MOD_EXP_LR_ADD(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� int�� ���� i
	int len = 0; // ������ N�� ��Ʈ ���̸� ���� int�� ���� len
	int result = 0; // �ش� ��Ʈ ���� ����� return �޴� int�� ���� result
	int x_len = 0; // X�� ���忭 ���̸� ������ int�� ���� x_len
	int t0_len = 0; // ������ ���忭 ���̸� ������ int�� ���� t0_len
	int ret;

	bigint* t0 = NULL;
	bigint* temp0 = NULL;
	bigint* trash_q = NULL; // Modular ������ ���� ���Ǵ� Binary_Long_Div()�� �� �Ű������� �� bigint �������� ����
	bigint* trash_r = NULL; // Modular ������ ���� ���Ǵ� Binary_Long_Div()�� ������ �Ű������� �� bigint �������� ����
	BI_Set_Zero(&t0); // t0�� 0���� ����

	BI_Get_Bit_Length(&len, N); // ������(N)�� ���� ��Ʈ ���� -> len ����

	for (i = len - 1; i >= 0; i--)
	{
		BI_Left_Shift(t0, 1); // t0 <- 2 * t0
		ret = Division(&trash_q, &trash_r, t0, M); // 2 * t0�� ���� Binary_Long_Div�� ���� MOD ����� �� -> 2 * t0 (mod M) == trash_r
		if (ret == ERROR)
			return ERROR;
		BI_Get_Word_Length(&t0_len, &trash_r); // �������� ���忭 ���� -> t0_len
		BI_Get_Word_Length(&x_len, &X); // X�� ���忭 ���� -> x_len
		BI_New(&temp0, MAX(t0_len, x_len)); // trash_r(= 2 * t0 (mod M))�� X�� ��������� ���� bigint temp0 ����
		result = BI_Get_j_th_Bit_of_BI(i, N); // �ݺ������� ��� ���� ���� i�� �̿��� N�� �ش� ��Ʈ�� 1���� 0���� ����

		if (result == 1)// �ش� ��Ʈ�� 1�̸�(If result is 1),
		{
			ret = ADD(&temp0, &trash_r, &X); // [line 4] trash_r + X �� ���� temp0�� ���� 
			if (ret == ERROR)
				return ERROR;
			ret = Division(&trash_q, &trash_r, temp0, M); // temp0�� modular ���� ���ؼ� trash_r�� �ٽ� ����
			if (ret == ERROR)
				return ERROR;
			BI_Assign(&t0, trash_r);  // �������� ���� �����Ƿ� �������� temp0�� assign. --> �ݺ������� �ٽ� �� �� t0�� ������ ������ �� �ֵ���
		}
		else // �ش� ��Ʈ�� 0�̸�(If result is 0),
		{
			BI_Assign(&t0, trash_r); // �������� ���� �����Ƿ� �������� temp0�� assign. --> �ݺ������� �ٽ� �� �� t0�� ������ ������ �� �ֵ��� 
		}

		BI_Delete(&temp0); // temp0�� delete.
		BI_Delete(&trash_q); // trash_q�� delete.
		BI_Delete(&trash_r); // trash_r�� delete.
	}
	BI_Assign(T, t0); // �ݺ����� ���� ���� ����� t0�� T�� assign
	BI_Delete(&t0); // t0�� delete.

	return SUCCESS;
}

/**
* @brief [Modular Exponentiation] BI_Right to BI_Left MUL function
* details

	[pseudo code]
	Input : T, X, N, M
	1 : (t0, t1) <- (1, X)
	2 :	for i <- 0 to l - 1 do
	3 :		t0 <- t0 * t1^(N_{i}) mod M ( * :optional, mod : optional )
	4 :		t1 <- t1 ^ 2 mod M ( ^ : optional, mod :optional )
	5 :	end for
	6 :	T <- t

* @param bigint** T Modular ���� ���� ����� �ش��ϴ� bigint ���� �������� ����
* @param bigint* X Modular ���� ���꿡�� �ؿ� �ش��ϴ� bigint �������� ����
* @param bigint* N Modular ���� ���꿡�� ������ �ش��ϴ� bigint �������� ����
* @param bigint* M Modular ���� ���꿡�� X^N �� T�� �յ����ִ� bigint �������� ����
* @return SUCCESS ���� ��
*/
int MOD_EXP_RL_MUL(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� int�� ���� i
	int len = 0; // ������ N�� ��Ʈ ���̸� ���� int�� ���� len
	int result = 0; // �ش� ��Ʈ ���� ����� return �޴� int�� ���� result
	int t0_len = 0; // t0�� ���忭 ���̸� ������ int�� ���� t0_len
	int t1_len = 0; // t1�� ���忭 ���̸� ������ int�� ���� t1_len
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL; // Modular ������ ���� ���Ǵ� Binary_Long_Div()�� �� �Ű������� �� bigint �������� ����
	bigint* trash_r = NULL; // Modular ������ ���� ���Ǵ� Binary_Long_Div()�� ������ �Ű������� �� bigint �������� ����

	BI_Set_One(&t0); // t0�� 1�� ����
	BI_Assign(&t1, X); // t1�� X�� assign

	BI_Get_Bit_Length(&len, N);  // ������(N)�� ���� ��Ʈ ���� -> len ����

	if ((len == 1) & (N->a[0] == 1)) // X ^ 1�� ��
	{
		BI_Assign(T, X);  // �� ��쿡���� T�� ���� X�̹Ƿ� assign
		ret = Division(&trash_q, &trash_r, *T, M); // X mod M ���� �� trash_r�� ����
		if (ret == ERROR)
			return ERROR;
		BI_Assign(T, trash_r); // X mod M ���� trash_r�� T�� assign
		BI_Delete(&trash_q); // �Ҵ��� trash_q delete
		BI_Delete(&trash_r); // �Ҵ��� trash_r delete
		BI_Delete(&t0); // �Ҵ��� t0 delete
		BI_Delete(&t1); // �Ҵ��� t1 delete
		return SUCCESS;
	}

	for (i = 0; i <= len - 1; i++) // ������(N)�� ������ ��Ʈ���� �ֻ��� ��Ʈ���� �ݺ���
	{
		BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
		BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

		BI_New(&temp1, t1_len * 2 + 1); // temp1(t1) = t1^2�� �����ϱ� ���� bigint ����

		result = BI_Get_j_th_Bit_of_BI(i, N);  // �ݺ������� ��� ���� ���� i�� �̿��� N�� �ش� ��Ʈ�� 1���� 0���� ����

		if (result == 1)// �ش� ��Ʈ�� 1�̸�,
			Multiplication(&temp0, t0, t1); // temp0�� t0 * t1 �� ����
		else // �ش� ��Ʈ�� 0�̸�,
			BI_Assign(&temp0, t0); // t0�� 1�� ���ϴ� ���̹Ƿ�, temp0�� t0�� assign --> �ݺ������� t0�� ������ ������ �� �ֵ���

		ret = Division(&trash_q, &trash_r, temp0, M); // ����� temp0(t0)�� Binary_Long_Div()�� �̿��� modular M ���� �� ������� trash_r�� ����
		if (ret == ERROR)
			return ERROR;

		BI_Assign(&t0, trash_r);
		BI_Delete(&temp0); // �Ҵ��� temp0 delete
		BI_Delete(&trash_q); // ������ ���� �Ҵ��� trash_q delete
		BI_Delete(&trash_r); // ������ ���� �Ҵ��� trash_r delete

		ret = Squaring(&temp1, t1);  // temp1(t1) <- t1 * t1
		if (ret == ERROR)
			return ERROR;
		ret = Division(&trash_q, &trash_r, temp1, M); // temp1( = t1 * t1)�� Binary_Long_Div()�� �̿��� modular M ���� �� ������� trash_r�� ����
		if (ret == ERROR)
			return ERROR;
		BI_Assign(&t1, trash_r); // t1 * t1 (mod M) ���� trash_r�� �ٽ� t1�� assign --> �ݺ������� t1�� ������ ������ �� �ֵ���
		BI_Delete(&trash_q); // ������ ���� �Ҵ��� trash_q delete
		BI_Delete(&trash_r); // ������ ���� �Ҵ��� trash_r delete
		BI_Delete(&temp1); // �Ҵ��� temp0 delete
	}
	BI_Assign(T, t0); // �ݺ����� ���� ����� t0�� T�� assign
	BI_Delete(&t0); // ������ ���� �Ҵ��� t0 delete
	BI_Delete(&t1); // ������ ���� �Ҵ��� t1 delete
	BI_Refine(*T); // ������� T�� refine ���ֱ�

	return SUCCESS;
}

/**
* @brief [Modular Exponentiation] BI_Right to BI_Left ADD function (used Binary Long Division)
* @details

	[pseudo code]
	Input : T, X, N, M
	1 : (t0, t1) <- (0, X)
	2 :	for i <- 0 to l - 1 do
	3 :		t0 <- t0 + N_{i} * t1 mod M ( mod : optional )
	4 :		t1 <- 2 * t1 mod M ( mod : optional )
	5 :	end for
	6 :	T <- t

* @param bigint** T Modular ���� ���� ����� �ش��ϴ� bigint ���� �������� ����
* @param bigint* X Modular ���� ���꿡�� �ؿ� �ش��ϴ� bigint �������� ����
* @param bigint* N Modular ���� ���꿡�� ������ �ش��ϴ� bigint �������� ����
* @param bigint* M Modular ���� ���꿡�� X^N �� T�� �յ����ִ� bigint �������� ����
* @return SUCCESS ���� ��
*/
int MOD_EXP_RL_ADD(bigint** T, bigint* X, bigint* N, bigint* M)
{
	int i = 0; // �ݺ����� ���� int�� ���� i
	int len = 0; // ������ N�� ��Ʈ ���̸� ���� int�� ���� len
	int result = 0; // �ش� ��Ʈ ���� ����� return �޴� int�� ���� result
	int t0_len = 0; // t0�� ���忭 ���̸� ������ int�� ���� t0_len
	int t1_len = 0; // t1�� ���忭 ���̸� ������ int�� ���� t1_len
	int ret;

	bigint* t0 = NULL;
	bigint* t1 = NULL;
	bigint* temp0 = NULL;
	bigint* temp1 = NULL;
	bigint* trash_q = NULL; // Modular ������ ���� ���Ǵ� Binary_Long_Div()�� �� �Ű������� �� bigint �������� ����
	bigint* trash_r = NULL; // Modular ������ ���� ���Ǵ� Binary_Long_Div()�� ������ �Ű������� �� bigint �������� ����

	BI_Set_Zero(&t0); // t0�� 0���� ����
	BI_Assign(&t1, X); // t1�� X�� assign

	BI_Get_Bit_Length(&len, N); // ������(N)�� ���� ��Ʈ ���� -> len ����

	if ((len == 1) & (N->a[0] == 1)) // 1 * X�� ��
	{
		BI_Assign(T, X);  // �� ��쿡���� T�� ���� X�̹Ƿ� assign
		ret = Division(&trash_q, &trash_r, *T, M); // X mod M ���� �� trash_r�� ����
		if (ret == ERROR)
			return ERROR;

		BI_Assign(T, trash_r); // X mod M ���� trash_r�� T�� assign
		BI_Delete(&trash_q); // �Ҵ��� trash_q delete
		BI_Delete(&trash_r); // �Ҵ��� trash_r delete
		BI_Delete(&t0); // �Ҵ��� t0 delete
		BI_Delete(&t1); // �Ҵ��� t1 delete
		return SUCCESS;
	}

	for (i = 0; i <= len - 1; i++) // ������(N)�� ������ ��Ʈ���� �ֻ��� ��Ʈ���� �ݺ���
	{
		BI_Get_Word_Length(&t0_len, &t0); // t0�� ���忭 ���� -> t0_len
		BI_Get_Word_Length(&t1_len, &t1); // t1�� ���忭 ���� -> t1_len

		result = BI_Get_j_th_Bit_of_BI(i, N);  // �ݺ������� ��� ���� ���� i�� �̿��� N�� �ش� ��Ʈ�� 1���� 0���� ����

		if (result == 1) // �ش� ��Ʈ�� 1�̸�,
		{
			BI_New(&temp0, MAX(t0_len, t1_len) + 1); // temp0(t0) = t0 + t1�� �����ϱ� ���� bigint ����
			ret = ADD(&temp0, &t0, &t1); // temp0�� t0 + t1 ����
			if (ret == ERROR)
				return ERROR;
		}
		else // �ش� ��Ʈ�� 0�̸�,
		{
			BI_Assign(&temp0, t0); // t0�� 0�� ���ϴ� ���̹Ƿ�, temp0�� t0�� assign.
		}
		ret = Division(&trash_q, &trash_r, temp0, M); // ����� temp0(t0)�� modular M ���� �� ������� trash_r�� ����
		if (ret == ERROR)
			return ERROR;
		BI_Delete(&temp0); // �Ҵ��� temp0 delete
		BI_Assign(&t0, trash_r); // t0 + result * t1 (mod M) ���� trash_r�� t0�� assign --> �ݺ������� t0�� ������ ������ �� �ֵ���
		BI_Delete(&trash_q); // ������ ���� �Ҵ��� trash_q delete
		BI_Delete(&trash_r); // ������ ���� �Ҵ��� trash_r delete

		BI_Left_Shift(t1, 1); // t1 <- 2 * t1
		ret = Division(&trash_q, &trash_r, t1, M); // 2 * t1�� modular M ���� �� ������� trash_r�� ����
		if (ret == ERROR)
			return ERROR;
		BI_Assign(&t1, trash_r); // 2 * t1 (mod M) ���� trash_r�� �ٽ� t1�� assign --> �ݺ������� t1�� ������ ������ �� �ֵ���
		BI_Delete(&trash_q); // ������ ���� �Ҵ��� trash_q delete
		BI_Delete(&trash_r); // ������ ���� �Ҵ��� trash_r delete
	}
	BI_Assign(T, t0); // �ݺ����� ���� ����� t0�� T�� assign
	BI_Delete(&t0); // ������ ���� �Ҵ��� t0 delete
	BI_Delete(&t1); // ������ ���� �Ҵ��� t1 delete
	BI_Refine(*T); // ������� T�� refine ���ֱ�

	return SUCCESS;
}
