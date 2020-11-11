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
    int len = 0;
    bigint* bi_1 = NULL;
    bigint* bi_2 = NULL;
    bigint* bi_re = NULL;

    for (i = 0; i < 20; i++) //20¹ø µ¡¼À ¿¬»êÇÏ±â
    {
        //printf("i = %d\n", i);
        BI_New(&bi_1, 2);
        BI_New(&bi_2, 3);
        len = Compare_WordLen(bi_1, bi_2);
        BI_New(&bi_re, len + 1);

        bi_1->sign = 0;
        bi_2->sign = 1;

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
    }

    
    /*
    for (i = 0; i < 20; i++) //10¹ø »¬¼À ¿¬»êÇÏ±â
    {
        BI_New(&bi_1, 2);
        BI_New(&bi_2, 2);
        BI_New(&bi_re, 2);
        
        bi_1->sign = rand() & 1;
        bi_2->sign = rand() & 1;
        
        bi_1->a[0] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_1->a[1] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_2->a[0] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_2->a[1] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
       
        SUB_BI_test(&bi_re, bi_1, bi_2);
        printf("\n");
        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    }

    for (i = 0; i < 20; i++) //10¹ø »¬¼À ¿¬»êÇÏ±â
    {
        BI_New(&bi_1, 1);
        BI_New(&bi_2, 1);
        BI_New(&bi_re, 2);

        bi_1->sign = rand() & 1;
        bi_2->sign = rand() & 1;

        bi_1->a[0] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        //bi_1->a[1] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        bi_2->a[0] = ((rand() * rand() * rand() * rand()) & 0xffffffff);
        //bi_2->a[1] = ((rand() * rand() * rand() * rand()) & 0xffffffff);

        MUL_MUL(&bi_re, bi_1, bi_2);
        printf("\n");
        BI_Delete(&bi_1);
        BI_Delete(&bi_2);
        BI_Delete(&bi_re);
    }
    */

    _CrtDumpMemoryLeaks();

    return 0;
}