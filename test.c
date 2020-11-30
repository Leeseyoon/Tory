#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bigint.h"

int addition_test()
{

    int i = 0;
    int size, size1, size2 = 0;
    int len = 0;
    int n;
    int sign_bi_1 = 0;
    int sign_bi_2 = 0;

    bigint* bi_1 = NULL;
    bigint* bi_2 = NULL;
    bigint* bi_re = NULL;
    bigint* bi_q = NULL;

    printf("print(\"WORD_BIT_LEN == %d\")\n", WORD_BIT_LEN);

    /* n번 덧셈 연산하기 */ // 8, 32, 64

    printf("\n\n");
    printf("print(\"Addition\")\n");

    for (i = 0; i < 20; i++) // n에 20 대입
    {
        size1 = (rand() & 0x3) + 1; // size1 : 1~16
        size2 = (rand() & 0x3) + 1; // size2 : 1~16

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

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
    int n;
    int sign_bi_1 = 0;
    int sign_bi_2 = 0;

    bigint* bi_1 = NULL;
    bigint* bi_2 = NULL;
    bigint* bi_re = NULL;
    bigint* bi_q = NULL;

    printf("print(\"WORD_BIT_LEN == %d\")\n", WORD_BIT_LEN);

    /* n번 뺄셈 연산하기 */ // 8, 32, 64

    printf("\n");
    printf("print(\"Subtraction\")\n");

    for (i = 0; i < 20; i++) // n에 20 대입
    {
        size1 = (rand() & 0x7) + 1; // size1 : 1~16
        size2 = (rand() & 0x7) + 1; // size2 : 1~16

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

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
    int size, size1, size2 = 0;
    int len = 0;
    int n;
    int sign_bi_1 = 0;
    int sign_bi_2 = 0;

    bigint* bi_1 = NULL;
    bigint* bi_2 = NULL;
    bigint* bi_re = NULL;
    bigint* bi_q = NULL;

    /* n번 Schoolbook 곱셈 연산하기 */ // 8, 32, 64

    printf("\n\n");
    printf("print(\"Schoolbook Multiplication\")\n");
    for (i = 0; i < 20; i++) //n번 곱셈 연산하기
    {
        size1 = (rand() & 0x7) + 1; // size1 : 1~16
        size2 = (rand() & 0x7) + 1; // size2 : 1~16

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        BI_New(&bi_re, size1 + size2);

        printf("A = ");
        BI_Show(bi_1, 16);
        printf("B = ");
        BI_Show(bi_2, 16);

        MUL_MUL(&bi_re, bi_1, bi_2);

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
    int size, size1, size2 = 0;
    int len = 0;
    int n;
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

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        printf("A = ");
        BI_Show(bi_1, 16);
        printf("B = ");
        BI_Show(bi_2, 16);

        len = (MAX(bi_1->wordlen, bi_2->wordlen) + 1) >> 1;
        len = 4 * len + 1;
        BI_New(&bi_re, len);

        sign_bi_1 = Get_Sign(bi_1);
        sign_bi_2 = Get_Sign(bi_2);

        if (sign_bi_1 == ERROR || sign_bi_2 == ERROR)
        {
            printf("get sign error\n");
            return ERROR;
        }

        if (sign_bi_1 == NEGATIVE)
            Flip_Sign(bi_1);
        if (sign_bi_2 == NEGATIVE)
            Flip_Sign(bi_2);

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

}

int binary_long_division_test()
{
    int i = 0;
    int size, size1, size2 = 0;
    int len = 0;
    int n;
    int sign_bi_1 = 0;
    int sign_bi_2 = 0;

    bigint* bi_1 = NULL;
    bigint* bi_2 = NULL;
    bigint* bi_re = NULL;
    bigint* bi_q = NULL;

    /* n번 Binary Long Division 연산하기 */ // 8, 32, 64

    printf("\n");
    printf("print(\"Binary Long Division\")\n"); // 메모리 릭 잡음
    for (i = 0; i < 20; i++)
    {
        size2 = (rand() & 3) + 1; // size2 : 1~4
        size1 = size2 + (rand() & 3) + 1; // size1은 size2 보다 길이 더 길게


        BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
        BI_New(&bi_2, size2); // size2 길이인 big integer인 bi_2 생성

        bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        bi_gen_rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        size = Compare_BI(&bi_1, &bi_2); // size1 과 size2 비교
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
    int n;
    int sign_bi_1 = 0;
    int sign_bi_2 = 0;

    bigint* bi_1 = NULL;
    bigint* bi_2 = NULL;
    bigint* bi_re = NULL;
    bigint* bi_q = NULL;

    /* n번 Multi Long Division 연산하기 */ // 32, 64 (8이고 size가 클 경우 몇 개 False)

    printf("\n");
    printf("print(\"Multi Long Division\")\n");
    for (i = 0; i < 20; i++)
    {
        size2 = (rand() & 0xf) + 20; // size2 : 1~4
        size1 = size2 + (rand() & 0xf);//size2 + (rand() & 3) + 1; // size1은 size2 보다 길이 더 길게

        BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
        BI_New(&bi_2, size2); // size2 길이인 big integer인 bi_2 생성

        bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        bi_gen_rand(&bi_2, 0, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        size = Compare_BI(&bi_1, &bi_2); // size1 과 size2 비교
        if (size == -1)
        {
            printf("print(\"B is larger than A\")\n");
            BI_Delete(&bi_1);
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
    int size, size1, size2 = 0;
    int len = 0;
    int n;
    int sign_bi_1 = 0;
    int sign_bi_2 = 0;

    bigint* bi_1 = NULL;
    bigint* bi_2 = NULL;
    bigint* bi_re = NULL;
    bigint* bi_q = NULL;

    /* n번 Squaring 곱셈 연산하기 */ // 8, 32, 64

    printf("\n");
    printf("print(\"Squaring\")\n");
    for (i = 0; i < 1; i++) //20번 squaring
    {
        size1 = (rand() & 0xf) + 100;
        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
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
    int size, size1, size2 = 0;
    int len = 0;
    int n;
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

        //bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        // A = 0xcf55c5
        // A = 0x 02 c1 47 a1 b4 75 d1 b9 8e f6 22 a6 8b 2c 69 69 fa 8d 4e 31 1b e7
        word arr[22] = { 0xe7, 0x1b, 0x31, 0x4e, 0x8d, 0xfa, 0x69, 0x69, 0x2c, 0x8b, 0xa6,
                         0x22, 0xf6, 0x8e, 0xb9, 0xd1, 0x75, 0xb4, 0xa1, 0x47, 0xc1, 0x02 };

        BI_New(&bi_1, size1);
        BI_Set_By_Array(&bi_1, NON_NEGATIVE, arr, size1);

        printf("A = ");
        BI_Show(bi_1, 16);

        //len = (bi_1->wordlen + 1) >> 1;
        //len = 4 * len + 1;

       // BI_New(&bi_re, len);

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

int montgomery_Exponentiation_multiplication_test()
{
    int i = 0;
    int size, size1, size2 = 0;
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
    printf("print(\"Montgomery Ladder exponentiation multiplacation\")\n");
    for (i = 0; i < 20; i++)
    {
        //size1 = (rand() & 0xf) + 50;
        size1 = (rand() & 0xf) + 1;
        n = (rand() & 0xf) + 1;

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        printf("A = ");
        BI_Show(bi_1, 16);

        Montgomery_Exp_mul(&bi_re, bi_1, n);
        BI_Refine(bi_re);

        printf("A ^ %d == ", n);
        BI_Show(bi_re, 16);

        printf("\n");

        BI_Delete(&bi_1);
        BI_Delete(&bi_re);
    }
    return 0;
}

int montgomery_ladder_Exponentiation_addtion_test()
{
    int i = 0;
    int size, size1, size2 = 0;
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
    printf("print(\"Montgomery Ladder exponentiation addition\")\n");
    for (i = 0; i < 20; i++)
    {
        size1 = (rand() & 0xf) + 100;
        //size1 = (rand() & 0xf) + 1;
        n = (rand() & 0xf) + 1;

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        printf("A = ");
        BI_Show(bi_1, 16);

        Montgomery_Exp_add(&bi_re, bi_1, n);
        BI_Refine(bi_re);

        printf("A * %d == ", n);
        BI_Show(bi_re, 16);

        printf("\n");

        BI_Delete(&bi_1);
        BI_Delete(&bi_re);
    }
    return 0;
}

int montgomery_ladder_Modular_Exponentiation_multiplication_test()
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

    /* n번 Montgomery Ladder multiplacation 연산하기 */
    printf("\n");
    printf("print(\"Montgomery Ladder modular exponentiation multiplacation\")\n");
    for (i = 0; i < 20; i++)
    {
        size1 = (rand() & 0xf) + 50;
        //size1 = (rand() & 0xf) + 1;
        size_M = (rand() & 0xf) + 1;
        n = (rand() & 0xf) + 1;
 
        bi_gen_rand(&bi_1, NON_NEGATIVE, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        bi_gen_rand(&bi_M, NON_NEGATIVE, size_M); // bi_M 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        printf("A = ");
        BI_Show(bi_1, 16);

        printf("M = ");
        BI_Show(bi_M, 16);

        Montgomery_Mod_Exp_mul(&bi_re, bi_1, n, bi_M);
        BI_Refine(bi_re);

        printf("(A ^ %d) %% M == ", n);
        BI_Show(bi_re, 16);

        printf("\n");

        BI_Delete(&bi_1);
        BI_Delete(&bi_re);
    }
    return 0;
}

int montgomery_ladder_Modular_Exponentiation_addtion_test()
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
    printf("print(\"Montgomery Ladder odular exponentiation addition\")\n");
    for (i = 0; i < 20; i++)
    {
        size1 = (rand() & 0xf) + 50;
        //size1 = (rand() & 0xf) + 1;
        size_M = (rand() & 0xf) + 1;
        n = (rand() & 0xf) + 1;

        bi_gen_rand(&bi_1, NON_NEGATIVE, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        bi_gen_rand(&bi_M, NON_NEGATIVE, size_M); // bi_M 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        printf("A = ");
        BI_Show(bi_1, 16);

        printf("M = ");
        BI_Show(bi_M, 16);

        Montgomery_Mod_Exp_add(&bi_re, bi_1, n, bi_M);
        BI_Refine(bi_re);

        printf("(A * %d) %% M == ", n);
        BI_Show(bi_re, 16);

        printf("\n");

        BI_Delete(&bi_1);
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
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Left to Right EXP ADD test\")\n");
	for (i = 0; i < 20; i++)
	{
		size1 = (rand() & 0x7) + 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		EXP_LR_ADD(&bi_re, bi_1, len, bi_2);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("%d * A == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
}

void Left_Right_EXP_MUL_test() // Only Exponentiation function L->R mul
{
	int i = 0;
	int size1 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	printf("\n");
	printf("print(\"Left to Right EXP MUL test\")\n");
	for (i = 0; i < 20; i++)
	{
		

		size1 = (rand() & 0x7) + 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		EXP_LR_MUL(&bi_re, bi_1, len, bi_2);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("A ^ %d == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
}

void Right_Left_EXP_ADD_test() // Only Exponentiation function R<-L add
{
	int i = 0;
	int size1 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;
	printf("\n");
	printf("print(\"Right to Left Exp ADD test\")\n");
	for (i = 0; i < 3; i++)
	{
		

		size1 = (rand() & 0x7) + 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		EXP_RL_ADD(&bi_re, bi_1, len, bi_2);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("%d * A == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_re);
	}
}

void Right_Left_EXP_MUL_test()  // Only Exponentiation function R<-L mul
{
	int i = 0;
	int size1 = 0;
	int len = 0;

	bigint* bi_1 = NULL;
	bigint* bi_2 = NULL;
	bigint* bi_re = NULL;


	printf("\n");
	printf("print(\"Right to Letf Exp MUL test\")\n");
	for (i = 0; i < 3; i++)
	{		
		size1 = (rand() & 0x7);
		len = (rand() & 0x7);

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		EXP_RL_MUL(&bi_re, bi_1, len, bi_2);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("A^%d == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
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
	bigint* bi_re = NULL;
	printf("\n");
	printf("print(\"Left to Right modular exp add test\")\n");
	for (i = 0; i < 3; i++)
	{
		size1 = (rand() & 0x7) + 50;
		size2 = size1 - (rand() & 0x3) - 30;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		bi_gen_rand(&bi_2, 0, size2); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_LR_ADD(&bi_re, bi_1, len, bi_2);
		printf("a = ");
		BI_Show(bi_1, 16);
		printf("b = ");
		BI_Show(bi_2, 16);
		printf("(%d * a) %% b == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
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
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Left to Right modular exp mul test\")\n");
	for (i = 0; i < 3; i++)
	{
		

		size1 = (rand() & 0x7) + 50;
		size2 = size1 - (rand() & 0x3) - 30;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		bi_gen_rand(&bi_2, 0, size2); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_RL_MUL(&bi_re, bi_1, len, bi_2);
		printf("a = ");
		BI_Show(bi_1, 16);
		printf("b = ");
		BI_Show(bi_2, 16);
		printf("(a ^ %d) %% b == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
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
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Left Modular Exp ADD test\")\n");
	for (i = 0; i < 3; i++)
	{
		size1 = (rand() & 0x7) + 50;
		size2 = size1 - (rand() & 0x3) - 30;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		bi_gen_rand(&bi_2, 0, size2); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_RL_ADD(&bi_re, bi_1, len, bi_2);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("(%d * A) %% B == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
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
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Left Modular EXP MUL test\")\n");
	for (i = 0; i < 3; i++)
	{
		size1 = (rand() & 0x7) + 50;
		size2 = size1 - (rand() & 0x3) - 30;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		bi_gen_rand(&bi_2, 0, size2); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_RL_MUL(&bi_re, bi_1, len, bi_2);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("(A ^ %d) %% B == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
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
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Left to Right modular exp add test (DIV func)\")\n");
	for (i = 0; i < 3; i++)
	{
		size1 = (rand() & 0x7) + 50;
		size2 = size1 - (rand() & 0x3) - 30;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		bi_gen_rand(&bi_2, 0, size2); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_LR_ADD_DIV(&bi_re, bi_1, len, bi_2);
		printf("a = ");
		BI_Show(bi_1, 16);
		printf("b = ");
		BI_Show(bi_2, 16);
		printf("(%d * a) %% b == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
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
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"left to right modular exp mul test (DIV func)\")\n");
	for (i = 0; i < 3; i++)
	{

		size1 = (rand() & 0x7) + 50;
		size2 = size1 - (rand() & 0x3) - 1;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		bi_gen_rand(&bi_2, 0, size2); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_RL_MUL_DIV(&bi_re, bi_1, len, bi_2);
		printf("a = ");
		BI_Show(bi_1, 16);
		printf("b = ");
		BI_Show(bi_2, 16);
		printf("(a ^ %d) %% b == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
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
	bigint* bi_re = NULL;
	
	printf("print(\"Right to Left Modular Exp ADD test (DIV func)\")\n");
	for (i = 0; i < 3; i++)
	{
		

		size1 = (rand() & 0x7) + 50;
		size2 = size1 - (rand() & 0x3) - 30;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		bi_gen_rand(&bi_2, 0, size2); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_RL_ADD_DIV(&bi_re, bi_1, len, bi_2);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("(%d * A) %% B == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
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
	bigint* bi_re = NULL;

	printf("\n");
	printf("print(\"Right to Left Modular EXP MUL test (DIV func)\")\n");
	for (i = 0; i < 3; i++)
	{
		size1 = (rand() & 0x7) + 50;
		size2 = size1 - (rand() & 0xf) - 30;
		if (size2 <= 0)
			size2 = 1;
		len = (rand() & 0x7) + 1;

		BI_New(&bi_1, size1); // size1 길이인 big integer인 bi_1 생성
		BI_New(&bi_2, size2);

		bi_gen_rand(&bi_1, 0, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
		bi_gen_rand(&bi_2, 0, size2); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

		MOD_EXP_RL_MUL_DIV(&bi_re, bi_1, len, bi_2);
		printf("A = ");
		BI_Show(bi_1, 16);
		printf("B = ");
		BI_Show(bi_2, 16);
		printf("(A ^ %d) %% B == ", len);
		BI_Show(bi_re, 16);

		BI_Delete(&bi_1);
		BI_Delete(&bi_2);
		BI_Delete(&bi_re);
	}
}

