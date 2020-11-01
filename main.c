#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bigint.h"

#define _CRT_SECURE_NO_WARNINGS

int main()
{
    /*
    int i = 0;
    int len;
    int result = 0;

    bigint* a1 = NULL;
    bigint* a2 = NULL;
    bigint* a3 = NULL;
    bigint* a4 = NULL;
    bigint* a5 = NULL;
    bigint* a6 = NULL;
    bigint* a7 = NULL;
    bigint* a8 = NULL;
    bigint* a9 = NULL;

    // 2.1 Create BigInt, Delete BigInt
    printf("\n[Chapter 2.1 ]\n");

    BI_New(&a1, 2);
    BI_New(&a2, 2);
    BI_New(&a3, 2);
    BI_New(&a5, 10);
    BI_New(&a6, 4);
    BI_New(&a7, 3);
    BI_New(&a8, 3);
    BI_New(&a9, 3);

    BI_Delete(&a5);
    BI_New(&a5, 10);

    a1->a[0] = 250;
    a1->a[1] = 2;

    a2->a[0] = 245;
    a2->a[1] = 2;


    printf("Word_Bit_Length : %d\n", WORD_BIT_LEN);

    // Chapter 2.2 Show BigInt Set, BI_Set_By_Array
    printf("\n[Chapter 2.2 ]\n");

    printf("Set Big_Integer : \n");

    word arr[4] = { 0x00, };
    arr[0] = 0x12;
    arr[1] = 0x34;
    arr[2] = 0x56;
    arr[3] = 0x78;

    BI_Set_By_Array(&a6, NON_NEGATIVE, arr, 4);
    printf("a6's sign = %d\n", (a6->sign));
    printf("a6's wordlen = %d\n", (a6->wordlen));
    for (i = 0; i < a6->wordlen; i++)
        printf("a6[%d] = %02x\n", i, (a6->a[i]));

    // 2진수로 변환
    word base = 2;
    word s1[30] = { 0x00, };

    printf("\n문자열을 입력하세요: ");
    scanf("%s", (char*)s1);
    fflush(stdin);

    char* str = (char*)s1;
    BI_Set_By_String(&a7, 0, str, base, sizeof(s1));

    printf("a7's sign = %d\n", (a7->sign));
    printf("a7's wordlen = %d\n", (a7->wordlen));
    for (i = 0; i < a8->wordlen; i++)
        printf("a7[%d] = %02x\n", i, (a7->a[i]));

    // 16진수로 변환
    base = 16;
    word s2[30] = { 0x00, };

    printf("\n문자열을 입력하세요: ");
    scanf("%s", (char*)s2);
    fflush(stdin);

    char* str2 = (char*)s2;
    BI_Set_By_String(&a5, 0, str2, base, sizeof(s2));

    printf("a5's sign = %d\n", (a5->sign));
    printf("a5's wordlen = %d\n", (a5->wordlen));
    for (i = 0; i < a5->wordlen; i++)
        printf("a5[%d] = %02x\n", i, (a5->a[i]));

    printf("\nShow Big_Integer : \n");
    BI_Show(a1, 16); //a1을 16진수로 출력하도록 -> 0x02fa
    BI_Show(a2, 16); //a2을 16진수로 출력하도록 -> 0x02f5

    // Chapter 2.3 Refine BigInt
    printf("\n[Chapter 2.3 Refine Big_Int]\n");

    a6->a[2] = 0x00;
    a6->a[3] = 0x00;

    printf("a6's sign = %d\n", (a6->sign));
    printf("a6's wordlen = %d\n", (a6->wordlen));
    for (i = 0; i < a6->wordlen; i++)
        printf("a6[%d] = %02x\n", i, (a6->a[i])); // 최상위 2개의 워드열 0으로 설정한 것 출력

    bi_refine(a6);

    printf("\na6's sign = %d\n", (a6->sign));
    printf("a6's wordlen = %d\n", (a6->wordlen));
    for (i = 0; i < a6->wordlen; i++)
        printf("a6[%d] = %02x\n", i, (a6->a[i])); // re define한 결과

    // Chpater 2.4 Assign BigInt
    printf("\n[Chapter 2.4 Assign Big_Int]\n");
    Assign_BI(&a3, a1);
    BI_Show(a3, 16); // a3에 a1이 잘 assign 되었는지 확인하기 위해 BI_Show 함수 호출

    // Chapter 2.5 Generate Random
    printf("\n[Chapter 2.5]\n");

    srand(time(NULL));
    bigint* a_GR = NULL;
    bi_gen_rand(&a_GR, 0, 3);

    printf("a_GR's sign = %d\n", (a_GR->sign));
    printf("a_GR's wordlen = %d\n", (a_GR->wordlen));
    for (i = 0; i < a1->wordlen; i++)
        printf("a_GR[%d] = %02x\n", i, (a_GR->a[i]));

    BI_Delete(&a_GR);
    bi_gen_rand(&a_GR, 0, 3);

    printf("Retry: \n");
    printf("a_GR's sign = %d\n", (a_GR->sign));
    printf("a_GR's wordlen = %d\n", (a_GR->wordlen));
    for (i = 0; i < a1->wordlen; i++)
        printf("a_GR[%d] = %02x\n", i, (a_GR->a[i]));

    // Chapter 2.6 Get Word Length/Bit Length/j-th Bit of BigInt
    printf("\n[Chapter 2.6]\n");
    Get_Word_Length(&len, a1);
    printf("Word length of ( ): %d\n", len); // 출력시 구조체의 이름을 어떻게 출력할 지 몰라서 ()로 쳤습니다..
    Bit_Length(&len, a1);
    printf("Bit length of ( ): %d\n", len); // 출력시 구조체의 이름을 어떻게 출력할 지 몰라서 ()로 쳤습니다..
    j_th_Bit_of_BI(2, a1);

    // Chpater 2.7 Get Sign & Flip Sign
    printf("\n[Chapter 2.7]\n");

    int Sign;
    Sign = Get_Sign(a1);
    printf("get sign = %d\n", Sign);

    Flip_Sign(a1);
    printf("a1's flip sign = %d\n", (a1->sign));
    printf("a1's wordlen = %d\n", (a1->wordlen));

    // Chapter 2.8 Set One, Set Zero, Is Zero?, Is One?
    printf("\n[Chapter 2.8 ]\n");
    BI_Set_One(&a1);
    printf("After Set 1 : ");
    BI_Show(a1, 16); // a1이 1으로 설정되었는지 확인하기 위해 BI_Show() 호출
    BI_Set_Zero(&a1);
    printf("After Set 0 : "); // a1이 0으로 설정되었는지 확인하기 위해 BI_Show() 호출
    BI_Show(a1, 16);
    printf("\n");

    int oneorzero;
    BI_Set_One(&a1);
    for (i = 0; i < a1->wordlen; i++)
        printf("a1[%d] = %d\n", i, (a1->a[i]));

    oneorzero = Is_One(&a1);
    printf("Is one? (True: 0, False: -1): %d\n", oneorzero);
    oneorzero = Is_Zero(&a1);
    printf("Is Zero? (True: 0, False: -1): %d\n\n", oneorzero);

    BI_Set_Zero(&a1);
    for (i = 0; i < a1->wordlen; i++)
        printf("a1[%d] = %d\n", i, (a1->a[i]));
    oneorzero = Is_One(&a1);
    printf("Is one? (True: 0, False: -1): %d\n", oneorzero);
    oneorzero = Is_Zero(&a1);
    printf("Is Zero? (True: 0, False: -1): %d\n", oneorzero);

    // Chapter 2.9 Compare
    printf("\n[Chapter 2.9 Compare ]\n");
    result = Compare_BI(&a1, &a2);
    printf("Result of Compare : %d\n", result);

    // Chapter 2.10 Left/Right Shift
    printf("\n[Chapter 2.10 Left/Right Shift ]\n");
    BI_Show(a2, 16);
    Right_Shift(a2, 7);
    printf("Right Shift->\n");
    BI_Show(a2, 16);

    printf("\n");

    word arr1[3] = { 0x00, };
    arr1[0] = 0xF2;
    arr1[1] = 0x0A;
    arr1[2] = 0xDF;

    BI_Set_By_Array(&a9, NON_NEGATIVE, arr1, 3);

    printf("a9's sign = %d\n", (a9->sign));
    printf("a9's wordlen = %d\n", (a9->wordlen));
    for (i = 0; i < a9->wordlen; i++)
        printf("a9[%d] = %02x\n", i, (a9->a[i]));
    printf("Left Shift->\n");

    Left_Shift(a9, 9);
    printf("a9's sign = %d\n", (a9->sign));
    printf("a9's wordlen = %d\n", (a9->wordlen));
    for (i = 0; i < a9->wordlen; i++)
        printf("a9[%d] = %02x\n", i, (a9->a[i]));


    // Chapter 2.11 Reduction
    printf("\n[Chapter 2.11 Reduction ]\n");
    a2->a[0] = 245;
    a2->a[1] = 2;
    Reduction_BI(&a2, 32);
    BI_Show(a2, 16); // Big Integer가 Reduction이 잘 되었는지 확인
    */
    
    int i;
    bigint* a1 = NULL;
    bigint* a2 = NULL;
    bigint* a3 = NULL;

    BI_New(&a1, 5);
    BI_New(&a2, 5);
    BI_New(&a3, 6);

    word arr1[5] = { 0x00, };
    arr1[0] = 0x1111111111111112;
    arr1[1] = 0x2222222211111134;
    arr1[2] = 0x2222222233333311;
    arr1[3] = 0x11111111eeeeeeef;
    arr1[4] = 0xffffffffffffff1e;

    BI_Set_By_Array(&a1, NON_NEGATIVE, arr1, 5);

    word arr2[5] = { 0x00, };
    arr2[0] = 0xf1f1f1f111111156;
    arr2[1] = 0x2222222211111178;
    arr2[2] = 0x22222222111111ff;
    arr2[3] = 0x22222222eeeefffe;
    arr2[4] = 0xffffffffffffffdf;

    BI_Set_By_Array(&a2, NON_NEGATIVE, arr2, 5);

    a1->sign = NON_NEGATIVE;
    a2->sign = NON_NEGATIVE;

    //int c = 0;
    //c = ADD_ABc(&a1, &a2, &a3, c, 0);
    //a3 = ADDC(&a1, &a2, a1->sign);

    printf("a3's sign = %d\n", (a3->sign));
    printf("a3's wordlen = %d\n", (a3->wordlen));
    for (i = 0; i < a3->wordlen; i++)
        printf("a3[%d] = %02x\n", i, (a3->a[i]));

    a3 = ADD(&a1, &a2);

    printf("a1's sign = %d\n", (a1->sign));
    printf("a1's wordlen = %d\n", (a1->wordlen));
    for (i = 0; i < a1->wordlen; i++)
        printf("a1[%d] = %llx\n", i, (a1->a[i]));
    
    printf("a2's sign = %d\n", (a2->sign));
    printf("a2's wordlen = %d\n", (a2->wordlen));
    for (i = 0; i < a2->wordlen; i++)
        printf("a2[%d] = %llx\n", i, (a2->a[i]));

    printf("a3's sign = %d\n", (a3->sign));
    printf("a3's wordlen = %d\n", (a3->wordlen));
    for (i = 0; i < a3->wordlen; i++)
        printf("a3[%d] = %llx\n", i, (a3->a[i]));

    return 0;
}