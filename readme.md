
# __Tory__ (cryp<u>to</u> libra<u>ry</u>)
Advanced Application Programming : Tory(cryp<u>to</u> libra<u>ry</u>)
## Se-yoon Lee, Seo-Woo Jung

## The library to implemented 'Big integer function' which used to 'Public-key cryptography'.


### Table of Contents:

0. Development environmnet
1. Description of Tory.
2. Getting Tory.
3. Compiling Tory.
4. Description of Tory's function.

---
### [0] Development environment

|OS | Version | Compiler _
|---|:---:|:---:|
|Window |10|Visual studio|
|Ubuntu |18.04|gcc|

---
### [1] Description of Tory.

파일 | 의미 
---|:---:
`main.c` | 빅넘버 연산 함수들을 테스트 해볼 수 있는 **테스트 벡터 생성** 및 **함수 연산 시간 측정** 
`bigint.h` | 빅넘버 관련 연산 함수의 헤더 파일
`bigint.c` | 빅넘버 관련 **연산 함수 구현**
`test.h` |빅넘버 관련 연산 함수들을 이용한 테스트 벡터 생성 및 시간 측정 함수의 헤더 파일
`test.c` | 빅넘버 관련 연산 함수들을 이용한 **테스트 벡터 생성** 및 **시간 측정 함수 구현**
`config.h` | 라이브러리의 특성을 반영한 헤더

---
### [2] Getting Tory.

