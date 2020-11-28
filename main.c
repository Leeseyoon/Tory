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
    
    /* test */
    addition_test();
    subtraction_test();
    schoolbook_multiplication_test();
    karatsuba_multiplication_test();
    binary_long_division_test();
    multi_long_division_test();
    squaring_test();
    //squaring_karatsuba_test();
    montgomery_ladder_multiplication_test();
    montgomery_ladder_addtion_test();
    
    
    _CrtDumpMemoryLeaks();
    return 0;
}
