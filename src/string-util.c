#include "sat-solver/string-util.h"
#include "string.h"

char *append(char *text1, char *text2) {
	strcpy(text1, text2);
	return text1 + strlen(text2);
}

char *print_n(char *x, char c, size_t n) {
	size_t i;
	for(i = 0; i < n; ++i) {
		x[i] = c;
	}
	return x + n;
}

