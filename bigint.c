#include "bigint.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define   zerorize

// Chpater 2.1
void array_init(word* a, int len)
{
    memset(a, 0, sizeof(word) * len);
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
    (*x)->wordlen = wordlen;
    (*x)->sign = NULL;
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
    hex = (char*)malloc(size * sizeof(word));
    Ascii_To_Hex(str, hex);

    if (base == 2) // 11 01101010 10101111 -> 3 byte �濡 a[2] a[1] a[0]
    {
        if ((strlen(str) % WORD_BIT_LEN) != 0)
        {
            for (int i = 0; i < (strlen(str) / WORD_BIT_LEN); i++)
            {
                for (int j = 0; j < WORD_BIT_LEN; j++)
                    (*x)->a[i] |= (hex[strlen(str) - 1 - WORD_BIT_LEN * i - j] << j);
            }

            for (int k = 0; k < (strlen(str) % WORD_BIT_LEN); k++)
                (*x)->a[(strlen(str) / WORD_BIT_LEN)] |= (hex[k] << ((strlen(str) % WORD_BIT_LEN) - k - 1));
        }

        else
        {
            for (int i = 0; i < (strlen(str) / WORD_BIT_LEN); i++)
            {
                for (int j = 0; j < WORD_BIT_LEN; j++)
                    (*x)->a[i] |= (hex[strlen(str) - 1 - WORD_BIT_LEN * i - j] << j);
            }
        }
    }


    else if (base == 10)
        printf("hello\n"); // ���Ŀ� ������ ��

    else if (base == 16) // "123456789" -> {0x89, 0x67, 0x45, 0x23, 0x01} / {0x6789, 0x2345, 0x0001}
    {
        if ((strlen(str)) % (WORD_BIT_LEN / 4) != 0) // 2���� 1 byte�ϱ�
        {
            for (int i = 0; i < (strlen(str) / (WORD_BIT_LEN / 4)); i++)
            {
                for (int j = 0; j < WORD_BIT_LEN / 4; j++)
                    (*x)->a[i] |= hex[strlen(str) - 1 - (WORD_BIT_LEN / 4) * i - j] << (4 * j);
            }

            for (int k = 0; k < (strlen(str) % (WORD_BIT_LEN / 4)); k++)
                (*x)->a[strlen(str) / (WORD_BIT_LEN / 4)] |= (hex[(strlen(str) % (WORD_BIT_LEN / 4)) - 1 + k] << (4 * k));
        }

        else
        {
            for (int i = 0; i < (strlen(str) / (WORD_BIT_LEN / 4)); i++)
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

    if (base == 10) // 10������ ��
    {
        printf("%d", x->a[0]);
        for (i = 1; i < x->wordlen; i++)
        {
            printf(" + %d * 2^%d", x->a[i], i * WORD_BIT_LEN);
        }
    }

    else if (base == 16) // 16������ ��
    {
        if (x->sign == 0)
            printf(" + ");
        else
            printf(" - ");
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

    else if (base == 2) // 2������ ��
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
        x->a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
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
    *len = (*x)->wordlen; // Big Integer x�� wordlen�� ����
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
    if ((*x)->sign < (*y)->sign) // A�� ���, B�� ������ �翬�� A�� ũ�Ƿ�
        return 1;
    else if ((*x)->sign > (*y)->sign) // A�� ����, B�� ����� �翬�� B�� ũ�Ƿ�
        return -1;
    else // A, B ��ȣ�� ���� ��
    {
        if ((*x)->sign > 0) // A, B ��ȣ�� ����� �� (��ȣ�� ���� �����Ƿ�, if ���� ���� �ϳ��� ��)
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
        else // A, B ��ȣ�� ������ ��
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

    bigint* cp = NULL;
    Assign_BI(&cp, x);

    wn = WORD_BIT_LEN * x->wordlen;

    if ((len % WORD_BIT_LEN) == 0)
        add_len = len / WORD_BIT_LEN;
    else
        add_len = (len / WORD_BIT_LEN) + 1;

    new_wordlen = add_len + x->wordlen;

    x->a = (word*)realloc(x->a, sizeof(word) * new_wordlen);
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
int ADD_ABc(bigint** A, bigint** B, bigint** C, int c, int i)
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
bigint* ADDC(bigint** A, bigint** B, int sign)
{
    int A_Len;
    int B_Len;
    int i;
    int carry;

    A_Len = (*A)->wordlen;
    B_Len = (*B)->wordlen;

    for (i = B_Len; i < A_Len; i++)
        (*B)->a[i] = 0;

    carry = 0;

    bigint* C = NULL;
    BI_New(&C, A_Len + 1);

    for (i = 0; i < A_Len; i++)
        carry = ADD_ABc(A, B, &C, carry, i);

    if (carry == 1)
        C->a[A_Len] = 1;
    else
        C->a[A_Len] = 0;

    bi_refine(C);

    if (sign == 0)
        C->sign = 0;
    else
        C->sign = 1;

    return C;
}

bigint* ADD(bigint** A, bigint** B)
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
        return *B;

    if (Is_Zero(B) == 0) // B is zero
        return *A;

    if ((A_sign == NON_NEGATIVE) && (B_sign == NEGATIVE))
    {
		bigint* temp = NULL;
		BI_New(&temp, B_Len);
		Assign_BI(&temp, *B);

        Flip_Sign(temp);
        return SUB_BI(A, &temp); // SUB �Լ� B�� ��ȣ�� �ٲ��ְ� ����ϸ� B�� ��ȣ�� �ٲ� ���·� ��������
    }

    if ((A_sign == NEGATIVE) && (B_sign == NON_NEGATIVE))
    {
		bigint* temp = NULL;
		BI_New(&temp, A_Len);
		Assign_BI(&temp, *A);

        Flip_Sign(temp);
        return SUB_BI(B, &temp); // SUB �Լ�
    }

    // A, B�� ������ ��ȣ�� ��
    if (A_Len >= B_Len)
        return ADDC(A, B, A_sign);
    else
        return ADDC(B, A, A_sign);
}

bigint* SUB_BI(bigint** a, bigint** b)
{
    int len1, len2 = 0;
    int borrow = 0;
    
    bigint* d = NULL; //
    bigint* temp = NULL;
    
    Get_Word_Length(&len1, a);
    Get_Word_Length(&len2, b);
    if (len1 >= len2)
        BI_New(&d, len1);
        
    else
        BI_New(&d, len2); // �����ؼ� ������ BI�� ���� ���ϱ�
        
    
    if (Is_Zero(a) == 0)
    {
        Assign_BI(&d, *b);
        Flip_Sign(d);
        return d;
    } // line 1~2

    if (Is_Zero(b) == 0)
    {
        Assign_BI(&d, *a);
        return d;
    } // line 4~5


    if (((*a)->sign ^ (*b)->sign) == 0) // A, B ��ȣ�� ���� ��
    {
        if (((*a)->sign & (*b)->sign) == 0) // A, B�� ��ȣ�� ��� ����� ��
        {
            if (Compare_BI(a, b) < 0) // A, B�� ���ؼ� A < B�� ��. Compare_BI(A, B)�� return : -1
            {
                SUBC_BI(&borrow, &d, b, a);
                Flip_Sign(d);
                return d;
            }
            else if (Compare_BI(a, b) == 0)
                return d; 
            else // A, B �� ���ؼ� A >= B�� ��. Compare_BI(A, B)'s return : 0, 1
            {
                SUBC_BI(&borrow, &d, a, b);
                return d;
            }
        }
        
        else // A, B�� ��ȣ�� ��� ������ ��
        {
            Flip_Sign(*a);
            Flip_Sign(*b);
            if (Compare_BI(a, b) < 0)
            {
                SUBC_BI(&borrow, &d, b, a);
                Flip_Sign(*a); // ��ȣ ����ġ
                Flip_Sign(*b); // ��ȣ ����ġ
                return d;
            }
            else
            {
                SUBC_BI(&borrow, &d, a, b);
                Flip_Sign(d);
                Flip_Sign(*a); // ��ȣ ����ġ
                Flip_Sign(*b); // ��ȣ ����ġ
                return d;
            }
        }
    }
    
    else // A,B ��ȣ�� �ٸ� �� ��
    {
        if ((*a)->sign == 0)
        {
            Flip_Sign(*b);
            d = ADD(a, b);
            Flip_Sign(*b);
            return d;
        }
        else
        {
            Flip_Sign(*a);
            d = ADD(a, b);
            Flip_Sign(d); // -ADD(|A|,B)
            Flip_Sign(*a); // ��ȣ ����ġ
            return d;
        }
    }
}

void SUBC_BI(int* borrow, bigint** c, bigint** a, bigint** b)
{
    int len, i = 0;
    bigint* temp = NULL; // a�� b�� ���̰� �ٸ� �� -> bigint** b�� ���̸� �ٲ� �� �����Ƿ� temp�� �������
    
    Get_Word_Length(&len, a); // b���� ū a�� ���̸� ������
    BI_New(&temp, len);  // a�� ���� ���̿� ���� temp �� ����
    for (i = 0; i < (*b)->wordlen; i++)
        temp->a[i] = (*b)->a[i]; // b�� ���� ���� ������ �־���ϰ�, �� ��� �������� ���� 0�� ���־����.
    // a�� b���� �� �� b�� ���̸� ��������ϴµ� b�� �ǵ��̸� b�� �ٲ�� ������ temp�� �̿�
        
    for (i = 0; i < len; i++) 
    {
        if (i == 0)
            *borrow = 0;

        (*c)->a[i] = (*a)->a[i] - (*borrow); // A - b�� ���� C �� ����
        (*c)->a[i] = (*c)->a[i] & ((1 << WORD_BIT_LEN) - 1); // mod 2 ^ (WORD_BIT_LEN)
        if ((*a)->a[i] < *borrow) // borrow �� ��
            *borrow = 1;
        else // borrow �ȵ� ��
        {
            *borrow = 0;
            if ((*c)->a[i] < temp->a[i])
                *borrow = *borrow + 1;
        }
        (*c)->a[i] -= temp->a[i]; // temp�� �־���� b�� ���� ����
        (*c)->a[i] = (*c)->a[i] & ((1 << WORD_BIT_LEN) - 1); // mod 2 ^ (WORD_BIT_LEN)
    }
}
