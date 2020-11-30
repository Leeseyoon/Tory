#ifndef _BIG_INT_
#define _BIG_INT_

#define NON_NEGATIVE   0
#define NEGATIVE   1
#define ERROR   -1
#define WORD_BIT_LEN    32   // 8 or 32 or 64

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
void BI_j_th_Bit_of_BI(int j, bigint* x); // To get j(th) bit int Big Integer

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
void SUBC(unsigned int* borrow, bigint** c, bigint** a, bigint** b);
void SUB(bigint** c, bigint* a, bigint* b);

// Chapter 5 Multiplication
void MUL_Word(word* c, word* a, word* b);
void MUL_Multi(bigint** result, bigint* A, bigint* B);

// Karatsuba test
void Karatsuba(bigint** C, bigint* A, bigint* B);

// Squaring
void Single_Squaring(bigint* C, bigint* A);
void SQU(bigint** C, bigint* A);
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

void BI_Bit_Length_of_number(int num, int* len); // 정수의 비트열의 개수 구하는 함수
int j_th_Bit_of_number(int j, int num); // 정수의 j번째 비트가 0인지 1인지 판단하는 함수

void EXP_LR_MUL(bigint** T, bigint* X, int n); // Only Exponentiation function L->R * func
void EXP_LR_ADD(bigint** T, bigint* X, int n); // Only Exponentiation function L->R + func
void EXP_RL_MUL(bigint** T, bigint* X, int n); // Only Exponentiation function L<-R * func
void EXP_RL_ADD(bigint** T, bigint* X, int n); // Only Exponentiation function L<-R + func

void MOD_EXP_LR_MUL(bigint** T, bigint* X, int n, bigint* M); // Modular Exponentiaition used Binary Long Division L->R * func
void MOD_EXP_LR_ADD(bigint** T, bigint* X, int n, bigint* M); // Modular Exponentiaition used Binary Long Division L->R + func
void MOD_EXP_RL_MUL(bigint** T, bigint* X, int n, bigint* M); // Modular Exponentiaition used Binary Long Division L<-R * func
void MOD_EXP_RL_ADD(bigint** T, bigint* X, int n, bigint* M); // Modular Exponentiaition used Binary Long Division L<-R + func

void MOD_EXP_LR_MUL_DIV(bigint** T, bigint* X, int n, bigint* M); // Modular Exponentiation used Multi Long Division L->R * func
void MOD_EXP_LR_ADD_DIV(bigint** T, bigint* X, int n, bigint* M); // Modular Exponentiation used Multi Long Division L->R + func
void MOD_EXP_RL_MUL_DIV(bigint** T, bigint* X, int n, bigint* M); // Modular Exponentiation used Multi Long Division L<-R * func
void MOD_EXP_RL_ADD_DIV(bigint** T, bigint* X, int n, bigint* M); // Modular Exponentiation used Multi Long Division L<-R + func

#endif
