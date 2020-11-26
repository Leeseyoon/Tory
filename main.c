#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__ )

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bigint.h"

#ifndef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif


int main()
{
    srand((unsigned)time(NULL));
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_crtBreakAlloc = 94;
    int i = 0;
    int size, size1, size2 = 0;
    int len = 0;
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

        //bi_re = Kara(bi_1, bi_2);
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
    

    /* n번 Squaring 곱셈 연산하기 */ // 32, 64 (8일 때 메모리 충돌)
    
    printf("\n");
    printf("print(\"Squaring\")\n");
    for (i = 0; i < 1; i++) //20번 squaring
    {
        size1 = (rand() & 0xf) + 100;

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        printf("A = ");
        BI_Show(bi_1, 16);

        BI_New(&bi_re, size1 * 2 + 1);

        Multi_Squaring(bi_re, bi_1);
        BI_Refine(bi_re);

        printf("A * A == ");
        BI_Show(bi_re, 16);

        printf("\n");

        BI_Delete(&bi_1);
        BI_Delete(&bi_re);
    }
    
    
    
    
    _CrtDumpMemoryLeaks();
    return 0;
}
