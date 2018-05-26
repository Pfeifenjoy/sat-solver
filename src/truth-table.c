#include "sat-solver/truth-table.h"
#include "sat-solver/string-util.h"
#include "sat-solver/judgement.h"
#include "stdbool.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

#define CHAR_SIZE (sizeof(char) * 8)

size_t header_size(
		const unsigned char *spaces,
		size_t spaces_length
	) {
	size_t size = 0;
	size_t i;
	for(i = 0; i < spaces_length; ++i) {
		size += 2; // "| "
		size += spaces[i] + 1;
		size += 1; // " "
	}
	size += 2; // "|\n"
	size += 1; // zero byte
	return size;
}

void generate_header(
		char *buffer,
		char **variables,
		size_t variables_length,
		const struct formular *formular
	) {
	size_t i;
	for(i = 0; i < variables_length; ++i) {
		buffer = append(buffer, "| ");
		buffer = append(buffer, variables[i]);
		buffer = append(buffer, " ");
	}
	buffer = append(buffer, "| ");
	char *formular_text = formular_stringify(formular);
	buffer = append(buffer, formular_text);
	free(formular_text);
	memcpy(buffer, " |\n", 4);
}

void generate_bar(
		char *buffer,
		unsigned char *spaces,
		size_t spaces_length
	) {
	size_t i;
	for(i = 0; i < spaces_length; ++i) {
		buffer = append(buffer, "|");
		buffer = print_n(buffer, '-', spaces[i] + 3);
	}
	memcpy(buffer, "|\n", 3);
}

void get_spaces(
		const struct formular *formular,
		char **variables,
		size_t variables_length,
		unsigned char **spaces,
		size_t *spaces_length
	) {
	*spaces_length = variables_length + 1;
	*spaces = malloc(*spaces_length * sizeof(unsigned char));

	size_t i;
	for(i = 0; i < variables_length; ++i) {
		(*spaces)[i] = strlen(variables[i]) - 1;
	}

	(*spaces)[*spaces_length - 1] = formular_stringify_get_length(formular) - 1;
}

void fprint_truth_table_body(
		FILE *out,
		char *buffer,
		size_t buffer_size,
		const struct index_formular *formular,
		struct index_judgement *judgement,
		unsigned char *spaces,
		size_t spaces_length
	) {
	do {
		size_t i = 0;
		size_t position = 0;
		for(i = 0; i < spaces_length - 1; ++i) {
			buffer[position++] = '|';
			memset(buffer + position, ' ', spaces[i] + 1);
			position += spaces[i] + 1;
			buffer[position++] = index_judgement_get(judgement, i) ? '1' : '0';
			buffer[position++] = ' ';
		}
		buffer[position++] = '|';
		memset(buffer + position, ' ', spaces[spaces_length - 1] + 1);
		position += spaces[spaces_length - 1] + 1;
		buffer[position++] = index_judgement_eval(judgement, formular)
			? '1' : '0';
		memcpy(buffer + position, " |\n", 4);
		fwrite(buffer, sizeof(char), buffer_size, out);
	} while(index_judgement_next(judgement));
}

void fprint_truth_table(FILE *out, const struct formular *formular) {
	//get variables
	char **variables;
	size_t variables_length;
	formular_get_variables(formular, &variables, &variables_length);

	//spaces
	unsigned char *spaces;
	size_t spaces_length;
	get_spaces(formular, variables, variables_length, &spaces, &spaces_length);

	//buffer
	size_t buffer_size = header_size(spaces, spaces_length);
	char *buffer = (char *) malloc(buffer_size * sizeof(char));

	//formular / judgement
	struct index_formular_holder holder;
	make_index_formular(&holder, formular);
	struct index_judgement index_judgement;
	index_judgement_init(&index_judgement, &holder);

	//header
	generate_header(buffer, variables, variables_length, formular);
	fwrite(buffer, sizeof(char), buffer_size, out);
	generate_bar(buffer, spaces, spaces_length);
	fwrite(buffer, sizeof(char), buffer_size, out);

	//body
	fprint_truth_table_body(out, buffer, buffer_size, holder.formulars,
			&index_judgement, spaces, spaces_length);

	//free
	free(variables);
	free(spaces);
	free(buffer);
	index_formular_free(&holder);
	index_judgement_destroy(&index_judgement);
}

void print_truth_table(const struct formular *table) {
	fprint_truth_table(stdout, table);
}
