#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <math.h>

//////////////////////////////////////
// Definitions & Macros
//////////////////////////////////////
/** 최대 입력 자리수 */
#define MAX_NUMS 10
/** 최대 숫자 입력 개수 */
#define MAX_NINPUT 10
/** 최소 숫자 입력 개수 */
#define MIN_NINPUT 5

/** 함수 반환값 열거형 */
enum STATUS
{
	UNKNOWN = -1,	/** 알 수 없는 값 입력 */
	EQUAL = 0,		/** 일치 (strncmp 함수에서 사용) */
	FAIL = 0,		/** 실패 */
	SUCCESS,		/** 성공 */
	EXIT,			/** 입력 종료 */
	FINISH,			/** 프로그램 종료 */
	AGAIN,			/** 재진행 */
	CLEAR,			/** 초기화 */
	DISPLAY			/** 조회 */
};

/** 입력 중단 코드 문자열 */
const char *exit_code = "q";
/** 입력 초기화 문자열 */
const char *clear_code = "cls";
/** 입력 조회 문자열 */
const char *display_code = "dis";
/** 프로그램 종료 코드 문자열 */
const char *finish_code = "end";

/**
 * @brief input_data_t struct
 * 입력 받은 정수 리스트와 개수를 관리한다.
 */
typedef struct input_data_s input_data_t;
struct input_data_s
{
	int size;			/**< 입력 받은 정수들의 개수 */
	int nums[MAX_NUMS]; /**< 입력 받은 정수 리스트 */
};

//////////////////////////////////////
// Local functions
//////////////////////////////////////
void clear_numbers(input_data_t *data);
int check_buffer(char *buf);
int confirm_finish_again(const char *buf);
int check_command(const char *code);
int convert_buffer_to_integer(const char *buf);
int input_number(int *num);
int input_numbers(input_data_t *data);
void print_current_numbers(input_data_t *data);
void print_odd_numbers(input_data_t *data);

/**
 * @fn int check_buffer( char *buf)
 * @brief 매개변수로 전달 받은 문자열이 정수인지 확인하는 함수
 * @param data 데이터 초기화할 input_data_t 포인터 변수
 * @return 반환값 없음
 */
void clear_numbers(input_data_t *data)
{
	data->size = 0;
	data->nums[0] = '\0';
	printf("\t| @ 데이터 초기화 완료.\n");
}

/**
 * @fn int check_buffer( char *buf)
 * @brief 매개변수로 전달 받은 문자열이 정수인지 확인하는 함수
 * @param buf 정수인지 확인할 문자열 변수
 * @return 문자열이 정수인지 검사 여부 반환. 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int check_buffer(char *buf)
{
	/** 검사 성공 여부를 설정할 변수 */
	int return_value = SUCCESS;

	/** 매개변수로 전달 받은 문자열 변수가 NULL 인 경우, return_value 에 FAIL 을 설정한다. */
	if (buf == NULL)
	{
		printf("\t| ! [DEBUG] 버퍼 NULL 오류 발생.\n");
		return FAIL;
	}

	/** 음수 기호 카운트 변수 */
	int minus_count = 0;
	/** 반복문 인덱스 변수 */
	size_t loop_index = 0;
	/** 전달 받은 문자열의 길이 변수 */
	size_t buf_size = strlen(buf);

	for (; loop_index < buf_size; loop_index++)
	{
		/** 해당 문자가 개행 문자일 경우, 해당 문자열의 끝이므로 반복문 종료 (by fgets function) */
		if (buf[loop_index] == '\n')
			break;
		/** 해당 문자가 '-'일 경우, 음수를 뜻하므로 계속 진행 */
		if (buf[loop_index] == '-')
		{
			minus_count++;
			/** 아래의 경우에 해당되면 잘못된 정수이므로 return_value 에 FAIL 을 설정한다. */
			/** 1) '-' 문자가 숫자 사이에 있는 경우 */
			/** 2) '-' 문자가 개행 문자 바로 전에 있는 경우 (숫자 뒤에 있는 경우) */
			/** 3) '-' 문자가 두 번 이상 카운트되는 경우 */
			/** 4) '-' 문자와 숫자 사이에 다른 문자가 있는 경우 (공백 포함) */
			if (((loop_index > 0) && (isdigit(buf[loop_index + 1])) != 0) || (buf[loop_index + 1] == '\n') || (minus_count > 1) || ((loop_index > 0) && (isspace(buf[loop_index + 1])) != 0))
			{
				return_value = FAIL;
				break;
			}
			continue;
		}
		/** 해당 문자가 정수가 아닌 경우, isdigit 함수가 0 을 반환 */
		if (isdigit(buf[loop_index]) == 0)
		{
			return_value = FAIL;
			break;
		}
	}

	return return_value;
}

