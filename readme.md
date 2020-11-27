
# TORY(cryp'TO' libar'RY') 


## The library to implemented 'Big integer function' which used to 'Public-key cryptography'.
## Se-yoon Lee, Seo-Woo Jung

### Table of Contents:

[0] Development environmnet
[1] Description of Tory.
[2] Getting Tory.
[3] Compiling Tory.
[4] Description of Tory's function.

─────────────────────────────────────────────────────

### [0] Development environment

< 1 >
    OS : Window 10
    Compiler : Visual studio

< 2 >
    OS : Ubuntu 18.04
    Compiler : gcc

─────────────────────────────────────────────────────

### [1] Description of Tory.

이 문서는 토리(크립'토' + 라이브러'리') 팀이 작성한 Readme.txt 파일이다.

- main.c --> 빅넘버 연산 함수들을 테스트 해볼 수 있는 테스트 벡터들을 생성
- bigint.h --> 빅넘버 관련 연산 함수들의 헤더 파일
- bigint.c --> 빅넘버 관련 연산 함수들이 구현

─────────────────────────────────────────────────────

### [2] Getting Tory.

1. Tory is available via Git at https://github.com/Leeseyoon/Tory .

2. To obatin Tory enter into the your prefered folder and write the following syntax :

    $ git clone https://github.com/Leeseyoon/Tory.git

─────────────────────────────────────────────────────

### [3] Compiling Tory.

1. Use 'visual studio' Compiler

2. Use gcc Compiler in linux

                    1st      2nd     3rd      4th     <-- arg
    $ gcc -o [filename] main.c bigint.c bigint.h

─────────────────────────────────────────────────────
### [4] Description of Tory's function.

In 'bigint.h'

#define NEGATIVE : Big_Integer의 부호가 음수일 때 
#define NON_NEGATIVE : Big_Integer의 부호가 양수일 때 
#define ERROR : 오류 발생 시 반환값
#define WORD_BIT_LEN : 워드 단위에 대한 값(8, 32, 64-bit)


/*-----------------------------------------------------------------------------------------------------*/

// Chapter 2.1
void BI_New() // Create Big_Integer
      input : bigint** x, int wordlen
      1st arg : 새로 선언할 Big_Integer
      2nd arg : 새로 선언할 Big_Integer의 길이
[동작 순서]
1. 생성하고자 하는 bigint형 포인터 x가 nullptr이 아닐 시 BI_Delete를 이용해 제거한다.
2. 워드열의 길이에 wordlen을 대입해 준다.
3. 워드열의 부호에 NON_NEGATIVE를 대입해 준다.
4. 워드열의 길이에 맞춰 워드열을 동적 메모리 할당해 준다.

void BI_Delete(bigint** x) // Delete Big_Integer
      input : bigint** x
      1st arg: 삭제할 Big_Integer의 더블 포인터
[동작 순서]
1. bigint 구조체를 가리키는 포인터가 NULL이면 return 해 준다.
2. 내부의 값 초기화해 준다.
3. bigint 구조체 내부의 워드의 포인터를 free 해 준다.
4. bigint 구조체 포인터를 free 해 준다.
5. free 해 준 구조체 포인터를 NULL로 바꿔 준다.

void array_init() // Zerorize
   input : word* a, int len
   1st : 워드의 값을 가리키는 word형 포인터
   2nd : word의 길이
[동작 순서]
1. 가리키고 있는 워드의 값을 0으로 memset한다

/*-----------------------------------------------------------------------------------------------------*/

// Chapter 2.2
void BI_Set_By_Array()  // Set Array -> Big_Integer
   input : bigint** x, int sign, word* a, int wordlen
   1st arg : 배열을 저장할 Big_Integer의 더블 포인터
   2nd arg : 저장할 배열의 부호
   3rd arg : 저장할 배열의 포인터
   4th arg : 저장할 배열의 크기
[동작순서]
1. 배열의 부호 및 길이를 Big_Integer 구조체 내부의 sign, wordlen 변수에 대입한다.
2. wordlen 길이만큼 넘겨 받은 word* a의 배열 값을 Big_Integer 구조체 워드열에 대입한다.


void BI_Set_By_String() // Set String -> Big_Integer
   input : bigint** x, int sign, char* str, word base, int size
   1st arg : 문자열을 변환한 값을 저장할 Big_Integer의 더블 포인터
   2nd arg : 값의 부호
   3rd arg : 입력받은 문자열의 포인터
   4th arg : 문자열을 변환할 진수 (ex. 2, 10, 16) // 10진수는 추후에 구현하도록 하겠습니다!
   5th arg : 문자열의 길이
