#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bigint.h"

int addition_test()
{

	int i = 0;
	int size, size1, size2 = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	/* n번 덧셈 연산하기 */ // 8, 32, 64

	printf("\n");
	printf("print(\"Addition\")\n");

	for (i = 0; i < 50; i++) // n에 20 대입
	{
		size1 = (rand() & 0x7) + 30; // size1 : 1~16
		size2 = (rand() & 0x7) + 30; // size2 : 1~16

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = MAX(bi_1->wordlen, bi_2->wordlen);
		BI_New(&bi_re, size + 1); // 더 긴 길이인 size로 덧셈 연산 결과인 big integer 생성

		// 출력 부분
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		ADD(&bi_re, &bi_1, &bi_2); // ADD함수에 덧셈 결과인 bi_re , 피연산자 1인 bi_1, 피연산자 2인 bi_2
		printf("A + B == ");
		BI_Show(bi_re, 16);
		printf("\n");

		// 메모리 free
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
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;
	
	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n번 뺄셈 연산하기 */ // 8, 32, 64

	printf("\n");
	printf("print(\"Subtraction\")\n");

	for (i = 0; i < 50; i++) // n에 20 대입
	{
		size1 = (rand() & 0x7) + 30; // size1 : 1~16
		size2 = (rand() & 0x7) + 30; // size2 : 1~16

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = (size1 > size2) ? size1 : size2; // size1 과 size2 비교해서 size 변수에 대입
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

int schoolbook_multiplication_test()
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

	/* n번 Schoolbook 곱셈 연산하기 */ // 8, 32, 64

	printf("\n");
	printf("print(\"Schoolbook Multiplication\")\n");
	for (i = 0; i < 50; i++) //n번 곱셈 연산하기
	{
		size1 = (rand() & 0x7) + 30; // size1 : 1~16
		size2 = (rand() & 0x7) + 30; // size2 : 1~16

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		BI_New(&bi_re, size1 + size2);

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);

		MUL_Multi(&bi_re, bi_1, bi_2);

		printf("A * B == ");
		BI_Show(bi_re, 16);
		printf("\n");
		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}
	return 0;
}

int karatsuba_multiplication_test()
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

	/* n번 Karatsuba 곱셈 연산하기 */ // 8, 32, 64

	printf("\n\n");
	printf("print(\"Karatsuba Multiplication\")\n");
	for (i = 0; i < 1; i++) //20번 카라추바 곱셈 연산하기
	{
		size1 = (rand() & 0x3) + 100;
		size2 = (rand() & 0x3) + 100;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);

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


		Karatsuba(&bi_re, bi_1, bi_2);
		BI_Refine(bi_re);

		if ((sign_bi_1 ^ sign_bi_2) == 1)
			bi_re->sign = NEGATIVE;
			
		else
			bi_re->sign = NON_NEGATIVE;
			
		printf("A * B == ");
		BI_Show(bi_re, 16);

		printf("\n");

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}

	return 0;
}

int binary_long_division_test()
{
	int i = 0;
	int size, size1, size2 = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n번 Binary Long Division 연산하기 */ // 8, 32, 64

	printf("\n");
	printf("print(\"Binary Long Division\")\n"); // 메모리 릭 잡음
	for (i = 0; i < 50; i++)
	{
		size2 = (rand() & 0x7) + 20; // size2 : 1~4
		size1 = size2 + (rand() & 0x7) +30; // size1은 size2 보다 길이 더 길게


		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2); // size2 길이인 big integer인 bi_2 생성

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = BI_Compare(&bi_1, &bi_2); // size1 과 size2 비교
		if (size == -1)
		{
			printf("print(\"B is larger than A\")\n");
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
		}
		else
		{
			Binary_Long_Div(&bi_q, &bi_re, bi_1, bi_2);
						
			// 출력 부분
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
			// 메모리 free
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_re);
			BI_Delete(&bi_q);
		}
	}
	return 0;
}

