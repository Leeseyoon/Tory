#ifndef _BIG_INT_
#define _BIG_INT_

#include "config.h"

// Chapter 3 Additon
unsigned int ADD_ABc(bigint** C, bigint** A, bigint** B, unsigned int c, int i);
int ADDC(bigint** C, bigint** A, bigint** B, int sign);
int ADD(bigint** C, bigint** A, bigint** B); // ****
int ADDC_AAB(bigint** C, bigint** A, bigint** B, int sign);
int ADD_AAB(bigint** C, bigint** A, bigint** B);

int Compare_WordLen(bigint* A, bigint* B);

// Chapter 4 Subtraction
int SUBC(bigint** c, bigint** a, bigint** b);
int SUB(bigint** c, bigint* a, bigint* b); // ****

// Chapter 5 Multiplication
int Multiplication(bigint** C, bigint* A, bigint* B); // ****

int MUL_Word(word* c, word* a, word* b);
int MUL_Multi(bigint** result, bigint* A, bigint* B); // ****

// Karatsuba multiplication
int MUL_Karatsuba(bigint** C, bigint* A, bigint* B); // ****

// Squaring
int Squaring(bigint** C, bigint* A); // ****

int Single_Squaring(bigint* C, bigint* A);
int SQU(bigint** C, bigint* A); // ****
int SQUC(bigint** C, bigint* A);
int SQUC_Karatsuba(bigint** C, bigint* A); // ****

// Division
int Division(bigint** Q, bigint** R, bigint* A, bigint* B); // ****

// Binary Division
int Binary_Long_Div(bigint** Q, bigint** R, bigint* A, bigint* B); // ****
int ADDC_DIV(bigint** C, bigint** A, bigint** B, int sign); // 프로토타입으로
int ADD_DIV(bigint** C, bigint** A, bigint** B); // 프로토타입으로

// Multi Division
int DIV(bigint** Q, bigint** R, bigint* A, bigint* B); // ****
int DIVC(bigint** Q, bigint** R, bigint* A, bigint* B);
int DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B);
int DIVCC_n_m(bigint** Q, bigint* A, bigint* B, int m); // 프로토타입으로
int DIVCC_n_m1(bigint** Q, bigint* A, bigint* B, int m); // 프로토타입으로

// Modular Exponentiation
int Modular_Exponentiation_MUL(bigint** C, bigint* A, bigint* N, bigint* M); // ****
int Modular_Exponentiation_ADD(bigint** C, bigint* A, bigint* N, bigint* M); // ****

int EXP_Montgomery_MUL(bigint** T, bigint* X, bigint* N); // 프로토타입으로
int EXP_Montgomery_ADD(bigint** T, bigint* X, bigint* N); // 프로토타입으로
int MOD_EXP_Montgomery_MUL(bigint** T, bigint* X, bigint* N, bigint* M); // ****
int MOD_EXP_Montgomery_ADD(bigint** T, bigint* X, bigint* N, bigint* M); // ****

int EXP_LR_MUL(bigint** T, bigint* X, bigint* N); // Only Exponentiation function L->R * func
int EXP_LR_ADD(bigint** T, bigint* X, bigint* N); // Only Exponentiation function L->R + func
int EXP_RL_MUL(bigint** T, bigint* X, bigint* N); // Only Exponentiation function L<-R * func
int EXP_RL_ADD(bigint** T, bigint* X, bigint* N); // Only Exponentiation function L<-R + func
// 프로토타입으로

int MOD_EXP_LR_MUL(bigint** T, bigint* X, bigint* N, bigint* M);  // Modular Exponentiaition L->R * func // ****
int MOD_EXP_LR_ADD(bigint** T, bigint* X, bigint* N, bigint* M);  // Modular Exponentiaition L->R + func // ****
int MOD_EXP_RL_MUL(bigint** T, bigint* X, bigint* N, bigint* M);  // Modular Exponentiaition L<-R * func // ****
int MOD_EXP_RL_ADD(bigint** T, bigint* X, bigint* N, bigint* M);  // Modular Exponentiaition L<-R + func // ****

#endif