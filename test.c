#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "BasicOperation.h"
#include "Calculation.h"
#include "test.h"

int addition_test()
{
	int i = 0;
	int size, size1, size2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	/* n�� ���� �����ϱ� */ // 8, 32, 64

	printf("\n");
	printf("print(\"Addition\")\n");

	for (i = 0; i < TEST_COUNT; i++) // n�� 20 ����
	{
		size1 = (rand() & 0x7) + 30; // size1 : 1~16
		size2 = (rand() & 0x7) + 30; // size2 : 1~16

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		size = MAX(bi_1->wordlen, bi_2->wordlen);
		BI_New(&bi_re, size + 1); // �� �� ������ size�� ���� ���� ����� big integer ����

		// ��� �κ�
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		ADD(&bi_re, &bi_1, &bi_2); // ADD�Լ��� ���� ����� bi_re , �ǿ����� 1�� bi_1, �ǿ����� 2�� bi_2
		printf("A + B == ");
		BI_Show(bi_re, 16);
		printf("\n");

		// �޸� free
		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}
	return 0;
}

int subtraction_test()
{
	int i = 0;
	int size, size1, size2 = 0;
	//int len = 0;
	//int sign_bi_1 = 0;
	//int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	//bigint* bi_q = NULL;

	/* n�� ���� �����ϱ� */ // 8, 32, 64

	printf("\n");
	printf("print(\"Subtraction\")\n");

	for (i = 0; i < TEST_COUNT; i++) // n�� 20 ����
	{
		size1 = (rand() & 0x7) + 30; // size1 : 1~16
		size2 = (rand() & 0x7) + 30; // size2 : 1~16

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		size = (size1 > size2) ? size1 : size2; // size1 �� size2 ���ؼ� size ������ ����
		BI_New(&bi_re, size + 1); //size + 1?

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);

		SUB(&bi_re, bi_1, bi_2);

		printf("A - B == ");
		BI_Show(bi_re, 16);
		printf("\n");
		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}

	return 0;
}

int multiplication_test()
{
	int i = 0;
	int size1, size2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	/* n�� Schoolbook ���� �����ϱ� */ // 8, 32, 64

	printf("\n");
	if (FLAG_MUL == TEXTBOOK)
		printf("print(\"Multiplication [vTextbook ver]\")\n");
	else if (FLAG_MUL == KARATSUBA)
		printf("print(\"Multiplication [Karatsuba ver]\")\n");

	for (i = 0; i < TEST_COUNT; i++) //n�� ���� �����ϱ�
	{
		size1 = (rand() & 0xf) + 60;
		size2 = (rand() & 0xf) + 60;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);

		Multiplication(&bi_re, bi_1, bi_2);

		printf("A * B == ");
		BI_Show(bi_re, 16);
		printf("\n");
		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}
	return 0;
}

int division_test()
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	//int re = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n�� Multi Long Division �����ϱ� */ // 8, 32, 64

	printf("\n");
	if (FLAG_DIV == BINARY_LONG)
		printf("print(\"Division [Binary long ver]\")\n");
	else if (FLAG_DIV == MULTI_PRE)
		printf("print(\"Division [Multi-precision ver]\")\n");

	for (i = 0; i < TEST_COUNT; i++)
	{
		size2 = (rand() & 0x7) + 20; // size2 : 1~4
		size1 = size2 + (rand() & 0x7) + 10;//size2 + (rand() & 3) + 1; // size1�� size2 ���� ���� �� ���

		BI_New(&bi_1, size1); // size1 ������ big integer�� bi_1 ����
		BI_New(&bi_2, size2); // size2 ������ big integer�� bi_2 ����

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		Division(&bi_q, &bi_re, bi_1, bi_2);

		// ��� �κ�
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("Q = ");
		BI_Show(bi_q, 16);
		printf("R = ");
		BI_Show(bi_re, 16);
		printf("A == B * Q + R");

		printf("\n");

		// �޸� free
		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
		BI_Delete(&bi_q);
	}
	return 0;
}

