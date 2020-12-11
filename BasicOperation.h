#ifndef _BASIC_OPERATION_
#define _BASIC_OPERATION_

#include "config.h"

int array_init(word* a, int len);
int BI_New(bigint** x, int wordlen); // Big Integer Create Function
int BI_Delete(bigint** x); // Big Integer Delete Function

int BI_Set_By_Array(bigint** x, int sign, word* a, int wordlen);
int BI_Set_By_String(bigint** x, int sign, char* str, word base);
int Ascii_To_Hex(char* str, char* hex);
int BI_Show(bigint* x, int base); // Print Big Integer by (base

int BI_Refine(bigint* x); // Refine Big Integer Function
int BI_Assign(bigint** y, bigint* x); // Assign Big Integer x to y Function

int BI_Gen_Rand(bigint** x, int sign, int wordlen); // Generate Random Number and Substitute to Big integer
int Array_Rand(word* dst, int wordlen);

int BI_Get_Word_Length(int* len, bigint** x); // To get word length in Big Integer
int BI_Get_Bit_Length(int* len, bigint* x); // To get bit length in Big Integer
int BI_Get_j_th_Bit_of_BI(int j, bigint* x); // To get j(th) bit int Big Integer

int BI_Get_Sign(bigint* x);
int BI_Flip_Sign(bigint* x);

int BI_Set_One(bigint** x);
int BI_Set_Zero(bigint** x);
int BI_Is_One(bigint** x); // Determine Big Integer is 1
int BI_Is_Zero(bigint** x); // Determine Big Integer is 0

int BI_Compare(bigint** x, bigint** y);

int BI_Left_Shift(bigint* x, int len);
int BI_Right_Shift(bigint* x, int r);

int BI_Reduction(bigint** x, int r);

int Compare_WordLen(bigint* A, bigint* B); // return wordlen 큰 사이즈

#endif