1. Tory is available via Git at [link](https://github.com/Leeseyoon/Tory)

2. To obatin Tory enter into the your prefered folder and write the following syntax :

    ` $ git clone https://github.com/Leeseyoon/Tory.git `

---
### [3] Compiling Tory.

1. Use 'visual studio' Compiler
    1.  [2] Getting Tory를 참조해 library를 윈도우에 다운로드
    2.  main.c 열기
    3.  빅넘버 연산 함수를 이용해 test vector 생성을 원한다면, 해당 함수(...test())의 주석 제거 후 시작
    4.  빅넘버 연산 함수를 이용해 시간 측정을 원한다면, 해당 함수(...time())의 주석 제거 후 시작
2. Use gcc Compiler in linux
    1.  [2] Getting Tory를 참조해 library를 윈도우에 다운로드  
    2.  ` $ vi Tory `
    3.  test vector 생성 및 시간측정을 원한다면, ` $ gcc -o [ File name ] main.c bigint.c test.c config.h bigint.h test.h  `
    4.  사용자가 빅넘버 함수들을 이용해 코드를 짠 후 컴파일한다면, ` $ gcc -o [ File name ] main.c bigint.c config.h bigint.h `
    5.  ` $ ./[ File name ] ` 

---
### [4] Description of Tory's function.

- define
```c
#define NON_NEGATIVE    0 : Big_Integer의 부호가 양수일 때  
#define NEGATIVE        1 : Big_Integer의 부호가 음수일 때 
#define ERROR           -1: 오류 발생 시 반환값
#define SUCCESS         0 : 성공할 시 반환값
#define TRUE            0
#define FALSE           -1
// Multiplication flag
#define KARATSUBA       10 // multiplication()에서 KARATSUBA의 사용을 원할 경우, FLAG_MUL, FLAG_SQU에 값 대입
#define TEXTBOOK        11 // multiplication()에서 TEXTBOOK의 사용을 원할 경우, FLAG_MUL, FLAG_SQU에 값 대입
// Division flag
#define BINARY_LONG     12 // division()에서 Binary_Long_Division Alg의 사용을 원할 경우, FLAG_DIV에 값 대입
#define MULTI_PRE       13 // division()에서 MULti_Long_Division Alg의 사용을 원할 경우, FLAG_DIV에 값 대입
// Modular exponentiation flag
#define LTOR            14 // modular_exponentiaion()에서 Left to Right Alg의 사용을 원할 경우, FLAG_EXP에 값 대입
#define RTOL            15 // modular_exponentiaion()에서 Right to Left Alg의 사용을 원할 경우, FLAG_EXP에 값 대입
#define MONTGOMERY      16 // modular_exponentiaion()에서 Mongtomery Alg의 사용을 원할 경우, FLAG_EXP에 값 대입
```
- 사용자가 테스트할 때 결정해야하는 값
```c
#define WORD_BIT_LEN : 워드 단위에 대한 값(8, 32, 64-bit)

#define TEST_COUNT      // Test 하고자하는 횟수 입력 ex) 100, 1000, ...
#define TEST_SIZ        // Big Integer의 크기 입력. ex) SIZE_1024, SIZE_2048

#define FLAG_MUL        // Multiplication flag: KARATSUBA, TEXTBOOK
#define FLAG_DIV        // Division flag: BINARY_LONG, MULTI_PRE
#define FLAG_SQU        // Squaring flag: KARATSUBA, TEXTBOOK
#define FLAG_EXP        // Modular exponentiation flag: LTOR, RTOL, MONTGOMERY
```
---
#### Chapter 2.1
##### Create Big Integer function
```c
int BI_New(bigint** x, int wordlen) // Create Big_Integer

    bigint** x : 새로 선언할 Big_Integer
    int wordlen : 새로 선언할 Big_Integer의 길이

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Delete Big_Integer function
```c
int BI_Delete(bigint** x) // Delete Big_Integer
      
    bigint** x : 삭제할 Big_Integer의 더블 포인터
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Zerorize function
```c
int array_init(word* a, int len) // Zerorize

    word* a : 워드의 값을 가리키는 word형 포인터
    int len : word의 길이

    return SUCCESS (성공 시) / ERROR(실패 시)
```
---
#### Chapter 2.2
##### Array --> Big integer
```c
int BI_Set_By_Array((bigint** x, int sign, word* a, int wordlen)  // Set Array -> Big_Integer
  
    bigint** x : 배열을 저장할 Big_Integer의 더블 포인터
    int sign : 저장할 배열의 부호
    word* a : 저장할 배열의 포인터
    int wordlen : 저장할 배열의 크기

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### String --> Big Integer
```c
int BI_Set_By_String(bigint** x, int sign, char* str, word base, int size) // Set String -> Big_Integer
   
    bigint** x : 문자열을 변환한 값을 저장할 Big_Integer의 더블 포인터
    int sign : 값의 부호
    char* str : 입력받은 문자열의 포인터
    word base : 문자열을 변환할 진수 (ex. 2, 10, 16) // 10진수는 추후에 구현하도록 하겠습니다!
    int size : 문자열의 길이

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Ascii --> Hex
```c
int Ascii_To_Hex(char* str, char* hex) // Ascii -> Hex
   
    char* str : Ascii 형태로 되어 있는 문자열을 가리키는 포인터
    char *hex : Hex 형태로 바뀐 숫자를 가리키는 포인터

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Show(Print) Big Integer
```c
int BI_Show(bigint* x, int base) // Show Big_Integer
    
    bigint* x : 선언한 Big_Integer의 주소값
    int base : 어떤 진수로 출력해서 보고 싶은지에 대한 값(2, 10, 16진수)

    return SUCCESS (성공 시) / ERROR(실패 시)
```
---
#### Chapter 2.3
##### Re define Big Integer function
```c
int bi_refine(bigint* x) // re define Big_Integer
  
    bigint* x : 재정의해 줄 Big_Integer의 포인터

    return SUCCESS (성공 시) / ERROR(실패 시)
```
---
#### Chpater 2.4
##### Assign Big Integer function
```c
int Assign_BI(bigint** y, bigint* x) // Assign Big_Integer
   
   bigint** y, bigint* x : 2nd arg의 Big_Integer와 같게 복사될 Big_Integer형 더블 포인터
   bigint** y, bigint* x : 복사할 Big_Integer형 포인터
   
   return SUCCESS (성공 시) / ERROR(실패 시)
```
---
#### Chpater 2.5
##### Generate Random number function
```c
int bi_gen_rand(bigint** x, int sign, int wordlen) // generate random number
   
    bigint** x, int sign, int wordlen : random number를 저장할 Big_Integer의 더블 포인터
    bigint** x, int sign, int wordlen : random number의 부호
    bigint** x, int sign, int wordlen : random number의 워드 길이
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Set Random Number function
```c
int array_rand(word* dst, int wordlen) // set random number
   
    word* dst : random number를 담을 워드열의 포인터
    int wordlen : random number의 워드 길이
  
    return SUCCESS (성공 시) / ERROR(실패 시)
```
---
#### Chpater 2.6
##### Get Word Length of Big Integer function
```c
int Get_Word_Length(int *len, bigint* x) // Get Word Length of Big_Integer
   
    int *len : 워드열의 길이를 가리키는 int형 포인터
    bigint* x : 워드 길이를 잴 bigint형 포인터

    return (*x)->wordlen / ERROR(실패 시)
```
***
##### Get Bit Length of Big Integer function
```c
int Bit_Length(int *len, bigint* x) // Get Bit Length of Big_Integer
   
    int *len : 워드열의 비트 길이를 가리키는 int형 포인터
    bigint* x : 비트 길이를 잴 bigint형 포인터

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Find j-th Bit of Big Integer function
```c
int j_th_Bit_of_BI(int j, bigint* x) // j-th Bit of Big_Integer
   
    int j : 입력을 통해 알아내고 싶은 bigint형 포인터 x의 j번째 비트
    bigint* x : 조사당할 bigint형 포인터
   
    return 0 (j번째 bit가 0일 때) / 1 (j번째 bit가 1일 때)
```
---
#### Chpater 2.7
##### Get Sign function
```c
int Get_Sign(bigint* x) // Get Sign
   
    bigint* x : 부호를 알아 올 Big_Integer의 포인터
   
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Flip Sign function
```c
int Flip_Sign(bigint* x) // Flip Sign
   
    bigint* x: 부호를 뒤집을 Big_Integer의 포인터
  
    return SUCCESS (성공 시) / ERROR(실패 시)
```
---
#### Chpater 2.8
##### Set Big Integer function
```c
int BI_Set_One (bigint** x); // Set Big_Integer 1

    bigint** x: 1로 설정할 Big_Integer의 더블 포인터
   
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Set Big Integer --> 0
```c
int BI_Set_Zero(bigint** x); //Set Big_Integer 0

    bigint** x : 0으로 설정할 Big_Integer의 더블 포인터
   
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Is 1 ?
```c
int Is_One(bigint** x) // Determine if one or not

    bigint** x : 1인지 아닌지 판단할 Big_Integer의 더블 포인터
   
    return 0(TRUE) / -1(FALSE)
```
***
##### Is 0 ?
```c
int Is_Zero(bigint** x) // Determine if zero or not
   
    bigint** x : 0인지 아닌지 판단할 Big_Integer의 더블 포인터

    return 0(TRUE) / -1(FALSE)
```
---
#### Chpater 2.9
##### Compare Big Integer A with Big Integer B
```c
int Compare_BI(bigint**x, bigint** y); //Compare Big_Integer
  
    bigint** x : y 와 비교 대상인 Big_Integer의 더블 포인터
    bigint** y : x 와 비교 대상인 Big_Integer의 더블 포인터

    return 1(1st argument > 2nd argumetn), 0(1st argument = 2nd argument), 2(1st argument < 2nd argument) / ERROR(-1, 실패 시)
```
---
#### Chpater 2.10
##### Left Shift function
```c
int Left_Shift(bigint* x, int len)
   
    bigint* x : shift 시킬 Big_Integer의 포인터
    int len : shift 시킬 비트의 길이

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Right Shift function
```c
int Right_Shift(bigint* x, int len)
   
    bigint* x : shift 시킬 Big_Integer의 포인터
    int len : shift 시킬 비트의 길이

    return SUCCESS (성공 시) / ERROR(실패 시)
```
---
#### Chpater 2.11
##### Reduction Big Integer
```c
int Reduction_BI(bigint **x, int r) //Reduction Integer
   
    bigint **x : reduction 대상인 Big_Integer의 더블 포인터
    int r : 나누고자 하는 수

    return SUCCESS (성공 시) / ERROR(실패 시)
```
---
#### Chapter 3 Addition
##### Addition (A + B + carry) function
```c
unsigned int ADD_ABc(bigint** C, bigint** A, bigint** B, unsigned int c, int i)
    
    bigint** C : 덧셈 결과를 저장할 bigint 형 더블포인터 변수
    bigint** A : 덧셈을 수행할 bigint 형 더블포인터 변수
    bigint** B : 덧셈을 수행할 bigint 형 더블포인터 변수
    unsigned int c : 이전 연산에서 받아 온 캐리 (0 또는 1)
    int i : 덧셈을 수행할 배열의 index

    return carry;(캐리 여부 0 or 1)
```
***
##### Addition Core function (WordLen(A) >= WordLen(B), Sign(A) = Sign(B))
```c
int ADDC(bigint** C, bigint** A, bigint** B, int sign)
    
    bigint** C : 덧셈 결과를 저장할 bigint 형 더블포인터 변수
    bigint** A : 덧셈을 수행할 bigint 형 더블포인터 변수
    bigint** B : 덧셈을 수행할 bigint 형 더블포인터 변수
    Int sign : 덧셈 결과 C의 부호

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Addition Core function (WordLen(A) >= WordLen(B), Sign(A) = Sign(B), A = A + B)
```c
int ADDC_AAB(bigint** C, bigint** A, bigint** B, int sign)
    
    bigint** C : 덧셈 결과를 저장할 bigint 형 더블포인터 변수
    bigint** A : 덧셈을 수행할 bigint 형 더블포인터 변수
    bigint** B : 덧셈을 수행할 bigint 형 더블포인터 변수
    Int sign : 덧셈 결과 C의 부호

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Addition function 
```c
int ADD(bigint** C, bigint** A, bigint** B)
    
    bigint** C : 덧셈 결과를 저장할 bigint 형 더블포인터 변수
    bigint** A : 덧셈을 수행할 bigint 형 더블포인터 변수
    bigint** B : 덧셈을 수행할 bigint 형 더블포인터 변수

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Addition function (A = A + B)
```c
int ADD_AAB(bigint** C, bigint** A, bigint** B)
    
    bigint** C : 덧셈 결과를 저장할 bigint 형 더블포인터 변수
    bigint** A : 덧셈을 수행할 bigint 형 더블포인터 변수
    bigint** B : 덧셈을 수행할 bigint 형 더블포인터 변수

    return SUCCESS (성공 시) / ERROR(실패 시)
```
---
#### Chapter 4 Subtraction
##### Subtraction Core function
```c
int SUBC(bigint** C, bigint** A, bigint** B)

    unsigned int* borrow 단일 뺄셈 연산에서 return 되는 borrow에 해당하는 unsigned int 형 포인터 변수
    bigint** C : 단일 뺄셈 연산의(A - B) 결과를 저장할 bigint 형 더블 포인터 변수
    bigint** A : 단일 뺄셈 연산의(A - B) A에 해당하는 bigint 형 포인터 변수
    bigint** B : 단일 뺄셈 연산의(A - B) B에 해당하는 bigint 형 포인터 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Subtraction
```c
int SUB(bigint** C, bigint* A, bigint* B)
    
    bigint** C : 뺄셈 연산의(A - B) 결과를 저장할 bigint 형 더블 포인터 변수
    bigint** A : 뺄셈 연산의(A - B) A에 해당하는 bigint 형 포인터 변수
    bigint** B : 뺄셈 연산의(A - B) B에 해당하는 bigint 형 포인터 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
---
#### Chapter 5 Multiplication
##### Multiplication Word
```c
int MUL_Word(word* C, word* A, word* B)

    word* C : 단일 워드 곱셈 연산의 결과를 저장할 word 형 포인터 변수
    word* A : 단일 워드 곱셈 연산의 곱하는 수인 word 형 포인터 변수
    word* B : 단일 워드 곱셈 연산의 곱하는 수인 word 형 포인터 변수

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Multiplication Mutli Word
```c
int MUL_Word(word* C, word* A, word* B)
    
    bigint** C : 다중 워드 곱셈 연산의 결과를 저장할 bigint 형 더블 포인터 변수
    bigint* A : 다중 워드 곱셈 연산의 곱하는 수인 bigint 형 포인터 변수
    bigint* B : 다중 워드 곱셈 연산의 곱하는 수인 bigint 형 포인터 변수
    
    return SUCCESS 성공 시 /  ERROR(실패 시)
```
***
##### Multiplication Karatsuba
```c
int MUL_Karatsuba(bigint** C, bigint* A, bigint* B)

    bigint** C : 카라추바 곱셈 결과를 저장할 bigint 형 더블포인터 변수
    bigint** A : 카라추바 곱셈을 수행할 bigint 형 더블포인터 변수
    bigint** B : 카라추바 곱셈을 수행할 bigint 형 더블포인터 변수

    return SUCCESS (성공 시) / ERROR(실패 시)
```
---
#### Chapter 6 Division
##### Binary Long Division Algorithm
```c
int Binary_Long_Div(bigint** Q, bigint** R, bigint* A, bigint* B)

    bigint** Q : Binary Long Divsion 연산의 몫에 대한 결과를 저장할 bigint 형 더블 포인터 변수
    bigint* R : Binary Long Divsion 연산의 나머지에 대한 결과를 저장할 bigint 형 더블 포인터 변수
    bigint* A : Binary Long Divsion 연산의 나누려는 수인 bigint 형 포인터 변수
    bigint* B : Binary Long Divsion 연산의 나누는 수인 bigint 형 포인터 변수

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Multi Long Division Algorithm
```c
int DIV(bigint** Q, bigint** R, bigint* A, bigint* B)

    bigint** Q Multi Long Divsion 연산의 몫에 대한 결과를 저장할 bigint 형 더블 포인터 변수
    bigint* R Multi Long Divsion 연산의 나머지에 대한 결과를 저장할 bigint 형 더블 포인터 변수
    bigint* A Multi Long Divsion 연산의 나누려는 수인 bigint 형 포인터 변수
    bigint* B Multi Long Divsion 연산의 나누는 수인 bigint 형 포인터 변수

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Multi Long Division core
```c
int DIVC(bigint** Q, bigint** R, bigint* A, bigint* B)

    bigint** Q : DIV( )의 몫에 대한 결과를 저장할 bigint 형 더블 포인터 변수
    bigint* R : DIV( )의 나머지에 대한 결과를 저장할 bigint 형 더블 포인터 변수
    bigint* A : DIV( )의 나누려는 수인 bigint 형 포인터 변수
    bigint* B : DIV( )의 나누는 수인 bigint 형 포인터 변수

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Multi Long Division core's core
```c
int DIVCC(bigint** Q, bigint** R, bigint* A, bigint* B)

     bigint** Q : DIVC( )의 몫에 대한 결과를 저장할 bigint 형 더블 포인터 변수
     bigint* R : DIVC( )의 나머지에 대한 결과를 저장할 bigint 형 더블 포인터 변수
     bigint* A : DIVC( )의 나누려는 수인 bigint 형 포인터 변수
     bigint* B : DIVC( )의 나누는 수인 bigint 형 포인터 변수

     return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Multi Long Division core's core condition (n == m)
```c
int DIVCC_n_m(bigint** Q, bigint* A, bigint* B, int m)

    bigint** Q : DIVCC()에서 Q hat인 bigint 형 더블 포인터 변수
    bigint* A : DIVCC()에서 나눠지는 수인 A의 bigint 형 포인터 변수
    bigint* B : DIVCC()에서 나누려는 수인 B의 bigint 형 포인터 변수
    int m : DIVCC()에서 B의 wordlen을 나타내는 int형 변수

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Multi Long Division core's core condition (n == m + 1)
```c
int DIVCC_n_m1(bigint** Q, bigint* A, bigint* B, int m)

    bigint** Q : DIVCC()에서 Q hat인 bigint 형 더블 포인터 변수
    bigint* A : DIVCC()에서 나눠지는 수인 A의 bigint 형 포인터 변수
    bigint* B : DIVCC()에서 나누려는 수인 B의 bigint 형 포인터 변수
    int m : DIVCC()에서 B의 wordlen을 나타내는 int형 변수
    return SUCCESS (성공 시) / ERROR(실패 시)

```
===
#### Chapter 7 Squaring
##### Single Squaring
```c
int Single_Squaring(bigint* C, bigint* A)

    bigint* C : 제곱 연산을 수행한 결과를 저장할 bigint 형 포인터 변수
    bigint* A : 제곱 연산을 수행할 bigint 형 포인터 변수

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Squaring
```c
int SQU(bigint** C, bigint* A)

    bigint** C : 제곱 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
    bigint* A : 제곱 연산을 수행할 bigint 형 포인터 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Squaring Core function
```c
int SQUC(bigint** C, bigint* A)

    bigint** C : 제곱 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
    bigint* A : 제곱 연산을 수행할 bigint 형 포인터 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Squaring Core Karatsuba function
```c
int SQUC_Karatsuba(bigint** C, bigint* A)

    bigint** C : 제곱 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
    bigint* A : 제곱 연산을 수행할 bigint 형 포인터 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
===
#### Chapter 8 Modular Exponentiation
##### Montgomery ADD Exponentiation func
```c
int EXP_Montgomery_ADD(bigint** T, bigint* X, bigint* N)

    bigint** T : exponentiation 덧셈 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
    bigint* X : exponentiation 덧셈 연산을 수행할 bigint 형 포인터 변수
    bigint* N : exponentiation 덧셈 연산에서의 지수(덧셈할 횟수)를 가리키는 bigint 형 포인터 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Montgomery MUL Exponentiation func
```c
int EXP_Montgomery_MUL(bigint** T, bigint* X, bigint* N)

    bigint** T : 지수승 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
    bigint* X : 지수승 연산을 수행할 bigint 형 포인터 변수
    bigint* N : 지수승 연산에서의 지수를 가리키는 bigint 형 포인터 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Left to Right ADD Exponentiation func
```c
int EXP_LR_ADD(bigint** T, bigint* X, bigint* N)

    bigint** T : Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
    bigint* X : Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
    bigint* N : Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Left to Right MUL Exponentiation func
```c
int EXP_LR_MUL(bigint** T, bigint* X, bigint* N)

    bigint** T : Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
    bigint* X : Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
    bigint* N : Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Right to Left ADD Exponentiation func
```c
int EXP_RL_ADD(bigint** T, bigint* X, bigint* N)

    bigint** T : Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
    bigint* X : Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
    bigint* N : Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수

    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Right to Left MUL Exponentiation func
```c
int EXP_RL_MUL(bigint** T, bigint* X, bigint* N)

    bigint** T : Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
    bigint* X : Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
    bigint* N : Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Montgomery ADD Modular Exponentiation func
```c
int EXP_Montgomery_ADD(bigint** T, bigint* X, bigint* N)

    bigint** T : 모듈러 지수 덧셈 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
    bigint* X : 모듈러 지수 덧셈 연산을 수행할 bigint 형 포인터 변수
    bigint* N : 모듈러 지수 덧셈 연산에서의 지수(덧셈할 횟수)를 가리키는 bigint 형 포인터 변수
    bigint* M : 모듈러 지수 연산에서의 모듈러를 수행할 bigint 형 포인터 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Montgomery MUL Modular Exponentiation func
```c
int MOD_EXP_Montgomery_MUL(bigint** T, bigint* X, bigint* N, bigint* M)

    bigint** T : 모듈러 지수승 연산을 수행한 결과를 저장할 bigint 형 더블포인터 변수
    bigint* X : 모듈러 지수승 연산을 수행할 bigint 형 포인터 변수
    bigint* N : 모듈러 지수승 연산에서의 지수를 가리키는 bigint 형 포인터 변수
    bigint* M : 모듈러 지수승 연산에서의 모듈러를 수행할 bigint 형 포인터 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Left to Right ADD Modular Exponentiation func
```c
int MOD_EXP_LR_ADD(bigint** T, bigint* X, bigint* N, bigint* M)

    bigint** T : Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
    bigint* X : Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
    bigint* N : Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
    bigint* M : Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Left to Right MUL Modular Exponentiation func
```c
int MOD_EXP_LR_MUL(bigint** T, bigint* X, bigint* N, bigint* M)

    bigint** T : Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
    bigint* X : Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
    bigint* N : Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
    bigint* M : Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Right to Left ADD Modular Exponentiation func
```c
int MOD_EXP_RL_ADD(bigint** T, bigint* X, bigint* N, bigint* M)

    bigint** T : Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
    bigint* X : Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
    bigint* N : Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
    bigint* M : Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
***
##### Right to Left MUL Modular Exponentiation func
```c
int MOD_EXP_RL_MUL(bigint** T, bigint* X, bigint* N, bigint* M)

    bigint** T : Modular 지수 연산 결과에 해당하는 bigint 더블 포인터형 변수
    bigint* X : Modular 지수 연산에서 밑에 해당하는 bigint 포인터형 변수
    bigint* N : Modular 지수 연산에서 지수에 해당하는 bigint 포인터형 변수
    bigint* M : Modular 지수 연산에서 X^N 과 T를 합동해주는 bigint 포인터형 변수
    
    return SUCCESS (성공 시) / ERROR(실패 시)
```
---