int multi_long_division_test()
{
	int i = 0;
	int size, size1, size2 = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n번 Multi Long Division 연산하기 */ // 32, 64 (8이고 size가 클 경우 몇 개 False)

	printf("\n");
	printf("print(\"Multi Long Division\")\n");
	for (i = 0; i < 50; i++)
	{
		size2 = (rand() & 0x7) + 20; // size2 : 1~4
		size1 = size2 + (rand() & 0x7) + 10;//size2 + (rand() & 3) + 1; // size1은 size2 보다 길이 더 길게

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2); // size2 길이인 big integer인 bi_2 생성
		
		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = BI_Compare(&bi_1, &bi_2); // size1 과 size2 비교
		if (size == -1)
		{
			printf("print(\"B is larger than A\")\n");
			BI_Delete(&bi_1); // 크기보다 더 썼을 때 [30]짜리 배열인데 사용자가 임의로 31크기만큼에 값을 주었을때도 예외처리? 1201
			BI_Delete(&bi_2);
		}
		else
		{
			DIV(&bi_q, &bi_re, bi_1, bi_2);
						
			// 출력 부분
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
			// 메모리 free
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
			BI_Delete(&bi_re);
			BI_Delete(&bi_q);
		}
	}
	return 0;
}

int squaring_test()
{
	int i = 0;
	int size1, size2 = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	//bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	//bigint* bi_q = NULL;

	/* n번 Squaring 곱셈 연산하기 */ // 8, 32, 64

	printf("\n");
	printf("print(\"Squaring\")\n");
	for (i = 0; i < 20; i++) //20번 squaring
	{
		size1 = (rand() & 0xf) + 100;
		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		printf("A = ");
		BI_Show(bi_1, 16);

		//BI_New(&bi_re, size1 * 2 + 1);

		SQU(&bi_re, bi_1);
		BI_Refine(bi_re);

		printf("A * A == ");
		BI_Show(bi_re, 16);

		printf("\n");

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
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

	/* n번 Squaring Karatsuba 곱셈 연산하기 */ // 8, 32, 64 길이 21 이상부터 안 됨

	printf("\n");
	printf("print(\"Squaring Karatsuba\")\n");
	for (i = 0; i < 1; i++)
	{
		//size1 = (rand() & 0xf) + 100;
		size1 = 22;

		//BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		// A = 0xcf55c5
		// A = 0x 02 c1 47 a1 b4 75 d1 b9 8e f6 22 a6 8b 2c 69 69 fa 8d 4e 31 1b e7
		word arr[22] = { 0xe7, 0x1b, 0x31, 0x4e, 0x8d, 0xfa, 0x69, 0x69, 0x2c, 0x8b, 0xa6,
						 0x22, 0xf6, 0x8e, 0xb9, 0xd1, 0x75, 0xb4, 0xa1, 0x47, 0xc1, 0x02 };

		BI_New(&bi_1, size1);
		BI_Set_By_Array(&bi_1, NON_NEGATIVE, arr, size1);
		
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

int montgomery_Exponentiation_multiplication_test() // 너무 느림... ㅠ
{
	int i = 0;
	int size1 = 0;
	int size_n = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_n = NULL;
	bigint* bi_re = NULL;

	/* n번 Montgomery Ladder multiplacation 연산하기 */
	printf("\n");
	printf("print(\"Montgomery Ladder exponentiation multiplacation\")\n");
	for (i = 0; i < 20; i++)
	{
		//size1 = (rand() & 0xf) + 50;
		//size1 = (rand() & 0xf) + 1;
		//size_n = (rand() & 0xf) + 1;
		size1 = 2;
		size_n = 1;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_n, NON_NEGATIVE, size_n);

		printf("A = ");
		BI_Show(bi_1, 16);

		printf("n = ");
		BI_Show(bi_n, 16);

		Montgomery_Exp_Mul(&bi_re, bi_1, bi_n);
		BI_Refine(bi_re);

		printf("A ^ n == ");
		BI_Show(bi_re, 16);

		printf("\n");

		BI_Delete(&bi_1);
		BI_Delete(&bi_n);
		BI_Delete(&bi_re);
	}
	return 0;
}

int montgomery_ladder_Exponentiation_addtion_test()
{
	int i = 0;
	int size1, size_n = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_n = NULL;
	bigint* bi_re = NULL;

	/* n번 Montgomery Ladder multiplacation 연산하기 */
	printf("\n");
	printf("print(\"Montgomery Ladder exponentiation addition\")\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0xf) + 50;
		//size1 = (rand() & 0xf) + 1;
		size_n = (rand() & 0xf) + 1;
		//size1 = 2;
		//size_n = 2;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_n, NON_NEGATIVE, size_n);

		printf("A = ");
		BI_Show(bi_1, 16);

		printf("n = ");
		BI_Show(bi_n, 16);

		Montgomery_Exp_Add(&bi_re, bi_1, bi_n);
		BI_Refine(bi_re);

		printf("A * n == ");
		BI_Show(bi_re, 16);

		printf("\n");

		BI_Delete(&bi_1);
		BI_Delete(&bi_n);
		BI_Delete(&bi_re);
	}
	return 0;
}

int montgomery_ladder_Modular_Exponentiation_multiplication_test()
{
	int i = 0;
	int size1 = 0;
	int size_M = 0;
	int size_n = 0;
	int len = 0;
	int n;

	bigint* bi_1 = NULL;
	bigint* bi_n = NULL;
	bigint* bi_re = NULL;
	bigint* bi_M = NULL;

	/* n번 Montgomery Ladder multiplacation 연산하기 */
	printf("\n");
	printf("print(\"Montgomery Ladder modular exponentiation multiplacation\")\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0xf) + 50;
		//size1 = (rand() & 0xf) + 1;
		size_M = (rand() & 0xf) + 10;
		size_n = (rand() & 0xf) + 1;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_n, NON_NEGATIVE, size_n); // bi_n의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성

		printf("A = ");
		BI_Show(bi_1, 16);

		printf("n = ");
		BI_Show(bi_n, 16);

		printf("M = ");
		BI_Show(bi_M, 16);

		Montgomery_Mod_Exp_Mul(&bi_re, bi_1, bi_n, bi_M);
		BI_Refine(bi_re);

		printf("power_mod(A, n, M) == ");
		BI_Show(bi_re, 16);

		printf("\n");

		BI_Delete(&bi_1);
		BI_Delete(&bi_n);
		BI_Delete(&bi_re);
		BI_Delete(&bi_M);
	}
	return 0;
}

int montgomery_ladder_Modular_Exponentiation_addtion_test()
{
	int i = 0;
	int size1 = 0;
	int size_n = 0;
	int size_M = 0;
	int len = 0;
	int n;

	bigint* bi_1 = NULL;
	bigint* bi_n = NULL;
	bigint* bi_re = NULL;
	bigint* bi_M = NULL;

	/* n번 Montgomery Ladder addition 연산하기 */
	printf("\n");
	printf("print(\"Montgomery Ladder odular exponentiation addition\")\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0xf) + 50;
		size_n = (rand() & 0xf) + 10;
		size_M = (rand() & 0xf) + 1;
		n = (rand() & 0xf) + 1;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_n, NON_NEGATIVE, size_n); // bi_n의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M의 부호를 양수로 하여 랜덤한 배열을 담은 bigint 생성

		printf("A = ");
		BI_Show(bi_1, 16);

		printf("n = ");
		BI_Show(bi_n, 16);

		printf("M = ");
		BI_Show(bi_M, 16);

		Montgomery_Mod_Exp_Add(&bi_re, bi_1, bi_n, bi_M);
		BI_Refine(bi_re);

		printf("(A * n) %% M == ");
		BI_Show(bi_re, 16);

		printf("\n");

		BI_Delete(&bi_1);
		BI_Delete(&bi_n);
		BI_Delete(&bi_M);
		BI_Delete(&bi_re);
	}
	return 0;
}


