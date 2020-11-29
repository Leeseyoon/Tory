#ifndef _TEST_
#define _TEST

int addition_test();
int subtraction_test();

int schoolbook_multiplication_test();
int karatsuba_multiplication_test();

int binary_long_division_test();
int multi_long_division_test();

int squaring_test();
int squaring_karatsuba_test();

int montgomery_Exponentiation_multiplication_test();
int montgomery_ladder_Exponentiation_addtion_test();
int montgomery_ladder_Modular_Exponentiation_multiplication_test();
int montgomery_ladder_Modular_Exponentiation_addtion_test();

void Left_Right_EXP_ADD_test(); // Test of 'Only Exponentiation function L->R +'
void Left_Right_EXP_MUL_test(); // Test of 'Only Exponentiation function L->R *'
void Right_Left_EXP_ADD_test(); // Test of 'Only Exponentiation function L<-R +'
void Right_Left_EXP_MUL_test(); // Test of 'Only Exponentiation function L<-R *'
void Left_Right_EXP_ADD_MOD_test(); // Test of 'Modular Exponentiaition used Binary Long Division L->R +'
void Left_Right_EXP_MUL_MOD_test(); // Test of 'Modular Exponentiaition used Binary Long Division L->R *'
void Right_Left_EXP_ADD_MOD_test(); // Test of 'Modular Exponentiaition used Binary Long Division L<-R +'
void Right_Left_EXP_MUL_MOD_test(); // Test of 'Modular Exponentiaition used Binary Long Division L<-R *'
void Left_Right_EXP_ADD_MOD_DIV_test(); // Test of 'Modular Exponentiation used Multi Long Division L->R +'
void Left_Right_EXP_MUL_MOD_DIV_test(); // Test of 'Modular Exponentiation used Multi Long Division L->R *'
void Right_Left_EXP_ADD_MOD_DIV_test(); // Test of 'Modular Exponentiation used Multi Long Division L<-R +'
void Right_Left_EXP_MUL_MOD_DIV_test(); // Test of 'Modular Exponentiation used Multi Long Division L<-R *'

#endif
