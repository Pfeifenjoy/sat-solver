#include "sat-solver/formular.h"
#include "sat-solver/string-util.h"
#include "string.h"
#include "assert.h"

#define TRUE_SYMBOL "true"
#define FALSE_SYMBOL "false"
#define NEG_SYMBOL "!"
#define AND_SYMBOL " & "
#define OR_SYMBOL " | "
#define LEFT_PARANTHESES "("
#define RIGHT_PARANTHESES ")"

struct formular *malloc_formular() {
	return (struct formular *) malloc(sizeof(struct formular));
}
struct formular *make_verum_formular() {
	struct formular *formular = malloc_formular();
	formular->type = VERUM_FORMULAR;
	return formular;
}
struct formular *make_falsum_formular() {
	struct formular *formular = malloc_formular();
	formular->type = FALSUM_FORMULAR;
	return formular;
}
struct formular *make_literal_formular(char *name) {
	struct formular *formular = malloc_formular();
	formular->type = LITERAL_FORMULAR;
	formular->value.literal = name;
	return formular;
}
struct formular *make_neg_formular(struct formular *neg) {
	struct formular *formular = malloc_formular();
	formular->type = NEG_FORMULAR;
	formular->value.neg = neg;
	return formular;
}
struct formular *make_binary_formular(
		size_t type,
		struct formular *left,
		struct formular *right
	) {
	struct formular *formular = malloc_formular();
	formular->type = type;
	formular->value.binary.left = left;
	formular->value.binary.right = right;
	return formular;
}
struct formular *make_and_formular(struct formular *left, struct formular *right) {
	return make_binary_formular(AND_FORMULAR, left, right);
}
struct formular *make_or_formular(struct formular *left, struct formular *right) {
	return make_binary_formular(OR_FORMULAR, left, right);
}

void formular_free(struct formular *formular) {
	switch(formular->type) {
		case VERUM_FORMULAR:
		case FALSUM_FORMULAR:
		case LITERAL_FORMULAR:
		case NEG_FORMULAR:
			break;
		case AND_FORMULAR:
		case OR_FORMULAR:
			formular_free(formular->value.binary.left);
			formular_free(formular->value.binary.right);
			break;
	}
	free(formular);
}

size_t formular_stringify_get_length(const struct formular *formular) {
	switch(formular->type) {
		case VERUM_FORMULAR:
			return strlen(TRUE_SYMBOL);
		case FALSUM_FORMULAR:
			return strlen(FALSE_SYMBOL);
		case LITERAL_FORMULAR:
			return strlen(formular->value.literal);
		case NEG_FORMULAR:
			return strlen(NEG_SYMBOL)
				+ formular_stringify_get_length(formular->value.neg);
		case AND_FORMULAR:
			return strlen(LEFT_PARANTHESES)
				+ formular_stringify_get_length(formular->value.binary.left)
				+ strlen(AND_SYMBOL)
				+ formular_stringify_get_length(formular->value.binary.right)
				+ strlen(RIGHT_PARANTHESES);
		case OR_FORMULAR:
			return strlen(LEFT_PARANTHESES)
				+ formular_stringify_get_length(formular->value.binary.left)
				+ strlen(OR_SYMBOL)
				+ formular_stringify_get_length(formular->value.binary.right)
				+ strlen(RIGHT_PARANTHESES);
	}
}

char *formular_stringify_impl(const struct formular *formular, char *output) {
	switch(formular->type) {
		case VERUM_FORMULAR:
			return append(output, "true");
		case FALSUM_FORMULAR:
			return append(output, "false");
		case LITERAL_FORMULAR:
			return append(output, formular->value.literal);
		case NEG_FORMULAR:
			output = append(output, "!");
			return formular_stringify_impl(formular->value.neg, output);
		case AND_FORMULAR:
			output = append(output, "(");
			output = formular_stringify_impl(formular->value.binary.left, output);
			output = append(output, " & ");
			output = formular_stringify_impl(formular->value.binary.right, output);
			return append(output, ")");
		case OR_FORMULAR:
			output = append(output, "(");
			output = formular_stringify_impl(formular->value.binary.left, output);
			output = append(output, " | ");
			output = formular_stringify_impl(formular->value.binary.right, output);
			return append(output, ")");
	}
}

char *formular_stringify(const struct formular *formular) {
	size_t length = formular_stringify_get_length(formular);
	char *output = (char *) malloc((length + 1) * sizeof(char));
	formular_stringify_impl(formular, output);
	output[length] = 0;
	return output;
}

