//////////////////////////////////////
// definition & macro
//////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

/** 종료 */
#define EXIT		2
/** 일치 */
#define EQUAL		0
/** 성공 */
#define SUCCESS		1
/** 실패 */
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
 * @brief 매개변수로 전달 받은 문자열이 정수인지 확인하는 함수
 * @param buf 정수인지 확인할 문자열
 * @return 문자열이 정수인지 검사 여부 반환. 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int check_digit( char *buf){
	int loop_index = 0;				/** 반복문 인덱스 변수 */
	int buf_size = strlen( buf);	/** 전달 받은 문자열의 길이 변수 */
	int return_value = SUCCESS;		/** 검사 성공 여부를 설정할 변수 */

	for( ; loop_index < buf_size; loop_index++){
		if( buf[loop_index] == '\n') break;
		if( isdigit( buf[loop_index]) == 0) { /** 해당 문자가 정수가 아닌 경우, isdigit 함수가 0 을 반환 */
			return_value = FAIL;
			break;
		}
	}

	return return_value;
}

/**
 * @fn int input_number( int *num)
 * @brief 숫자 하나를 입력받는 함수, fgets 함수를 통해 최대 10 자리수까지 입력받는다.
 * @param num 입력 받을 int 형 정수 변수
 * @return 입력 성공 여부 반환. 성공 시 SUCCESS, 실패 시 FAIL, 종료 시 EXIT 반환
 */
int input_number( int *num){
	int return_value = FAIL;	/** 검사 성공 여부를 설정할 변수 */
	char buf[MAX_NUMS];			/** 입력 받을 문자열을 담을 변수 */

	printf("\t| @ 입력 : ");
	if( fgets( buf, MAX_NUMS, stdin) == NULL){
		printf("\t| ! 입력 실패, fgets 함수 오류\n");
		perror("fgets");
		return_value = FAIL;
	}

	/** 입력 후 정수 또는 문자열인지 검사 */
	/** 정수면 저장, 문자열이면 종료 코드인지 확인 */
	return_value = check_digit( buf);
	if( return_value == SUCCESS){ /** 숫자를 입력 받은 경우 */
		*num = atoi( buf);
	}
	else{ /** 입력 종료 코드(문자열)를 입력 받은 경우 */
		if( strncmp( buf, exit_code, MAX_NUMS) == EQUAL){
			printf("\t| @ 입력 종료\n");
			return_value = EXIT;
		}
		else{ /** 입력 종료 코드(문자열)가 아닌 문자열을 입력 받은 경우 */
			printf("\t| ! 입력 실패, 알 수 없는 입력\n");
		}
	}

	return return_value;
}

/**
 * @fn void test4_odd_input_numbers( test4_odd_t *odd)
 * @brief 지정된 정수 입력 개수 범위 내에서 정수들을 입력 받는 함수
 * @param odd 지정된 정수 입력 개수 범위 내에서 입력된 정수들을 저장하고 관리하기 위한 test4_odd_t 구조체 객체 변수
 * @return 입력 성공 여부 반환. 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int test4_odd_input_numbers( test4_odd_t *odd){
	int return_value = SUCCESS;

	if( odd == NULL){ /** 구조체 객체가 NULL 인 경우, 프로그램 종료 */
		printf("\t| ! 입력 실패, 알 수 없는 객체 (NULL)\n");
		return_value = FAIL;
	}
	else{
		printf("\t| @ 프로그램 시작\n");
		printf("\t| @ 최소 입력 개수 : %d\n", MIN_INPUT);
		printf("\t| @ 최대 입력 개수 : %d\n\n", MAX_INPUT);

		int number = 0; /** 임시 입력 변수 */
		while( 1){
			/** 입력 사이클 시작 */
			return_value = input_number( &number);
			if(( return_value == FAIL) || ( return_value == EXIT)) break; /** 입력 실패이거나 종료 코드를 입력 받으면, 입력 루틴 종료 */
			else{
				odd->size++; /** 먼저 개수를 하나 증가시켜서 최대 입력 개수를 초과하는지 확인한다. */
				if( odd->size >= MAX_INPUT){ /** 초과하면 입력 루틴 종료 */
					printf("\t| ! 입력 실패, 최대 입력 개수 초과\n");
					break;
				}
				else{ /** 초과하지 않으면 입력 성공, odd 객체의 배열에 저장 */
					odd->nums[odd->size] = number;
					//printf("\t| @ 입력 성공 : %d\n", odd->nums[odd->size]);
				}
			}
			/** 입력 사이클 종료 */
		}

		/** 입력 루틴이 종료되면, 입력된 정수의 개수가 최소 입력 개수 미만인지 검사한다. */
		if( odd->size < MIN_INPUT){ /** 미만이면 총 몇개를 입력했는지 출력하고, 입력 루틴 실패 반환 */
			printf("\t| ! 입력 실패, 최소 입력 개수 미만 / 입력 개수 : %d\n", odd->size);
			return_value = FAIL;
		}
		/** 이상이면 입력 루틴 성공 반환 */
	}

	return return_value;
}

/**
 * @fn void test4_odd_print_odd_numbers( test4_odd_t *odd)
 * @brief 입력된 정수들 중 홀수만 출력하는 함수
 * @param odd 입력된 정수들에 대한 정보(리스트, 개수)를 가지고 있는 test4_odd_t 구조체 객체 변수
 * @return 반환값 없음
 */
void test4_odd_print_odd_numbers( test4_odd_t *odd){
	if( odd == NULL){ /** 구조체 객체가 NULL 인 경우, 프로그램 종료 */
		printf("\t| ! 출력 실패, 알 수 없는 객체 (NULL)\n");
		return ;
	}
	else{
		int loop_index = 0;	/** 반복문 인덱스 변수 */
		int is_odd = 0;		/** 홀수 카운트 변수 */

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
	else{ /** return_value 가 EXIT 또는 SUCCESS 인 경우 */
		test4_odd_print_odd_numbers( odd);
	}

	free( odd);
	odd = NULL;
}

