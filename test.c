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

    /* n�� ���� �����ϱ� */ // 8, 32, 64

    printf("\n\n");
    printf("print(\"Addition\")\n");

    for (i = 0; i < 20; i++) // n�� 20 ����
    {
        size1 = (rand() & 0x3) + 1; // size1 : 1~16
        size2 = (rand() & 0x3) + 1; // size2 : 1~16

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

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
    int len = 0;
    int n;
    int sign_bi_1 = 0;
    int sign_bi_2 = 0;

    bigint* bi_1 = NULL;
    bigint* bi_2 = NULL;
    bigint* bi_re = NULL;
    bigint* bi_q = NULL;

    printf("print(\"WORD_BIT_LEN == %d\")\n", WORD_BIT_LEN);

    /* n�� ���� �����ϱ� */ // 8, 32, 64

    printf("\n");
    printf("print(\"Subtraction\")\n");

    for (i = 0; i < 20; i++) // n�� 20 ����
    {
        size1 = (rand() & 0x7) + 1; // size1 : 1~16
        size2 = (rand() & 0x7) + 1; // size2 : 1~16

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

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

    /* n�� Schoolbook ���� �����ϱ� */ // 8, 32, 64

    printf("\n\n");
    printf("print(\"Schoolbook Multiplication\")\n");
    for (i = 0; i < 20; i++) //n�� ���� �����ϱ�
    {
        size1 = (rand() & 0x7) + 1; // size1 : 1~16
        size2 = (rand() & 0x7) + 1; // size2 : 1~16

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

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

    /* n�� Karatsuba ���� �����ϱ� */ // 8, 32, 64

    printf("\n\n");
    printf("print(\"Karatsuba Multiplication\")\n");
    for (i = 0; i < 1; i++) //20�� ī���߹� ���� �����ϱ�
    {
        size1 = (rand() & 0x3) + 100;
        size2 = (rand() & 0x3) + 100;

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

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

    /* n�� Binary Long Division �����ϱ� */ // 8, 32, 64

    printf("\n");
    printf("print(\"Binary Long Division\")\n"); // �޸� �� ����
    for (i = 0; i < 20; i++)
    {
        size2 = (rand() & 3) + 1; // size2 : 1~4
        size1 = size2 + (rand() & 3) + 1; // size1�� size2 ���� ���� �� ���


        BI_New(&bi_1, size1); // size1 ������ big integer�� bi_1 ����
        BI_New(&bi_2, size2); // size2 ������ big integer�� bi_2 ����

        bi_gen_rand(&bi_1, 0, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
        bi_gen_rand(&bi_2, 0, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

        size = Compare_BI(&bi_1, &bi_2); // size1 �� size2 ��
        if (size == -1)
        {
            printf("print(\"B is larger than A\")\n");
            BI_Delete(&bi_1);
            BI_Delete(&bi_2);
        }
        else
        {
            Binary_Long_Div(&bi_q, &bi_re, bi_1, bi_2);
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

    /* n�� Multi Long Division �����ϱ� */ // 32, 64 (8�̰� size�� Ŭ ��� �� �� False)

    printf("\n");
    printf("print(\"Multi Long Division\")\n");
    for (i = 0; i < 20; i++)
    {
        size2 = (rand() & 0xf) + 20; // size2 : 1~4
        size1 = size2 + (rand() & 0xf);//size2 + (rand() & 3) + 1; // size1�� size2 ���� ���� �� ���

        BI_New(&bi_1, size1); // size1 ������ big integer�� bi_1 ����
        BI_New(&bi_2, size2); // size2 ������ big integer�� bi_2 ����


        bi_gen_rand(&bi_1, 0, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
        bi_gen_rand(&bi_2, 0, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

        size = Compare_BI(&bi_1, &bi_2); // size1 �� size2 ��
        if (size == -1)
        {
            printf("print(\"B is larger than A\")\n");
            BI_Delete(&bi_1);
            BI_Delete(&bi_2);
        }
        else
        {
            DIV(&bi_q, &bi_re, bi_1, bi_2);
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

    /* n�� Squaring ���� �����ϱ� */ // 32, 64 (8�� �� �޸� �浹)

    printf("\n");
    printf("print(\"Squaring\")\n");
    for (i = 0; i < 20; i++) //20�� squaring
    {
        size1 = (rand() & 0xf) + 100;

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

        printf("A = ");
        BI_Show(bi_1, 16);

        BI_New(&bi_re, size1 * 2 + 1);

        SQUC(bi_re, bi_1);
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

    /* n�� Squaring Karatsuba ���� �����ϱ� */

    printf("\n");
    printf("print(\"Squaring Karatsuba\")\n");
    for (i = 0; i < 1; i++) // ���� 21�� ������ �� ��
    {
        //size1 = (rand() & 0xf) + 100;
        size1 = 22;

        //bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
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

        SQUC_Karatsuba(bi_re, bi_1);
        BI_Refine(bi_re);

        printf("A * A == ");
        BI_Show(bi_re, 16);

        printf("\n");

        BI_Delete(&bi_1);
        BI_Delete(&bi_re);
    }
    return 0;
}

int montgomery_ladder_multiplication_test()
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

    /* n�� Montgomery Ladder multiplacation �����ϱ� */
    printf("\n");
    printf("print(\"Montgomery Ladder multiplacation\")\n");
    for (i = 0; i < 20; i++)
    {
        //size1 = (rand() & 0xf) + 50;
        size1 = (rand() & 0xf) + 1;
        n = (rand() & 0xf) + 1;

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

        printf("A = ");
        BI_Show(bi_1, 16);

        Montgomery_Ladder_mul(&bi_re, bi_1, n);
        BI_Refine(bi_re);

        printf("A ^ %d == ", n);
        BI_Show(bi_re, 16);

        printf("\n");

        BI_Delete(&bi_1);
        BI_Delete(&bi_re);
    }
    return 0;
}

int montgomery_ladder_addtion_test()
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

    /* n�� Montgomery Ladder addition �����ϱ� */
    printf("\n");
    printf("print(\"Montgomery Ladder addition\")\n");
    for (i = 0; i < 20; i++)
    {
        size1 = (rand() & 0xf) + 100;
        //size1 = (rand() & 0xf) + 1;
        n = (rand() & 0xf) + 1;

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

        printf("A = ");
        BI_Show(bi_1, 16);

        Montgomery_Ladder_add(&bi_re, bi_1, n);
        BI_Refine(bi_re);

        printf("A * %d == ", n);
        BI_Show(bi_re, 16);

        printf("\n");

        BI_Delete(&bi_1);
        BI_Delete(&bi_re);
    }
    return 0;
}