[동작순서]
1. 입력받은 문자열을 hex 값으로 변환해 준다.
2. base = 2) 문자열의 오른쪽부터 word 단위로 잘라 하위 배열부터 shift를 이용하여 2진수로 변환한 값을 넣는다.
   ex) word: 8 - "110110101010101111"을 입력받았을 때 3 byte의 배열에 a[0] = 10101111(af), a[1] = 01101010(6a), a[2] = 11(03)이 들어가도록
   base = 16) 문자열의 오른쪽부터 word 단위로 잘라 하위 배열부터 shift를 이용하여 16진수로 변환한 값을 넣는다.
   ex) word: 8 - "123456789" -> {0x89, 0x67, 0x45, 0x23, 0x01}


void Ascii_To_Hex() // Ascii -> Hex
   input : char* str, char *hex
   1st arg : Ascii 형태로 되어 있는 문자열을 가리키는 포인터
   2nd arg : Hex 형태로 바뀐 숫자를 가리키는 포인터
[동작순서]
1. 입력받은 게 0~9까지의 숫자면 -0x30, A~F까지의 문자면 -0x31을 해 준다.


void BI_Show() // Show Big_Integer
   input : bigint* x, int base
   1st arg : 선언한 Big_Integer의 주소값
   2nd arg : 어떤 진수로 출력해서 보고 싶은지에 대한 값(2, 10, 16진수)
[동작 순서]
1. 함수 내부에서는 두 번째 매개변수에 따라 조건문을 이용해 2, 10, 16진수에 따른 출력하도록 작성
2. 2진수) 이중 for문을 이용하여 최상위 워드열부터 최하위 워드열까지의 최상위 비트부터 최하위 비트까지 검사.
   이 때 *마스킹을 이용해 각 비트에 1이 들어가 있는지, 0이 들어가 있는지 판별하고 바로 출력.
3. 10진수) 워드열(A[0], A[1], ..)을 10진수로 표현하고, 이를 A[0] + A[1]*2^WORD_BIT_LEN + ... 로 표현.
4. 16진수) 16진수인 경우에, 2진수와 마찬가지로 마스킹을 이용해 최상위 워드열부터 최하위 워드열까지 판별.
   하지만 한 바이트씩 마스킹해 비교하고 출력.
   ex) bigint* a1에 a1->a[0] = 250 a1->a[1] = 2을 대입했을 시
   BI_Show(a1, 16) 호출시 0x02fa가 출력된다.
(WORD_BIT_LEN에 따른 마스킹 작업이 달라서 조건문을 사용해 다르게 출력.)

/*-----------------------------------------------------------------------------------------------------*/

// Chapter 2.3
void bi_refine() // re define Big_Integer
   input : bigint* x
   1st arg : 재정의해 줄 Big_Integer의 포인터
[동작순서]
1. Big_Integer의 최상위 워드열이 0인지 확인한다.
2. 1번에서 최상위 워드열이 0인 경우, 최상위 워드열부터 0으로 채워진 워드열의 크기만큼 줄여 워드열의 크기를 재설정해 준다.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.4
void Assign_BI() // Assign Big_Integer
   input : bigint** y, bigint* x
   1st arg : 2nd arg의 Big_Integer와 같게 복사될 Big_Integer형 더블 포인터
   2nd arg : 복사할 Big_Integer형 포인터
[동작 순서]
1. 새로 할당하고자 하는 Big_Integer가 존재할 시, BI_Delete()를 이용해 제거한다.
2. x의 워드열 길이만큼의 Big_Integer를 BI_New()를 이용해 생성한다.
3. x의 부호를 y에 대입한다.
4. 반복문을 사용해 x의 워드열에서 워드 단위 별로 최하위 워드부터 대입한다.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.5
void bi_gen_rand() // generate random number
   input : bigint** x, int sign, int wordlen
   1st arg : random number를 저장할 Big_Integer의 더블 포인터
   2nd arg : random number의 부호
   3rd arg : random number의 워드 길이
[동작순서]
1. Big_Integer 구조체를 생성한다.
2. sign 값 대입한 뒤, array_rand 함수를 통해 구조체의 워드열에 random number를 대입한다.
3. bi_refine 함수를 이용하여 0으로 되어 있는 최상위 워드열을 잘라낸다.


void array_rand() // set random number
   input : word* dst, int wordlen
   1st arg : random number를 담을 워드열의 포인터
   2nd arg : random number의 워드 길이
