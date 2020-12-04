#ifndef _BASIC_OPERATION_
#define _BASIC_OPERATION_

#include "config.h"

int array_init(word* a, int len);

//Chapter 2.1
int BI_New(bigint** x, int wordlen); // Big Integer Create Function
int BI_Delete(bigint** x); // Big Integer Delete Function

// Chapter 2.2 Show BigInt
int BI_Set_By_Array(bigint** x, int sign, word* a, int wordlen);
int BI_Set_By_String(bigint** x, int sign, char* str, word base);
int Ascii_To_Hex(char* str, char* hex);
int BI_Show(bigint* x, int base); // Print Big Integer by (base

// Chapter 2.3  Refine BigInt
int BI_Refine(bigint* x); // Refine Big Integer Function

// Chapter 2.4 Assign BigInt
int BI_Assign(bigint** y, bigint* x); // Assign Big Integer x to y Function

// Chapter 2.5 Generate Random BigInt
int BI_Gen_Rand(bigint** x, int sign, int wordlen); // Generate Random Number and Substitute to Big integer
int Array_Rand(word* dst, int wordlen);

// Chapter 2.6 Get Word Length / Bit Length / j-th Bit of Bit-Int
int BI_Get_Word_Length(int* len, bigint** x); // To get word length in Big Integer
int BI_Get_Bit_Length(int* len, bigint* x); // To get bit length in Big Integer
int BI_Get_j_th_Bit_of_BI(int j, bigint* x); // To get j(th) bit int Big Integer

// Chapter 2.7 /* negative: 1, non-negative: 0 */
int BI_Get_Sign(bigint* x);
int BI_Flip_Sign(bigint* x);

// Chapter 2.8 Set One / Set Zero
int BI_Set_One(bigint** x);
int BI_Set_Zero(bigint** x);
int BI_Is_One(bigint** x); // Determine Big Integer is 1
int BI_Is_Zero(bigint** x); // Determine Big Integer is 0

// Chapter 2.9 Compare
int BI_Compare(bigint** x, bigint** y);

// Chapter 2.10 Left/Right Shift
int BI_Left_Shift(bigint* x, int len);
int BI_Right_Shift(bigint* x, int r);

// Chapter 2.11 Reduction
int BI_Reduction(bigint** x, int r);

#endif