#ifndef FORMULAR_H_AJLRUDIE
#define FORMULAR_H_AJLRUDIE

#include "stdlib.h"

struct formular {
	enum {
		VERUM_FORMULAR,
		FALSUM_FORMULAR,
		LITERAL_FORMULAR,
		AND_FORMULAR,
		OR_FORMULAR,
		NEG_FORMULAR
	} type;
	union {
		char *literal;

		struct {
			struct formular *left;
			struct formular *right;
		} binary;

		struct formular *neg;
	} value;
};

void formular_free(struct formular *);
size_t stringify_get_length(struct formular *);
char *stringify(struct formular *);


#endif /* end of include guard: FORMULAR_H_AJLRUDIE */
