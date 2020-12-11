#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "BasicOperation.h"
#include "Calculation.h"
#include "test.h"


int main()
{
	srand((unsigned)time(NULL));
	
	/* Test Function */
	addition_test();
	subtraction_test();
	multiplication_test();
	squaring_test();
	division_test();
	modular_exponentiation_multiplication_test();
	modular_exponentiation_addition_test();

	/* Measuring Time Function */
	//addition_time();
	//subtraction_time();
	//schoolbook_multiplication_time();
	//karatsuba_multiplication_time();
	//binary_long_division_time();
	//multi_long_division_time();
	//squaring_time();
	//squaring_karatsuba_time();
	//montgomery_ladder_Modular_Exponentiation_multiplication_time();
	//montgomery_ladder_Modular_Exponentiation_addtion_time();
	//Left_Right_EXP_ADD_MOD_time(); 
	//Left_Right_EXP_MUL_MOD_time();
	//Right_Left_EXP_ADD_MOD_time();
	//Right_Left_EXP_MUL_MOD_time();

	/* Compare Time Function */
	//compare_text_kara_mul();
	//compare_text_kara_squ();
	//compare_binary_multi_long_div();
	//compare_mod_exp_Montgomery_LtoR();

	return 0;
}
