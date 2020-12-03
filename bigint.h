#ifndef _BIG_INT_
#define _BIG_INT_

#include "config.h"

void array_init(word* a, int len);

//Chapter 2.1
void BI_New(bigint** x, int wordlen); // Big Integer Create Function
void BI_Delete(bigint** x); // Big Integer Delete Function

// Chapter 2.2 Show BigInt
void BI_Set_By_Array(bigint** x, int sign, word* a, int wordlen);
void BI_Set_By_String(bigint** x, int sign, char* str, word base, int size);
void Ascii_To_Hex(char* str, char* hex);
void BI_Show(bigint* x, int base); // Print Big Integer by (base

// Chapter 2.3  Refine BigInt
void BI_Refine(bigint* x); // Refine Big Integer Function

// Chapter 2.4 Assign BigInt
void BI_Assign(bigint** y, bigint* x); // Assign Big Integer x to y Function

// Chapter 2.5 Generate Random BigInt
void BI_Gen_Rand(bigint** x, int sign, int wordlen); // Generate Random Number and Substitute to Big integer
void Array_Rand(word* dst, int wordlen);

// Chapter 2.6 Get Word Length / Bit Length / j-th Bit of Bit-Int
void BI_Get_Word_Length(int* len, bigint** x); // To get word length in Big Integer
void BI_Bit_Length(int* len, bigint* x); // To get bit length in Big Integer
int BI_j_th_Bit_of_BI(int j, bigint* x); // To get j(th) bit int Big Integer

// Chapter 2.7 /* negative: 1, non-negative: 0 */
int BI_Get_Sign(bigint* x);
void BI_Flip_Sign(bigint* x);

// Chapter 2.8 Set One / Set Zero
void BI_Set_One(bigint** x);
void BI_Set_Zero(bigint** x);
int BI_Is_One(bigint** x); // Determine Big Integer is 1
int BI_Is_Zero(bigint** x); // Determine Big Integer is 0

// Chapter 2.9 Compare
int BI_Compare(bigint** x, bigint** y);

// Chapter 2.10 Left/Right Shift
void Left_Shift(bigint* x, int len);
void Right_Shift(bigint* x, int r);

// Chapter 2.11 Reduction
void BI_Reduction(bigint** x, int r);

// Chapter 3 Additon
unsigned int ADD_ABc(bigint** C, bigint** A, bigint** B, unsigned int c, int i);
void ADDC(bigint** C, bigint** A, bigint** B, int sign);
void ADD(bigint** C, bigint** A, bigint** B);
void ADDC_AAB(bigint** C, bigint** A, bigint** B, int sign);
void ADD_AAB(bigint** C, bigint** A, bigint** B);

int Compare_WordLen(bigint* A, bigint* B);

// Chapter 4 Subtraction
int SUBC(bigint** c, bigint** a, bigint** b);
int SUB(bigint** c, bigint* a, bigint* b);

// Chapter 5 Multiplication
int Multiplication(bigint** C, bigint* A, bigint* B);

int MUL_Word(word* c, word* a, word* b);
int MUL_Multi(bigint** result, bigint* A, bigint* B);

// Karatsuba multiplication
void MUL_Karatsuba(bigint** C, bigint* A, bigint* B);

// Squaring
int Squaring(bigint** C, bigint* A);

void Single_Squaring(bigint* C, bigint* A);
void SQU(bigint** C, bigint* A);
void SQUC(bigint** C, bigint* A);
void SQUC_Karatsuba(bigint** C, bigint* A);

// Division
int Division(bigint** Q, bigint** R, bigint* A, bigint* B);

// Binary Division
int Binary_Long_Div(bigint** Q, bigint** R, bigint* A, bigint* B);
int ADDC_DIV(bigint** C, bigint** A, bigint** B, int sign); // 프로토타입으로
int ADD_DIV(bigint** C, bigint** A, bigint** B); // 프로토타입으로

// Multi Division
int DIV(bigint** Q, bigint** R, bigint* A, bigint* B);
int DIVC(bigint** Q, bigint** R, bigint* A, bigint* B);
int DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B);
int DIVCC_n_m(bigint** Q, bigint* A, bigint* B, int m); // 프로토타입으로
int DIVCC_n_m1(bigint** Q, bigint* A, bigint* B, int m); // 프로토타입으로

// Modular Exponentiation
int Modular_Exponentiation_MUL(bigint** C, bigint* A, bigint* N, bigint* M);
int Modular_Exponentiation_ADD(bigint** C, bigint* A, bigint* N, bigint* M);

void EXP_Montgomery_MUL(bigint** C, bigint* A, bigint* N); // 프로토타입으로
void EXP_Montgomery_ADD(bigint** C, bigint* A, bigint* N); // 프로토타입으로
void MOD_EXP_Montgomery_MUL(bigint** C, bigint* A, bigint* N, bigint* M);
void MOD_EXP_Montgomery_ADD(bigint** C, bigint* A, bigint* N, bigint* M);

int EXP_LR_MUL(bigint** T, bigint* X, bigint* N); // Only Exponentiation function L->R * func
int EXP_LR_ADD(bigint** T, bigint* X, bigint* N); // Only Exponentiation function L->R + func
int EXP_RL_MUL(bigint** T, bigint* X, bigint* N); // Only Exponentiation function L<-R * func
int EXP_RL_ADD(bigint** T, bigint* X, bigint* N); // Only Exponentiation function L<-R + func
// 프로토타입으로

int MOD_EXP_LR_MUL(bigint** T, bigint* X, bigint* N, bigint* M);  // Modular Exponentiaition L->R * func
int MOD_EXP_LR_ADD(bigint** T, bigint* X, bigint* N, bigint* M);  // Modular Exponentiaition L->R + func
int MOD_EXP_RL_MUL(bigint** T, bigint* X, bigint* N, bigint* M);  // Modular Exponentiaition L<-R * func
int MOD_EXP_RL_ADD(bigint** T, bigint* X, bigint* N, bigint* M);  // Modular Exponentiaition L<-R + func

#endif