bool variable_exists(char *variable, char **variables, size_t variable_length) {
	size_t i;
	for(i = 0; i < variable_length; ++i) {
		if(variables[i] == variable) {
			return true;
		}
	}
	return false;
}

size_t count_max_variables(const struct formular *formular) {
	switch(formular->type) {
		case VERUM_FORMULAR:
		case FALSUM_FORMULAR:
			return 0;
		case LITERAL_FORMULAR:
			return 1;
		case AND_FORMULAR:
		case OR_FORMULAR:
			return	count_max_variables(formular->value.binary.left)
					+ count_max_variables(formular->value.binary.right);
		case NEG_FORMULAR:
			return count_max_variables(formular->value.neg);
	};
}

void get_literal_names_impl(
		const struct formular *formular,
		char **variables,
		size_t *variable_length
	) {
	switch(formular->type) {
		case VERUM_FORMULAR:
		case FALSUM_FORMULAR:
			return;
		case LITERAL_FORMULAR:
			if(!variable_exists(formular->value.literal, variables, *variable_length)) {
				variables[*variable_length] = formular->value.literal;
				*variable_length += 1;
			} else {
				/* ignore */
			}
			break;
		case AND_FORMULAR:
		case OR_FORMULAR:
			get_literal_names_impl(formular->value.binary.left, variables, variable_length);
			get_literal_names_impl(formular->value.binary.right, variables, variable_length);
			break;
		case NEG_FORMULAR:
			get_literal_names_impl(formular->value.neg, variables, variable_length);
			break;
	}
}

void formular_get_variables(
		const struct formular *formular,
		char ***variables,
		size_t *variables_length
	) {
	size_t max_variables = count_max_variables(formular);
	*variables = (char **) malloc(max_variables * sizeof(char *));
	*variables_length = 0;
	get_literal_names_impl(formular, *variables, variables_length);
	assert(*variables_length <= max_variables);
	if(*variables_length != max_variables) {
		*variables = (char **) realloc(*variables, *variables_length * sizeof(char *));
	}
}

size_t count_formulars(const struct formular *formular) {
	switch(formular->type) {
		case AND_FORMULAR:
		case OR_FORMULAR:
			return count_formulars(formular->value.binary.left)
				+ count_formulars(formular->value.binary.right)
				+ 1;
		case NEG_FORMULAR:
			return count_formulars(formular->value.neg) + 1;
		default:
			return 1;
	}
}


void convert_to_index(
		const struct formular *formular,
		struct index_formular_holder *holder,
		struct index_formular **current,
		size_t *lookup_count
	) {
	(*current)->type = formular->type;
	struct index_formular *old = *current;
	switch((*current)->type) {
		case VERUM_FORMULAR:
		case FALSUM_FORMULAR:
			break;
		case LITERAL_FORMULAR:
			(*current)->value.literal = trie_search(holder->lookup, formular->value.literal);
			if((*current)->value.literal == -1) {
				trie_set(holder->lookup, formular->value.literal, *lookup_count);
				(*current)->value.literal = *lookup_count;
				*lookup_count += 1;
			}
			break;
		case NEG_FORMULAR:
			(*current)->value.neg = *current + 1;
			*current = *current + 1;
			convert_to_index(formular->value.neg, holder, current, lookup_count);
			break;
		case AND_FORMULAR:
		case OR_FORMULAR:
			old->value.binary.left = *current + 1;
			*current = *current + 1;
			convert_to_index(formular->value.binary.left, holder, current, lookup_count);
			old->value.binary.right = *current + 1;
			*current = *current + 1;
			convert_to_index(formular->value.binary.right, holder, current, lookup_count);
			break;
	}
}

void make_index_formular(
		struct index_formular_holder *holder,
		const struct formular *formular
	) {
	holder->lookup = make_trie();
	holder->index_formular_length = count_formulars(formular);
	holder->formulars = malloc(holder->index_formular_length * sizeof(struct index_formular));
	size_t lookup_count = 0;
	struct index_formular *current = holder->formulars;
	convert_to_index(formular, holder, &current, &lookup_count);
}

void index_formular_free(struct index_formular_holder *holder) {
	trie_free(holder->lookup);
	free(holder->formulars);
}

size_t index_formular_holder_highest_id(const struct index_formular_holder * holder) {
	size_t max = 0;
	for(size_t i = 0; i < holder->index_formular_length; ++i) {
		max = holder->formulars[i].type == LITERAL_FORMULAR
			&& holder->formulars[i].value.literal > max ?
			holder->formulars[i].value.literal : max;
	}
	return max;
}