/**
 * @fn int confirm_finish_again(const char *buf)
 * @brief 입력 또는 프로그램 종료를 사용자로부터 재확인하는 함수
 * @param code 종료의 유형을 출력할 문자열 변수
 * @return 종료가 확실하면 AGAIN, 아니면 FAIL, 최대 입력 개수 초과 시 UNKNOWN 반환
 */
int confirm_finish_again(const char *buf)
{
	int return_value = FAIL;
	if (buf == NULL)
	{
		printf("\t| ! [DEBUG] 알 수 없는 문자열 포인터(NULL).\n");
		return return_value;
	}

	while (1)
	{
		while( getchar() != '\n');
		char _buf[MAX_NUMS];
		printf("\n\t| @ %s하시겠습니까?\n", buf);
		printf("\t| @ (입력된 데이터가 초기화됩니다.)\n");
		printf("\t| @ (y/n) : ");
		if (scanf_s("%[^\n]", _buf, sizeof(_buf)) <= 0)
		{
			return UNKNOWN;
		}

		if (strncmp(_buf, "y", MAX_NUMS) == EQUAL)
		{
			break;
		}
		else if (strncmp(_buf, "n", MAX_NUMS) == EQUAL)
		{
			return_value = AGAIN;
			break;
		}
		else
		{
			printf("\t| ! 잘못된 입력입니다.\n");
		}
	}

	return return_value;
}

/**
 * @fn int check_command( const char *code)
 * @brief 매개변수로 전달 받은 문자열이 어떤 명령어 코드인지 검사하는 함수
 * 종료 코드이면, 해당 명령을 실행하기 전에 사용자 오류를 방지하기 위해 재확인을 받는다.
 * @param code 검사할 문자열 변수
 * @return 성공 시 해당 명령어의 열거형, 실패 시 FAIL 반환
 */
int check_command(const char *code)
{
	int return_value = FAIL;
	if (code == NULL)
	{
		printf("\t| ! [DEBUG] 알 수 없는 문자열 포인터(NULL).\n");
		return return_value;
	}

	/** 입력 종료 코드(문자열)를 입력 받은 경우 */
	if (strncmp(code, exit_code, MAX_NUMS) == EQUAL)
	{
		return_value = confirm_finish_again("입력 종료");
		if (return_value != AGAIN)
		{
			return_value = EXIT;
		}
	}
	/** 입력 초기화 코드(문자열)를 입력 받은 경우 */
	else if (strncmp(code, clear_code, MAX_NUMS) == EQUAL)
	{
		return_value = confirm_finish_again("입력 초기화");
		if (return_value != AGAIN)
		{
			return_value = CLEAR;
		}
	}
	/** 입력 조회 코드(문자열)를 입력 받은 경우 */
	else if (strncmp(code, display_code, MAX_NUMS) == EQUAL)
	{
		return_value = DISPLAY;
	}
	/** 프로그램 종료 코드(문자열)를 입력 받은 경우 */
	else if (strncmp(code, finish_code, MAX_NUMS) == EQUAL)
	{
		return_value = confirm_finish_again("프로그램 종료");
		if (return_value != AGAIN)
		{
			return_value = FINISH;
		}
	}
	/** 입력 종료 코드(문자열)가 아닌 알 수 없는 문자열을 입력 받은 경우 */
	else
	{
		printf("\t| ! 입력 실패, 알 수 없는 입력.\n");
		return_value = UNKNOWN;
	}

	return return_value;
}

