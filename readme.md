# __Tory__ (cryp<u>to</u> libra<u>ry</u>) 
## Se-yoon Lee, Seo-Woo Jung

## The library to implemented 'Big integer function' which used to 'Public-key cryptography'.


### Table of Contents:

0. Development environmnet
1. Description of Tory.
2. Getting Tory.
3. Compiling Tory.
4. Description of Tory's function.

────────────────────────────────────────────────────────────

### [0] Development environment

1.
    - OS : Window 10
    - Compiler : Visual studio

2.
    - OS : Ubuntu 18.04
    - Compiler : gcc

────────────────────────────────────────────────────────────

### [1] Description of Tory.

- main.c --> 빅넘버 연산 함수들을 테스트 해볼 수 있는 테스트 벡터들을 생성
- bigint.h --> 빅넘버 관련 연산 함수들의 헤더 파일
- bigint.c --> 빅넘버 관련 연산 함수들이 구현

────────────────────────────────────────────────────────────

### [2] Getting Tory.

1. Tory is available via Git at [link](https://github.com/Leeseyoon/Tory)

2. To obatin Tory enter into the your prefered folder and write the following syntax :

` $ git clone https://github.com/Leeseyoon/Tory.git `

────────────────────────────────────────────────────────────

### [3] Compiling Tory.

1. Use 'visual studio' Compiler

2. Use gcc Compiler in linux


` $ gcc -o [ File name ] main.c bigint.c bigint.h `

────────────────────────────────────────────────────────────
### [4] Description of Tory's function.

- define
```c
#define NEGATIVE : Big_Integer의 부호가 음수일 때 
#define NON_NEGATIVE : Big_Integer의 부호가 양수일 때 
#define ERROR : 오류 발생 시 반환값
#define WORD_BIT_LEN : 워드 단위에 대한 값(8, 32, 64-bit)
```
- Chapter 2.1
```c
void BI_New() // Create Big_Integer

    input : bigint** x, int wordlen
    1st arg : 새로 선언할 Big_Integer
    2nd arg : 새로 선언할 Big_Integer의 길이
```
- 동작 순서
 1. 생성하고자 하는 bigint형 포인터 x가 nullptr이 아닐 시 BI_Delete를 이용해 제거한다.
 2. 워드열의 길이에 wordlen을 대입해 준다.
 3. 워드열의 부호에 NON_NEGATIVE를 대입해 준다.
 4. 워드열의 길이에 맞춰 워드열을 동적 메모리 할당해 준다.
