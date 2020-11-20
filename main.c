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
    int i, j, len;
    int size, size1, size2 = 0;
    int sign_bi_1, sign_bi_2;

    bigint* bi_1 = NULL;
    bigint* bi_2 = NULL;
    bigint* bi_re = NULL;

    /*
    printf("print(\"Addition\")\n");
    //n번 덧셈 연산하기
    for (i = 0; i < 20; i++) // n에 20 대입
    {
        size1 = (rand() & 7) + 1; // size1 : 1~8
        size2 = (rand() & 7) + 1; // size2 : 1~8

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        size = MAX(bi_1->wordlen, bi_2->wordlen);
        BI_New(&bi_re, size + 1); // 더 긴 길이인 size로 덧셈 연산 결과인 big integer 생성

        ADD(&bi_re, &bi_1, &bi_2); // ADD함수에 덧셈 결과인 bi_re , 피연산1인 bi_1, 피연산자2인 bi_2

        // 출력 부분
        printf("A = ");
        BI_Show(bi_1, 16);
        printf("B = ");
        BI_Show(bi_2, 16);
        printf("A + B == ");
        BI_Show(bi_re, 16);
        printf("\n");
        // 메모리 free
        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    }

    
    printf("print(\"Subtraction\")\n");
    //n번 뺄셈 연산하기
    for (i = 0; i < 20; i++) // n에 20 대입
    {
        size1 = (rand() & 7) + 1; // size1 : 1~8
        size2 = (rand() & 7) + 1; // size2 : 1~8

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        size = (size1 > size2) ? size1 : size2; // size1 과 size2 비교해서 size 변수에 대입
        BI_New(&bi_re, size + 1); //size + 1?

        SUB(&bi_re, bi_1, bi_2);

        printf("A = ");
        BI_Show(bi_1, 16);
        printf("B = ");
        BI_Show(bi_2, 16);
        printf("A - B == ");
        BI_Show(bi_re, 16);
        printf("\n");
        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    }
    
    
    printf("print(\"Schoolbook Multiplication\")\n");
    for (i = 0; i < 20; i++) //n번 곱셈 연산하기
    {
        size1 = (rand() & 7) + 1; // size1 : 1~8
        size2 = (rand() & 7) + 1; // size2 : 1~8

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 의 부호를 랜덤하게 만들어서 랜덤한 배열을 담은 bigint 생성

        BI_New(&bi_re, size1 + size2);

        MUL_MUL(&bi_re, bi_1, bi_2);

        printf("A = ");
        BI_Show(bi_1, 16);
        printf("B = ");
        BI_Show(bi_2, 16);
        printf("A * B == ");
        BI_Show(bi_re, 16);
        printf("\n");
        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    } */
    
    printf("print(\"Karatsuba Multiplication\")\n");
 /*
    for (i = 0; i < 20; i++) //20번 카라추바 곱셈 연산하기
    {
        size1 = (rand() & 7) + 1;
        size2 = (rand() & 7) + 1;
        printf("i = %d\n", i);
        printf("size1 = %d, size2 = %d\n", size1, size2);

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
        bi_refine(bi_re);

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
    } */
    
    BI_New(&bi_1, 5);
    BI_New(&bi_2, 3);

    len = (MAX(bi_1->wordlen, bi_2->wordlen) + 1) >> 1;
    len = 4 * len + 1;
    BI_New(&bi_re, len);

    //word arr1_a[2] = { 0xb257d8c8, 0xff7e8c00 };
    //word arr1_b[3] = { 0xa38b4600, 0xbc301ea8, 0xb30ba8e8 };

    //A = -0x ff7e8c00 b257d8c8
    //B = 0x b30ba8e8 bc301ea8 a38b4600
    //A * B = -0x b2b11ee7 d31b6129 543e30fd eac4a8be 11deb000

    word arr1_a[5] = { 0xd8244f96, 0xbb7e7398, 0xc99e2a5a, 0xaaa72f24, 0xecc48dcd };
    word arr1_b[3] = { 0xb8530e57, 0x9970b09a, 0x5b754890 };

    // 틀리는 거
    //size1 = 5, size2 = 3
    //A = -0x ecc48dcd aaa72f24 c99e2a5a bb7e7398 d8244f96
    //B = 0x 5b754890 9970b09a b8530e57
    //A * B == -0x900f2efb9f182f272602c9313c7f520d12fddd3a5b18c4473f32de8a6c62d1f90e533ffa

    //size1 = 7, size2 = 2
    //A = 0xd91789bf3b7afd4479ac5b869155a800beb0b4e7aa2bece6d640601d
    //B = -0x73b12c550aab183f
    //A * B == -0x7a7f111a2447fdfb43e4c3f934517806b511b4c25de0b70699b2f9f467f1674af086774718395f23

    BI_Set_By_Array(&bi_1, NEGATIVE, arr1_a, 5);
    BI_Set_By_Array(&bi_2, NON_NEGATIVE, arr1_b, 3);

    printf("A = ");
    BI_Show(bi_1, 16);
    printf("B = ");
    BI_Show(bi_2, 16);

    sign_bi_1 = Get_Sign(bi_1);
    sign_bi_2 = Get_Sign(bi_2);

    if (sign_bi_1 == NEGATIVE)
        Flip_Sign(bi_1);
    if (sign_bi_2 == NEGATIVE)
        Flip_Sign(bi_2);

    Karatsuba(&bi_re, bi_1, bi_2);
    bi_refine(bi_re);

    if ((sign_bi_1 ^ sign_bi_2) == 1)
        bi_re->sign = NEGATIVE;
    else
        bi_re->sign = NON_NEGATIVE;

    printf("A * B == ");
    BI_Show(bi_re, 16);

    BI_Delete(&bi_1);
    BI_Delete(&bi_2);
    BI_Delete(&bi_re);
    
  
    _CrtDumpMemoryLeaks();
    return 0;
}
