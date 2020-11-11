#include "bigint.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define   zerorize

// Chpater 2.1
void array_init(word* a, int len)
{
    memset(a, 0, sizeof(word) * len - 1);
}
void BI_Delete(bigint** x)
{
    if (*x == NULL)
        return;

#ifdef zerorize
    array_init((*x)->a, (*x)->wordlen);
#endif 
    free((*x)->a);
    free(*x);
    *x = NULL;
}
void BI_New(bigint** x, int wordlen)
{
    if (*x != NULL)
        BI_Delete(x);

    *x = (bigint*)malloc(sizeof(bigint));
    (*x)->sign = NON_NEGATIVE;
    (*x)->wordlen = wordlen;
    (*x)->a = (word*)calloc(wordlen, sizeof(word));
}

// Chapter 2.2 Show BigInt
void BI_Set_By_Array(bigint** x, int sign, word* a, int wordlen)
{
    int i;

    (*x)->sign = sign;
    (*x)->wordlen = wordlen;

    for (i = 0; i < wordlen; i++)
        (*x)->a[i] = a[i];
}
void BI_Set_By_String(bigint** x, int sign, char* str, word base, int size)
{
    char* hex;
    int i, j, k;

    hex = (char*)malloc(size * sizeof(word));
    Ascii_To_Hex(str, hex);

    if (base == 2) // 11 01101010 10101111 -> 3 byte 방에 a[2] a[1] a[0]
    {
        if ((strlen(str) % WORD_BIT_LEN) != 0)
        {
            for (i = 0; i < (int)(strlen(str) / WORD_BIT_LEN); i++)
            {
                for (j = 0; j < WORD_BIT_LEN; j++)
                    (*x)->a[i] |= (hex[strlen(str) - 1 - WORD_BIT_LEN * i - j] << j);
            }

            for (k = 0; k < (int)(strlen(str) % WORD_BIT_LEN); k++)
                (*x)->a[(strlen(str) / WORD_BIT_LEN)] |= (hex[k] << ((strlen(str) % WORD_BIT_LEN) - k - 1));
        }

        else
        {
            for (i = 0; i < (int)(strlen(str) / WORD_BIT_LEN); i++)
            {
                for (j = 0; j < WORD_BIT_LEN; j++)
                    (*x)->a[i] |= (hex[strlen(str) - 1 - WORD_BIT_LEN * i - j] << j);
            }
        }
    }


    else if (base == 10)
        printf("hello\n"); // 추후에 구현할 것

    else if (base == 16) // "123456789" -> {0x89, 0x67, 0x45, 0x23, 0x01} / {0x6789, 0x2345, 0x0001}
    {
        if ((strlen(str)) % (WORD_BIT_LEN / 4) != 0) // 2개가 1 byte니까
        {
            for (int i = 0; i < (int)(strlen(str) / (WORD_BIT_LEN / 4)); i++)
            {
                for (int j = 0; j < WORD_BIT_LEN / 4; j++)
                    (*x)->a[i] |= hex[strlen(str) - 1 - (WORD_BIT_LEN / 4) * i - j] << (4 * j);
            }

            for (int k = 0; k < (int)(strlen(str) % (WORD_BIT_LEN / 4)); k++)
                (*x)->a[strlen(str) / (WORD_BIT_LEN / 4)] |= (hex[(strlen(str) % (WORD_BIT_LEN / 4)) - 1 + k] << (4 * k));
        }

        else
        {
            for (int i = 0; i < (int)(strlen(str) / (WORD_BIT_LEN / 4)); i++)
            {
                for (int j = 0; j < WORD_BIT_LEN / 4; j++)
                    (*x)->a[i] |= hex[strlen(str) - 1 - (WORD_BIT_LEN / 4) * i - j] << (4 * j);
            }
        }
    }

}
void Ascii_To_Hex(char* str, char* hex)
{
    int len = 0;
    len = strlen(str);

    for (int i = 0; i < len; i++)
    {
        if (str[i] < 0x40)
            hex[i] = str[i] - 0x30;
        else if (str[i] < 0x47)
            hex[i] = str[i] - 0x31;
    }

}
void BI_Show(bigint* x, int base)
{
    int i = 0;
    int j = 0;
    int k = 0;

    if (base == 10) // 10진수일 때
    {
        printf("%d", x->a[0]);
        for (i = 1; i < x->wordlen; i++)
        {
            printf("%d * 2^%d", x->a[i], i * WORD_BIT_LEN);
        }
    }

    else if (base == 16) // 16진수일 때
    {
        if (x->sign == 1)
            printf("-");
        printf("0x");
        for (i = x->wordlen - 1; i >= 0; i--)
        {
            if (WORD_BIT_LEN == 32)
                for (j = 0; j < 4; j++)
                    printf("%02x", ((x->a[i]) >> (32 - (j + 1) * 8)) & 255);

            else if (WORD_BIT_LEN == 64)
                for (j = 0; j < 8; j++)
                    printf("%02x", (x->a[i]) >> (64 - (j + 1) * 8) & 255);

            else
                printf("%02x", (x->a[i]));
        }
    }

    else if (base == 2) // 2진수일 때
    {
        printf("0b");
        for (i = x->wordlen - 1; i >= 0; i--)
            for (j = WORD_BIT_LEN - 1; j >= 0; j--)
            {
                k = 1;
                k = k << j;
                printf("%d", (x->a[i] & k) >> j); //
            }
    }
    printf("\n");
}

