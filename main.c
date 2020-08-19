//////////////////////////////////////
// definition & macro
//////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

/** 일치 여부 */
#define EQUAL		1
/** 성공 여부 */
#define SUCCESS		1
/** 실패 여부 */
#define FAIL		0
/** 최대 입력 자리수 */
#define MAX_NUMS	10
/** 최대 숫자 입력 개수 */
#define MAX_INPUT	10
/** 최소 숫자 입력 개수 */
#define MIN_INPUT	5

/** 입력 중단 코드 문자열 */
const char *exit_code = "q";

/**@brief test4_odd_t struct
 * 입력 받은 정수와 개수를 관리한다.
 */
typedef test4_odd_s test4_odd_t;
struct test4_odd_s{
	int size;				/**< 입력 받은 정수들의 개수 */
	int nums[MAX_NUMS];		/**< 입력 받은 정수 리스트 */
};

//////////////////////////////////////
// functions
//////////////////////////////////////

/**
 * @fn void main()
 * @brief 정해진 숫자 개수 범위에 따라 입력 받은 정수들 중 홀수만 출력하는 프로그램
 * @param 매개변수 없음
 * @return 반환값 없음
 */
void main(){
	test4_odd_t *odd = ( test4_odd_t*)malloc(sizeof( test4_odd_t));
	input_numbers( odd);
	print_odd_numbers( odd);
}

/**
 * @fn int check_boundary( int num)
 * @brief 입력 받은 정수들의 개수가 지정된 범위에 속한지 확인하는 함수
 * @param 
 * @return 
 */
int check_boundary( int num){
	int return_value = ;
	if( ){

	}
}

/**
 * @fn 
 * @brief 
 * @param 
 * @return 
 */
int check_digit( char *buf){
	int return_value = 0;

	for( ){
		if( isdigit( buf)){
			return_value = atoi( buf);
		}
	}

	return return_value;
}

/**
 * @fn 
 * @brief 
 * @param 
 * @return 
 */
int input_number( int *num){
	int return_value = 0;
	int check_digit = 0;
	char buf[MAX_NUMS] = {0, };
	gets( buf, MAX_NUMS, stdin);

	return_value = check_digit( buf);
	if( return_value == SUCCESS){ // input number
		
	}
	else{ // input exit code
		if( strncmp( buf, exit_code, MAX_NUMS) == EQUAL){
			
		}
		else{
			printf("\t| ! 입력 실패, 값이 정수로만 구성되어있지않음.\n");
			return return_value;
		}
	}
}

/**
 * @fn 
 * @brief 
 * @param 
 * @return 
 */
void test4_odd_input_numbers( test4_odd_t *odd){
	
}

void test4_odd_print_odd_numbers( test4_odd_t *odd){

}

