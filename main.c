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
    bigint* bi_q = NULL;

    printf("print(\"Binary Long Division\")\n");

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

    /*
    printf("print(\"Addition\")\n");
    //n�� ���� �����ϱ�
    for (i = 0; i < 20; i++) // n�� 20 ����
    {
        size1 = (rand() & 0xf) + 1; // size1 : 1~16
        size2 = (rand() & 0xf) + 1; // size2 : 1~16

        bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����
        bi_gen_rand(&bi_2, rand() & 1, size2); // bi_2 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

        size = MAX(bi_1->wordlen, bi_2->wordlen);
        //BI_New(&bi_re, size + 1); // �� �� ������ size�� ���� ���� ����� big integer ����

        // ��� �κ�
        printf("A = ");
        BI_Show(bi_1, 16);
        printf("B = ");
        BI_Show(bi_2, 16);

        ADD(&bi_re, &bi_1, &bi_2); // ADD�Լ��� ���� ����� bi_re , �ǿ���1�� bi_1, �ǿ�����2�� bi_2

        printf("A + B == ");
        BI_Show(bi_re, 16);
        BI_Show(bi_1, 16);
        printf("\n");
        // �޸� free
        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        //BI_Delete(&bi_re);
    }
    */
    /*
    printf("print(\"Subtraction\")\n");
    //n�� ���� �����ϱ�
    for (i = 0; i < 20; i++) // n�� 20 ����
    {
        size1 = (rand() & 0xf) + 1; // size1 : 1~16
        size2 = (rand() & 0xf) + 1; // size2 : 1~16

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


    printf("print(\"Schoolbook Multiplication\")\n");
    for (i = 0; i < 20; i++) //n�� ���� �����ϱ�
    {
        size1 = (rand() & 0xf) + 1; // size1 : 1~16
        size2 = (rand() & 0xf) + 1; // size2 : 1~16

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
    */
    // ************************ �޸� �� ���� ���� �� ���� *********************

    /*
    printf("print(\"Karatsuba Multiplication\")\n");

    for (i = 0; i < 20; i++) //20�� ī���߹� ���� �����ϱ�
    {
        size1 = (rand() & 0xf) + 1;
        size2 = (rand() & 0xf) + 1;

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
    }
    */
    printf("print(\"Squaring\")\n");
    /*
    for (i = 0; i < 1; i++) //20�� ī���߹� ���� �����ϱ�
    {
        //size1 = (rand() & 0xf) + 1;
        //size1 = 1;
        size1 = 3;
        //bi_gen_rand(&bi_1, rand() & 1, size1); // bi_1 �� ��ȣ�� �����ϰ� ���� ������ �迭�� ���� bigint ����

        BI_New(&bi_1, size1);
        //bi_1->a[0] = 0x74b3979d;
        bi_1->a[0] = 0x12123434;
        bi_1->a[1] = 0x33344556;
        bi_1->a[2] = 0x12345678;
        // 0x 14b 66dc 253d 21d3 6f56 99de / 3a35 fb9f e2c5 ab92 b1f5 2a90

        printf("A = ");
        BI_Show(bi_1, 16);

        BI_New(&bi_re, size1 * 2 + 1);

        //Single_Squaring(bi_1, bi_re, 0);
        Multi_Squaring(bi_re, bi_1);
        bi_refine(bi_re);

        printf("A * A == ");
        BI_Show(bi_re, 16);

        printf("\n");

        BI_Delete(&bi_1);
        BI_Delete(&bi_re);
    }
    */
    _CrtDumpMemoryLeaks();
    return 0;
}