[동작순서]
1. rand() 함수는 15 bit 출력값을 가지므로 포인터가 워드 단위가 아닌 바이트 단위로 넘어가도록 캐스팅해 준다.
2. rand() 함수를 이용하여 cnt(word length * sizeof(word))만큼 random number를 생성하여 워드열에 대입한다.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.6
void Get_Word_Length() // Get Word Length of Big_Integer
   input : int *len, bigint* x
   output : (*x)->wordlen
   1st arg : 워드열의 길이를 가리키는 int형 포인터
   2nd arg : 워드 길이를 잴 bigint형 포인터
[동작 순서]
1. x->wordlen을 *len에 대입한다.


void Bit_Length() // Get Bit Length of Big_Integer
   input : int *len, bigint* x
   1st arg : 워드열의 비트 길이를 가리키는 int형 포인터
   2nd arg : 비트 길이를 잴 bigint형 포인터
[동작 순서]
1. 최상위 워드를 제외한 워드열의 길이를 i에 대입 (최상위 워드에서 최상위 비트가 꼭 1이 아니므로)
2. i를 비트 단위로 표현하기 위해 WORD_BIT_LEN을 곱해준다.
3. 최상위 워드의 최상위 비트부터 최하위 비트까지 처음으로 1이 나올 때를 확인(마스킹을 이용)
4. 1이 나온 비트의 길이와 i를 더해 Big_Int의 비트 길이를 알아낸다.


void j_th_Bit_of_BI() // j-th Bit of Big_Integer
   input : int j, bigint* x
   output : 0 or 1
   1st arg : 입력을 통해 알아내고 싶은 bigint형 포인터 x의 j번째 비트
   2nd arg : 조사당할 bigint형 포인터
[동작 순서]
1. j 번째 비트의 위치가 워드열보다 크다면 return 해준다.
2. j 번째 비트가 워드열안에 속한다면, 워드 열 중 몇 번째 워드의 비트인지 확인한다. ex) 2번째 워드의 최상위 비트
3. 해당하는 워드의 비트의 값을 확인하기 위해(마스킹 작업) 새로운 변수를 선언한다.
4. 새로운 변수를 해당 비트에만 1이 채워지도록 1을 shift 연산처리한다. ex)  2번쨰 워드의 최상위 비트를 만들기 위해 1<<WORD_BIT_LEN
4. 이 후 두 값을 비교해 같으면 1, 다르면 0을 출력하도록 한다.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.7
int Get_Sign() // Get Sign
   input : bigint* x
   output : NON_NEGATIVE / NEGATIVE / ERROR
   1st arg : 부호를 알아 올 Big_Integer의 포인터
[동작 순서]
1. Big_Integer의 구조체의 sign 멤버를 이용하여 부호를 판별한다.


void Flip_Sign() // Flip Sign
   input : bigint* x
   1st arg : 부호를 뒤집을 Big_Integer의 포인터
[동작 순서]
1. Big_Integer의 구조체의 sign 멤버를 바꿔 준다.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.8
void BI_Set_One (); // Set Big_Integer 1
   input : bigint** x
   1st arg: 1로 설정할 Big_Integer의 더블 포인터
[동작 순서]
1. 워드열의 길이가 1인 워드열을 생성한다.
2. 부호를 나타내는 sign에 NON_NEGATIVE 대입한다.
3. 워드에 1 대입한다.


void BI_Set_Zero(); //Set Big_Integer 0
   input : bigint** x
   1st arg : 0으로 설정할 Big_Integer의 더블 포인터
[동작 순서]
1. 워드열의 길이가 1인 워드열을 생성한다.
2. 부호를 나타내는 sign에 NON_NEGATIVE 대입한다.
3. 워드에 0 대입한다.


int Is_One() // Determine if one or not
   input : bigint** x
   output : -1(FALSE) / 0(TRUE)
   1st arg : 1인지 아닌지 판단할 Big_Integer의 더블 포인터
[동작 순서]
1. sign이 음수이거나 최하위 워드열이 1이 아니면 False를 return 한다.
2. 최상위 워드열에서 1번째 워드열까지 전부 0인지 확인한다.


int Is_Zero() // Determine if zero or not
   input : bigint** x
   output : -1(FALSE) / 0(TRUE)
   1st arg : 0인지 아닌지 판단할 Big_Integer의 더블 포인터
