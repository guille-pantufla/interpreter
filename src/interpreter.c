#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "interpreter.h"
#include "utils.h"

void token_init(token* tok, enum token_type _type, int _value) {

	tok->type = _type;
	tok->value = _value;

}

interpreter_envoirment* ienv_init(const char * src) {

	interpreter_envoirment* ienv = calloc(1, sizeof (interpreter_envoirment));

	ienv->source_index = 0;
	ienv->source_length = strlen(src + 1);
	ienv->source_buffer = calloc(1, ienv->source_length + 1);
	strcpy(ienv->source_buffer, src);

	token_init(&(ienv->current_token), START, TVAL_NONE);
	ienv->current_char = ienv->source_buffer[0];

	return ienv;
}

void ienv_free(interpreter_envoirment* ienv) {

	free(ienv->source_buffer);
	free(ienv);

}

void ienv_advance(interpreter_envoirment*  ienv) {

	ienv->source_index++;

	if(ienv->source_index > ienv->source_length - 1)
		ienv->current_char = 0;
	else
		ienv->current_char = ienv->source_buffer[ienv->source_index];

}

void ienv_consume_whitespace(interpreter_envoirment* ienv) {

	while(ienv->current_char == ' ')
		ienv_advance(ienv);

}

int ienv_consume_integer(interpreter_envoirment* ienv) {

	int ret = 0;

	while(isdigit(ienv->current_char)) {
		ret *= 10;
		ret += char_to_byte(ienv->current_char);
		ienv_advance(ienv);
	}

	return ret;

}

token ienv_get_next_token(interpreter_envoirment* ienv) {

	token ret;

	while(ienv->current_char != 0) {

		if(ienv->current_char == ' ') {
			ienv_consume_whitespace(ienv);
			continue;
		}
		if(isdigit(ienv->current_char)) {
			token_init(&ret, INTEGER, ienv_consume_integer(ienv));
			return ret;
		}
		if(ienv->current_char == '+') {
			token_init(&ret, PLUS, TVAL_PLUS);
			ienv_advance(ienv);
			return ret;
		}
		if(ienv->current_char == '-') {
			token_init(&ret, MINUS, TVAL_MINUS);
			ienv_advance(ienv);
			return ret;
		}

		token_init(&ret, ERROR, TVAL_NONE);
		return ret;

	}

	token_init(&ret, ENDOF, TVAL_NONE);
	return ret;

}

void ienv_eat(interpreter_envoirment* ienv, enum token_type type) {

	if(ienv->current_token.type == type)
		ienv->current_token = ienv_get_next_token(ienv);
	else
		token_init(&(ienv->current_token), ERROR, TVAL_NONE);

}

int ienv_expr(interpreter_envoirment* ienv, int* result) {

	#define IEE_OK 0
	#define IEE_ERROR 1

	ienv->current_token = ienv_get_next_token(ienv);

	token left = ienv->current_token;
	ienv_eat(ienv, INTEGER);
	if(ienv->current_token.type == ERROR)
		return IEE_ERROR;

	token op = ienv->current_token;
	if(op.type == PLUS)
		ienv_eat(ienv, PLUS);
	else
		ienv_eat(ienv, MINUS);
	if(ienv->current_token.type == ERROR)
		return IEE_ERROR;

	token right = ienv->current_token;
	ienv_eat(ienv, INTEGER);
	if(ienv->current_token.type == ERROR)
		return IEE_ERROR;

	if(op.type == PLUS)
		*result = left.value + right.value;
	else
		*result = left.value - right.value;

	return IEE_OK;

}

int main(int argc, char** argv) {

	for (;;) {
		
		printf("calc> ");

		char raw_input[INTERPRETER_INPUT_MAX_SIZE];
		char* ret_str = fgets(raw_input, INTERPRETER_INPUT_MAX_SIZE, stdin);	

		if(ret_str == NULL) {
			printf("\n");
			break;
		}
		if(strlen(ret_str) == 0)
			continue;


		interpreter_envoirment* ienv = ienv_init(raw_input);
		int result;
		if(ienv_expr(ienv, &result) == IEE_ERROR) {
			printf("%s\n", INTERPRETER_INPUT_ERROR);
		} else {
			printf("%d\n", result);
		}

	}

	return 0;
}