[Readme.txt]

 �� ������ ������, �̼����� �丮(ũ��'��' + ���̺귯'��') ���� �ۼ��� Readme.txt �����̴�.
main.c �� �� �Լ����� �׽�Ʈ�� ������ ������

//bigint.h
#define NEGATIVE : Big_Integer�� ��ȣ�� ������ �� 
#define NON_NEGATIVE : Big_Integer�� ��ȣ�� ����� �� 
#define ERROR : ���� �߻� �� ��ȯ��
#define WORD_BIT_LEN : ���� ������ ���� ��(8, 32, 64-bit)

/*-----------------------------------------------------------------------------------------------------*/

// Chapter 2.1
void BI_New() // Create Big_Integer
      input : bigint** x, int wordlen
      1st arg : ���� ������ Big_Integer
      2nd arg : ���� ������ Big_Integer�� ����
[���� ����]
1. �����ϰ��� �ϴ� bigint�� ������ x�� nullptr�� �ƴ� �� BI_Delete�� �̿��� �����Ѵ�.
2. ���忭�� ���̿� wordlen�� ������ �ش�.
3. ���忭�� ��ȣ�� NON_NEGATIVE�� ������ �ش�.
4. ���忭�� ���̿� ���� ���忭�� ���� �޸� �Ҵ��� �ش�.

void BI_Delete(bigint** x) // Delete Big_Integer
      input : bigint** x
      1st arg: ������ Big_Integer�� ���� ������
[���� ����]
1. bigint ����ü�� ����Ű�� �����Ͱ� NULL�̸� return �� �ش�.
2. ������ �� �ʱ�ȭ�� �ش�.
3. bigint ����ü ������ ������ �����͸� free �� �ش�.
4. bigint ����ü �����͸� free �� �ش�.
5. free �� �� ����ü �����͸� NULL�� �ٲ� �ش�.

void array_init() // Zerorize
   input : word* a, int len
   1st : ������ ���� ����Ű�� word�� ������
   2nd : word�� ����
[���� ����]
1. ����Ű�� �ִ� ������ ���� 0���� memset�Ѵ�

/*-----------------------------------------------------------------------------------------------------*/

// Chapter 2.2
void BI_Set_By_Array()  // Set Array -> Big_Integer
   input : bigint** x, int sign, word* a, int wordlen
   1st arg : �迭�� ������ Big_Integer�� ���� ������
   2nd arg : ������ �迭�� ��ȣ
   3rd arg : ������ �迭�� ������
   4th arg : ������ �迭�� ũ��
[���ۼ���]
1. �迭�� ��ȣ �� ���̸� Big_Integer ����ü ������ sign, wordlen ������ �����Ѵ�.
2. wordlen ���̸�ŭ �Ѱ� ���� word* a�� �迭 ���� Big_Integer ����ü ���忭�� �����Ѵ�.


void BI_Set_By_String() // Set String -> Big_Integer
   input : bigint** x, int sign, char* str, word base, int size
   1st arg : ���ڿ��� ��ȯ�� ���� ������ Big_Integer�� ���� ������
   2nd arg : ���� ��ȣ
   3rd arg : �Է¹��� ���ڿ��� ������
   4th arg : ���ڿ��� ��ȯ�� ���� (ex. 2, 10, 16) // 10������ ���Ŀ� �����ϵ��� �ϰڽ��ϴ�!
   5th arg : ���ڿ��� ����
[���ۼ���]
1. �Է¹��� ���ڿ��� hex ������ ��ȯ�� �ش�.
2. base = 2) ���ڿ��� �����ʺ��� word ������ �߶� ���� �迭���� shift�� �̿��Ͽ� 2������ ��ȯ�� ���� �ִ´�.
   ex) word: 8 - "110110101010101111"�� �Է¹޾��� �� 3 byte�� �迭�� a[0] = 10101111(af), a[1] = 01101010(6a), a[2] = 11(03)�� ������
   base = 16) ���ڿ��� �����ʺ��� word ������ �߶� ���� �迭���� shift�� �̿��Ͽ� 16������ ��ȯ�� ���� �ִ´�.
   ex) word: 8 - "123456789" -> {0x89, 0x67, 0x45, 0x23, 0x01}


void Ascii_To_Hex() // Ascii -> Hex
   input : char* str, char *hex
   1st arg : Ascii ���·� �Ǿ� �ִ� ���ڿ��� ����Ű�� ������
   2nd arg : Hex ���·� �ٲ� ���ڸ� ����Ű�� ������
[���ۼ���]
1. �Է¹��� �� 0~9������ ���ڸ� -0x30, A~F������ ���ڸ� -0x31�� �� �ش�.