[동작 순서]
1. sign이 음수이거나 최하위 워드열이 0이 아니면 False를 return 한다.
2. 최상위 워드열에서 1번째 워드열까지 전부 0인지 확인한다.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.9
int Compare_BI(); //Compare Big_Integer
   input : bigint**x, bigint** y
   output : 1(1st argument > 2nd argumetn), 0(1st argument = 2nd argument), -1(1st argument < 2nd argument)
   1st arg : 2nd arg와 비교 대상인 Big_Integer의 더블 포인터
   2nd arg : 1st arg와 비교 대상인 Big_Integer의 더블 포인터
[동작 순서]
1. *x와 *y의 sign 변수인 부호를 비교한다.
2. 부호가 서로 같을 때 각각 워드열의 길이를 구해 비교한다.
3. 워드열의 길이마저 같을 경우, 최상위 워드열부터 최하위 워드열까지 각각 비교한다.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.10
void Left_Shift()
   input : bigint* x, int len
   1st arg : shift 시킬 Big_Integer의 포인터
   2nd arg : shift 시킬 비트의 길이
[동작 순서]
1. 입력받은 Big_Integer를 복사한다.
2. 추가적으로 필요한 워드열의 크기를 계산하여 워드열의 크기를 realloc으로 재할당한 뒤, 늘어난 배열을 0으로 초기화시킨다.
3. shift 시킬 비트위 길이가 WORD_LEN_BIT의 배수인지 아닌지 판단하여 shift를 해 준다.
4. 최상위 비트가 0으로 되어 있는 경우를 대비하여 bi_refine 함수로 배열을 재설정해 준다.


void Right_Shift()
   input : bigint* x, int len
   1st arg : shift 시킬 Big_Integer의 포인터
   2nd arg : shift 시킬 비트의 길이
[동작 순서]
1. shift 시킬 비트의 길이가 wn(WORD_BIT_LEN * wordlen)보다 큰지 작은지 비교한다.
2. shift 시킬 비트의 길이가 wn보다 큰 경우, 전부 0으로 바꿔 준다.
3. shift 시킬 비트의 길이가 wn보다 작은 경우, WORD_LEN_BIT의 배수인지 아닌지 판단하여 shift를 해 준다.

/*-----------------------------------------------------------------------------------------------------*/

// Chpater 2.11
void Reduction_BI() //Reduction Integer
   input : bigint **x, int r
   1st arg : reduction 대상인 Big_Integer의 더블 포인터
   2nd arg : 나누고자 하는 수
[동작 순서]
1. mod 연산 하고자 하는 길이(r)가 워드열보다 길거나 같을시 return  해준다.
2. mod 연산 하고자 하는 길이(r)가 워드열보다 짧으면서 WORD_BIT_LEN의 배수일 때,
나누는 수보다 작은 워드열은 그대로 두고, 큰 워드열은 0으로 초기화시켜준다.
3. 모두 아닌 경우, 나누는 수보다 작은 워드열은 그대로 두고, 그 다음 상위 열에 대한 mod 연산을 해준다.
그 이후 상위 워드열은 그대로 둔다.

/*-----------------------------------------------------------------------------------------------------*/

// Chapter 4 Subtraction
void SUBC_BI(int* borrow, bigint** c, bigint** a, bigint** b);
    input : int* borrow, bigint** c, bigint** a, bigint** b
    argv[0] : return 되는 borrow 값
    argv[1] : 뺄셈 연산할 때 A - B 연산 결과인 bigint형 더블 포인터
    argv[2] : 뺄셈 연산할 때 A - B 에서 A인 bigint형 더블 포인터
    argv[3] : 뺄셈 연산할 때 A - B 에서 B인 bigint형 더블 포인터

[동작 순서]
1. 단일 워드에 대해서 뺄셈 연산을 수행 (첫번째일 때는 borrow = 0)
2. 단일 워드에 대해서 뺄셈 연산 후 borrow 값과 c에 대입

bigint* SUB_BI(bigint** a, bigint** b);
    input : bigint** a, bigint** b
    argv[0] : 뺄셈 연산할 때 A - B 에서 A인 bigint형 더블 포인터
    argv[1] : 뺄셈 연산할 때 A - B 에서 B인 bigint형 더블 포인터

[동작 순서]
1. A와 B의 부호에 따라 조건문에 대입
2. 조건문에서 SUBC_BI()를 불러와 워드 뺄셈 연산 진행
3.  뺄셈 연산된 bigint 포인터형을 return
(수정사안 : 현재 bigint* 형 함수이지만 void를 써서 memory leakage를 줄이기)
(수정사안 : 테스트 벡터 만들어서 값이 맞는지 확인. ex) sage)