//Chapter 2.3
void bi_refine(bigint* x)
{
    word* temp;

    if (x == NULL)
        return;

    int new_wordlen = x->wordlen;

    while (new_wordlen > 1) // at least one word needed
    {
        if (x->a[new_wordlen - 1] != 0)
            break;
        new_wordlen--;
    }

    if (x->wordlen != new_wordlen)
    {
        x->wordlen = new_wordlen;
        temp = (word*)realloc(x->a, sizeof(word) * new_wordlen);

        if (temp != NULL)
            x->a = temp;
    }

    if ((x->wordlen == 1) && (x->a[0] == 0x0))
        x->sign = NON_NEGATIVE;
}

// Chapter 2.4 Assign BigInt
void Assign_BI(bigint** y, bigint* x)
{
    int i = 0;
    if (*y != NULL)
        BI_Delete(y);
    BI_New(y, x->wordlen);
    (*y)->sign = x->sign;
    for (i = 0; i < x->wordlen; i++)
        (*y)->a[i] = x->a[i];
    //BI_Delete(&x);
}

// Chapter 2.5 Generate Random BigInt
void bi_gen_rand(bigint** x, int sign, int wordlen)
{
    BI_New(x, wordlen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordlen);

    bi_refine(*x);
}
void array_rand(word* dst, int wordlen)
{
    unsigned char* p = (unsigned char*)dst;
    int cnt = wordlen * sizeof(word);
    while (cnt > 0)
    {
        *p = rand() & 0xff;
        p++;
        cnt--;
    }
}

