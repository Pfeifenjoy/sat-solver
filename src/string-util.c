#include "sat-solver/string-util.h"
#include "string.h"

char *append(char *text1, const char *text2) {
	strcpy(text1, text2);
	return text1 + strlen(text2);
}

char *print_n(char *text, char c, size_t n) {
	memset(text, c, n);
	return text + n;
}

