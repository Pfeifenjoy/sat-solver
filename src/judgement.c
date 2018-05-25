#include "sat-solver/judgement.h"

#define CHAR_SIZE (sizeof(char) * 8)

void judgement_init(struct judgement *judgement) {
	judgement->lookup = make_trie();
}

void judgement_destroy(struct judgement *judgement) {
	trie_free(judgement->lookup);
}

void judgement_set(struct judgement *judgement, const char *literal, bool value) {
	trie_set(judgement->lookup, literal, value);
}

bool judgement_get(const struct judgement *judgement, const char *literal) {
	return trie_search(judgement->lookup, literal) != 0;
}

bool judgement_eval(const struct judgement *judgement, const struct formular *formular) {
	switch(formular->type) {
		case VERUM_FORMULAR:
			return true;
		case FALSUM_FORMULAR:
			return false;
		case LITERAL_FORMULAR:
			return judgement_get(judgement, formular->value.literal);
		case AND_FORMULAR:
			return judgement_eval(judgement, formular->value.binary.left)
				&& judgement_eval(judgement, formular->value.binary.right);
		case OR_FORMULAR:
			return judgement_eval(judgement, formular->value.binary.left)
				|| judgement_eval(judgement, formular->value.binary.right);
		case NEG_FORMULAR:
			return !judgement_eval(judgement, formular->value.neg);
	}
}

void index_judgement_init(
		struct index_judgement *judgement,
		struct index_formular_holder *holder
	) {
	judgement->variables_length = index_formular_holder_highest_id(holder) + 1;
	judgement->values_length = judgement->variables_length / CHAR_SIZE + 1;
	judgement->values = calloc(judgement->values_length, sizeof(char));
}
void index_judgement_destroy(struct index_judgement *judgement) {
	free(judgement->values);
}
void index_judgement_set(struct index_judgement *judgement, size_t index, bool value) {
	size_t i = index / CHAR_SIZE;
	char mask = 1 << (index - i * 8);
	if(value) {
		judgement->values[i] |= mask;
	} else {
		judgement->values[i] &= ~mask;
	}
}

bool index_judgement_get(const struct index_judgement *judgement, size_t index) {
	size_t i = index / CHAR_SIZE;
	char mask = 1 << (index - i * 8);
	return (judgement->values[i] & mask) != 0;
}

bool index_judgement_eval(
		const struct index_judgement *judgement,
		const struct index_formular *formular
	) {
	switch(formular->type) {
		case VERUM_FORMULAR:
			return true;
		case FALSUM_FORMULAR:
			return false;
		case LITERAL_FORMULAR:
			return index_judgement_get(judgement, formular->value.literal);
		case NEG_FORMULAR:
			return !index_judgement_eval(judgement, formular->value.neg);
		case AND_FORMULAR:
			return index_judgement_eval(judgement, formular->value.binary.left)
				&& index_judgement_eval(judgement, formular->value.binary.right);
		case OR_FORMULAR:
			return index_judgement_eval(judgement, formular->value.binary.left)
				|| index_judgement_eval(judgement, formular->value.binary.right);
	}
}

bool index_judgement_next(struct index_judgement *judgement) {
	//increment sub configurations
	//thus the first n - 1 variables starting from the back
	size_t i;
	for(i = 0; i < judgement->values_length - 1; ++i) {
		if((++judgement->values[judgement->values_length - 1 - i]) != 0) {
			goto DONE;
		}
	}

	//the last (n) sub configuration must be treated seperatly
	unsigned char offset = judgement->variables_length % CHAR_SIZE;
	char mask = (1 << offset) - 1;
	judgement->values[0] = ((++judgement->values[0]) & mask);

	//check if increment is still possible
	return judgement->values[0] != 0;

DONE:;
		 //can still increment
		 return true;
}
