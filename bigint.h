#ifndef _BIG_INT_
#define _BIG_INT_

#define NON_NEGATIVE   0
#define NEGATIVE   1
#define ERROR   -1
#define WORD_BIT_LEN   32   // 8 or 32 or 64

#if WORD_BIT_LEN == 8
typedef unsigned char word;
#elif WORD_BIT_LEN == 32
typedef unsigned int word;
#else
typedef unsigned long long word;
#endif

typedef struct {
    int sign;
    int wordlen;
    word* a;
}bigint;

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
void bi_refine(bigint* x);

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
int ADD_ABc(bigint** A, bigint** B, bigint** C, int c, int i);
bigint* ADDC(bigint** A, bigint** B, int sign);
bigint* ADD(bigint** A, bigint** B);

// Chapter 4 Subtraction
void SUBC_BI(int* borrow, bigint** c, bigint** a, bigint** b);
bigint* SUB_BI(bigint** a, bigint** b);

#endif