void Left_Right_EXP_ADD_test() // Only Exponentiation function L->R add
{
	int i = 0;
	int size1 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Left to Right EXP ADD test\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		len = 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_N, len);
		
		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성
		bi_N->a[0] = 0x0a;
		//BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성

		EXP_LR_ADD(&bi_re, bi_1, bi_N);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("N * A == ");
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
}

void Left_Right_EXP_MUL_test() // Only Exponentiation function L->R mul
{
	int i = 0;
	int size1 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;
	printf("\n");
	printf("print(\"Left to Right EXP MUL test\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		len = 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_N, len); // len 길이인 big integer인 bi_N 생성
		
		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성
		bi_N->a[0] = 0x0a; //BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성

		EXP_LR_MUL(&bi_re, bi_1, bi_N);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("A ^ N == ");
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
}

void Right_Left_EXP_ADD_test() // Only Exponentiation function R<-L add
{
	int i = 0;
	int size1 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Left Exp ADD test\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		len = 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_N, len); // len 길이인 big integer인 bi_N 생성

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성
		bi_N->a[0] = 0x0a; //BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성

		EXP_RL_ADD(&bi_re, bi_1, bi_N);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("N * A == ");
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
}

void Right_Left_EXP_MUL_test()  // Only Exponentiation function R<-L mul
{
	int i = 0;
	int size1 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Letf Exp MUL test\")\n"); // bi_1의 size가 0일 떄의 예외처리
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		len = 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_N, len); // len 길이인 big integer인 bi_N 생성

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성
		bi_N->a[0] = 0x0a; //BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성

		EXP_RL_MUL(&bi_re, bi_1, bi_N);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("A^N == ");
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);

	}
}

