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

    size1 = 22;

    // A = 0x 02 c1 47 a1 b4 75 d1 b9 8e f6 22 a6 8b 2c 69 69 fa 8d 4e 31 1b e7
    word arr[22] = { 0xe7, 0x1b, 0x31, 0x4e, 0x8d, 0xfa, 0x69, 0x69, 0x2c, 0x8b, 0xa6,
                        0x22, 0xf6, 0x8e, 0xb9, 0xd1, 0x75, 0xb4, 0xa1, 0x47, 0xc1, 0x02 };

    BI_New(&bi_1, size1);
    BI_Set_By_Array(&bi_1, NON_NEGATIVE, arr, size1);

    printf("A = ");
    BI_Show(bi_1, 16);

    SQUC_Karatsuba(&bi_re, bi_1); // 마지막 ADDC 부분 중 realloc에서 에러
    BI_Refine(bi_re);

    printf("A * A == ");
    BI_Show(bi_re, 16);

    printf("\n");

    BI_Delete(&bi_1);
    BI_Delete(&bi_re);

    return 0;
}