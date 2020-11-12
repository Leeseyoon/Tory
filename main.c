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
    
    int i = 0;
    int j = 0;
    int len = 0;
    int Len_1 = 0;
    int Len_2 = 0;
    int size, size1, size2 = 0;

    bigint* bi_1 = NULL;
    bigint* bi_2 = NULL;
    bigint* bi_re = NULL;


    /*
    for (i = 0; i < 20; i++) //20¹ø µ¡¼À ¿¬»êÇÏ±â
    {
        //printf("i = %d\n", i);
        BI_New(&bi_1, 2);
        BI_New(&bi_2, 3);
        len = Compare_WordLen(bi_1, bi_2);
        BI_New(&bi_re, len + 1);

        bi_1->sign = rand() & 1;
        bi_2->sign = rand() & 1;

        bi_1->a[0] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_1->a[1] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_2->a[0] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_2->a[1] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_2->a[2] = ((rand() * rand() * rand() * rand()) & 0xffffffff);

        ADD_BI_test(&bi_re, &bi_1, &bi_2);
        printf("\n");

        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    } */

    printf("print(\"Addition\")\n");

    //n¹ø µ¡¼À ¿¬»êÇÏ±â
    for (i = 0; i < 20; i++)
    {
        size1 = (rand() & 7) + 1;
        size2 = (rand() & 7) + 1;

        BI_New(&bi_1, size1);
        BI_New(&bi_2, size2);
        size = Compare_WordLen(bi_1, bi_2);
        BI_New(&bi_re, size + 1);
        bi_1->sign = rand() & 1;
        bi_2->sign = rand() & 1;

        for (j = 0; j < size1; j++)
            bi_1->a[j] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        for (j = 0; j < size2; j++)
            bi_2->a[j] = ((rand() * rand() * rand() * rand()) & 0xffffffff);


        ADD_BI_test(&bi_re, &bi_1, &bi_2);
        printf("\n");
        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    }

    printf("print(\"Subtraction\")\n");
    //n¹ø »¬¼À ¿¬»êÇÏ±â
    for (i = 0; i < 20; i++)
    {
        size1 = (rand() & 7) + 1;
        size2 = (rand() & 7) + 1;

        size = (size1 > size2) ? size1 : size2;
        BI_New(&bi_1, size1);
        BI_New(&bi_2, size2);
        BI_New(&bi_re, size + 1);

        bi_1->sign = rand() & 1;
        bi_2->sign = rand() & 1;

        for (j = 0; j < size1; j++)
            bi_1->a[j] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        for (j = 0; j < size2; j++)
            bi_2->a[j] = ((rand() * rand() * rand() * rand()) & 0xffffffff);


        SUB_BI_test(&bi_re, bi_1, bi_2);
        printf("\n");
        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    }
    /*
    printf("print(\"Schoolbook Multiplication\")\n");
    for (i = 0; i < 20; i++) //n¹ø °ö¼À ¿¬»êÇÏ±â
    {
        size1 = 2;// (rand() & 7) + 1;
        size2 = 2;// (rand() & 7) + 1;

        BI_New(&bi_1, size1);
        BI_New(&bi_2, size2);
        BI_New(&bi_re, size1 + size2);
        bi_1->sign = rand() & 1;
        bi_2->sign = rand() & 1;

        for (j = 0; j < size1; j++)
            bi_1->a[j] = ((rand() * rand() * rand() * rand()) & 0xffffffff); //rand() & (char)0xff; //
        for (j = 0; j < size2; j++)
            bi_2->a[j] = ((rand() * rand() * rand() * rand()) & 0xffffffff); //rand() & (char)0xff; //

        MUL_MUL(&bi_re, bi_1, bi_2);
        printf("\n");
        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    }
    
    printf("print(\"Karatsuba Multiplication\")\n");
    for (i = 0; i < 20; i++) //20¹ø Ä«¶óÃß¹Ù °ö¼À ¿¬»êÇÏ±â
    {
        printf("i = %d\n", i);
        BI_New(&bi_1, 5);
        BI_New(&bi_2, 4);
        
        Get_Word_Length(&Len_1, &bi_1);
        Get_Word_Length(&Len_2, &bi_2);
        BI_New(&bi_re, Len_1 + Len_2);

        bi_1->sign = rand() & 1;
        bi_2->sign = rand() & 1;

        bi_1->a[0] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_1->a[1] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_1->a[2] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_1->a[3] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_1->a[4] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_2->a[0] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_2->a[1] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_2->a[2] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_2->a[3] = ((rand() * rand() * rand() * rand()) & 0xffffffff);

        Karatsuba(&bi_re, bi_1, bi_2);
        printf("\n");

        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    }
    */
    _CrtDumpMemoryLeaks();

    return 0;
}