void Left_Right_EXP_ADD_MOD_test() // Modular Exponentiaition used Binary Long Division L->R +
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Left to Right modular exp add test\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x7) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_LR_ADD(&bi_re, bi_1, bi_N, bi_2);

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("(N * A) %% B == ");
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
}

void Left_Right_EXP_MUL_MOD_test() // Modular Exponentiaition used Binary Long Division L->R *
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Left to Right modular exp mul test\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_LR_MUL(&bi_re, bi_1, bi_N, bi_2);

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("power_mod(A, N, B) == ");
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
}

void Right_Left_EXP_ADD_MOD_test() // Modular Exponentiaition used Binary Long Division L<-R +
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Left Modular Exp ADD test\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_RL_ADD(&bi_re, bi_1, bi_N, bi_2);

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("(N * A) %% B == ");
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
}

void Right_Left_EXP_MUL_MOD_test() // Modular Exponentiaition used Binary Long Division L<-R *
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Left Modular EXP MUL test\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		bi_N->a[0] = 0x2;
		MOD_EXP_RL_MUL(&bi_re, bi_1, bi_N, bi_2);

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("power_mod(A, N, B) == ");
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
}

void Left_Right_EXP_ADD_MOD_DIV_test() // Modular Exponentiation used Multi Long Division L->R +
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Left to Right modular exp add test (DIV func)\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_LR_ADD_DIV(&bi_re, bi_1, bi_N, bi_2);

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("(N * A) %% B == ");
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
}

void Left_Right_EXP_MUL_MOD_DIV_test() // Modular Exponentiation used Multi Long Division L->R *
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"left to right modular exp mul test (DIV func)\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_LR_MUL_DIV(&bi_re, bi_1, bi_N, bi_2);

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("power_mod(A, N, B) == ");
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
}

void Right_Left_EXP_ADD_MOD_DIV_test() // Modular Exponentiation used Multi Long Division L<-R +
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("print(\"Right to Left Modular Exp ADD test (DIV func)\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_RL_ADD_DIV(&bi_re, bi_1, bi_N, bi_2);

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("(N * A) %% B == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
}

void Right_Left_EXP_MUL_MOD_DIV_test() // Modular Exponentiation used Multi Long Division L<-R *
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;
	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Left Modular EXP MUL test (DIV func)\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0xf) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_RL_MUL_DIV(&bi_re, bi_1, bi_N, bi_2);

		printf("A = ");
		BI_Show(bi_1, 16);
		printf("N = ");
		BI_Show(bi_N, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("power_mod(A, N, B) == ");
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
}

