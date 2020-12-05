#ifndef _BIG_INT_
#define _BIG_INT_

#include "config.h"

// Additon
int ADD(bigint** C, bigint** A, bigint** B);

// Subtraction
int SUB(bigint** c, bigint* a, bigint* b);

// Multiplication
int Multiplication(bigint** C, bigint* A, bigint* B); // Select (Textbook Multiplication / Karatsuba Multiplication)
int MUL_Multi(bigint** result, bigint* A, bigint* B); // Textbook Multiplication
int MUL_Karatsuba(bigint** C, bigint* A, bigint* B); // Karatsuba Multiplication

// Squaring
int Squaring(bigint** C, bigint* A); // Select (Textbook Squaring / Karatsuba Squaring)
int SQU(bigint** C, bigint* A); // Textbook Squaring
int SQUC_Karatsuba(bigint** C, bigint* A); // Karatsuba Squaring

// Division
int Division(bigint** Q, bigint** R, bigint* A, bigint* B); // Select (Binary Long Division / Multi Long Division)
int Binary_Long_Div(bigint** Q, bigint** R, bigint* A, bigint* B); // Binary Long Division
int DIV(bigint** Q, bigint** R, bigint* A, bigint* B); // Multi Long Division

// Modular Exponentiation
int Modular_Exponentiation_MUL(bigint** C, bigint* A, bigint* N, bigint* M); // Select (Montgomery / Left to Right / Right to Left) (MUL ver)
int Modular_Exponentiation_ADD(bigint** C, bigint* A, bigint* N, bigint* M); // Select (Montgomery / Left to Right / Right to Left) (ADD ver)

int MOD_EXP_Montgomery_MUL(bigint** T, bigint* X, bigint* N, bigint* M); // Montgomery Modular Exponentiaition (MUL ver)
int MOD_EXP_Montgomery_ADD(bigint** T, bigint* X, bigint* N, bigint* M); // Montgomery Modular Exponentiaition (ADD ver)

int MOD_EXP_LR_MUL(bigint** T, bigint* X, bigint* N, bigint* M); // Left to Right Modular Exponentiaition (MUL ver)
int MOD_EXP_LR_ADD(bigint** T, bigint* X, bigint* N, bigint* M); // Left to Right Modular Exponentiaition (ADD ver)

int MOD_EXP_RL_MUL(bigint** T, bigint* X, bigint* N, bigint* M); // Right to Left Modular Exponentiaition (MUL ver)
int MOD_EXP_RL_ADD(bigint** T, bigint* X, bigint* N, bigint* M); // Right to Left Modular Exponentiaition (ADD ver)

#endif