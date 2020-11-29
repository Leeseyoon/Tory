#ifndef _BIG_INT_
#define _BIG_INT_

#define NON_NEGATIVE   0
#define NEGATIVE   1
#define ERROR   -1
#define WORD_BIT_LEN    8   // 8 or 32 or 64

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#if WORD_BIT_LEN == 8
typedef unsigned char word;
#define word_mask 0xff

#elif WORD_BIT_LEN == 32
typedef unsigned int word;
#define word_mask 0xffffffff

#else
typedef unsigned long long word;
#define word_mask 0xffffffffffffffff

#endif

typedef struct {
    int sign;
    int wordlen;
    word* a;
} bigint;

void array_init(word* a, int len);

//Chapter 2.1
void BI_New(bigint** x, int wordlen);
void BI_Delete(bigint** x);

// Chapter 2.2 Show BigInt
void BI_Set_By_Array(bigint** x, int sign, word* a, int wordlen);
void BI_Set_By_String(bigint** x, int sign, char* str, word base, int size);
void Ascii_To_Hex(char* str, char* hex);
void BI_Show(bigint* x, int base);

// Chapter 2.3  Refine BigInt
void BI_Refine(bigint* x);

// Chapter 2.4 Assign BigInt
void Assign_BI(bigint** y, bigint* x);

// Chapter 2.5 Generate Random BigInt
void bi_gen_rand(bigint** x, int sign, int wordlen);
void array_rand(word* dst, int wordlen);

// Chapter 2.6 Get Word Length / Bit Length / j-th Bit of Bit-Int
void Get_Word_Length(int* len, bigint** x);
void Bit_Length(int* len, bigint* x);
void j_th_Bit_of_BI(int j, bigint* x);

// Chapter 2.7 /* negative: 1, non-negative: 0 */
int Get_Sign(bigint* x);
void Flip_Sign(bigint* x);

// Chapter 2.8 Set One / Set Zero
void BI_Set_One(bigint** x);
void BI_Set_Zero(bigint** x);
int Is_One(bigint** x);
int Is_Zero(bigint** x);

// Chapter 2.9 Compare
int Compare_BI(bigint** x, bigint** y);

// Chapter 2.10 Left/Right Shift
void Left_Shift(bigint* x, int len);
void Right_Shift(bigint* x, int r);

// Chapter 2.11 Reduction
void Reduction_BI(bigint** x, int r);

// Chapter 3 Additon
unsigned int ADD_ABc(bigint** C, bigint** A, bigint** B, unsigned int c, int i);
void ADDC(bigint** C, bigint** A, bigint** B, int sign);
void ADD(bigint** C, bigint** A, bigint** B);
void ADDC_AAB(bigint** C, bigint** A, bigint** B, int sign);
void ADD_AAB(bigint** C, bigint** A, bigint** B);

int Compare_WordLen(bigint* A, bigint* B);

// Chapter 4 Subtraction
void SUBC_BI(unsigned int* borrow, bigint** c, bigint** a, bigint** b);
void SUB(bigint** c, bigint* a, bigint* b);

// Chapter 5 Multiplication
void MUL_Test(word* c, word* a, word* b);
void MUL_MUL(bigint** result, bigint* A, bigint* B);

// Karatsuba test
void Karatsuba(bigint** C, bigint* A, bigint* B);

// Squaring
void Single_Squaring(bigint* C, bigint* A);
void SQUC(bigint** C, bigint* A);
void SQUC_Karatsuba(bigint** C, bigint* A);

// Binary Division
void Binary_Long_Div(bigint** Q, bigint** R, bigint* A, bigint* B);
void ADDC_DIV(bigint** C, bigint** A, bigint** B, int sign);
void ADD_DIV(bigint** C, bigint** A, bigint** B);

// Multi Division
void DIV(bigint** Q, bigint** R, bigint* A, bigint* B);
void DIVC(bigint** Q, bigint** R, bigint* A, bigint* B);
void DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B);
void DIVCC_n_m(bigint** Q, bigint* A, bigint* B, int m);
void DIVCC_n_m1(bigint** Q, bigint* A, bigint* B, int m);

// Modular Exponentiation
void Montgomery_Exp_mul(bigint** C, bigint* A, int n);
void Montgomery_Exp_add(bigint** C, bigint* A, int n);
void Montgomery_Mod_Exp_mul(bigint** C, bigint* A, int n, bigint* M);
void Montgomery_Mod_Exp_add(bigint** C, bigint* A, int n, bigint* M);

void EXP_LR_MUL(bigint** T, bigint* X, int n, bigint* M);
void EXP_LR_ADD(bigint** T, bigint* X, int n, bigint* M);
void EXP_RL_MUL(bigint** T, bigint* X, int n, bigint* M);
void EXP_RL_ADD(bigint** T, bigint* X, int n, bigint* M);

void MOD_EXP_LR_MUL(bigint** T, bigint* X, int n, bigint* M);
void MOD_EXP_LR_ADD(bigint** T, bigint* X, int n, bigint* M);
void MOD_EXP_RL_MUL(bigint** T, bigint* X, int n, bigint* M);
void MOD_EXP_RL_ADD(bigint** T, bigint* X, int n, bigint* M);

void MOD_EXP_LR_MUL_DIV(bigint** T, bigint* X, int n, bigint* M);
void MOD_EXP_LR_ADD_DIV(bigint** T, bigint* X, int n, bigint* M);
void MOD_EXP_RL_MUL_DIV(bigint** T, bigint* X, int n, bigint* M);
void MOD_EXP_RL_ADD_DIV(bigint** T, bigint* X, int n, bigint* M);

void Bit_Length_of_number(int num, int* len); // 정수의 비트열의 개수 구하는 함수
int j_th_Bit_of_number(int j, int num); // 정수의 j번째 비트가 0인지 1인지 판단하는 함수

#endif
