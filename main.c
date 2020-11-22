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
    
    /*
    BI_New(&bi_1, 2);
    BI_New(&bi_2, 3);

    len = (MAX(bi_1->wordlen, bi_2->wordlen) + 1) >> 1;
    len = 4 * len + 1;
    BI_New(&bi_re, len);

    word arr1_a[2] = { 0xb257d8c8, 0xff7e8c00 };
    word arr1_b[3] = { 0xa38b4600, 0xbc301ea8, 0xb30ba8e8 };

    //A = -0x ff7e8c00 b257d8c8
    //B = 0x b30ba8e8 bc301ea8 a38b4600
    //A * B = -0x b2b11ee7 d31b6129 543e30fd eac4a8be 11deb000

   // word arr1_a[7] = { 0x4165dc75, 0x4b41fa33, 0xc9fd207d, 0x65e6d02e,
   //                    0x6c672b95, 0x82d0061b, 0x95573f26 };
   //word arr1_b[7] = { 0x6c0f5d51, 0xea50f227, 0xf95b9a2e, 0x0ecf51ae,
   //                    0x321ace5a, 0xa8e90526, 0xd0e2ef58 };

    //Ʋ���� ��
    //A = 0x95573f26 82d0061b 6c672b95 65e6d02e c9fd207d 4b41fa33 4165dc75
    //B = -0xd0e2ef58 a8e90526 321ace5a 0ecf51ae f95b9a2e ea50f227 6c0f5d51

    BI_Set_By_Array(&bi_1, NEGATIVE, arr1_a, 2);
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