/**
 * @fn int convert_buffer_to_integer( const char *buf)
 * @brief 매개변수로 전달 받은 문자열을 정수로 바꾸는 함수
 * @param buf 정수로 바꿀 문자열 변수
 * @return 성공 시 변환된 정수, 실패 시 FAIL 반환
 */
int convert_buffer_to_integer(const char *buf)
{
	int return_value = FAIL;
	int number = atoi(buf);
	if (number != 0)
	{
		return_value = number;
		return return_value;
	}

	/** 문자열이어서 atoi 반환값이 0 인 경우 */
	if (isdigit(buf[0]) == 0)
	{
		printf("\t| ! 정수 변환 실패, atoi 함수 오류.\n");
		return_value = FAIL;
	}
	/** 실제로 입력값이 0 이어서 atoi 반환값이 0 인 경우 */
	else
	{
		return_value = number;
	}

	return return_value;
}

/**
 * @fn int input_number( int *num)
 * @brief 숫자 하나를 입력받는 함수
 * @param num 입력 받을 정수 변수의 주소를 가리키는 포인터
 * @return 성공 시 SUCCESS, 실패 시 FAIL, 입력 종료 시 EXIT, 
 * 최대 입력 개수 초과 시 AGAIN, 알 수 없는 입력 시 UNKNOWN, 
 * 입력 데이터 초기화 시 CLEAR, 입력 데이터 조회 시 DISPLAY, 프로그램 종료 시 FINISH 반환
 */
int input_number(int *num)
{
	/** 함수 진행 성공 여부를 설정할 변수 */
	int return_value = FAIL;
	/** 입력 받을 문자열을 담을 변수 */
	char buf[MAX_NUMS + 1];

	printf("\n\t| @ 입력 : ");
	if (scanf_s(" %[^\n]", buf, sizeof(buf)) <= 0)
	{
		/** 어떠한 입력도 받지 못하거나 오류가 발생하면 재입력을 위해 AGAIN 을 반환한다. */
		while (getchar() != '\n');
		memset(buf, 0, sizeof(buf));
		printf("\t| ! 입력 재진행, 최대 입력 개수 초과.\n");
		return AGAIN;
	}

	/** 1. 입력 후 정수 또는 문자열인지 검사 */
	/** 2. 정수면 저장, 문자열이면 어떤 종료 코드인지 확인 */
	return_value = check_buffer(buf);
	switch (return_value)
	{
	/** 숫자를 입력 받은 경우 */
	case SUCCESS:
		*num = convert_buffer_to_integer(buf);
		if (*num >= INT_MAX)
			*num = FAIL;
		break;
	/** 문자열을 입력 받은 경우 */
	case FAIL:
		return_value = check_command(buf);
		break;
	/** return_value 가 해당 함수 로직에서 설정한 반환값이 아닌 경우 */
	default:
		printf("\t| ! [DEBUG] check_buffer 함수에서 알 수 없는 반환값 발생.\n");
		break;
	}

	return return_value;
}

/**
 * @fn void input_numbers( input_data_t *data)
 * @brief 지정된 입력 개수 범위 내에서 정수들을 입력 받는 함수
 * @param data 입력된 정수들을 저장하고 관리하기 위한 input_data_t 구조체 포인터
 * @return 성공 시 SUCCESS, 실패 시 FAIL, 프로그램 재시작 시 AGAIN, 입력 종료 시 EXIT, 프로그램 종료 시 FINISH 반환
 */
