#ifndef _TEST_
#define _TEST

int addition_test();
int subtraction_test();

int multiplication_test();

int division_test();

int squaring_test();
int squaring_karatsuba_test();

int modular_exponentiation_multiplication_test();
int modular_exponentiation_addition_test();

//////////////////////////////////////////////////////////////////////////////////////////////

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

#endif