void BI_Show() // Show Big_Integer
   input : bigint* x, int base
   1st arg : ������ Big_Integer�� �ּҰ�
   2nd arg : � ������ ����ؼ� ���� �������� ���� ��(2, 10, 16����)
[���� ����]
1. �Լ� ���ο����� �� ��° �Ű������� ���� ���ǹ��� �̿��� 2, 10, 16������ ���� ����ϵ��� �ۼ�
2. 2����) ���� for���� �̿��Ͽ� �ֻ��� ���忭���� ������ ���忭������ �ֻ��� ��Ʈ���� ������ ��Ʈ���� �˻�.
   �� �� *����ŷ�� �̿��� �� ��Ʈ�� 1�� �� �ִ���, 0�� �� �ִ��� �Ǻ��ϰ� �ٷ� ���.
3. 10����) ���忭(A[0], A[1], ..)�� 10������ ǥ���ϰ�, �̸� A[0] + A[1]*2^WORD_BIT_LEN + ... �� ǥ��.
4. 16����) 16������ ��쿡, 2������ ���������� ����ŷ�� �̿��� �ֻ��� ���忭���� ������ ���忭���� �Ǻ�.
   ������ �� ����Ʈ�� ����ŷ�� ���ϰ� ���.
   ex) bigint* a1�� a1->a[0] = 250 a1->a[1] = 2�� �������� ��
   BI_Show(a1, 16) ȣ��� 0x02fa�� ��µȴ�.
(WORD_BIT_LEN�� ���� ����ŷ �۾��� �޶� ���ǹ��� ����� �ٸ��� ���.)

/*-----------------------------------------------------------------------------------------------------*/

// Chapter 2.3
void bi_refine() // re define Big_Integer
   input : bigint* x
   1st arg : �������� �� Big_Integer�� ������
[���ۼ���]
1. Big_Integer�� �ֻ��� ���忭�� 0���� Ȯ���Ѵ�.
2. 1������ �ֻ��� ���忭�� 0�� ���, �ֻ��� ���忭���� 0���� ä���� ���忭�� ũ�⸸ŭ �ٿ� ���忭�� ũ�⸦ �缳���� �ش�.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.4
void Assign_BI() // Assign Big_Integer
   input : bigint** y, bigint* x
   1st arg : 2nd arg�� Big_Integer�� ���� ����� Big_Integer�� ���� ������
   2nd arg : ������ Big_Integer�� ������
[���� ����]
1. ���� �Ҵ��ϰ��� �ϴ� Big_Integer�� ������ ��, BI_Delete()�� �̿��� �����Ѵ�.
2. x�� ���忭 ���̸�ŭ�� Big_Integer�� BI_New()�� �̿��� �����Ѵ�.
3. x�� ��ȣ�� y�� �����Ѵ�.
4. �ݺ����� ����� x�� ���忭���� ���� ���� ���� ������ ������� �����Ѵ�.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.5
void bi_gen_rand() // generate random number
   input : bigint** x, int sign, int wordlen
   1st arg : random number�� ������ Big_Integer�� ���� ������
   2nd arg : random number�� ��ȣ
   3rd arg : random number�� ���� ����
[���ۼ���]
1. Big_Integer ����ü�� �����Ѵ�.
2. sign �� ������ ��, array_rand �Լ��� ���� ����ü�� ���忭�� random number�� �����Ѵ�.
3. bi_refine �Լ��� �̿��Ͽ� 0���� �Ǿ� �ִ� �ֻ��� ���忭�� �߶󳽴�.


void array_rand() // set random number
   input : word* dst, int wordlen
   1st arg : random number�� ���� ���忭�� ������
   2nd arg : random number�� ���� ����
[���ۼ���]
1. rand() �Լ��� 15 bit ��°��� �����Ƿ� �����Ͱ� ���� ������ �ƴ� ����Ʈ ������ �Ѿ���� ĳ������ �ش�.
2. rand() �Լ��� �̿��Ͽ� cnt(word length * sizeof(word))��ŭ random number�� �����Ͽ� ���忭�� �����Ѵ�.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.6
void Get_Word_Length() // Get Word Length of Big_Integer
   input : int *len, bigint* x
   output : (*x)->wordlen
   1st arg : ���忭�� ���̸� ����Ű�� int�� ������
   2nd arg : ���� ���̸� �� bigint�� ������
[���� ����]
1. x->wordlen�� *len�� �����Ѵ�.


void Bit_Length() // Get Bit Length of Big_Integer
   input : int *len, bigint* x
   1st arg : ���忭�� ��Ʈ ���̸� ����Ű�� int�� ������
   2nd arg : ��Ʈ ���̸� �� bigint�� ������
[���� ����]
1. �ֻ��� ���带 ������ ���忭�� ���̸� i�� ���� (�ֻ��� ���忡�� �ֻ��� ��Ʈ�� �� 1�� �ƴϹǷ�)
2. i�� ��Ʈ ������ ǥ���ϱ� ���� WORD_BIT_LEN�� �����ش�.
3. �ֻ��� ������ �ֻ��� ��Ʈ���� ������ ��Ʈ���� ó������ 1�� ���� ���� Ȯ��(����ŷ�� �̿�)
4. 1�� ���� ��Ʈ�� ���̿� i�� ���� Big_Int�� ��Ʈ ���̸� �˾Ƴ���.


void j_th_Bit_of_BI() // j-th Bit of Big_Integer
   input : int j, bigint* x
   output : 0 or 1
   1st arg : �Է��� ���� �˾Ƴ��� ���� bigint�� ������ x�� j��° ��Ʈ
   2nd arg : ������� bigint�� ������
[���� ����]
1. j ��° ��Ʈ�� ��ġ�� ���忭���� ũ�ٸ� return ���ش�.
2. j ��° ��Ʈ�� ���忭�ȿ� ���Ѵٸ�, ���� �� �� �� ��° ������ ��Ʈ���� Ȯ���Ѵ�. ex) 2��° ������ �ֻ��� ��Ʈ
3. �ش��ϴ� ������ ��Ʈ�� ���� Ȯ���ϱ� ����(����ŷ �۾�) ���ο� ������ �����Ѵ�.
4. ���ο� ������ �ش� ��Ʈ���� 1�� ä�������� 1�� shift ����ó���Ѵ�. ex)  2���� ������ �ֻ��� ��Ʈ�� ����� ���� 1<<WORD_BIT_LEN
4. �� �� �� ���� ���� ������ 1, �ٸ��� 0�� ����ϵ��� �Ѵ�.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.7
int Get_Sign() // Get Sign
   input : bigint* x
   output : NON_NEGATIVE / NEGATIVE / ERROR
   1st arg : ��ȣ�� �˾� �� Big_Integer�� ������
[���� ����]
1. Big_Integer�� ����ü�� sign ����� �̿��Ͽ� ��ȣ�� �Ǻ��Ѵ�.


void Flip_Sign() // Flip Sign
   input : bigint* x
   1st arg : ��ȣ�� ������ Big_Integer�� ������
[���� ����]
1. Big_Integer�� ����ü�� sign ����� �ٲ� �ش�.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.8
void BI_Set_One (); // Set Big_Integer 1
   input : bigint** x
   1st arg: 1�� ������ Big_Integer�� ���� ������
[���� ����]
1. ���忭�� ���̰� 1�� ���忭�� �����Ѵ�.
2. ��ȣ�� ��Ÿ���� sign�� NON_NEGATIVE �����Ѵ�.
3. ���忡 1 �����Ѵ�.


void BI_Set_Zero(); //Set Big_Integer 0
   input : bigint** x
   1st arg : 0���� ������ Big_Integer�� ���� ������
[���� ����]
1. ���忭�� ���̰� 1�� ���忭�� �����Ѵ�.
2. ��ȣ�� ��Ÿ���� sign�� NON_NEGATIVE �����Ѵ�.
3. ���忡 0 �����Ѵ�.


int Is_One() // Determine if one or not
   input : bigint** x
   output : -1(FALSE) / 0(TRUE)
   1st arg : 1���� �ƴ��� �Ǵ��� Big_Integer�� ���� ������
[���� ����]
1. sign�� �����̰ų� ������ ���忭�� 1�� �ƴϸ� False�� return �Ѵ�.
2. �ֻ��� ���忭���� 1��° ���忭���� ���� 0���� Ȯ���Ѵ�.


int Is_Zero() // Determine if zero or not
   input : bigint** x
   output : -1(FALSE) / 0(TRUE)
   1st arg : 0���� �ƴ��� �Ǵ��� Big_Integer�� ���� ������
[���� ����]
1. sign�� �����̰ų� ������ ���忭�� 0�� �ƴϸ� False�� return �Ѵ�.
2. �ֻ��� ���忭���� 1��° ���忭���� ���� 0���� Ȯ���Ѵ�.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.9
int Compare_BI(); //Compare Big_Integer
   input : bigint**x, bigint** y
   output : 1(1st argument > 2nd argumetn), 0(1st argument = 2nd argument), -1(1st argument < 2nd argument)
   1st arg : 2nd arg�� �� ����� Big_Integer�� ���� ������
   2nd arg : 1st arg�� �� ����� Big_Integer�� ���� ������
[���� ����]
1. *x�� *y�� sign ������ ��ȣ�� ���Ѵ�.
2. ��ȣ�� ���� ���� �� ���� ���忭�� ���̸� ���� ���Ѵ�.
3. ���忭�� ���̸��� ���� ���, �ֻ��� ���忭���� ������ ���忭���� ���� ���Ѵ�.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.10
void Left_Shift()
   input : bigint* x, int len
   1st arg : shift ��ų Big_Integer�� ������
   2nd arg : shift ��ų ��Ʈ�� ����
[���� ����]
1. �Է¹��� Big_Integer�� �����Ѵ�.
2. �߰������� �ʿ��� ���忭�� ũ�⸦ ����Ͽ� ���忭�� ũ�⸦ realloc���� ���Ҵ��� ��, �þ �迭�� 0���� �ʱ�ȭ��Ų��.
3. shift ��ų ��Ʈ�� ���̰� WORD_LEN_BIT�� ������� �ƴ��� �Ǵ��Ͽ� shift�� �� �ش�.
4. �ֻ��� ��Ʈ�� 0���� �Ǿ� �ִ� ��츦 ����Ͽ� bi_refine �Լ��� �迭�� �缳���� �ش�.


void Right_Shift()
   input : bigint* x, int len
   1st arg : shift ��ų Big_Integer�� ������
   2nd arg : shift ��ų ��Ʈ�� ����
[���� ����]
1. shift ��ų ��Ʈ�� ���̰� wn(WORD_BIT_LEN * wordlen)���� ū�� ������ ���Ѵ�.
2. shift ��ų ��Ʈ�� ���̰� wn���� ū ���, ���� 0���� �ٲ� �ش�.
3. shift ��ų ��Ʈ�� ���̰� wn���� ���� ���, WORD_LEN_BIT�� ������� �ƴ��� �Ǵ��Ͽ� shift�� �� �ش�.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.11
void Reduction_BI() // Reduction Integer
   input : bigint **x, int r
   1st arg : reduction ����� Big_Integer�� ���� ������
   2nd arg : �������� �ϴ� ��
[���� ����]
1. mod ���� �ϰ��� �ϴ� ����(r)�� ���忭���� ��ų� ������ return  ���ش�.
2. mod ���� �ϰ��� �ϴ� ����(r)�� ���忭���� ª���鼭 WORD_BIT_LEN�� ����� ��,
������ ������ ���� ���忭�� �״�� �ΰ�, ū ���忭�� 0���� �ʱ�ȭ�����ش�.
3. ��� �ƴ� ���, ������ ������ ���� ���忭�� �״�� �ΰ�, �� ���� ���� ���� ���� mod ������ ���ش�.
�� ���� ���� ���忭�� �״�� �д�.

/*-----------------------------------------------------------------------------------------------------*/

// Chapter 3 Addition

int ADD_ABc() // ĳ���� ������ ���� ����
   input : bigint** A, bigint** B, bigint** C, int c, int i
   output : int carry
   1st arg : addition�� ������ Big_Integer�� ���� ������
   2nd arg : addition�� ������ Big_Integer�� ���� ������
   3rd arg : ���� �ڸ����� carry
   4th arg : addition�� ������ �� �迭�� index
[���� ����]
1. A ����ü ���� �迭�� i��°�� B ����ü ���� �迭�� i��°�� ���� �ش�.
2. 1������ ����� ���� A ����ä ���� �迭�� i��° ������ ������ ���Ͽ� carry ���θ� �Ǵ��� �ش�.


bigint* ADDC() // WordLen(A) >= WordLen(B)
   input : bigint** A, bigint** B, int sign
   output : bigint* C (= A + B)
   1st arg : addition�� ������ Big_Integer�� ���� ������
   2nd arg : addition�� ������ Big_Integer�� ���� ������
   3rd arg : addition�� ������ ����� Big_Integer�� ��ȣ
[���� ����]
1. addition ������� ������ �� (WordLen(A) + 1)��ŭ�� Big_Integer�� ������ �ش�.
2. 0���� WordLen(A) - 1 ��°���� A, B ������ �迭�� ���� ������ �̿��Ͽ� ���� �ش�.
3. 1������ ������ Big_Integer�� input�� sign�� �־� �ش�.


bigint* ADD()
   input : bigint** A, bigint** B
   output : bigint* C (= A + B)
   1st arg : addition�� ������ Big_Integer�� ���� ������
   2nd arg : addition�� ������ Big_Integer�� ���� ������
[���� ����]
1. A�� 0�̸� A + B = B, B�� 0�̸� A + B=A�� ��ȯ�� �ش�.
2. A�� ����̰�, B�� �����̸� B�� ������ ���� �� A - B�� �����Ѵ�.
3. A�� �����̰�, B�� ����̸� A�� ������ ���� �� B - A�� �����Ѵ�.
4. A, B�� ������ ��ȣ�� ���, ADDC() �Լ��� ������ �ش�.