int addition_time()
{

	int i = 0;
	int size, size1, size2 = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;
	double total_time = 0;
	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n번 덧셈 연산하기 */ // 8, 32, 64

	
	printf("\"Addition Test\"\n");

	for (i = 0; i < 50; i++) // n에 20 대입
	{
		size1 = (rand() & 0x7) + 30; // size1 : 1~16
		size2 = (rand() & 0x7) + 30; // size2 : 1~16

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = MAX(bi_1->wordlen, bi_2->wordlen);
		BI_New(&bi_re, size + 1); // 더 긴 길이인 size로 덧셈 연산 결과인 big integer 생성

		// 출력 부분
		/*printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);*/
		clock_t start = clock();
		ADD(&bi_re, &bi_1, &bi_2); // ADD함수에 덧셈 결과인 bi_re , 피연산자 1인 bi_1, 피연산자 2인 bi_2
		clock_t end = clock();
		total_time += (end - start);
		/*printf("A + B == ");
		BI_Show(bi_re, 16);
		printf("\n");*/

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
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;
	double total_time = 0;
	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n번 뺄셈 연산하기 */ // 8, 32, 64

	printf("\n");
	printf("\"Subtraction\"\n");

	for (i = 0; i < 50; i++) // n에 20 대입
	{
		size1 = (rand() & 0x7) + 30; // size1 : 1~16
		size2 = (rand() & 0x7) + 30; // size2 : 1~16

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = (size1 > size2) ? size1 : size2; // size1 과 size2 비교해서 size 변수에 대입
		BI_New(&bi_re, size + 1); //size + 1?

		/*printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);*/

		clock_t start = clock();
		SUB(&bi_re, bi_1, bi_2);
		clock_t end = clock();

		total_time += (double)(end - start);
		/*printf("A - B == ");
		BI_Show(bi_re, 16);
		printf("\n");*/
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
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n번 Schoolbook 곱셈 연산하기 */ // 8, 32, 64

	printf("\"Schoolbook Multiplication\"\n");
	for (i = 0; i < 50; i++) //n번 곱셈 연산하기
	{
		size1 = (rand() & 0x7) + 30; // size1 : 1~16
		size2 = (rand() & 0x7) + 30; // size2 : 1~16

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		BI_New(&bi_re, size1 + size2);

		/*printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);*/

		clock_t start = clock();
		MUL_Multi(&bi_re, bi_1, bi_2);
		clock_t end = clock();
		total_time += (end - start);

		/*printf("A * B == ");
		BI_Show(bi_re, 16);
		printf("\n");*/
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

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n번 Karatsuba 곱셈 연산하기 */ // 8, 32, 64

	printf("\n");
	printf("\"Karatsuba Multiplication\"\n");
	for (i = 0; i < 1; i++) //20번 카라추바 곱셈 연산하기
	{
		size1 = (rand() & 0x3) + 100;
		size2 = (rand() & 0x3) + 100;

		BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		/*printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);*/

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


		Karatsuba(&bi_re, bi_1, bi_2);
		BI_Refine(bi_re);

		if ((sign_bi_1 ^ sign_bi_2) == 1)
			bi_re->sign = NEGATIVE;

		else
			bi_re->sign = NON_NEGATIVE;

		/*printf("A * B == ");
		BI_Show(bi_re, 16);
		printf("\n");*/

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}

	return 0;
}

int binary_long_division_time()
{
	int i = 0;
	int size, size1, size2 = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n번 Binary Long Division 연산하기 */ // 8, 32, 64

	printf("\n");
	printf("\"Binary Long Division\"\n"); // 메모리 릭 잡음
	for (i = 0; i < 50; i++)
	{
		size2 = (rand() & 0x7) + 20; // size2 : 1~4
		size1 = size2 + (rand() & 0x7) + 30; // size1은 size2 보다 길이 더 길게


		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2); // size2 길이인 big integer인 bi_2 생성

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = BI_Compare(&bi_1, &bi_2); // size1 과 size2 비교
		if (size == -1)
		{
			printf("print(\"B is larger than A\")\n");
			BI_Delete(&bi_1);
			BI_Delete(&bi_2);
		}
		else
		{
			clock_t start = clock();
			Binary_Long_Div(&bi_q, &bi_re, bi_1, bi_2);
			clock_t end = clock();
			total_time += (end - start);

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
	int size, size1, size2 = 0;
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;
	double total_time = 0;
	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n번 Multi Long Division 연산하기 */ // 32, 64 (8이고 size가 클 경우 몇 개 False)

	printf("\"Multi Long Division\"\n");
	for (i = 0; i < 50; i++)
	{
		size2 = (rand() & 0x7) + 20; // size2 : 1~4
		size1 = size2 + (rand() & 0x7) + 10;//size2 + (rand() & 3) + 1; // size1은 size2 보다 길이 더 길게

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2); // size2 길이인 big integer인 bi_2 생성

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		size = BI_Compare(&bi_1, &bi_2); // size1 과 size2 비교
		if (size == -1)
		{
			printf("print(\"B is larger than A\")\n");
			BI_Delete(&bi_1); // 크기보다 더 썼을 때 [30]짜리 배열인데 사용자가 임의로 31크기만큼에 값을 주었을때도 예외처리? 1201
			BI_Delete(&bi_2);
		}
		else
		{
			clock_t start = clock();
			DIV(&bi_q, &bi_re, bi_1, bi_2);
			clock_t end = clock();
			total_time += (end - start);

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
	int len = 0;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_re = NULL;

	/* n번 Squaring 곱셈 연산하기 */ // 8, 32, 64

	printf("\n");
	printf("\"Squaring\"\n");
	for (i = 0; i < 20; i++) //20번 squaring
	{
		size1 = (rand() & 0xf) + 100;
		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성 //음수일 떄 안되는 것 같아보임 1201
		/*printf("A = ");
		BI_Show(bi_1, 16);*/

		//BI_New(&bi_re, size1 * 2 + 1);

		SQU(&bi_re, bi_1);
		BI_Refine(bi_re);

		/*printf("A * A == ");
		BI_Show(bi_re, 16);

		printf("\n");*/

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
	return 0;
}

int squaring_karatsuba_time()
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

	/* n번 Squaring Karatsuba 곱셈 연산하기 */ // 8, 32, 64 길이 21 이상부터 안 됨

	printf("\n");
	printf("print(\"Squaring Karatsuba\")\n");
	for (i = 0; i < 1; i++)
	{
		//size1 = (rand() & 0xf) + 100;
		size1 = 22;

		//BI_Gen_Rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		// A = 0xcf55c5
		// A = 0x 02 c1 47 a1 b4 75 d1 b9 8e f6 22 a6 8b 2c 69 69 fa 8d 4e 31 1b e7
		word arr[22] = { 0xe7, 0x1b, 0x31, 0x4e, 0x8d, 0xfa, 0x69, 0x69, 0x2c, 0x8b, 0xa6,
						 0x22, 0xf6, 0x8e, 0xb9, 0xd1, 0x75, 0xb4, 0xa1, 0x47, 0xc1, 0x02 };

		BI_New(&bi_1, size1);
		BI_Set_By_Array(&bi_1, NON_NEGATIVE, arr, size1);

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

int montgomery_Exponentiation_multiplication_time()
{
	int i = 0;
	int size1, size2 = 0;
	int len = 0;
	int n;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n번 Montgomery Ladder multiplacation 연산하기 */
	printf("\n");
	printf("\"Montgomery Ladder exponentiation multiplacation\"\n");
	for (i = 0; i < 20; i++)
	{
		//size1 = (rand() & 0xf) + 50;
		size1 = (rand() & 0xf) + 1;
		n = (rand() & 0xf) + 1;

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		printf("A = ");
		BI_Show(bi_1, 16);

		Montgomery_Exp_Mul(&bi_re, bi_1, n);
		BI_Refine(bi_re);

		/*printf("A ^ %d == ", n);
		BI_Show(bi_re, 16);

		printf("\n");*/

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
	return 0;
}

int montgomery_ladder_Exponentiation_addtion_time()
{
	int i = 0;
	int size1, size2 = 0;
	int len = 0;
	int n;
	int sign_bi_1 = 0;
	int sign_bi_2 = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_q = NULL;

	/* n번 Montgomery Ladder addition 연산하기 */
	printf("\n");
	printf("\"Montgomery Ladder exponentiation addition\"\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0xf) + 100;
		//size1 = (rand() & 0xf) + 1;
		n = (rand() & 0xf) + 1;

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		/*printf("A = ");
		BI_Show(bi_1, 16);*/

		Montgomery_Exp_Add(&bi_re, bi_1, n);
		BI_Refine(bi_re);

		/*printf("A * %d == ", n);
		BI_Show(bi_re, 16);

		printf("\n");*/

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
	return 0;
}

int montgomery_ladder_Modular_Exponentiation_multiplication_time()
{
	int i = 0;
	int size1 = 0;
	int size_M = 0;
	int len = 0;
	int n;

	bigint* bi_1 = NULL;
	//bigint* bi_2 = NULL; 삭제바람
	bigint* bi_re = NULL;
	bigint* bi_M = NULL;

	/* n번 Montgomery Ladder multiplacation 연산하기 */
	printf("\n");
	printf("\"Montgomery Ladder modular exponentiation multiplacation\"\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0xf) + 50;
		//size1 = (rand() & 0xf) + 1;
		size_M = (rand() & 0xf) + 1;
		n = (rand() & 0xf) + 1;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		/*printf("A = ");
		BI_Show(bi_1, 16);

		printf("M = ");
		BI_Show(bi_M, 16);*/

		Montgomery_Mod_Exp_Mul(&bi_re, bi_1, n, bi_M);
		BI_Refine(bi_re);

		/*printf("(A ^ %d) %% M == ", n);
		BI_Show(bi_re, 16);

		printf("\n");*/

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
		BI_Delete(&bi_M);
	}
	return 0;
}

int montgomery_ladder_Modular_Exponentiation_addtion_time()
{
	int i = 0;
	int size1 = 0;
	int size_M = 0;
	int len = 0;
	int n;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	bigint* bi_M = NULL;

	/* n번 Montgomery Ladder addition 연산하기 */
	printf("\n");
	printf("\"Montgomery Ladder odular exponentiation addition\"\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0xf) + 50;
		//size1 = (rand() & 0xf) + 1;
		size_M = (rand() & 0xf) + 1;
		n = (rand() & 0xf) + 1;

		BI_Gen_Rand(&bi_1, NON_NEGATIVE, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		BI_Gen_Rand(&bi_M, NON_NEGATIVE, size_M); // bi_M 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		/*printf("A = ");
		BI_Show(bi_1, 16);

		printf("M = ");
		BI_Show(bi_M, 16);*/

		Montgomery_Mod_Exp_Add(&bi_re, bi_1, n, bi_M);
		BI_Refine(bi_re);

		/*printf("(A * %d) %% M == ", n);
		BI_Show(bi_re, 16);

		printf("\n");*/

		BI_Delete(&bi_1);
		BI_Delete(&bi_M);
		BI_Delete(&bi_re);
	}
	return 0;
}

void Left_Right_EXP_ADD_time() // Only Exponentiation function L->R add
{
	int i = 0;
	int size1 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Left to Right EXP ADD time\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		len = 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_N, len);

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성
		bi_N->a[0] = 0x0a; // 너무 오래걸려서
		//BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_N);
			return;
		}
			
		clock_t start = clock();
		EXP_LR_ADD(&bi_re, bi_1, bi_N);
		clock_t end = clock();
		total_time += (double)end - (double)start;
		
		
		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
}

void Left_Right_EXP_MUL_time() // Only Exponentiation function L->R mul
{
	int i = 0;
	int size1 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;
	printf("\n");
	printf("print(\"Left to Right EXP MUL time\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		len = 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_N, len); // len 길이인 big integer인 bi_N 생성

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성
		bi_N->a[0] = 0x0a; //BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_N);
			return;
		}

		clock_t start = clock();
		EXP_LR_MUL(&bi_re, bi_1, bi_N);
		clock_t end = clock();
		total_time += (double)end - (double)start;
				
		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
}

void Right_Left_EXP_ADD_time() // Only Exponentiation function R<-L add
{
	int i = 0;
	int size1 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Left Exp ADD time\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		len = 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_N, len); // len 길이인 big integer인 bi_N 생성

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성
		bi_N->a[0] = 0x0a; // 길이가 길어서 원래는 아래줄
		//BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_N);
			return;
		}
		
		clock_t start = clock();
		EXP_RL_ADD(&bi_re, bi_1, bi_N);
		clock_t end = clock();
		total_time += (double)end - (double)start;

		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
}

void Right_Left_EXP_MUL_time()  // Only Exponentiation function R<-L mul
{
	int i = 0;
	int size1 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Letf Exp MUL time\")\n"); // bi_1의 size가 0일 떄의 예외처리
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		len = 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_N, len); // len 길이인 big integer인 bi_N 생성

		BI_Gen_Rand(&bi_1, 0, size1); // bi_1 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성
		bi_N->a[0] = 0x0a; // test용. 원래는 아랫 줄
		//BI_Gen_Rand(&bi_N, 0, len); // bi_N 의 부호가 양수인 랜덤한 배열을 담은 bigint 생성

		sign1 = BI_Get_Sign(bi_1);
		sign2 = BI_Get_Sign(bi_1);
		if (sign1 | sign2)
		{
			BI_Delete(&bi_1);
			BI_Delete(&bi_N);
			return;
		}

		clock_t start = clock();
		EXP_RL_MUL(&bi_re, bi_1, bi_N);
		clock_t end = clock();
		total_time += (double)end - (double)start;
				
		BI_Delete(&bi_1);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
}

void Left_Right_EXP_ADD_MOD_time() // Modular Exponentiaition used Binary Long Division L->R +
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Left to Right modular exp add time\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x7) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

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
			return;
		}

		clock_t start = clock();
		MOD_EXP_LR_ADD(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (double)end - (double)start;
		
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
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Left to Right modular exp mul time\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

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
			return;
		}

		clock_t start = clock();
		MOD_EXP_LR_MUL(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (double)end - (double)start;
		
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
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Left Modular Exp ADD time\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

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
			return;
		}

		clock_t start = clock();
		MOD_EXP_RL_ADD(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (double)end - (double)start;
		
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
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Left Modular EXP MUL time\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

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
			return;
		}

		clock_t start = clock();
		MOD_EXP_RL_MUL(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (double)end - (double)start;
		
		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
}

void Left_Right_EXP_ADD_MOD_DIV_time() // Modular Exponentiation used Multi Long Division L->R +
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Left to Right modular exp add time (DIV func)\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

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
			return;
		}

		clock_t start = clock();
		MOD_EXP_LR_ADD_DIV(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (double)end - (double)start;

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
}

void Left_Right_EXP_MUL_MOD_DIV_time() // Modular Exponentiation used Multi Long Division L->R *
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"left to right modular exp mul time (DIV func)\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

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
			return;
		}

		clock_t start = clock();
		MOD_EXP_LR_MUL_DIV(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (double)end - (double)start;
		
		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
}