// Chapter 2.6 Get Word Length / Bit Length / j-th Bit of Big-Int
void Get_Word_Length(int* len, bigint** x)
{
    *len = (*x)->wordlen; // Big Integer x의 wordlen를 대입
}
void Bit_Length(int* len, bigint* x)
{
    int i = 0;
    int j = 0;
    int k = 0;
    i = (x->wordlen) - 1;

    for (j = WORD_BIT_LEN - 1; j >= 0; j--)
    {
        k = 1;
        k = k << j;
        if ((x->a[i] & k) != 0)
            break;
    }
    j += i * WORD_BIT_LEN;
    *len = j;
}
void j_th_Bit_of_BI(int j, bigint* x)
{
    char* z = NULL;
    int i = 0;
    int k = 0;
    printf("j_th bit : ");
    if (j >= (x->wordlen) * WORD_BIT_LEN)
        return;
    else
    {
        k = j / WORD_BIT_LEN;
        i = 1;
        i = i << j;

        if (i == (x->a[k] & i))
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

// Chapter 2.7 /* negative: 1, non-negative: 0 */
int Get_Sign(bigint* x)
{
    if ((x->sign) == NON_NEGATIVE)
        return NON_NEGATIVE;
    else if ((x->sign) == NEGATIVE)
        return NEGATIVE;
    else
        return ERROR;
}
void Flip_Sign(bigint* x)
{
    if ((x->sign) == NON_NEGATIVE)
        x->sign = NEGATIVE;
    else if ((x->sign) == NEGATIVE)
        x->sign = NON_NEGATIVE;
}

// Chapter2.8 Set One, Set Zero, Is Zero, Is One
void BI_Set_One(bigint** x)
{
    BI_New(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x1; // 0x1
}
void BI_Set_Zero(bigint** x)
{
    BI_New(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x00;
}
int Is_One(bigint** x)
{
    int i = 0;

    if ((*x)->sign == NEGATIVE || (*x)->a[0] != 1)
        return -1; // False

    for (i = (*x)->wordlen - 1; i > 0; i--)
    {
        if ((*x)->a[i] != 0)
            return -1;
    }
    return 0;
}
int Is_Zero(bigint** x)
{
    int i = 0;

    if ((*x)->sign == NEGATIVE || (*x)->a[0] != 0)
        return -1; // False

    for (i = (*x)->wordlen - 1; i > 0; i--)
    {
        if ((*x)->a[i] != 0)
            return -1;
    }
    return 0;
}

// Chapter 2.9 Compare
int Compare_BI(bigint** x, bigint** y)
{
    int i = 0;
    int len_x, len_y = 0;
    int len = 0;
    if ((*x)->sign < (*y)->sign) // A가 양수, B가 음수면 당연히 A가 크므로
        return 1;
    else if ((*x)->sign > (*y)->sign) // A가 음수, B가 양수면 당연히 B가 크므로
        return -1;
    else // A, B 부호가 같을 때
    {
        if ((*x)->sign == 0) // A, B 부호가 양수일 때 (부호가 서로 같으므로, if 문을 통해 하나만 비교)
        {
            Get_Word_Length(&len_x, x);
            Get_Word_Length(&len_y, y);
            if (len_x > len_y)
                return 1;
            else if (len_x < len_y)
                return -1;
            else
            {
                for (i = (int)len_x - 1; i >= 0; i--)
                {
                    if ((*x)->a[i] > (*y)->a[i])
                        return 1;
                    else if ((*x)->a[i] < (*y)->a[i])
                        return -1;
                }
                return 0;
            }
        }
        else // A, B 부호가 음수일 때
        {
            Get_Word_Length(&len_x, x);
            Get_Word_Length(&len_y, y);
            if (len_x > len_y)
                return -1;
            else if (len_x < len_y)
                return 1;
            else
            {
                for (i = (int)len_x - 1; i >= 0; i--)
                {
                    if ((*x)->a[i] > (*y)->a[i])
                        return 1;
                    else if ((*x)->a[i] < (*y)->a[i])
                        return -1;
                }
                return 0;
            }
        }

    }
}

// Chapter 2.10 Left/Right Shift
void Left_Shift(bigint* x, int len)
{
    int length = 0;
    length = x->wordlen;
    int add_len = 0;
    int q = 0;
    int new_wordlen = 0;
    int i = 0;
    int wn = 0;
    int count = 0;
    int r = 0;
    word* temp;

    bigint* cp = NULL;
    Assign_BI(&cp, x);

    wn = WORD_BIT_LEN * x->wordlen;

    if ((len % WORD_BIT_LEN) == 0)
        add_len = len / WORD_BIT_LEN;
    else
        add_len = (len / WORD_BIT_LEN) + 1;

    new_wordlen = add_len + x->wordlen;

    temp = (word*)realloc(x->a, sizeof(word) * new_wordlen);
    if (temp != NULL)
        x->a = temp;

    x->wordlen = new_wordlen;

    for (i = 0; i < add_len; i++)
        x->a[length + i] = 0;

    q = len / WORD_BIT_LEN;
    r = len % WORD_BIT_LEN;

    if (len % WORD_BIT_LEN == 0)
    {
        for (i = length - 1; i >= 0; i--)
            x->a[i + add_len] = x->a[i];
        for (i = 0; i < add_len; i++)
            x->a[i] = 0;
    }

    else if (len % WORD_BIT_LEN != 0)
    {
        for (i = 0; i < q; i++)
            x->a[i] = 0;

        x->a[q] = cp->a[0] << r;

        for (i = 1; i < x->wordlen - 1; i++)
            x->a[i + q] = (cp->a[i] << r) | (cp->a[i - 1] >> (WORD_BIT_LEN - r));

        x->a[x->wordlen - 1] = cp->a[cp->wordlen - 1] >> (WORD_BIT_LEN - r);
    }

    bi_refine(x);
}
void Right_Shift(bigint* x, int len)
{
    int i = 0;
    int wn = 0;
    int count = 0;
    int r = 0;
    int q = 0;

    wn = WORD_BIT_LEN * x->wordlen;
    q = len / WORD_BIT_LEN;
    r = len % WORD_BIT_LEN;

    bigint* cp = NULL;
    Assign_BI(&cp, x);

    if (len >= wn)
    {
        for (i = 0; i < x->wordlen; i++)
            x->a[i] = 0;
    }

    else if ((len < wn) && (len % WORD_BIT_LEN == 0))
    {
        count = len / WORD_BIT_LEN;
        for (i = 0; i < x->wordlen - count; i++)
            x->a[i] = x->a[i + count];
        for (i = x->wordlen - count; i < x->wordlen; i++)
            x->a[i] = 0;
    }

    else if ((len < wn) && (len % WORD_BIT_LEN != 0))
    {
        for (i = q; i < x->wordlen - 1; i++) {
            x->a[i] = (cp->a[i + 1] << ((WORD_BIT_LEN - r)) | (cp->a[i] >> r));
        }

        x->a[x->wordlen - 1] = cp->a[cp->wordlen - 1] >> r;
    }

}

// Chapter 2.11 Reduction
void Reduction_BI(bigint** x, int r)
{
    int i = 0;
    int wn = 0;
    int count = 0;
    wn = WORD_BIT_LEN * ((*x)->wordlen);

    if (r >= wn)
    {
        return;
    }
    else if ((r < wn) && (r % WORD_BIT_LEN == 0))
    {
        count = r / WORD_BIT_LEN;
        /*for (i = 0; i < count; i++)
           (*x)->a[i] = (*x)->a[i] & (((unsigned long long)1 << WORD_BIT_LEN) - 1);*/
        for (i = count; i < (*x)->wordlen; i++)
            (*x)->a[i] = 0;
    }
    else
    {
        count = r / WORD_BIT_LEN;
        for (i = 0; i < count; i++)
            (*x)->a[i] = (*x)->a[i] & (((unsigned long long)1 << WORD_BIT_LEN) - 1);
        r = r % WORD_BIT_LEN;
        (*x)->a[i] = (*x)->a[i] & (((unsigned long long)1 << r) - 1);
        for (i = count + 1; i < (*x)->wordlen; i++)
            (*x)->a[i] = 0;
    }

}

//Chapter 3 Addition
// 캐리 포함한 단일 덧셈
int ADD_ABc(bigint** C, bigint** A, bigint** B, int c, int i)
{
    int carry = 0;

    (*C)->a[i] = (*A)->a[i] + (*B)->a[i];

    if ((*C)->a[i] < (*A)->a[i])
        carry = 1;

    (*C)->a[i] = (*C)->a[i] + c;

    if ((*C)->a[i] < c)
        carry = carry + 1;

    return carry;
}

// WordLen(A) >= WordLen(B)
void ADDC(bigint** C, bigint** A, bigint** B, int sign)
{
    int A_Len;
    int B_Len;
    int i;
    int carry;

    A_Len = (*A)->wordlen;
    B_Len = (*B)->wordlen;

    word* temp;

    (*B)->wordlen = A_Len;
    temp = (word*)realloc((*B)->a, sizeof(word) * A_Len);
    if (temp != NULL)
        (*B)->a = temp;

    for (i = B_Len; i < A_Len; i++)
        (*B)->a[i] = 0;

    carry = 0;

    for (i = 0; i < A_Len; i++)
        carry = ADD_ABc(C, A, B, carry, i);

    if (carry == 1)
        (*C)->a[A_Len] = 1;
    else
        (*C)->a[A_Len] = 0;

    bi_refine(*B);
    bi_refine(*C);

    if (sign == 0)
        (*C)->sign = 0;
    else
        (*C)->sign = 1;
}

void ADD(bigint** C, bigint** A, bigint** B) // no print
{
    int A_Len = 0;
    int B_Len = 0;
    int A_sign;
    int B_sign;
    int i; 
    A_sign = Get_Sign(*A);
    B_sign = Get_Sign(*B);

    Get_Word_Length(&A_Len, A);
    Get_Word_Length(&B_Len, B);

    if (Is_Zero(A) == 0) // A is zero
    {
        (*C)->sign = (*B)->sign;
        (*C)->wordlen = (*B)->wordlen;
        for (i = 0; i < (*C)->wordlen; i++)
            (*C)->a[i] = (*B)->a[i];

        return;
    }

    if (Is_Zero(B) == 0) // B is zero
    {
        (*C)->sign = (*A)->sign;
        (*C)->wordlen = (*A)->wordlen;
        for (i = 0; i < (*C)->wordlen; i++)
            (*C)->a[i] = (*A)->a[i];

        return;
    }

    if ((A_sign == NON_NEGATIVE) && (B_sign == NEGATIVE))
    {
        bigint* temp = NULL;
        BI_New(&temp, B_Len);
        Assign_BI(&temp, *B);

        Flip_Sign(temp);
        SUB_BI_test(C, *A, temp); // SUB 함수
        //bi_refine(C);

        BI_Delete(&temp);

        return;
    }

    if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
    {
        bigint* temp = NULL;
        BI_New(&temp, A_Len);
        Assign_BI(&temp, *A);

        Flip_Sign(temp);
        SUB_BI_test(C, *B, temp); // SUB 함수
        //bi_refine(C);

        BI_Delete(&temp);

        return;
    }

    // A, B가 동일한 부호일 때
    if (A_Len >= B_Len)
    {
        ADDC(C, A, B, A_sign);
        return;
    }
    else
    {
        ADDC(C, B, A, A_sign);
        return;
    }

}

void ADD_BI_test(bigint** C, bigint** A, bigint** B)
{
    int A_Len = 0;
    int B_Len = 0;
    int A_sign;
    int B_sign;

    A_sign = Get_Sign(*A);
    B_sign = Get_Sign(*B);

    Get_Word_Length(&A_Len, A);
    Get_Word_Length(&B_Len, B);

    if (Is_Zero(A) == 0) // A is zero
    {
        Assign_BI(C, *B);
        bi_refine(C);

        printf("A = ");
        BI_Show(*A, 16);
        printf("B = ");
        BI_Show(*B, 16);
        printf("A + B == ");
        BI_Show(*C, 16);

        return;
    }

    if (Is_Zero(B) == 0) // B is zero
    {
        Assign_BI(C, *A);
        bi_refine(C);

        printf("A = ");
        BI_Show(*A, 16);
        printf("B = ");
        BI_Show(*B, 16);
        printf("A + B == ");
        BI_Show(*C, 16);

        return;
    }

    if ((A_sign == NON_NEGATIVE) && (B_sign == NEGATIVE))
    {
        bigint* temp = NULL;
        BI_New(&temp, B_Len);
        Assign_BI(&temp, *B);

        Flip_Sign(temp);
        SUB_BI_test(C, *A, temp); // SUB 함수
        bi_refine(C);

        printf("A = ");
        BI_Show(*A, 16);
        printf("B = ");
        BI_Show(*B, 16);
        printf("A + B == ");
        BI_Show(*C, 16);

        BI_Delete(&temp);

        return;
    }

    if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
    {
        bigint* temp = NULL;
        BI_New(&temp, A_Len);
        Assign_BI(&temp, *A);

        Flip_Sign(temp);
        SUB_BI_test(C, *B, temp); // SUB 함수
        bi_refine(C);

        printf("A = ");
        BI_Show(*A, 16);
        printf("B = ");
        BI_Show(*B, 16);
        printf("A + B == ");
        BI_Show(*C, 16);

        BI_Delete(&temp);

        return;
    }

    // A, B가 동일한 부호일 때
    if (A_Len >= B_Len)
    {
        ADDC(C, A, B, A_sign);

        printf("A = ");
        BI_Show(*A, 16);
        printf("B = ");
        BI_Show(*B, 16);
        printf("A + B == ");
        BI_Show(*C, 16);

        return;
    }
    else
    {
        ADDC(C, B, A, A_sign);

        printf("A = ");
        BI_Show(*A, 16);
        printf("B = ");
        BI_Show(*B, 16);
        printf("A + B == ");
        BI_Show(*C, 16);

        return;
    }

}

int Compare_WordLen(bigint* A, bigint* B) // return wordlen 큰 사이즈
{
    int A_Len, B_Len;

    Get_Word_Length(&A_Len, &A);
    Get_Word_Length(&B_Len, &B);

    if (A_Len > B_Len)
        return A_Len;
    else
        return B_Len;
}

void SUB_BI(bigint** c, bigint* a, bigint* b) // no print
{
    int borrow = 0;
    int len = 0;
    bigint* d = NULL;

    Get_Word_Length(&len, &a);
    Get_Word_Length(&borrow, &b);
    /*if (len > borrow)
        BI_New(c, len);
    else
    {
        BI_New(c, borrow);
        len = borrow;
    }*/

    if (Is_Zero(&a) == 0)
    {
        Assign_BI(c, b);
        return; // memory leackege X
    } // line 1~2

    if (Is_Zero(&b) == 0)
    {
        Assign_BI(c, a);
        return;// memory leackege X
    } // line 4~5


    if ((a->sign ^ b->sign) == 0) // A, B 부호가 같을 때
    {
        if ((a->sign & b->sign) == 0) // A, B의 부호가 모두 양수일 때
        {
            if (Compare_BI(&a, &b) < 0) // A, B를 비교해서 A < B일 때. Compare_BI(A, B)의 return : -1
            {

                SUBC_BI(&borrow, c, &b, &a);
                Flip_Sign(&c);

                return;// memory leackege X
            }
            else if (Compare_BI(&a, &b) == 0)
            {
                return;// memory leackege X
            }
            else // A, B 를 비교해서 A >= B일 때. Compare_BI(A, B)'s return : 0, 1
            {
                SUBC_BI(&borrow, c, &a, &b);

                return;// memory leackege X
            }
        }

        else // A, B의 부호가 모두 음수일 때
        {
            Flip_Sign(a);
            Flip_Sign(b);
            if (Compare_BI(&a, &b) < 0)
            {
                SUBC_BI(&borrow, c, &b, &a);
                Flip_Sign(a); // 부호 원위치
                Flip_Sign(b); // 부호 원위치
               
                return;// memory leackege X
            }
            else
            {
                SUBC_BI(&borrow, c, &a, &b);
                Flip_Sign(*c);
                Flip_Sign(a); // 부호 원위치
                Flip_Sign(b); // 부호 원위치
                
                return;// memory leackege X
            }
        }
    }

    else // A,B 부호가 다를 때 때
    {
        if (a->sign == 0)
        {
            Flip_Sign(b);
            ADD_BI_test(c, &a, &b); //수정하면 바꿀 곳
            Flip_Sign(b);
        
            return;
        }
        else
        {
            Flip_Sign(a);
            ADD(c, &a, &b); //수정하면 바꿀 곳
            //Flip_Sign(d); // -ADD(|A|,B)
            Flip_Sign(a); // 부호 원위치
            Flip_Sign(*c);
            
            return;
        }
    }
}

void SUB_BI_test(bigint** c, bigint* a, bigint* b)
{
    int borrow = 0;
    int len = 0;
    bigint* d = NULL;

    Get_Word_Length(&len, &a);
    Get_Word_Length(&borrow, &b);
    /*if (len > borrow)
        BI_New(c, len);
    else
    {
        BI_New(c, borrow);
        len = borrow;
    }*/
    
    if (Is_Zero(&a) == 0)
    {
        Assign_BI(c, b);
        printf("A = ");
        BI_Show(a, 16);
        printf("B = ");
        BI_Show(b, 16);
        printf("A - B == ");
        BI_Show(*c, 16);
        return; // memory leackege X
    } // line 1~2

    if (Is_Zero(&b) == 0)
    {
        Assign_BI(c, a);
        printf("A = ");
        BI_Show(a, 16);
        printf("B = ");
        BI_Show(b, 16);
        printf("A - B == ");
        BI_Show(*c, 16);
        return;// memory leackege X
    } // line 4~5


    if ((a->sign ^ b->sign) == 0) // A, B 부호가 같을 때
    {
        if ((a->sign & b->sign) == 0) // A, B의 부호가 모두 양수일 때
        {
            if (Compare_BI(&a, &b) < 0) // A, B를 비교해서 A < B일 때. Compare_BI(A, B)의 return : -1
            {

                SUBC_BI(&borrow, c, &b, &a);
                Flip_Sign(&c);
                printf("A = ");
                BI_Show(a, 16);
                printf("B = ");
                BI_Show(b, 16);
                Flip_Sign(*c);
                printf("A - B == ");
                BI_Show(*c, 16);
        
                return;// memory leackege X
            }
            else if (Compare_BI(&a, &b) == 0)
            {
                printf("A = ");
                BI_Show(a, 16);
                printf("B = ");
                BI_Show(b, 16);
                printf("A - B == ");
                BI_Show(*c, 16);
            
                return ;// memory leackege X
            }
            else // A, B 를 비교해서 A >= B일 때. Compare_BI(A, B)'s return : 0, 1
            {
                SUBC_BI(&borrow, c, &a, &b);
                printf("A = ");
                BI_Show(a, 16);
                printf("B = ");
                BI_Show(b, 16);
                printf("A - B == ");
                BI_Show(*c, 16);
                
                return;// memory leackege X
            }
        }

        else // A, B의 부호가 모두 음수일 때
        {
            Flip_Sign(a);
            Flip_Sign(b);
            if (Compare_BI(&a, &b) < 0)
            {
                SUBC_BI(&borrow, c, &b, &a);
                Flip_Sign(a); // 부호 원위치
                Flip_Sign(b); // 부호 원위치
                printf("A = ");
                BI_Show(a, 16);
                printf("B = ");
                BI_Show(b, 16);
                printf("A - B == ");
                BI_Show(*c, 16);

                return;// memory leackege X
            }
            else
            {
                SUBC_BI(&borrow, c, &a, &b);
                Flip_Sign(*c);
                Flip_Sign(a); // 부호 원위치
                Flip_Sign(b); // 부호 원위치
                printf("A = ");
                BI_Show(a, 16);
                printf("B = ");
                BI_Show(b, 16);
                printf("A - B == ");
                BI_Show(*c, 16);
                
                return;// memory leackege X
            }
        }
    }

    else // A,B 부호가 다를 때 때
    {
        if (a->sign == 0)
        {
            Flip_Sign(b);
            ADD(c, &a, &b); //수정하면 바꿀 곳
            Flip_Sign(b);
            printf("A = ");
            BI_Show(a, 16);
            printf("B = ");
            BI_Show(b, 16);
            printf("A - B == ");
            BI_Show(*c, 16);
            
            return;
        }
        else
        {
            Flip_Sign(a);
            ADD(c, &a, &b); 
            
            Flip_Sign(a); // 부호 원위치
            Flip_Sign(*c); // 부호 원위치
            printf("A = ");
            BI_Show(a, 16);
            printf("B = ");
            BI_Show(b, 16);
            
            printf("A - B == ");
            BI_Show(*c, 16);
            //BI_Delete(&d);
            
            return;
        }
    }
}

bigint* SUBC_BI(int* borrow, bigint** c, bigint** a, bigint** b)
{
    int len, i = 0;
    bigint* temp = NULL; // a와 b의 길이가 다를 때 -> bigint** b의 길이를 바꿀 수 없으므로 temp를 만들어줌

    Get_Word_Length(&len, a); // b보다 큰 a의 길이를 구하자
    BI_New(&temp, len);  // a의 워드 길이와 같게 temp 를 생성
    for (i = 0; i < (*b)->wordlen; i++)
        temp->a[i] = (*b)->a[i]; // b와 같은 값을 가지고 있어야하고, 더 길게 생성됐을 때는 0이 들어가있어야함.
    // a가 b보다 길 때 b의 길이를 맞춰줘야하는데 b를 건들이면 b가 바뀌기 때문에 temp를 이용

    for (i = 0; i < len; i++)
    {
        if (i == 0)
            *borrow = 0;

        (*c)->a[i] = (*a)->a[i] - (*borrow); // A - b의 값을 C 에 대입
        (*c)->a[i] = (*c)->a[i] & ((1 << WORD_BIT_LEN) - 1); // mod 2 ^ (WORD_BIT_LEN)
        if ((*a)->a[i] < *borrow) // borrow 될 때
            *borrow = 1;
        else // borrow 안될 때
        {
            *borrow = 0;
            if ((*c)->a[i] < temp->a[i])
                *borrow = *borrow + 1;
        }
        (*c)->a[i] -= temp->a[i]; // temp에 넣어놓은 b와 뺄셈 연산
        (*c)->a[i] = (*c)->a[i] & ((1 << WORD_BIT_LEN) - 1); // mod 2 ^ (WORD_BIT_LEN)
    }
    BI_Delete(&temp);
}
/*
bigint* SUBC_BI_test(int* borrow, bigint** c, bigint** a, bigint** b) // 길이가 다를 때를 대비해서 수정해야함
{
    int len, i, j = 0;
    bigint* temp = NULL; // a와 b의 길이가 다를 때 -> bigint** b의 길이를 바꿀 수 없으므로 temp를 만들어줌

    bi_refine(*a);
    bi_refine(*b);
    Get_Word_Length(&len, a); // b보다 큰 a의 길이를 구하자
    Get_Word_Length(&i, b); //
    if (len > i)
    {
        BI_New(&temp, len);  // a의 워드 길이와 같게 temp 를 생성
        for (j = 0; j < i; j++)
            temp->a[j] = (*b)->a[j];
        for (j = len; j > i; j--)
            temp->a[j - 1] = 0;
    }
    else
    {
        BI_New(&temp, i);   // b의 워드 길이와 같게 temp 를 생성
        for (j = 0; j < len; j++)
            temp->a[j] = (*a)->a[j];
        for (j = i; j > len; j--)
            temp->a[j - 1] = 0;
        len = i; // 밑에 for구문에서 len으로 볼거여서
    }

    for (i = 0; i < len; i++)
    {
        if (i == 0)
            *borrow = 0;

        (*c)->a[i] = (*a)->a[i] - (*borrow); // A - b의 값을 C 에 대입
        (*c)->a[i] = (*c)->a[i] & ((1 << WORD_BIT_LEN) - 1); // mod 2 ^ (WORD_BIT_LEN)
        if ((*a)->a[i] < *borrow) // borrow 될 때
            *borrow = 1;
        else // borrow 안될 때
        {
            *borrow = 0;
            if ((*c)->a[i] < temp->a[i])
                *borrow = *borrow + 1;
        }
        (*c)->a[i] -= temp->a[i]; // temp에 넣어놓은 b와 뺄셈 연산
        (*c)->a[i] = (*c)->a[i] & ((1 << WORD_BIT_LEN) - 1); // mod 2 ^ (WORD_BIT_LEN)
    }
    //BI_Delete(&temp);
}
*/
void MUL_Test(word* c, word* a, word* b)
{
    int len = 0, i = 0, carry0 = 0, carry1 = 0;
    
    word sum1 = 0, sum2 = 0, mul0 = 0, mul1 = 0, a1, b1, a0 = 0, b0 = 0;

    a1 = ((*a) >> (WORD_BIT_LEN >> 1));
    b1 = ((*b) >> (WORD_BIT_LEN >> 1));
    a0 = (*a) & (word)((1 << (WORD_BIT_LEN >> 1)) - 1);
    b0 = (*b) & (word)((1 << (WORD_BIT_LEN >> 1)) - 1);
    mul1 = a1 * b1;
    mul0 = a0 * b0;
    sum1 = a1 * b0;
    sum1 += a0 * b1;
    if (sum1 < a0 * b1)
        carry1 += 1;
    sum2 = (sum1 & (word)((1 << (WORD_BIT_LEN >> 1)) - 1));
    sum2 = sum2 << (WORD_BIT_LEN >> 1); //sum1의 뒷부분
    sum1 = sum1 >> (WORD_BIT_LEN >> 1); // sum1의 앞부분
    mul0 = sum2 + mul0;
    if (mul0 < sum2)
        carry0 += 1;
    mul1 = sum1 + mul1 + carry0 + (carry1 << (WORD_BIT_LEN >> 1));
    
    *c = mul0;
    *(c + 1) = mul1;
}

void MUL_BI(bigint** a, bigint** b)
{
    int len = 0, i = 0, carry0 = 0, carry1 = 0;
    bigint* c = NULL;
    word sum1 = 0, sum2 = 0, mul0 = 0, mul1 = 0, a1, b1, a0 = 0, b0 = 0;

    Get_Word_Length(&len, a);
    Get_Word_Length(&i, b);
    len += i; // 곱셈 연산한 C의 length
    BI_New(&c, len);
    a1 = ((*a)->a[0] >> (WORD_BIT_LEN >> 1));
    b1 = ((*b)->a[0] >> (WORD_BIT_LEN >> 1));
    a0 = (*a)->a[0] & (word)((1 << (WORD_BIT_LEN >> 1)) - 1);
    b0 = (*b)->a[0] & (word)((1 << (WORD_BIT_LEN >> 1)) - 1);
    mul1 = a1 * b1; 
    mul0 = a0 * b0;
    sum1 = a1 * b0;
    sum1 += a0 * b1;
    if (sum1 < a0 * b1)
        carry1 += 1;
    sum2 = (sum1 & (word)((1 << (WORD_BIT_LEN >> 1)) - 1));
    sum2 = sum2 << (WORD_BIT_LEN >> 1); //sum1의 뒷부분
    sum1 = sum1 >> (WORD_BIT_LEN >> 1); // sum1의 앞부분
    mul0 = sum2 + mul0;
    if (mul0 < sum2)
        carry0 += 1;
    mul1 = sum1 + mul1 + carry0 + (carry1 << (WORD_BIT_LEN >> 1));
    c->a[1] = mul1;
    c->a[0] = mul0;

    BI_Show(c, 16);
    BI_Delete(&c);
}

void MUL_MUL(bigint** result, bigint* a, bigint* b)
{
    int i, j, len = 0;
    //bigint* c = NULL;
    bigint* d = NULL;
    //bigint* e = NULL;

    Get_Word_Length(&len, &a);
    Get_Word_Length(&i, &b);
    len += i; // 곱셈 연산한 C의 length
    //BI_New(&c, len); // 최종 곱셈 연산된 big integer c
    BI_New(&d, len); // 단일 워드 곱셈 연산의 결과를 저장할 big integer d
    
    //Assign_BI(result, c);
    for (i = 0; i < a->wordlen; i++)
    {
        for (j = 0; j < b->wordlen; j++)
        {
            MUL_Test(&d->a[i + j], &a->a[j], &b->a[i]);
            ADD(result, result, &d); //ADD_BI_test(result, &c, &d); 이상함
            d->a[i + j] = 0;
            d->a[i + j + 1] = 0;
        }
    }
    //Assign_BI(result, c);
    //BI_Delete(&e);
    //BI_Delete(&c);
    //BI_Delete(&d);
    printf("A = ");
    BI_Show(a, 16);
    printf("B = ");
    BI_Show(b, 16);
    (*result)->sign = a->sign ^ b->sign;
    printf("A * B == ");
    bi_refine(*result);
    BI_Show(*result, 16);
}

void Kara(bigint** result, bigint* a, bigint* b)
{
    int i, len = 0;
    int j = 0;
    bigint* a1 = NULL;
    bigint* a0 = NULL;
    bigint* a1b1 = NULL;
    bigint* aabb = NULL;
    bigint* a0b0 = NULL;
    
    bigint* b1 = NULL;
    bigint* b0 = NULL;

    i = a->wordlen;
    len = b->wordlen;

    len = ((i > len) ? i : len); // if len is 12
    BI_New(result, len + 1); // (덧셈 연산을 통해 늘어날 수도 있으므로 + 1해서) 13짜리 result 생성
    BI_New(&a0b0, len); // 12짜리 bigint a0b0 생성 (A0 * B0)을 담을 bigint
    BI_New(&aabb, len); // 12짜리 bigint aabb 생성 (A0 - A1) * (B1 - B0)을 담을 bigint
    BI_New(&a1b1, len); // 12짜리 bigint a1b1 생성 (A1 * B1)을 담을 bigint
    
    len = (len + 1) >> 1; // left shift --> 6
    i = 0; // i 초기화

    BI_New(&a1, len); // length 6th bigint a1 생성 / A1을 담을 bigint
    BI_New(&a0, len); // length 6th bigint a0 생성 / A0을 담을 bigint
    BI_New(&b1, len); // length 6th bigint b1 생성 / B1을 담을 bigint
    BI_New(&b0, len); // length 6th bigint b0 생성 / B0을 담을 bigint

    for (j = 0; j < i; j++)
    {
        a0->a[j] = a->a[j]; 
        b0->a[j] = b->a[j];
    }
    for (j = i; j < len; j++)
    {
        a0->a[j] = a->a[j];
        b0->a[j] = b->a[j];
    }
    MUL_MUL(&a0b0, a0, b0);
   
}
