//////////////////////////////////////
// definition & macro
//////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>


/** 최대 입력 자리수 */
#define MAX_NUMS	10
/** 최대 숫자 입력 개수 */
#define MAX_NINPUT	10
/** 최소 숫자 입력 개수 */
#define MIN_NINPUT	5

/** 함수 반환값 열거형 */
enum STATUS{
	UNKNOWN		= -1,	/** 알 수 없는 값 입력 */
	EQUAL		= 0,	/** 일치 (strncmp 함수에서 사용) */
	FAIL		= 0,	/** 실패 */
	SUCCESS,			/** 성공 */
	EXIT,				/** 입력 종료 */
	FINISH,				/** 프로그램 종료 */
	AGAIN				/** 재진행 */
};

/** 입력 중단 코드 문자열 */
const char *exit_code = "q\n";
/** 프로그램 종료 코드 문자열 */
const char *finish_code = "end\n";

/**
 * @brief input_data_t struct
 * 입력 받은 정수 리스트와 개수를 관리한다.
 */
typedef struct input_data_s input_data_t;
struct input_data_s{
	int size;				/**< 입력 받은 정수들의 개수 */
	int nums[MAX_NUMS];		/**< 입력 받은 정수 리스트 */
};

//////////////////////////////////////
// local function
//////////////////////////////////////
/**
 * @fn int check_buffer( char *buf)
 * @brief 매개변수로 전달 받은 문자열이 정수인지 확인하는 함수
 * @param buf 정수인지 확인할 문자열 변수 (char* 변수)
 * @return 문자열이 정수인지 검사 여부 반환. 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int check_buffer( const char *buf){
	/** 검사 성공 여부를 설정할 변수 */
	int return_value = SUCCESS;

	/** 매개변수로 전달 받은 문자열 변수가 NULL 인 경우, return_value 에 FAIL 을 설정한다. */
	if( buf == NULL){
		printf("\t| ! [DEBUG] 버퍼 NULL 오류 발생\n");
		return FAIL;
	}

	/** 음수 기호 카운트 변수 */
	int minus_count = 0;
	/** 반복문 인덱스 변수 */
	size_t loop_index = 0;
	/** 전달 받은 문자열의 길이 변수 */
	size_t buf_size = strlen( buf);

	/** 아무것도 입력하지 않고, 엔터만 입력 시 return_value 에 FAIL 을 설정한다. 재입력 진행. */
	if( buf[0] == '\n'){
		return AGAIN;
	}


	for( ; loop_index < buf_size; loop_index++){
		/** 해당 문자가 개행 문자일 경우, 해당 문자열의 끝이므로 반복문 종료 (by fgets function) */
		if( buf[loop_index] == '\n') break;
		/** 해당 문자가 '-'일 경우, 음수를 뜻하므로 계속 진행 */
		if( buf[loop_index] == '-'){
			minus_count++;
			/** 아래의 경우에 해당되면 잘못된 정수이므로 return_value 에 FAIL 을 설정한다. */
			/** 1) '-' 문자가 숫자 사이에 있는 경우 */
			/** 2) '-' 문자가 개행 문자 바로 전에 있는 경우 (숫자 뒤에 있는 경우) */
			/** 3) '-' 문자가 두 번 이상 카운트되는 경우 */
			/** 4) '-' 문자와 숫자 사이에 다른 문자가 있는 경우 (공백 포함) */
			if((( loop_index > 0) && (isdigit( buf[loop_index+1])) != 0) 
					|| ( buf[loop_index+1] == '\n')
					|| ( minus_count > 1)
					|| (( loop_index > 0) && (isspace( buf[loop_index+1])) != 0)
			  ){
				return_value = FAIL;
				break;
			}
			continue;
		}
		/** 해당 문자가 정수가 아닌 경우, isdigit 함수가 0 을 반환 */
		if( isdigit( buf[loop_index]) == 0) {
			return_value = FAIL;
			break;
		}
	}

	return return_value;
}

/**
 * @fn int convert_buffer_to_integer( char *buf)
 * @brief 매개변수로 전달 받은 문자열을 정수로 바꾸는 함수
 * @param buf 정수로 바꿀 문자열 변수 (char* 변수)
 * @return 성공 시 변환된 정수, 실패 시 FAIL 반환
 */
int convert_buffer_to_integer( const char *buf){
	int return_value = FAIL;
	int number = atoi( buf);
	if( number != 0){
		return_value = number;
	}
	else{
		/** 문자열이어서 atoi 반환값이 0 인 경우 */
		if( isdigit( buf[0]) == 0){
			printf("\t| ! 정수 변환 실패, atoi 함수 오류\n");
			return_value = FAIL;
		}
		/** 실제로 입력값이 0 이어서 atoi 반환값이 0 인 경우 */
		else return_value = number;
	}
	return return_value;
}