int squaring_test()
{
	int i = 0;
	int size1 = 0;
	//int len = 0;
	//int sign_bi_1 = 0;
	//int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_re = NULL;;

	/* n�� Squaring ���� �����ϱ� */ // 8, 32, 64

	printf("\n");
	if (FLAG_SQU == TEXTBOOK)
		printf("print(\"Squaring [Textbook ver]\")\n");
	else if (FLAG_SQU == KARATSUBA)
		printf("print(\"Squaring [Karatsuba ver]\")\n");

	for (i = 0; i < TEST_COUNT; i++) //n�� ���� �����ϱ�
	{
		size1 = (rand() & 0xf) + 1;
		size1 = 1;
		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		//BI_New(&bi_1, 1);
		//word a = 0x7aafd8aa;
		//BI_Set_By_Array(&bi_1, 0, &a, 1);

		printf("A = ");
		BI_Show(bi_1, 16);

		Squaring(&bi_re, bi_1);

		printf("A * A == ");
		BI_Show(bi_re, 16);
		printf("\n");
		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
	return SUCCESS;
}

int squaring_karatsuba_test()
{
	int i = 0;
	int size1 = 0;
	int len = 0;
	//int sign_bi_1 = 0;
	//int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	//bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	//bigint* bi_q = NULL;

	/* n�� Squaring Karatsuba ���� �����ϱ� */ // 8, 32, 64 ���� 21 �̻���� �� ��

	printf("\n");
	printf("print(\"Squaring Karatsuba\")\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = (rand() & 0xf) + 100;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		printf("A = ");
		BI_Show(bi_1, 16);

		len = (bi_1->wordlen + 1) >> 1;
		len = 4 * len + 1;

		BI_New(&bi_re, len);

		SQUC_Karatsuba(&bi_re, bi_1);
		BI_Refine(bi_re);

		printf("A * A == ");
		BI_Show(bi_re, 16);

		printf("\n");

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
	return 0;
}

int modular_exponentiation_multiplication_test()
{
	int i = 0;
	int size1 = 0;
	int size_M = 0;
	int size_N = 0;

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

	for (i = 0; i < TEST_COUNT; i++) // TEST_COUNT
	{
		size1 = (rand() & 0xf) + 10;
		size_N = (rand() & 0xf) + 1;
		size_M = (rand() & 0xf) + 5;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1�� ��ȣ�� ����� �Ͽ� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N); // bi_n�� ��ȣ�� ����� �Ͽ� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M�� ��ȣ�� ����� �Ͽ� ������ �迭�� ���� bigint ����

		printf("X = ");
		BI_Show(bi_1, 16);

		printf("N = ");
		BI_Show(bi_N, 16);

		printf("M = ");
		BI_Show(bi_M, 16);

		Modular_Exponentiation_MUL(&bi_re, bi_1, bi_N, bi_M);

		printf("power_mod(X, N, M) == ");
		BI_Show(bi_re, 16);

		printf("\n");

		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
		BI_Delete(&bi_M);
	}

	return SUCCESS;
}

int modular_exponentiation_addition_test()
{
	int i = 0;
	int size1 = 0;
	int size_M = 0;
	int size_N = 0;

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

	for (i = 0; i < TEST_COUNT; i++)
	{
		size1 = (rand() & 0xf) + 50;
		size_N = (rand() & 0xf) + 10;
		size_M = (rand() & 0xf) + 5;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1�� ��ȣ�� ����� �Ͽ� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_N, NON_NEGATIVE, size_N); // bi_n�� ��ȣ�� ����� �Ͽ� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M�� ��ȣ�� ����� �Ͽ� ������ �迭�� ���� bigint ����

		printf("X = ");
		BI_Show(bi_1, 16);

		printf("N = ");
		BI_Show(bi_N, 16);

		printf("M = ");
		BI_Show(bi_M, 16);

		Modular_Exponentiation_ADD(&bi_re, bi_1, bi_N, bi_M);

		printf("(X * N) %% M == ");
		BI_Show(bi_re, 16);

		printf("\n");

		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
		BI_Delete(&bi_M);
	}
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
	//bigint* bi_q = NULL;

	/* n�� ���� �����ϱ� */ // 8, 32, 64


	printf("\"Addition Test\"\n");

	for (i = 0; i < TEST_COUNT; i++) // n�� 20 ����
	{
		size1 = (rand() & 0x7) + 30; // size1 : 1~16
		size2 = (rand() & 0x7) + 30; // size2 : 1~16

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		size = MAX(bi_1->wordlen, bi_2->wordlen);
		BI_New(&bi_re, size + 1); // �� �� ������ size�� ���� ���� ����� big integer ����

		clock_t start = clock();
		ADD(&bi_re, &bi_1, &bi_2); // ADD�Լ��� ���� ����� bi_re , �ǿ����� 1�� bi_1, �ǿ����� 2�� bi_2
		clock_t end = clock();
		total_time += (end - start);
		// �޸� free
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
	//int len = 0;
	//int sign_bi_1 = 0;
	//int sign_bi_2 = 0;
	float total_time = 0;
	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	//bigint* bi_q = NULL;

	/* n�� ���� �����ϱ� */ // 8, 32, 64

	printf("\n");
	printf("\"Subtraction\"\n");

	for (i = 0; i < TEST_COUNT; i++) // n�� 20 ����
	{
		size1 = (rand() & 0x7) + 30; // size1 : 1~16
		size2 = (rand() & 0x7) + 30; // size2 : 1~16

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		size = (size1 > size2) ? size1 : size2; // size1 �� size2 ���ؼ� size ������ ����
		BI_New(&bi_re, size + 1); //size + 1?

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
	//int len = 0;
	//int sign_bi_1 = 0;
	//int sign_bi_2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	//bigint* bi_q = NULL;

	/* n�� Schoolbook ���� �����ϱ� */ // 8, 32, 64

	printf("\"Schoolbook Multiplication\"\n");
	for (i = 0; i < TEST_COUNT; i++) //n�� ���� �����ϱ�
	{
		size1 = SIZE_1024;
		size2 = SIZE_1024;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

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
	//bigint* bi_q = NULL;

	/* n�� Karatsuba ���� �����ϱ� */ // 8, 32, 64

	printf("\n");
	printf("\"Karatsuba Multiplication\"\n");
	for (i = 0; i < TEST_COUNT; i++) //20�� ī���߹� ���� �����ϱ�
	{
		size1 = SIZE_1024;
		size2 = SIZE_1024;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

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
	return 0;
}

int binary_long_division_time()
{
	int i = 0;
	int size, size1, size2 = 0;
	//int len = 0;
	//int sign_bi_1 = 0;
	//int sign_bi_2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n�� Binary Long Division �����ϱ� */ // 8, 32, 64

	printf("\n");
	printf("\"Binary Long Division\"\n"); // �޸� �� ����
	for (i = 0; i < TEST_COUNT; i++)
	{
		size2 = SIZE_1024;
		size1 = SIZE_1024;


		BI_New(&bi_1, size1); // size1 ������ big integer�� bi_1 ����
		BI_New(&bi_2, size2); // size2 ������ big integer�� bi_2 ����

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		size = BI_Compare(&bi_1, &bi_2); // size1 �� size2 ��
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

			// �޸� free
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
	int size, size1, size2 = 0;
	//int len = 0;
	//int sign_bi_1 = 0;
	//int sign_bi_2 = 0;
	float total_time = 0;
	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n�� Multi Long Division �����ϱ� */ // 32, 64 (8�̰� size�� Ŭ ��� �� �� False)

	printf("\n");
	printf("\"Multi Long Division\"\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		size2 = SIZE_1024;
		size1 = SIZE_1024;

		BI_New(&bi_1, size1); // size1 ������ big integer�� bi_1 ����
		BI_New(&bi_2, size2); // size2 ������ big integer�� bi_2 ����

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		size = BI_Compare(&bi_1, &bi_2); // size1 �� size2 ��
		if (size == 2)
		{
			BI_Delete(&bi_1); // ũ�⺸�� �� ���� �� [30]¥�� �迭�ε� ����ڰ� ���Ƿ� 31ũ�⸸ŭ�� ���� �־������� ����ó��? 1201
			BI_Delete(&bi_2);
			i--;
		}
		else
		{
			clock_t start = clock();
			DIV(&bi_q, &bi_re, bi_1, bi_2);
			clock_t end = clock();
			total_time += (float)(end - start);

			// �޸� free
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
	//int len = 0;
	//int sign_bi_1 = 0;
	//int sign_bi_2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_re = NULL;

	/* n�� Squaring ���� �����ϱ� */ // 8, 32, 64

	printf("\n");
	printf("\"Squaring\"\n");
	for (i = 0; i < TEST_COUNT; i++) //20�� squaring
	{
		size1 = SIZE_1024;
		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ���� //������ �� �ȵǴ� �� ���ƺ��� 1201

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
	//int sign_bi_1 = 0;
	//int sign_bi_2 = 0;
	float total_time = 0;

	bigint* bi_1 = NULL;
	//bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	//bigint* bi_q = NULL;

	/* n�� Squaring Karatsuba ���� �����ϱ� */ // 8, 32, 64 ���� 21 �̻���� �� ��

	printf("\n");
	printf("print(\"Squaring Karatsuba\")\n");
	for (i = 0; i < TEST_COUNT; i++)
	{
		//size1 = (rand() & 0xf) + 100;
		size1 = SIZE_1024;

		//BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		// A = 0xcf55c5
		// A = 0x 02 c1 47 a1 b4 75 d1 b9 8e f6 22 a6 8b 2c 69 69 fa 8d 4e 31 1b e7
		//word arr[22] = { 0xe7, 0x1b, 0x31, 0x4e, 0x8d, 0xfa, 0x69, 0x69, 0x2c, 0x8b, 0xa6,
		//				 0x22, 0xf6, 0x8e, 0xb9, 0xd1, 0x75, 0xb4, 0xa1, 0x47, 0xc1, 0x02 };

		BI_New(&bi_1, size1);
		//BI_Set_By_Array(&bi_1, NON_NEGATIVE, arr, size1);

		len = (bi_1->wordlen + 1) >> 1;
		len = 4 * len + 1;

		BI_New(&bi_re, len);

		clock_t start = clock();
		SQUC_Karatsuba(&bi_re, bi_1);
		clock_t end = clock();
		total_time += (float)(end - start);
		BI_Refine(bi_re);

		printf("\n");

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
	//int len = 0;
	int n;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;
	bigint* bi_M = NULL;

	/* n�� Montgomery Ladder multiplacation �����ϱ� */
	printf("\n");
	printf("\"Montgomery Ladder modular exponentiation multiplacation\"\n");
	for (i = 0; i < 10; i++)
	{
		size1 = TEST_SIZE;
		size_M = TEST_SIZE;
		n = 1;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_N, NON_NEGATIVE, n); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

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
	//int len = 0;
	int n;
	float total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;
	bigint* bi_M = NULL;

	/* n�� Montgomery Ladder addition �����ϱ� */
	printf("\n");
	printf("\"Montgomery Ladder odular exponentiation addition\"\n");
	for (i = 0; i < 20; i++)
	{
		size1 = TEST_SIZE; // (rand() & 0xf) + 50;
		size_M = TEST_SIZE; // (rand() & 0xf) + 1;
		n = 1;// (rand() & 0xf) + 1;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_N, NON_NEGATIVE, n); // bi_N �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		clock_t start = clock();
		MOD_EXP_Montgomery_ADD(&bi_re, bi_1, bi_N, bi_M);
		clock_t end = clock();
		total_time += (float)(end - start);
		BI_Refine(bi_re);



		BI_Delete(&bi_1);
		BI_Delete(&bi_M);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);

	return 0;
}

void Left_Right_EXP_ADD_MOD_time() // Modular Exponentiaition used Binary Long Division L->R +
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
	printf("print(\"Left to Right modular exp add time\")\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x7) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 ������ big integer�� bi_1 ����
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_N, 0, len); // bi_N �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_N);
			return;
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
}

void Left_Right_EXP_MUL_MOD_time() // Modular Exponentiaition used Binary Long Division L->R *
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
	printf("print(\"Left to Right modular exp mul time\")\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 ������ big integer�� bi_1 ����
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, 0, size2); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_N, 0, len); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_N);
			return;
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
}

void Right_Left_EXP_ADD_MOD_time() // Modular Exponentiaition used Binary Long Division L<-R +
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
	printf("print(\"Right to Left Modular Exp ADD time\")\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 ������ big integer�� bi_1 ����
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_N, 0, len); // bi_N �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_N);
			return;
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
}

void Right_Left_EXP_MUL_MOD_time() // Modular Exponentiaition used Binary Long Division L<-R *
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
	printf("print(\"Right to Left Modular EXP MUL time\")\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 ������ big integer�� bi_1 ����
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
		BI_Gen_Rand(&bi_N, 0, len); // bi_N �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_N);
			return;
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
}
