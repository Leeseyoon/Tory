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
    //_crtBreakAlloc = 74;
    int i, j = 0;
    int size, size1, size2 = 0;
    bigint* bi_1 = NULL;
    bigint* bi_2 = NULL;
    bigint* bi_re = NULL;

    BI_New(&bi_1, 2);
    BI_New(&bi_2, 3);
    BI_New(&bi_re, 5);

    word arr1_a[2] = { 0xb257d8c8, 0xff7e8c00 };
    word arr1_b[3] = { 0xa38b4600, 0xbc301ea8, 0xb30ba8e8 };

    //A = -0x ff7e8c00 b257d8c8
    //B = 0x b30ba8e8 bc301ea8 a38b4600
    //A * B = -0x b2b11ee7 d31b6129 543e30fd eac4a8be 11deb000
    //�Ҵ��� �� �ε������� �� ����ؼ� heap corruption ������ ���� ��...

    //A = -0x594f8a00aaacfce0
    //B = 0x7e0f118bdc660e5c1e63908c

    BI_Set_By_Array(&bi_1, NEGATIVE, arr1_a, 2);
    BI_Set_By_Array(&bi_2, NON_NEGATIVE, arr1_b, 3);

    printf("A = ");
    BI_Show(bi_1, 16);
    printf("B = ");
    BI_Show(bi_2, 16);

    Karatsuba(&bi_re, bi_1, bi_2);

    printf("A * B == ");
    BI_Show(bi_re, 16);

    BI_Delete(&bi_1);
    BI_Delete(&bi_2);
    BI_Delete(&bi_re);

    /*
    printf("print(\"Addition\")\n");

    //n�� ���� �����ϱ�
    for (i = 0; i < 20; i++) // n�� 20 ����
    {
        size1 = (rand() & 7) + 1; // size1 : 1~8
        size2 = (rand() & 7) + 1; // size2 : 1~8

        BI_New(&bi_1, size1); // size1 ������ big integer�� bi_1 ����
        BI_New(&bi_2, size2); // size2 ������ big integer�� bi_2 ����
        size = Compare_WordLen(bi_1, bi_2); // size1 �� size2 �� // Compare_WordLen ���� MAX �Լ� ���°� �� ���� �ʳ�??(����)
        BI_New(&bi_re, size + 1); // �� �� ������ size�� ���� ���� ����� big integer ����
        bi_1->sign = rand() & 1; // bi_1 �� ��ȣ�� �����ϰ�
        bi_2->sign = rand() & 1; // bi_2 �� ��ȣ�� �����ϰ�

        for (j = 0; j < size1; j++)
            bi_1->a[j] = ((rand() * rand() * rand() * rand()) & word_mask); //bi_1�� ���� �����ϰ� ���� //WORD_BIT_LEN�� ���� rand() ������ �� �ٲٱ�(����)
        for (j = 0; j < size2; j++)
            bi_2->a[j] = ((rand() * rand() * rand() * rand()) & word_mask); //bi_2�� ���� �����ϰ� ���� //WORD_BIT_LEN�� ���� rand() ������ �� �ٲٱ�(����)


        ADD(&bi_re, &bi_1, &bi_2); // ADD�Լ��� ���� ����� bi_re , �ǿ���1�� bi_1, �ǿ�����2�� bi_2

        // ��� �κ�
        printf("A = ");
        BI_Show(bi_1, 16);
        printf("B = ");
        BI_Show(bi_2, 16);
        printf("A + B == ");
        BI_Show(bi_re, 16);
        printf("\n");
        // �޸� free
        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    }

    printf("print(\"Subtraction\")\n");
    //n�� ���� �����ϱ�
    for (i = 0; i < 20; i++) // n�� 20 ����
    {
        size1 = (rand() & 7) + 1; // size1 : 1~8
        size2 = (rand() & 7) + 1; // size2 : 1~8

        size = (size1 > size2) ? size1 : size2; // size1 �� size2 ���ؼ� size ������ ����
        BI_New(&bi_1, size1); // size1 ������ big integer�� bi_1 ����
        BI_New(&bi_2, size2); // size2 ������ big integer�� bi_2 ����
        BI_New(&bi_re, size + 1); //size + 1?

        bi_1->sign = rand() & 1;
        bi_2->sign = rand() & 1;


        for (j = 0; j < size1; j++)
            bi_1->a[j] = ((rand() * rand() * rand() * rand()) & word_mask);
        for (j = 0; j < size2; j++)
            bi_2->a[j] = ((rand() * rand() * rand() * rand()) & word_mask);


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
    for (i = 0; i < 20; i++) //n�� ���� �����ϱ�
    {
        size1 = (rand() & 7) + 1;
        size2 = (rand() & 7) + 1;

        BI_New(&bi_1, size1);
        BI_New(&bi_2, size2);
        BI_New(&bi_re, size1 + size2);
        bi_1->sign = rand() & 1;
        bi_2->sign = rand() & 1;

        for (j = 0; j < size1; j++)
            bi_1->a[j] = ((rand() * rand() * rand() * rand()) & word_mask); //rand() & (char)0xff; //
        for (j = 0; j < size2; j++)
            bi_2->a[j] = ((rand() * rand() * rand() * rand()) & word_mask); //rand() & (char)0xff; //

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
    }
    
    printf("print(\"Karatsuba Multiplication\")\n");
    for (i = 0; i < 20; i++) //20�� ī���߹� ���� �����ϱ�
    {
        //size1 = (rand() & 7) + 1;
        //size2 = (rand() & 7) + 1;

        size1 = 2;
        size2 = 3;

        printf("i = %d\n", i);
        BI_New(&bi_1, size1);
        BI_New(&bi_2, size2);
        BI_New(&bi_re, size1 + size2);

        bi_1->sign = rand() & 1;
        bi_2->sign = rand() & 1;

        for (j = 0; j < size1; j++)
            bi_1->a[j] = ((rand() * rand() * rand() * rand()) & 0xffffffff); //rand() & (char)0xff; //
        for (j = 0; j < size2; j++)
            bi_2->a[j] = ((rand() * rand() * rand() * rand()) & 0xffffffff); //rand() & (char)0xff; //

        printf("A = ");
        BI_Show(bi_1, 16);
        printf("B = ");
        BI_Show(bi_2, 16);

        Karatsuba(&bi_re, bi_1, bi_2);

        printf("A * B == ");
        BI_Show(bi_re, 16);

        printf("\n");

        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    }
    */

    _CrtDumpMemoryLeaks();
    return 0;
}
