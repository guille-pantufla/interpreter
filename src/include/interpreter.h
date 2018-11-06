#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#define INTERPRETER_INPUT_MAX_SIZE 256

#define INTERPRETER_CONTINUE 0
#define INTERPRETER_EOF 1

#define INTERPRETER_INPUT_ERROR "Error parsing input"

#define TVAL_PLUS 0
#define TVAL_MINUS 1
#define TVAL_NONE 2

enum token_type {
	START = 0, 
	INTEGER = 1,
	PLUS = 2,
	MINUS = 3,
	ENDOF = 4,
	ERROR = 5
};

typedef struct {

	enum token_type type;
	int value;

} token;

void token_init(token * tok, enum token_type _type, int _value);

typedef struct {

	char* source_buffer;
	unsigned int source_length;
	unsigned int source_index;
	token current_token;
	char current_char;

} interpreter_envoirment;

interpreter_envoirment* ienv_init(const char * src);
void ienv_free(interpreter_envoirment*  ienv);

void ienv_advance(interpreter_envoirment*  ienv);
void ienv_consume_whitespace(interpreter_envoirment* ienv);
int ienv_consume_integer(interpreter_envoirment* ienv);
token ienv_get_next_token(interpreter_envoirment* ienv);
void ienv_eat(interpreter_envoirment* ienv, enum token_type type);
int ienv_expr(interpreter_envoirment* ienv, int* result);

#endif //INTERPRETER H