/**
 * @fn int input_number( int *num)
 * @brief 숫자 하나를 입력받는 함수
 * @param num 입력 받을 정수 변수의 주소를 가리키는 포인터
 * @return 입력 성공 여부 반환. 성공 시 SUCCESS, 실패 시 FAIL, 입력 종료 시 EXIT, 개행 문자 입력 시 AGAIN, 알 수 없는 입력 시 UNKNOWN, 프로그램 종료 시 FINISH 반환
 */
int input_number( int *num){
	/** 입력 성공 여부를 설정할 변수 */
	int return_value = FAIL;
	/** 입력 받을 문자열을 담을 변수 */
	char buf[MAX_NUMS+1];

	printf("\t| @ 입력 : ");
	if( fgets( buf, MAX_NUMS+2, stdin) == NULL){
		/** 어떠한 입력도 받지 못하거나 오류가 발생하면 NULL 을 반환한다. */
		printf("\t| ! [DEBUG] 입력 실패, fgets 함수 오류\n");
		perror("\t| ! [DEBUG] fgets");
		return return_value;
	}
	/** 입력 후 정수 또는 문자열인지 검사 */
	/** 정수면 저장, 문자열이면 종료 코드인지 확인 */

	return_value = check_buffer( buf);
	switch( return_value){
		/** 숫자를 입력 받은 경우 */
		case SUCCESS:
			*num = convert_buffer_to_integer( buf);
			if( *num >= INT_MAX) *num = FAIL;
			break;
		/** 문자열을 입력 받은 경우 */
		case FAIL:
			//TODO
			/** 입력 종료 코드(문자열)를 입력 받은 경우 */
			if( strncmp( buf, exit_code, MAX_NUMS) == EQUAL){
				printf("\t| @ 입력 종료\n");
				return_value = EXIT;
			}
			/** 프로그램 종료 코드(문자열)를 입력 받은 경우 */
			else if( strncmp( buf, finish_code, MAX_NUMS) == EQUAL){
				return_value = FINISH;
			}
			/** 입력 종료 코드(문자열)가 아닌 알 수 없는 문자열을 입력 받은 경우 */
			else{
				printf("\t| ! 입력 실패, 알 수 없는 입력\n");
				return_value = UNKNOWN;
			}
			break;
		/** 엔터만 입력 시 재입력 */
		case AGAIN:
			break;
		/** return_value 가 해당 함수 로직에서 설정한 반환값이 아닌 경우 */
		default:
			printf("\t| ! [DEBUG] check_buffer 함수에서 알 수 없는 반환값 발생\n");
		   break;
	}

	return return_value;
}

/**
 * @fn void input_numbers( input_data_t *odd)
 * @brief 지정된 입력 개수 범위 내에서 정수들을 입력 받는 함수
 * @param odd 입력된 정수들을 저장하고 관리하기 위한 input_data_t 구조체 포인터
 * @return 성공 시 SUCCESS, 실패 시 FAIL, 프로그램 재시작 시 AGAIN, 입력 종료 시 EXIT, 프로그램 종료 시 FINISH 반환
 */
int input_numbers( input_data_t *odd){
	/** input_numbers 반환값 변수 */
	int return_value_nums = SUCCESS;
	/** input_number 반환값 변수 */
	int return_value_num = SUCCESS;

	/** 구조체 객체 포인터가 NULL 인 경우, 프로그램 종료 */
	if( odd == NULL){
		printf("\t| ! [DEBUG] 입력 실패, 알 수 없는 객체 포인터 (NULL)\n");
		return FAIL;
	}

	printf("\n\t| --------------------------------------------\n");
	printf("\t| @ 홀수 출력 프로그램\n");
	printf("\t| @ (정수만 입력 가능)\n");
	printf("\t| @ 최소 입력 개수 : %d\n", MIN_NINPUT);
	printf("\t| @ 최대 입력 개수 : %d\n", MAX_NINPUT);
	printf("\t| @ 숫자 최대 입력 자리수 : %d\n", MAX_NUMS);
	printf("\t| @ 입력 종료 : q \n");
	printf("\t| @ 프로그램 종료 : end \n\n");

	odd->size = UNKNOWN;
	/** 임시 입력 변수 */
	int number = 0;
	/** 무한 루프 탈출 확인 변수 */
	int is_loop_break = 0;

	while( 1){
		/** 입력 사이클 시작 */
		if( return_value_num != AGAIN){
			/** 먼저 개수를 하나 증가시켜서 최대 입력 개수를 초과하는지 확인한다. */
			odd->size++;
			/** 초과하면 입력 루틴 종료, odd->size 는 배열의 인덱스값이므로 MAX_NINPUT 값과 같으면 안된다. */
			if( odd->size >= MAX_NINPUT){
				break;
			}
		}

		return_value_num = input_number( &number);
		switch( return_value_num){
			/** 아래 3 가지 경우일 때 입력 루틴을 종료한다. */
			/** 입력 실패 */
			case FAIL:
			/** 입력 종료 코드 입력 */
			case EXIT:
			/** 프로그램 종료 코드 입력 */
			case FINISH:
				is_loop_break++;
				break;
			/** 엔터만 입력 시 재입력 */
			case AGAIN:
				break;
			/** 입력 종료 코드가 아닌 문자열 입력 시 재입력 진행 */
			case UNKNOWN:
				continue;
			/** 입력 성공 시 해당 정수 저장 */
			case SUCCESS:
				odd->nums[odd->size] = number;
				break;
			/** return_value 가 해당 함수 로직에서 설정한 반환값이 아닌 경우 */
			default:
				printf("\t| ! [DEBUG] input_number 함수에서 알 수 없는 반환값 발생\n");
				is_loop_break++;
				break;
		}
		if( is_loop_break > 0) break;
		/** 입력 사이클 종료 */
	}

	/** 입력 루틴이 종료되면, 입력된 정수의 개수가 최소 입력 개수 미만인지 검사한다. return value 가 SUCCESS 이거나 EXIT 일 때 실행된다. */
	if((( return_value_num == SUCCESS) || ( return_value_num == EXIT)) && ( odd->size < MIN_NINPUT)){ /** 미만이면 총 몇개를 입력했는지 출력하고, 입력 루틴 실패 반환 */
		printf("\t| ! 입력 실패, 최소 입력 개수 미만 / 입력 개수 : %d\n", odd->size + 1);
		return_value_nums = AGAIN;
	}
	else return_value_nums = return_value_num;
	/** 이상이면 입력 루틴 성공 반환 */

	return return_value_nums;
}

