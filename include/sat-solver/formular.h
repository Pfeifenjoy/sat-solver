#ifndef FORMULAR_H_AJLRUDIE
#define FORMULAR_H_AJLRUDIE

#include "stdlib.h"
#include "stdbool.h"
#include "trie/trie.h"

enum formular_type {
	VERUM_FORMULAR,
	FALSUM_FORMULAR,
	LITERAL_FORMULAR,
	AND_FORMULAR,
	OR_FORMULAR,
	NEG_FORMULAR
};

struct formular {
	enum formular_type type;
	union {
		char *literal;

		struct {
			struct formular *left;
			struct formular *right;
		} binary;

		struct formular *neg;
	} value;
};

struct formular *make_verum_formular();
struct formular *make_falsum_formular();
struct formular *make_literal_formular(char *);
struct formular *make_neg_formular(struct formular *);
struct formular *make_and_formular(struct formular *, struct formular *);
struct formular *make_or_formular(struct formular *, struct formular *);

void formular_free(struct formular *);
size_t formular_stringify_get_length(const struct formular *);
char *formular_stringify(const struct formular *);
void formular_get_variables(
	const struct formular *,
	char ***variables,
	size_t *variables_length
);

struct index_formular {
	enum formular_type type;
	union {
		size_t literal;

		struct {
			struct index_formular *left;
			struct index_formular *right;
		} binary;

		struct index_formular *neg;
	} value;
};

struct index_formular_holder {
	struct trie *lookup;
	size_t index_formular_length;
	struct index_formular *formulars;
};


void make_index_formular(
	struct index_formular_holder *holder,
	const struct formular *formular
);
void index_formular_free(struct index_formular_holder *);
void get_literal_names(
	const struct formular *formular,
	char **variables,
	size_t *variable_length
);

size_t index_formular_holder_highest_id(const struct index_formular_holder *);

#endif /* end of include guard: FORMULAR_H_AJLRUDIE */