int input_numbers(input_data_t *data)
{
	/** input_numbers 반환값 변수 */
	int return_value_nums = SUCCESS;
	/** input_number 반환값 변수 */
	int return_value_num = SUCCESS;

	/** 구조체 객체 포인터가 NULL 인 경우, 프로그램 종료 */
	if (data == NULL)
	{
		printf("\t| ! [DEBUG] 입력 실패, 알 수 없는 객체 포인터(NULL).\n");
		return FAIL;
	}

	printf("\n\t| --------------------------------------------\n");
	printf("\t| @ 홀수 출력 프로그램\n");
	printf("\t| @ (정수만 입력 가능)\n");
	printf("\t| @ 최소 입력 개수		: %d\n", MIN_NINPUT);
	printf("\t| @ 최대 입력 개수		: %d\n", MAX_NINPUT);
	printf("\t| @ 숫자 최대 입력 자리수	: %d\n", MAX_NUMS);
	printf("\t| @ 입력 조회			: dis \n");
	printf("\t| @ 입력 초기화			: cls \n");
	printf("\t| @ 입력 종료			: q \n");
	printf("\t| @ 프로그램 종료		: end \n\n");

	clear_numbers(data);

	/** 임시 입력 변수 */
	int number = UNKNOWN;
	/** 무한 루프 탈출 확인 변수 */
	int is_loop_break = FAIL;
	/** 입력 재진행 여부 변수 */
	int is_input_again = FAIL;

	while (1)
	{
		/** 입력 사이클 시작 */
		if ((is_input_again == FAIL) && (data->size >= MAX_NINPUT))
		{
			/** 초과하면 입력 루틴 종료, data->size 는 배열의 인덱스값이므로 MAX_NINPUT 값과 같으면 안된다. */
			break;
		}
		/** 입력 사이클 재진행 */
		else if (is_input_again == SUCCESS)
		{
			is_input_again = FAIL;
			continue;
		}

		return_value_num = input_number(&number);
		switch (return_value_num)
		{
		/** @ 아래 3 가지 경우일 때 입력 루틴을 종료한다. */
			/** 1. 입력 실패 */
		case FAIL:
			/** 2. 입력 종료 코드 입력 */
		case EXIT:
			/** 3. 프로그램 종료 코드 입력 */
		case FINISH:
			is_loop_break = SUCCESS;
			break;
		/** @ 아래 4 가지 경우일 때 입력 루틴을 재시작한다. */
			/** 1. 입력한 데이터 조회 */
		case DISPLAY:
			print_current_numbers(data);
			is_input_again = SUCCESS;
			break;
			/** 2. 입력한 데이터 초기화 */
		case CLEAR:
			clear_numbers(data);
			is_input_again = SUCCESS;
			break;
			/** 3. 최대 입력 개수 초과 시 재입력 */
		case AGAIN:
			/** 4. 입력 종료 코드가 아닌 문자열 입력 시 재입력 진행 */
		case UNKNOWN:
			is_input_again = SUCCESS;
			break;
		/** 입력 성공 시 해당 정수 저장 */
		case SUCCESS:
			data->nums[data->size] = number;
			data->size++;
			break;
		/** return_value 가 해당 함수 로직에서 설정한 반환값이 아닌 경우 */
		default:
			printf("\t| ! [DEBUG] input_number 함수에서 알 수 없는 반환값 발생.\n");
			is_loop_break = SUCCESS;
			break;
		}
		if (is_loop_break == SUCCESS)
		{
			break;
		}
		/** 입력 사이클 종료 */
	}

	/** 입력 루틴이 종료되면, 입력된 정수의 개수가 최소 입력 개수 미만인지 검사한다. return value 가 SUCCESS 이거나 EXIT 일 때 실행된다. */
	if (((return_value_num == SUCCESS) || (return_value_num == EXIT)) && (data->size < MIN_NINPUT))
	{ /** 미만이면 총 몇개를 입력했는지 출력하고, 입력 루틴 실패 반환 */
		printf("\t| ! 입력 실패, 최소 입력 개수 미만 / 입력 개수 : %d\n", data->size);
		return_value_nums = AGAIN;
	}
	else
	{
		return_value_nums = return_value_num;
	}
	/** 이상이면 입력 루틴 성공 반환 */

	return return_value_nums;
}

/**
 * @fn void print_current_numbers(input_data_t *data)
 * @brief 현재 입력된 정수들을 출력하는 함수
 * @param data input_data_t 구조체 주소를 가리키는 구조체 포인터
 * @return 반환값 없음
 */
