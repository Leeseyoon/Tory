#ifndef _TEST_
#define _TEST

/* Test Function */
int addition_test();
int subtraction_test();
int multiplication_test();
int squaring_test();
int division_test();
int modular_exponentiation_multiplication_test();
int modular_exponentiation_addition_test();

/* Measuring Time Function */
int addition_time();
int subtraction_time();

int schoolbook_multiplication_time();
int karatsuba_multiplication_time();

int binary_long_division_time();
int multi_long_division_time();

int squaring_time();
int squaring_karatsuba_time();

int montgomery_ladder_Modular_Exponentiation_multiplication_time();
int montgomery_ladder_Modular_Exponentiation_addtion_time();
int Left_Right_EXP_ADD_MOD_time();
int Left_Right_EXP_MUL_MOD_time();
int Right_Left_EXP_ADD_MOD_time();
int Right_Left_EXP_MUL_MOD_time();

/* Compare Time Function */
int compare_text_kara_mul();
int compare_text_kara_squ();
int compare_binary_multi_long_div();
int compare_mod_exp_Montgomery_LtoR();

#endif
