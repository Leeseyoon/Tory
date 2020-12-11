#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "BasicOperation.h"
#include "Calculation.h"
#include "test.h"

//#define TEST_CASE // random test vector 외의 다른 예외 처리 등의 test vector

int addition_test()
{
	int i = 0;
	int size, size1, size2 = 0;
	int ret = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Addition\")\n");

	/* 랜덤한 테스트 벡터 */
	printf("print(\"[Case: Randdom test vector]\")\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = (rand() & 0x7) + 30;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = MAX(bi_1->wordlen, bi_2->wordlen);
		BI_New(&bi_re, size + 1); // 더 긴 길이인 size로 덧셈 연산 결과인 big integer 생성

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);

		ret = ADD(&bi_re, &bi_1, &bi_2); // ADD함수에 덧셈 결과인 bi_re, 피연산자 1인 bi_1, 피연산자 2인 bi_2
		if (ret == ERROR)
			printf("FAILED\n\n");
		else
		{
			printf("A + B == ");
			BI_Show(bi_re, 16);
			printf("\n");
		}

		// 메모리 free
		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}

#ifdef TEST_CASE
	/* 피연산자가 NULL인 경우 */
	printf("[Case: Operand is NULL]\n");
	ret = ADD(&bi_re, &bi_1, &bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");

	/* 피연산자가 모두 0인 경우 */
	printf("[Case: Operands are 0]\n");

	BI_Set_Zero(&bi_1);
	BI_Set_Zero(&bi_2);

	size = MAX(bi_1->wordlen, bi_2->wordlen);
	BI_New(&bi_re, size + 1);

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = ADD(&bi_re, &bi_1, &bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("A + B == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_2);
	BI_Delete(&bi_re);

	/* 피연산자가 하나만 0인 경우 */
	printf("[Case: One operand is 0]\n");

	BI_Set_Zero(&bi_1);
	
	size2 = (rand() & 0x7) + 30;
	BI_Gen_Rand(&bi_2, rand() & 1, size2);

	size = MAX(bi_1->wordlen, bi_2->wordlen);
	BI_New(&bi_re, size + 1);

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = ADD(&bi_re, &bi_1, &bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("A + B == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_2);
	BI_Delete(&bi_re);
#endif
	return 0;
}

int subtraction_test()
{
	int i = 0;
	int size, size1, size2 = 0;
	int ret = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Subtraction\")\n");
	
	printf("print(\"[Case: Randdom test vector]\")\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = (rand() & 0x7) + 30;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = (size1 > size2) ? size1 : size2; // size1 과 size2 비교해서 size 변수에 대입
		BI_New(&bi_re, size + 1);

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);

		ret = SUB(&bi_re, bi_1, bi_2);
		if (ret == ERROR)
			printf("FAILED\n\n");
		else
		{
			printf("A - B == ");
			BI_Show(bi_re, 16);
			printf("\n");
		}

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}
#ifdef TEST_CASE
	/* 피연산자가 NULL인 경우 */
	printf("[Case: Operand is NULL]\n");
	ret = SUB(&bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");

	/* 피연산자가 모두 0인 경우 */
	printf("[Case: Operands are 0]\n");

	BI_Set_Zero(&bi_1);
	BI_Set_Zero(&bi_2);

	size = (size1 > size2) ? size1 : size2;
	BI_New(&bi_re, size + 1);

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = SUB(&bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("A - B == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_2);
	BI_Delete(&bi_re);

	/* 피연산자가 하나만 0인 경우 */
	printf("[Case: One operand is 0]\n");

	BI_Set_Zero(&bi_1);

	size2 = (rand() & 0x7) + 30;
	BI_Gen_Rand(&bi_2, rand() & 1, size2);

	size = (size1 > size2) ? size1 : size2;
	BI_New(&bi_re, size + 1);

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = SUB(&bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("A - B == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_2);
	BI_Delete(&bi_re);
#endif

	return 0;
}

int multiplication_test()
{
	int i = 0;
	int size1, size2 = 0;
	int ret = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	if (FLAG_MUL == TEXTBOOK)
		printf("print(\"Multiplication [Textbook ver]\")\n");
	else if (FLAG_MUL == KARATSUBA)
		printf("print(\"Multiplication [Karatsuba ver]\")\n");

	/* 랜덤한 테스트 벡터 */
	printf("print(\"[Case: Randdom test vector]\")\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = (rand() & 0xf) + 60;
		size2 = (rand() & 0xf) + 60;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);

		ret = Multiplication(&bi_re, bi_1, bi_2);
		if (ret == ERROR)
			printf("FAILED\n\n");
		else
		{
			printf("A * B == ");
			BI_Show(bi_re, 16);
			printf("\n");
		}

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}
#ifdef TEST_CASE
	/* 피연산자가 NULL인 경우 */
	printf("[Case: Operand is NULL]\n");
	ret = Multiplication(&bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");

	/* 피연산자가 모두 0인 경우 */
	printf("[Case: Operands are 0]\n");

	BI_Set_Zero(&bi_1);
	BI_Set_Zero(&bi_2);

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = Multiplication(&bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("A * B == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_2);
	BI_Delete(&bi_re);

	/* 피연산자가 하나만 0인 경우 */
	printf("[Case: One operand is 0]\n");

	BI_Set_Zero(&bi_1);

	size2 = (rand() & 0x7) + 30;
	BI_Gen_Rand(&bi_2, 1, size2);

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = Multiplication(&bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("A * B == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	/* 피연산자가 모두 1인 경우 */
	printf("[Case: Operands are 1]\n");

	BI_Set_One(&bi_1);
	BI_Set_One(&bi_2);

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = Multiplication(&bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("A * B == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_2);
	BI_Delete(&bi_re);

	/* 피연산자가 하나만 1인 경우 */
	printf("[Case: One operand is 1]\n");

	BI_Set_One(&bi_1);

	size2 = (rand() & 0x7) + 30;
	BI_Gen_Rand(&bi_2, rand() & 1, size2);

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = Multiplication(&bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("A * B == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_2);
	BI_Delete(&bi_re);
#endif
	return 0;
}

int squaring_test()
{
	int i = 0;
	int size1 = 0;
	int ret = 0;

	bigint* bi_1 = NULL;
	bigint* bi_re = NULL;;

	printf("\n");
	if (FLAG_SQU == TEXTBOOK)
		printf("print(\"Squaring [Textbook ver]\")\n");
	else if (FLAG_SQU == KARATSUBA)
		printf("print(\"Squaring [Karatsuba ver]\")\n");

	/* 랜덤한 테스트 벡터 */
	printf("print(\"[Case: Randdom test vector]\")\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = (rand() & 0xf) + 30;
		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		printf("A = ");
		BI_Show(bi_1, 16);

		ret = Squaring(&bi_re, bi_1);
		if (ret == ERROR)
			printf("FAILED\n\n");
		else
		{
			printf("A * A == ");
			BI_Show(bi_re, 16);
			printf("\n");
		}

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
#ifdef TEST_CASE
	/* 피연산자가 NULL인 경우 */
	printf("[Case: Operand is NULL]\n");
	ret = Squaring(&bi_re, bi_1);
	if (ret == ERROR)
		printf("FAILED\n\n");

	/* 피연산자가 0인 경우 */
	printf("[Case: Operand is 0]\n");

	BI_Set_Zero(&bi_1);

	printf("A = ");
	BI_Show(bi_1, 16);

	ret = Squaring(&bi_re, bi_1);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("A * A == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_re);

	/* 피연산자가 1인 경우 */
	printf("[Case: Operand is 1]\n");

	BI_Set_One(&bi_1);

	printf("A = ");
	BI_Show(bi_1, 16);

	ret = Squaring(&bi_re, bi_1);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("A * A == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_re);

	/* 피연산자가 -1인 경우 */
	printf("[Case: Operand is -1]\n");

	BI_Set_One(&bi_1);
	bi_1->sign = NEGATIVE;

	printf("A = ");
	BI_Show(bi_1, 16);

	ret = Squaring(&bi_re, bi_1);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("A * A == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_re);
#endif
	return SUCCESS;
}

int division_test()
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int ret = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	printf("\n");
	if (FLAG_DIV == BINARY_LONG)
		printf("print(\"Division [Binary long ver]\")\n");
	else if (FLAG_DIV == MULTI_PRE)
		printf("print(\"Division [Multi-precision ver]\")\n");

	/* 랜덤한 테스트 벡터 */
	printf("print(\"[Case: Randdom test vector]\")\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size2 = (rand() & 0x7) + 20;
		size1 = size2 + (rand() & 0x7) + 10; // size1은 size2 보다 길이 더 길게

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		// 출력 부분
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);

		ret = Division(&bi_q, &bi_re, bi_1, bi_2);
		if (ret == ERROR)
			printf("FAILED\n\n");
		else
		{
			printf("Q = ");
			BI_Show(bi_q, 16);
			printf("R = ");
			BI_Show(bi_re, 16);
			printf("A == B * Q + R");
			printf("\n\n");
		}

		// 메모리 free
		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
		BI_Delete(&bi_q);
	}
#ifdef TEST_CASE
	/* 피연산자가 NULL인 경우 */
	printf("[Case: Operand is NULL]\n");
	ret = Division(&bi_q, &bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");

	/* 나눗셈을 수행하는 값이 0인 경우 */
	printf("[Case: Operand is 0]\n");

	BI_Set_Zero(&bi_1);
	
	size2 = (rand() & 0x7) + 1;
	BI_Gen_Rand(&bi_2, 0, size2);

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = Division(&bi_q, &bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("Q = ");
		BI_Show(bi_q, 16);
		printf("R = ");
		BI_Show(bi_re, 16);
		printf("A == B * Q + R");
		printf("\n\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_2);
	BI_Delete(&bi_re);
	BI_Delete(&bi_q);

	/* 0으로 나누는 경우 */
	printf("[Case: Divide with 0]\n");

	size1 = (rand() & 0x7) + 1;
	BI_Gen_Rand(&bi_1, 0, size1);

	BI_Set_Zero(&bi_2);

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = Division(&bi_q, &bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("Q = ");
		BI_Show(bi_q, 16);
		printf("R = ");
		BI_Show(bi_re, 16);
		printf("A == B * Q + R");
		printf("\n\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_2);
	BI_Delete(&bi_re);
	BI_Delete(&bi_q);

	/* 1로 나누는 경우 */
	printf("[Case: Divide with 1]\n");

	size1 = (rand() & 0x7) + 1;
	BI_Gen_Rand(&bi_1, 0, size1);

	BI_Set_One(&bi_2);

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = Division(&bi_q, &bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("Q = ");
		BI_Show(bi_q, 16);
		printf("R = ");
		BI_Show(bi_re, 16);
		printf("A == B * Q + R");
		printf("\n\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_2);
	BI_Delete(&bi_re);
	BI_Delete(&bi_q);

	/* 나눗셈을 수행하는 값이 음수인 경우 */
	printf("[Case: Operand is negative]\n");

	size2 = (rand() & 0x7) + 5;
	size1 = size2 + (rand() & 0x7) + 5;

	BI_Gen_Rand(&bi_1, 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
	BI_Gen_Rand(&bi_2, 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

	printf("A = ");
	BI_Show(bi_1, 16);
	printf("B = ");
	BI_Show(bi_2, 16);

	ret = Division(&bi_q, &bi_re, bi_1, bi_2);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("Q = ");
		BI_Show(bi_q, 16);
		printf("R = ");
		BI_Show(bi_re, 16);
		printf("A == B * Q + R");
		printf("\n\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_2);
	BI_Delete(&bi_re);
	BI_Delete(&bi_q);
#endif
	return 0;
}

int modular_exponentiation_multiplication_test()
{
	int i = 0;
	int size1 = 0;
	int size_M = 0;
	int size_N = 0;
	int ret = 0;

	bigint* bi_1 = NULL;
	bigint* bi_M = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	if (FLAG_EXP == LTOR)
		printf("print(\"Modular exponentiation of multiplication [Left to Right ver]\")\n");
	else if (FLAG_EXP == RTOL)
		printf("print(\"Modular exponentiation of multiplication [Right to Left ver]\")\n");
	else if (FLAG_EXP == MONTGOMERY)
		printf("print(\"Modular exponentiation of multiplication [Montgomery Ladder ver]\")\n");

	/* 랜덤한 테스트 벡터 */
	printf("print(\"[Case: Randdom test vector]\")\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = (rand() & 0xf) + 10;
		size_N = (rand() & 0xf) + 1;
		size_M = (rand() & 0xf) + 5;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N); // bi_n의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성

		printf("X = ");
		BI_Show(bi_1, 16);

		printf("N = ");
		BI_Show(bi_N, 16);

		printf("M = ");
		BI_Show(bi_M, 16);

		ret = Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);
		if (ret == ERROR)
			printf("FAILED\n\n");
		else
		{
			printf("power_mod(X, N, M) == ");
			BI_Show(bi_re, 16);
			printf("\n");
		}

		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
		BI_Delete(&bi_M);
	}
#ifdef TEST_CASE
	/* 피연산자가 NULL인 경우 */
	printf("[Case: Operand is NULL]\n");

	ret = Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");

	/* 지수승을 수행하는 값이 0인 경우 */
	printf("[Case: Operand is 0]\n");

	BI_Set_Zero(&bi_1);
	size_N = (rand() & 0xf) + 1;
	size_M = (rand() & 0xf) + 5;
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("power_mod(X, N, M) == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 지수승을 수행하는 값이 1인 경우 */
	printf("[Case: Operand is 1]\n");

	BI_Set_One(&bi_1);
	size_N = (rand() & 0xf) + 1;
	size_M = (rand() & 0xf) + 5;
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("power_mod(X, N, M) == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 지수승을 수행하는 값이 음수인 경우 */
	printf("[Case: Operand is negative]\n");

	size1 = (rand() & 0xf) + 10;
	size_N = (rand() & 0xf) + 1;
	size_M = (rand() & 0xf) + 5;

	BI_Gen_Rand(&bi_1, NEGATIVE, size1);
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("power_mod(X, N, M) == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 모듈러 취하는 값이 0인 경우 */
	printf("[Case: Modular with 0]\n");

	BI_Set_Zero(&bi_M);
	size1 = (rand() & 0xf) + 10;
	size_N = (rand() & 0xf) + 1;
	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("power_mod(X, N, M) == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 모듈러 취하는 값이 1인 경우 */
	printf("[Case: Modular with 1]\n");

	BI_Set_One(&bi_M);
	size1 = (rand() & 0xf) + 10;
	size_N = (rand() & 0xf) + 1;
	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("power_mod(X, N, M) == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 모듈러 취하는 값이 음수인 경우 */
	printf("[Case: Modular with negative number]\n");

	size1 = (rand() & 0xf) + 10;
	size_N = (rand() & 0xf) + 1;
	size_M = (rand() & 0xf) + 5;

	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);
	BI_Gen_Rand(&bi_M, NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("power_mod(X, N, M) == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 지수가 0인 경우 */
	printf("[Case: Exponent is 0]\n");

	BI_Set_Zero(&bi_N);
	size1 = (rand() & 0xf) + 10;
	size_M = (rand() & 0xf) + 5;
	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("power_mod(X, N, M) == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 지수가 1인 경우 */
	printf("[Case: Exponent is 1]\n");

	BI_Set_One(&bi_N);
	size1 = (rand() & 0xf) + 10;
	size_M = (rand() & 0xf) + 5;
	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("power_mod(X, N, M) == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 지수가 음수인 경우 */
	printf("[Case: Exponent is negative]\n");

	size1 = (rand() & 0xf) + 10;
	size_N = (rand() & 0xf) + 1;
	size_M = (rand() & 0xf) + 5;

	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_N, NEGATIVE, size_N);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);;

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("power_mod(X, N, M) == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);
#endif
	return SUCCESS;
}

int modular_exponentiation_addition_test()
{
	int i = 0;
	int size1 = 0;
	int size_M = 0;
	int size_N = 0;
	int ret = 0;

	bigint* bi_1 = NULL;
	bigint* bi_M = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	if (FLAG_EXP == LTOR)
		printf("print(\"Modular exponentiation of addition [Left to Right ver]\")\n");
	else if (FLAG_EXP == RTOL)
		printf("print(\"Modular exponentiation of addition [Right to Left ver]\")\n");
	else if (FLAG_EXP == MONTGOMERY)
		printf("print(\"Modular exponentiation of addition [Montgomery Ladder ver]\")\n");

	/* 랜덤한 테스트 벡터 */
	printf("print(\"[Case: Randdom test vector]\")\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = (rand() & 0xf) + 50;
		size_N = (rand() & 0xf) + 10;
		size_M = (rand() & 0xf) + 5;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N); // bi_n의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성

		printf("X = ");
		BI_Show(bi_1, 16);

		printf("N = ");
		BI_Show(bi_N, 16);

		printf("M = ");
		BI_Show(bi_M, 16);

		ret = Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);
		if (ret == ERROR)
			printf("FAILED\n\n");
		else
		{
			printf("(X * N) %% M == ");
			BI_Show(bi_re, 16);
			printf("\n");
		}

		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
		BI_Delete(&bi_M);
	}
#ifdef TEST_CASE
	/* 피연산자가 NULL인 경우 */
	printf("[Case: Operand is NULL]\n");

	ret = Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");

	/* 지수승을 수행하는 값이 0인 경우 */
	printf("[Case: Operand is 0]\n");

	BI_Set_Zero(&bi_1);
	size_N = (rand() & 0xf) + 10;
	size_M = (rand() & 0xf) + 5;
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("(X * N) %% M == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 지수승을 수행하는 값이 1인 경우 */
	printf("[Case: Operand is 1]\n");

	BI_Set_One(&bi_1);
	size_N = (rand() & 0xf) + 10;
	size_M = (rand() & 0xf) + 5;
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("(X * N) %% M == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 지수승을 수행하는 값이 음수인 경우 */
	printf("[Case: Operand is negative]\n");

	size1 = (rand() & 0xf) + 50;
	size_N = (rand() & 0xf) + 10;
	size_M = (rand() & 0xf) + 5;

	BI_Gen_Rand(&bi_1, NEGATIVE, size1);
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("(X * N) %% M == ");;
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 모듈러 취하는 값이 0인 경우 */
	printf("[Case: Modular with 0]\n");

	BI_Set_Zero(&bi_M);
	size1 = (rand() & 0xf) + 50;
	size_N = (rand() & 0xf) + 10;
	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("(X * N) %% M == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 모듈러 취하는 값이 1인 경우 */
	printf("[Case: Modular with 1]\n");

	BI_Set_One(&bi_M);
	size1 = (rand() & 0xf) + 50;
	size_N = (rand() & 0xf) + 10;
	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("(X * N) %% M == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 모듈러 취하는 값이 음수인 경우 */
	printf("[Case: Modular with negative number]\n");

	size1 = (rand() & 0xf) + 50;
	size_N = (rand() & 0xf) + 10;
	size_M = (rand() & 0xf) + 5;

	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N);
	BI_Gen_Rand(&bi_M, NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("(X * N) %% M == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 지수가 0인 경우 */
	printf("[Case: Exponent is 0]\n");

	BI_Set_Zero(&bi_N);
	size1 = (rand() & 0xf) + 50;
	size_M = (rand() & 0xf) + 5;
	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("(X * N) %% M == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 지수가 1인 경우 */
	printf("[Case: Exponent is 1]\n");

	BI_Set_One(&bi_N);
	size1 = (rand() & 0xf) + 50;
	size_M = (rand() & 0xf) + 5;
	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("(X * N) %% M == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);

	/* 지수가 음수인 경우 */
	printf("[Case: Exponent is negative]\n");

	size1 = (rand() & 0xf) + 50;
	size_N = (rand() & 0xf) + 10;
	size_M = (rand() & 0xf) + 5;

	BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1);
	BI_Gen_Rand(&bi_N, NEGATIVE, size_N);
	BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M);;

	printf("X = ");
	BI_Show(bi_1, 16);
	printf("N = ");
	BI_Show(bi_N, 16);
	printf("M = ");
	BI_Show(bi_M, 16);

	ret = Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);
	if (ret == ERROR)
		printf("FAILED\n\n");
	else
	{
		printf("(X * N) %% M == ");
		BI_Show(bi_re, 16);
		printf("\n");
	}

	BI_Delete(&bi_1);
	BI_Delete(&bi_N);
	BI_Delete(&bi_re);
	BI_Delete(&bi_M);
#endif

	return SUCCESS;
}

/************************************** Measuring Time ********************************************/

int addition_time()
{
	int i = 0;
	int size, size1, size2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	printf("\"Addition Test\"\n");

	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = TEST_SIZE;
		size2 = TEST_SIZE;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = MAX(bi_1->wordlen, bi_2->wordlen);
		BI_New(&bi_re, size + 1); // 더 긴 길이인 size로 덧셈 연산 결과인 big integer 생성

		clock_t start = clock();
		ADD(&bi_re, &bi_1, &bi_2); // ADD함수에 덧셈 결과인 bi_re , 피연산자 1인 bi_1, 피연산자 2인 bi_2
		clock_t end = clock();
		total_time += (end - start);

		// 메모리 free
		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
	return 0;
}

int subtraction_time()
{
	int i = 0;
	int size, size1, size2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("\"Subtraction\"\n");

	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = TEST_SIZE;
		size2 = TEST_SIZE;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = (size1 > size2) ? size1 : size2; // size1 과 size2 비교해서 size 변수에 대입
		BI_New(&bi_re, size + 1);

		clock_t start = clock();
		SUB(&bi_re, bi_1, bi_2);
		clock_t end = clock();
		total_time += (float)(end - start);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}

	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
	return 0;
}

int schoolbook_multiplication_time()
{
	int i = 0;
	int size1, size2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("\"Schoolbook Multiplication\"\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = TEST_SIZE;
		size2 = TEST_SIZE;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		BI_New(&bi_re, size1 + size2);

		clock_t start = clock();
		MUL_Multi(&bi_re, bi_1, bi_2);
		clock_t end = clock();
		total_time += (float)(end - start);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
	return 0;
}

int karatsuba_multiplication_time()
{
	int i = 0;
	int size1, size2 = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("\"Karatsuba Multiplication\"\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = TEST_SIZE;
		size2 = TEST_SIZE;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		len = (MAX(bi_1->wordlen, bi_2->wordlen) + 1) >> 1;
		len = 4 * len + 1;
		BI_New(&bi_re, len);

		sign_bi_1 = BI_Get_Sign(bi_1);
		sign_bi_2 = BI_Get_Sign(bi_2);

		if (sign_bi_1 == ERROR || sign_bi_2 == ERROR)
		{
			printf("get sign error\n");
			return ERROR;
		}

		if (sign_bi_1 == NEGATIVE)
			BI_Flip_Sign(bi_1);

		if (sign_bi_2 == NEGATIVE)
			BI_Flip_Sign(bi_2);

		clock_t start = clock();
		MUL_Karatsuba(&bi_re, bi_1, bi_2);
		clock_t end = clock();
		total_time += (float)(end - start);

		BI_Refine(bi_re);

		if ((sign_bi_1 ^ sign_bi_2) == 1)
			bi_re->sign = NEGATIVE;

		else
			bi_re->sign = NON_NEGATIVE;

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
	return 0;
}

int binary_long_division_time()
{
	int i = 0;
	int size = 0;
	int size1 = 0;
	int size2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	printf("\n");
	printf("\"Binary Long Division\"\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = TEST_SIZE;
		size2 = TEST_SIZE;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2); // size2 길이인 big integer인 bi_2 생성

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = BI_Compare(&bi_1, &bi_2); // size1 과 size2 비교
		if (size == 2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			i--;
		}
		else
		{
			clock_t start = clock();
			Binary_Long_Div(&bi_q, &bi_re, bi_1, bi_2);
			clock_t end = clock();
			total_time += (float)(end - start);

			// 메모리 free
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_re);
			BI_Delete(&bi_q);
		}
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
	return 0;
}

int multi_long_division_time()
{
	int i = 0;
	int size = 0;
	int size1 = 0;
	int size2 = 0;
	float total_time = 0;
	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	printf("\n");
	printf("\"Multi Long Division\"\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size2 = TEST_SIZE;
		size1 = TEST_SIZE;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2); // size2 길이인 big integer인 bi_2 생성

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = BI_Compare(&bi_1, &bi_2); // size1 과 size2 비교
		if (size == 2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			i--;
		}
		else
		{
			clock_t start = clock();
			DIV(&bi_q, &bi_re, bi_1, bi_2);
			clock_t end = clock();
			total_time += (float)(end - start);

			// 메모리 free
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_re);
			BI_Delete(&bi_q);
		}
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
	return 0;
}

int squaring_time()
{
	int i = 0;
	int size1, size2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("\"Squaring\"\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = TEST_SIZE;
		BI_Gen_Rand(&bi_1, 0, size1);

		clock_t start = clock();
		SQU(&bi_re, bi_1);
		clock_t end = clock();
		total_time += (float)(end - start);

		BI_Refine(bi_re);

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);

	return 0;
}

int squaring_karatsuba_time()
{
	int i = 0;
	int size1 = 0;
	int len = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("\"Squaring Karatsuba\"\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = TEST_SIZE;
		BI_New(&bi_1, size1);

		len = (bi_1->wordlen + 1) >> 1;
		len = 4 * len + 1;

		BI_New(&bi_re, len);

		clock_t start = clock();
		SQUC_Karatsuba(&bi_re, bi_1);
		clock_t end = clock();
		total_time += (float)(end - start);
		BI_Refine(bi_re);

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);

	return 0;
}

int montgomery_ladder_Modular_Exponentiation_multiplication_time()
{
	int i = 0;
	int size1 = 0;
	int size_M = 0;
	int size_n;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;
	bigint* bi_M = NULL;

	printf("\n");
	printf("\"Montgomery Ladder modular exponentiation multiplacation\"\n");
	for (i = 0; i < 10; i++)
	{
		size1 = TEST_SIZE;
		size_M = TEST_SIZE;
		size_n = 2;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_n); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		clock_t start = clock();
		MOD_EXP_Montgomery_MUL(&bi_re, bi_1, bi_N, bi_M);
		clock_t end = clock();
		total_time += (float)(end - start);
		BI_Refine(bi_re);

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
		BI_Delete(&bi_N);
		BI_Delete(&bi_M);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
	return 0;
}

int montgomery_ladder_Modular_Exponentiation_addtion_time()
{
	int i = 0;
	int size1 = 0;
	int size_M = 0;
	int size_n;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;
	bigint* bi_M = NULL;

	printf("\n");
	printf("\"Montgomery Ladder modular exponentiation addition\"\n");
	for (i = 0; i < 20; i++)
	{
		size1 = TEST_SIZE;
		size_M = TEST_SIZE;
		size_n = 2;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_n); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		clock_t start = clock();
		MOD_EXP_Montgomery_ADD(&bi_re, bi_1, bi_N, bi_M);
		clock_t end = clock();
		total_time += (float)(end - start);
		BI_Refine(bi_re);

		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_M);
		BI_Delete(&bi_re);
	}

	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);

	return 0;
}

int Left_Right_EXP_ADD_MOD_time() // Modular Exponentiaition used Binary Long Division L->R +
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("\"Left to Right modular exp add time\"\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x7) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_N);
			return 0;
		}

		clock_t start = clock();
		MOD_EXP_LR_ADD(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (float)(end - start);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);

	return 0;
}

int Left_Right_EXP_MUL_MOD_time() // Modular Exponentiaition used Binary Long Division L->R *
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("\"Left to Right modular exp mul time\"\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_N);
			return 0;
		}

		clock_t start = clock();
		MOD_EXP_LR_MUL(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (float)(end - start);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);

	return 0;
}

int Right_Left_EXP_ADD_MOD_time() // Modular Exponentiaition used Binary Long Division L<-R +
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("\"Right to Left Modular exp add time\"\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_N);
			return 0;
		}

		clock_t start = clock();
		MOD_EXP_RL_ADD(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (float)(end - start);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);

	return 0;
}

int Right_Left_EXP_MUL_MOD_time() // Modular Exponentiaition used Binary Long Division L<-R *
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("\"Right to Left Modular exm mul time\"\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_N);
			return 0;
		}

		clock_t start = clock();
		MOD_EXP_RL_MUL(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (float)(end - start);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);

	return 0;
}

int compare_text_kara_mul()
{
	int i = 0;
	int size1, size2 = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;
	float total_time_text = 0;
	float total_time_kara = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("\"Compare Textbook Multiplication with Karatsuba Multiplication\"\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = TEST_SIZE;
		size2 = TEST_SIZE;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		BI_New(&bi_re, size1 + size2);

		clock_t start = clock();
		MUL_Multi(&bi_re, bi_1, bi_2);
		clock_t end = clock();
		total_time_text += (float)(end - start);

		BI_Delete(&bi_re);

		len = (MAX(bi_1->wordlen, bi_2->wordlen) + 1) >> 1;
		len = 4 * len + 1;
		BI_New(&bi_re, len);

		sign_bi_1 = BI_Get_Sign(bi_1);
		sign_bi_2 = BI_Get_Sign(bi_2);

		if (sign_bi_1 == ERROR || sign_bi_2 == ERROR)
		{
			printf("get sign error\n");
			return ERROR;
		}

		if (sign_bi_1 == NEGATIVE)
			BI_Flip_Sign(bi_1);

		if (sign_bi_2 == NEGATIVE)
			BI_Flip_Sign(bi_2);

		clock_t start_2 = clock();
		MUL_Karatsuba(&bi_re, bi_1, bi_2);
		clock_t end_2 = clock();
		total_time_kara += (float)(end_2 - start_2);

		BI_Refine(bi_re);

		if ((sign_bi_1 ^ sign_bi_2) == 1)
			bi_re->sign = NEGATIVE;

		else
			bi_re->sign = NON_NEGATIVE;

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);

	}
	
	total_time_text = total_time_text / (CLOCKS_PER_SEC);
	total_time_kara = total_time_kara / (CLOCKS_PER_SEC);
	
	printf("Total[%d] time of test [textbook multiplication] : %fs\n", i, total_time_text);
	printf("Avearage time of test [textbook multiplication] : %fs\n\n", total_time_text / i);
	printf("Total[%d] time of test [karatsuba multiplication] : %fs\n", i, total_time_kara);
	printf("Avearage time of test [karatsuba multiplication] : %fs\n\n", total_time_kara / i);
	
	if (total_time_kara <= total_time_text)
		printf("[Karatsuba Multiplication] is faster [%.2f] times\n\n", (double)(total_time_text / total_time_kara));
	else
		printf("[Textbook Multiplication] is faster [%.2f] times\n\n", (double)(total_time_kara / total_time_text));

	return 0;
}

int compare_text_kara_squ()
{
	int i = 0;
	int size1, size2 = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;
	float total_time_text = 0;
	float total_time_kara = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("\"Compare Textbook Squaring with Karatsuba Squaring\"\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = TEST_SIZE;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		
		clock_t start = clock();
		SQU(&bi_re, bi_1);
		clock_t end = clock();
		total_time_text += (float)(end - start);

		BI_Refine(bi_re);
		BI_Delete(&bi_re);

		len = (bi_1->wordlen + 1) >> 1;
		len = 4 * len + 1;

		BI_New(&bi_re, len);

		clock_t start_2 = clock();
		SQUC_Karatsuba(&bi_re, bi_1);
		clock_t end_2 = clock();
		total_time_kara += (float)(end_2 - start_2);
		BI_Refine(bi_re);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}

	total_time_text = total_time_text / (CLOCKS_PER_SEC);
	total_time_kara = total_time_kara / (CLOCKS_PER_SEC);

	printf("Total[%d] time of test [textbook multiplication] : %fs\n", i, total_time_text);
	printf("Avearage time of test [textbook multiplication] : %fs\n\n", total_time_text / i);
	printf("Total[%d] time of test [karatsuba multiplication] : %fs\n", i, total_time_kara);
	printf("Avearage time of test [karatsuba multiplication] : %fs\n\n", total_time_kara / i);

	if (total_time_kara <= total_time_text)
		printf("[Karatsuba Squaring] is faster [%.2f] times\n\n", (double)(total_time_text / total_time_kara));
	else
		printf("[Textbook Squaring] is faster [%.2f] times\n\n", (double)(total_time_kara / total_time_text));

	return 0;
}

int compare_binary_multi_long_div()
{
	int i = 0;
	int size = 0;
	int size1 = 0;
	int size2 = 0;
	float total_time_binary = 0;
	float total_time_multi = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	printf("\n");
	printf("\"Compare Binary Long Division with Multi Precision Long Division\"\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = TEST_SIZE;
		size2 = TEST_SIZE;	

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = BI_Compare(&bi_1, &bi_2); // size1 과 size2 비교
		if (size == 2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			i--;
		}
		else
		{
			clock_t start = clock();
			Binary_Long_Div(&bi_q, &bi_re, bi_1, bi_2);
			clock_t end = clock();
			total_time_binary += (float)(end - start);

			BI_Delete(&bi_re);
			BI_Delete(&bi_q);

			clock_t start_2 = clock();
			DIV(&bi_q, &bi_re, bi_1, bi_2);
			clock_t end_2 = clock();
			total_time_multi += (float)(end_2 - start_2);

			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_re);
			BI_Delete(&bi_q);
		}
	}

	total_time_binary = total_time_binary / (CLOCKS_PER_SEC);
	total_time_multi = total_time_multi / (CLOCKS_PER_SEC);

	printf("Total[%d] time of test [textbook multiplication] : %fs\n", i, total_time_binary);
	printf("Avearage time of test [textbook multiplication] : %fs\n\n", total_time_binary / i);
	printf("Total[%d] time of test [karatsuba multiplication] : %fs\n", i, total_time_multi);
	printf("Avearage time of test [karatsuba multiplication] : %fs\n\n", total_time_multi / i);

	if (total_time_binary <= total_time_multi)
		printf("[Binary Long Division] is faster [%.2f] times\n\n", (double)(total_time_multi / total_time_binary));
	else
		printf("[Multi Precision Long Division] is faster [%.2f] times\n\n", (double)(total_time_binary / total_time_multi));

	return 0;
}

int compare_mod_exp_Montgomery_LtoR()
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int size_N = 0;
	int sign1 = 0;
	int sign2 = 0;

	float total_time_mont = 0;
	float total_time_ltor = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_N = NULL;

	printf("\n");
	printf("\"Compare Montgomerry Ladder Modular Exponentiation with Left to Right Modular Exponentiation\"\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		size_N = 2;
		
		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, NON_NEGATIVE, size2); // bi_M 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		clock_t start = clock();
		MOD_EXP_Montgomery_MUL(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time_mont += (float)(end - start);
		BI_Refine(bi_re);
		BI_Delete(&bi_re);

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_N);
			return 0;
		}

		clock_t start_2 = clock();
		MOD_EXP_LR_MUL(&bi_re, bi_1, bi_N, bi_2);
		clock_t end_2 = clock();
		total_time_ltor += (float)(end_2 - start_2);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}

	total_time_mont = total_time_mont / (CLOCKS_PER_SEC);
	total_time_ltor = total_time_ltor / (CLOCKS_PER_SEC);

	printf("Total[%d] time of test [textbook multiplication] : %fs\n", i, total_time_mont);
	printf("Avearage time of test [textbook multiplication] : %fs\n\n", total_time_mont / i);
	printf("Total[%d] time of test [karatsuba multiplication] : %fs\n", i, total_time_ltor);
	printf("Avearage time of test [karatsuba multiplication] : %fs\n\n", total_time_ltor / i);

	if (total_time_ltor <= total_time_mont)
		printf("[Left to Right Modular Exponentiation] is faster [%.2f] times\n\n", (double)(total_time_mont / total_time_ltor));
	else
		printf("[Montgomerry Ladder Modular Exponentiation] is faster [%.2f] times\n\n", (double)(total_time_ltor / total_time_mont));

	return 0;
}
