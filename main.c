#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, THIS_FILE, __LINE__ )

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bigint.h"
#include "test.h"

#ifndef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif


int main()
{
	srand((unsigned)time(NULL));
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_crtBreakAlloc = 94;
	addition_test(); // Memory 0
	subtraction_test(); // Memory 0
	schoolbook_multiplication_test(); // Memory 0
	karatsuba_multiplication_test(); // Memory 0
	binary_long_division_test(); // Memory 0
	multi_long_division_test(); // Memory 0
	squaring_test(); // Memory 0
	//squaring_karatsuba_test();
	montgomery_Exponentiation_multiplication_test(); // Memory 0
	montgomery_ladder_Exponentiation_addtion_test(); // Memory 0
	montgomery_ladder_Modular_Exponentiation_multiplication_test(); // Memory 0
	//montgomery_ladder_Modular_Exponentiation_addtion_test(); // Memory 0
	Left_Right_EXP_ADD_test(); // Memory 0
	//Left_Right_EXP_MUL_test(); // Memory 0
	Right_Left_EXP_ADD_test(); // Memory 0
	Right_Left_EXP_MUL_test(); // Memory 0
	Left_Right_EXP_ADD_MOD_test(); // Memory 0
	//Left_Right_EXP_MUL_M3OD_test(); // Memory 0
	Right_Left_EXP_ADD_MOD_test(); // Memory 0
	Right_Left_EXP_MUL_MOD_test(); // Memory 0
	//Left_Right_EXP_ADD_MOD_DIV_test(); // Memory 0
	//Left_Right_EXP_MUL_MOD_DIV_test(); // Memory 0
	//Right_Left_EXP_ADD_MOD_DIV_test(); // Memory 0
	//Right_Left_EXP_MUL_MOD_DIV_test(); // Memory 0
	
	_CrtDumpMemoryLeaks();
	return 0;
}