void print_current_numbers(input_data_t *data)
{
	/** 구조체 객체 포인터가 NULL 인 경우, 프로그램 종료 */
	if (data == NULL)
	{
		printf("\t| ! [DEBUG] 출력 실패, 알 수 없는 객체 포인터(NULL).\n");
		return;
	}

	/** 반복문 인덱스 변수 */
	int loop_index = 0;

	if (data->size <= 0)
	{
		return;
	}

	printf("\n\t| --------------------------------------------\n");
	printf("\t| @ 현재 입력 개수\t: %d\n", data->size);
	printf("\t| @ (순서)\t\t: (입력한 정수)\n");
	for (; loop_index < data->size; loop_index++)
	{
		printf("\t| @ %d\t\t\t: %d\n", (loop_index + 1), data->nums[loop_index]);
	}
}

/**
 * @fn void print_odd_numbers( input_data_t *data)
 * @brief 입력된 정수들 중 홀수만 출력하는 함수
 * @param data input_data_t 구조체 주소를 가리키는 구조체 포인터
 * @return 반환값 없음
 */
void print_odd_numbers(input_data_t *data)
{
	/** 구조체 객체 포인터가 NULL 인 경우, 프로그램 종료 */
	if (data == NULL)
	{
		printf("\t| ! [DEBUG] 출력 실패, 알 수 없는 객체 포인터(NULL).\n");
		return;
	}

	/** 반복문 인덱스 변수 */
	int loop_index = 0;
	/** 입력 받은 정수 리스트에서 홀수 개수를 카운트하는 변수 */
	int is_odd = 0;

	printf("\n\t| ############################################\n");
	printf("\t| @ 총 입력 개수\t: %d\n", data->size);
	printf("\t| @ (순서)\t\t: (입력한 정수)\n");
	for (; loop_index < data->size; loop_index++)
	{
		/** 홀수만 선택. 음수 계산을 위해 절대값 사용 */
		if (abs(data->nums[loop_index]) % 2 == 1)
		{
			printf("\t| @ %d\t\t\t: %d\n", (loop_index + 1), data->nums[loop_index]);
			is_odd++;
		}
	}

	/** 홀수 카운트가 0 보다 클 경우, 홀수 출력 성공 */
	if (is_odd > 0)
	{
		printf("\n\t| @ 출력 성공.\n\n");
	}
	/** 홀수 카운트가 0 보다 작거나 같을 경우, 홀수 출력 실패 */
	else
	{
		printf("\t| @ none\t\t: none\n");
		printf("\n\t| ! 짝수만 존재.\n\n");
	}
}

//////////////////////////////////////
// Main function
//////////////////////////////////////
/**
 * @fn int main()
 * @brief 정해진 숫자 개수 범위에 따라 입력 받은 정수들 중 홀수만 출력하는 프로그램
 * @return 프로그램 실행 성공 여부 반환. 성공 시 SUCCESS, 실패 시 FAIL 반환
 */
int main()
{
	/** 함수 실행 성공 여부를 설정할 변수 */
	int return_value = SUCCESS;

	/** 정수를 입력 받을 구조체를 할당해서 그 주소를 가지고 있는 구조체 포인터 변수 */
	input_data_t *data = (input_data_t *)malloc(sizeof(input_data_t));
	if (data == NULL)
	{
		printf("\t| ! [DEBUG] 객체 메모리 할당 실패, 프로그램 종료.\n");
		perror("\t| ! [DEBUG] malloc");
		return FAIL;
	}

	while (1)
	{
		/** 입력 진행 */
		return_value = input_numbers(data);
		switch (return_value)
		{
		case FAIL:
			printf("\t| ! 입력 실패, 프로그램 종료\n");
			break;
		case FINISH:
			printf("\t| @ 프로그램 종료\n");
			break;
		case AGAIN:
			printf("\t| ! 입력 재진행, 기존에 입력 받은 정수 모두 초기화.\n");
			continue;
		case SUCCESS:
		case EXIT:
			print_odd_numbers(data);
			break;
		/** return_value 가 해당 함수 로직에서 설정한 반환값이 아닌 경우 */
		default:
			printf("\t| ! input_data_input_numbers 함수에서 알 수 없는 반환값 발생.\n");
			break;
		}
		break;
		/** 입력 종료 */
	}

	free(data);
	return return_value;
}
