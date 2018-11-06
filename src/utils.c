#include <assert.h>

#include "utils.h"

unsigned char char_to_byte(char c) {
	assert(c >= '0' && c <= '9');
	return c - '0';
}