/**
 * @fn void print_odd_numbers( input_data_t *odd)
 * @brief 입력된 정수들 중 홀수만 출력하는 함수
 * @param odd input_data_t 구조체 주소를 가리키는 구조체 포인터
 * @return 반환값 없음
 */
void print_odd_numbers( input_data_t *odd){
	/** 구조체 객체 포인터가 NULL 인 경우, 프로그램 종료 */
	if( odd == NULL){
		printf("\t| ! [DEBUG] 출력 실패, 알 수 없는 객체 포인터 (NULL)\n");
		return ;
	}
	
	/** 반복문 인덱스 변수 */
	int loop_index = 0;
	/** 입력 받은 정수 리스트에서 홀수 개수를 카운트하는 변수 */
	int is_odd = 0;

	printf("\n");
	printf("\t| @ 총 입력 개수\t: %d\n", odd->size);
	printf("\t| @ (순서)\t\t: (입력한 정수)\n");
	for( ; loop_index <= odd->size; loop_index++){
		/** 홀수만 선택. 음수 계산을 위해 절대값 사용 */
		if( abs( odd->nums[loop_index]) % 2 == 1){
			printf("\t| @ %d\t\t\t: %d\n", ( loop_index + 1), odd->nums[loop_index]);
			is_odd++;
		}
	}
	/** 홀수 카운트가 0 보다 클 경우, 홀수 출력 성공 */
	if( is_odd > 0)	printf("\n\t| @ 출력 성공\n\n");
	/** 홀수 카운트가 0 보다 작거나 같을 경우, 홀수 출력 실패 */
	else printf("\n\t| ! 짝수만 존재\n\n");
}

//////////////////////////////////////
// main function
//////////////////////////////////////
/**
 * @fn int main()
 * @brief 정해진 숫자 개수 범위에 따라 입력 받은 정수들 중 홀수만 출력하는 프로그램
 * @param 매개변수 없음
 * @return 프로그램 실행 성공 여부 반환. 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int main(){
	/** 함수 실행 성공 여부를 설정할 변수 */
	int return_value = SUCCESS;

	/** 정수를 입력 받을 구조체를 할당해서 그 주소를 가지고 있는 구조체 포인터 변수 */
	input_data_t *odd = ( input_data_t*)malloc(sizeof( input_data_t));
	if( odd == NULL){
		printf("\t| ! [DEBUG] 객체 메모리 할당 실패, 프로그램 종료\n");
		perror("\t| ! [DEBUG] malloc");
		return FAIL;
	}

	while( 1){
		/** 입력 진행 */
		return_value = input_numbers( odd);
		switch( return_value){
			case FAIL:
				printf("\t| ! 입력 실패, 프로그램 종료\n");
				break;
			case FINISH:
				printf("\t| @ 프로그램 종료\n");
				break;
			case AGAIN:
				printf("\t| ! 입력 재진행, 기존에 입력 받은 정수 모두 초기화\n");
				memset( odd, 0, sizeof( input_data_t));
				continue;
			case SUCCESS:
			case EXIT:
				print_odd_numbers( odd);
				break;
			/** return_value 가 해당 함수 로직에서 설정한 반환값이 아닌 경우 */
			default:
				printf("\t| ! input_data_input_numbers 함수에서 알 수 없는 반환값 발생\n");
				break;
		}
		break;
		/** 입력 종료 */
	}

	free( odd);
	return return_value;
}