void Right_Left_EXP_ADD_MOD_DIV_time() // Modular Exponentiation used Multi Long Division L<-R +
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int len = 0;
	int sign1 = 0;
	int sign2 = 0;
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("print(\"Right to Left Modular Exp ADD time (DIV func)\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0x3) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

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
			return;
		}

		clock_t start = clock();
		MOD_EXP_RL_ADD_DIV(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (double)end - (double)start;

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
}

void Right_Left_EXP_MUL_MOD_DIV_time() // Modular Exponentiation used Multi Long Division L<-R *
{
	int i = 0;
	int size1 = 0;
	int size2 = 0;
	int sign1 = 0;
	int sign2 = 0;
	int len = 0;
	double total_time = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_N = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Left Modular EXP MUL test (DIV func)\")\n");
	for (i = 0; i < 10; i++)
	{
		size1 = (rand() & 0x7) + 30;
		size2 = size1 - (rand() & 0xf) - 10;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);
		BI_New(&bi_N, len);

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
			return;
		}
		
		clock_t start = clock();
		MOD_EXP_RL_MUL_DIV(&bi_re, bi_1, bi_N, bi_2);
		clock_t end = clock();
		total_time += (double)end - (double)start;

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_N);
		BI_Delete(&bi_re);
	}
	total_time = total_time / (CLOCKS_PER_SEC);
	printf("Total[%d] time of test : %fs\n", i, total_time);
	printf("Avearage time of test  : %fs\n", total_time / i);
}
