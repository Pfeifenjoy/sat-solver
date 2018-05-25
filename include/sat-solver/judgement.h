#ifndef JUDGEMENT_H_KX2RUWIV
#define JUDGEMENT_H_KX2RUWIV

#include "sat-solver/formular.h"
#include "trie/trie.h"

struct judgement {
	struct trie *lookup;
};

void judgement_init(struct judgement *);
void judgement_destroy(struct judgement *);
void judgement_set(struct judgement *judgement, const char *literal, bool value);
bool judgement_get(const struct judgement *judgement, const char *literal);
bool judgement_eval(const struct judgement *, const struct formular *);

struct index_judgement {
	char *values;
	size_t values_length;
	size_t variables_length;
};

void index_judgement_init(struct index_judgement *, struct index_formular_holder *);
void index_judgement_destroy(struct index_judgement *);
void index_judgement_set(struct index_judgement *, size_t, bool);
bool index_judgement_get(const struct index_judgement *, size_t);
bool index_judgement_eval(
	const struct index_judgement *,
	const struct index_formular *
);
bool index_judgement_next(struct index_judgement *);

#endif /* end of include guard: JUDGEMENT_H_KX2RUWIV */
