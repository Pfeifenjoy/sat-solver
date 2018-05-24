#include "sat-solver/formular.h"
#include "sat-solver/string-util.h"
#include "string.h"

#define TRUE_SYMBOL "true"
#define FALSE_SYMBOL "false"
#define NEG_SYMBOL "!"
#define AND_SYMBOL " & "
#define OR_SYMBOL " | "
#define LEFT_PARANTHESES "("
#define RIGHT_PARANTHESES ")"

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

size_t stringify_get_length(struct formular *formular) {
	switch(formular->type) {
		case VERUM_FORMULAR:
			return strlen(TRUE_SYMBOL);
		case FALSUM_FORMULAR:
			return strlen(FALSE_SYMBOL);
		case NEG_FORMULAR:
			return strlen(NEG_SYMBOL);
		case LITERAL_FORMULAR:
			return strlen(formular->value.literal);
		case AND_FORMULAR:
			return strlen(LEFT_PARANTHESES)
				+ stringify_get_length(formular->value.binary.left)
				+ strlen(AND_SYMBOL)
				+ stringify_get_length(formular->value.binary.right)
				+ strlen(RIGHT_PARANTHESES);
		case OR_FORMULAR:
			return strlen(LEFT_PARANTHESES)
				+ stringify_get_length(formular->value.binary.left)
				+ strlen(OR_SYMBOL)
				+ stringify_get_length(formular->value.binary.right)
				+ strlen(RIGHT_PARANTHESES);
	}
}

char *stringify_impl(struct formular *formular, char *output) {
	switch(formular->type) {
		case VERUM_FORMULAR:
			return append(output, "true");
		case FALSUM_FORMULAR:
			return append(output, "false");
		case NEG_FORMULAR:
			return append(output, "!");
		case LITERAL_FORMULAR:
			return append(output, formular->value.literal);
		case AND_FORMULAR:
			output = append(output, "(");
			output = stringify_impl(formular->value.binary.left, output);
			output = append(output, " & ");
			output = stringify_impl(formular->value.binary.right, output);
			return append(output, ")");
		case OR_FORMULAR:
			output = append(output, "(");
			output = stringify_impl(formular->value.binary.left, output);
			output = append(output, " | ");
			output = stringify_impl(formular->value.binary.right, output);
			return append(output, ")");
	}
}

char *stringify(struct formular *formular) {
	size_t length = stringify_get_length(formular);
	char *output = (char *) malloc((length + 1) * sizeof(char));
	stringify_impl(formular, output);
	output[length] = 0;
	return output;
}
