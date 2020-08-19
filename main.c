//////////////////////////////////////
// definition & macro
//////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

/** 일치 여부 */
#define EXIT		2
/** 일치 여부 */
#define EQUAL		0
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
const char *exit_code = "q\n";

/**
 * @brief test4_odd_t struct
 * 입력 받은 정수 리스트와 개수를 관리한다.
 */
typedef struct test4_odd_s test4_odd_t;
struct test4_odd_s{
	int size;				/**< 입력 받은 정수들의 개수 */
	int nums[MAX_NUMS];		/**< 입력 받은 정수 리스트 */
};

//////////////////////////////////////
// local function
//////////////////////////////////////
/**
 * @fn int check_digit( char *buf)
 * @brief 
 * @param 
 * @return 문자열이 정수인지 검사 여부 반환. 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int check_digit( char *buf){
	int loop_index = 0;
	int buf_size = strlen( buf);
	int return_value = SUCCESS;

	for( ; loop_index < buf_size; loop_index++){
		if( buf[loop_index] == '\n') break;
		if( isdigit( buf[loop_index]) == 0) {
			return_value = FAIL;
			break;
		}
	}

	return return_value;
}

/**
 * @fn int input_number( int *num)
 * @brief 
 * @param 
 * @return 입력 성공 여부 반환. 성공 시 SUCCESS, 실패 시 FAIL, 종료 시 EXIT 반환
 */
int input_number( int *num){
	int return_value = 0;
	char buf[MAX_NUMS];

	printf("\t| @ 입력 : ");
	fgets( buf, MAX_NUMS, stdin);

	return_value = check_digit( buf);
	if( return_value == SUCCESS){ // input number
		*num = atoi( buf);
		return_value = SUCCESS;
	}
	else{ // input exit code
		if( strncmp( buf, exit_code, MAX_NUMS) == EQUAL){
			printf("\t| @ 입력 종료\n");
			return_value = EXIT;
		}
		else{
			printf("\t| ! 입력 실패, 알 수 없는 입력\n");
			return_value = FAIL;
		}
	}

	return return_value;
}

/**
 * @fn void test4_odd_input_numbers( test4_odd_t *odd)
 * @brief 지정된 정수 입력 개수 범위 내에서 정수들을 입력 받는다.
 * @param odd 지정된 정수 입력 개수 범위 내에서 입력된 정수들을 저장하고 관리하기 위한 test4_odd_t 구조체 객체 변수
 * @return 입력 성공 여부 반환. 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int test4_odd_input_numbers( test4_odd_t *odd){
	int return_value = SUCCESS;

	if( odd == NULL){
		printf("\t| ! 입력 실패, 알 수 없는 객체 (NULL)\n");
		return_value = FAIL;
	}
	else{
		printf("\t| @ 프로그램 시작\n");
		printf("\t| @ 최소 입력 개수 : %d\n", MIN_INPUT);
		printf("\t| @ 최대 입력 개수 : %d\n\n", MAX_INPUT);

		int number = 0;
		while( 1){
			return_value = input_number( &number);
			if(( return_value == FAIL) || ( return_value == EXIT)) break;
			else{
				odd->size++;
				if( odd->size >= MAX_INPUT){
					printf("\t| ! 입력 실패, 최대 입력 개수 초과\n");
					break;
				}
				else{
					odd->nums[odd->size] = number;
					printf("\t| @ 입력 성공 : %d\n", odd->nums[odd->size]);
				}
			}
		}

		if( odd->size < MIN_INPUT){
			printf("\t| ! 입력 실패, 최소 입력 개수 미만 / 입력 개수 : %d\n", odd->size);
			return_value = FAIL;
		}
	}

	return return_value;
}

/**
 * @fn void test4_odd_print_odd_numbers( test4_odd_t *odd)
 * @brief 입력된 정수들을 홀수만 출력한다.
 * @param odd 지정된 정수 입력 개수 범위 내에서 입력된 정수들 중 홀수만 출력하기 위한 test4_odd_t 구조체 객체 변수
 * @return 반환값 없음
 */
void test4_odd_print_odd_numbers( test4_odd_t *odd){
	if( odd == NULL){
		printf("\t| ! 출력 실패, 알 수 없는 객체 (NULL)\n");
		return ;
	}
	else{
		int loop_index = 0;
		int is_odd = 0;

		printf("\n");
		printf("\t| @ 총 입력 개수 : %d\n", odd->size);
		for( ; loop_index < odd->size; loop_index++){
			if( odd->nums[loop_index] % 2 == 1){
				printf("\t| @ %d : %d\n", ( loop_index + 1), odd->nums[loop_index]);
				is_odd++;
			}
		}
		if( is_odd > 0)	printf("\t| @ 출력 성공\n");
		else printf("\t| ! 출력 실패\n");
	}
}

//////////////////////////////////////
// main function
//////////////////////////////////////
/**
 * @fn void main()
 * @brief 정해진 숫자 개수 범위에 따라 입력 받은 정수들 중 홀수만 출력하는 프로그램
 * @param 매개변수 없음
 * @return 반환값 없음
 */
void main(){
	int return_value = SUCCESS;

	test4_odd_t *odd = ( test4_odd_t*)malloc(sizeof( test4_odd_t));
	odd->size = -1;

	return_value = test4_odd_input_numbers( odd);
	if( return_value == FAIL){
		printf("\t| ! 입력 실패, 프로그램 종료\n");
		return ;
	}
	else{ // return_value == EXIT or SUCCESS 
		test4_odd_print_odd_numbers( odd);
	}

	free( odd);
	odd = NULL;
}

