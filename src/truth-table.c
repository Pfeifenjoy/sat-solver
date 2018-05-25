#include "sat-solver/truth-table.h"
#include "sat-solver/string-util.h"
#include "sat-solver/judgement.h"
#include "stdbool.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

#define CHAR_SIZE (sizeof(char) * 8)

size_t header_size(
		const struct formular *formular,
		char **variables,
		size_t variables_length
	) {
	size_t size = 0;
	size_t i;
	for(i = 0; i < variables_length; ++i) {
		size += 2; // "| "
		size += strlen(variables[i]);
		size += 1; // " "
	}
	size += 2; // "| "
	size += formular_stringify_get_length(formular);
	size += 3; // " |\n"
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
	buffer = append(buffer, " |\n");
	*buffer = 0;
}

void generate_bar(
		char *buffer,
		char **variables,
		size_t variables_length,
		const struct formular *formular
	) {
	size_t i;
	for(i = 0; i < variables_length; ++i) {
		buffer = append(buffer, "|-");
		buffer = print_n(buffer, '-', strlen(variables[i]));
		buffer = append(buffer, "-");
	}
	buffer = append(buffer, "|-");
	buffer = print_n(buffer, '-', formular_stringify_get_length(formular));
	buffer = append(buffer, "-|\n");
	*buffer = 0;
}

void fprint_truth_table(FILE *out, const struct formular *formular) {
	//get variables
	char **variables;
	size_t variables_length;
	formular_get_variables(formular, &variables, &variables_length);

	struct index_formular_holder holder;
	make_index_formular(&holder, formular);

	struct index_judgement index_judgement;
	index_judgement_init(&index_judgement, &holder);

	//buffer
	size_t buffer_size = header_size(formular, variables, variables_length);
	char *buffer = (char *) malloc(buffer_size * sizeof(char));

	//header
	generate_header(buffer, variables, variables_length, formular);
	fwrite(buffer, sizeof(char), buffer_size, out);
	generate_bar(buffer, variables, variables_length, formular);
	fwrite(buffer, sizeof(char), buffer_size, out);

	//body
	do {
		size_t i;
		for(i = 0; i < variables_length; ++i) {
			printf("| ");
			unsigned char j;
			for(j = 0; j < strlen(variables[i]) - 1; ++j) {
				printf(" ");
			}
			printf("%d ", index_judgement_get(&index_judgement, i));
		}
		printf("| ");
		for(size_t i = 0; i < formular_stringify_get_length(formular) - 1; ++i) {
			printf(" ");
		}
		printf("%d |\n", index_judgement_eval(&index_judgement, holder.formulars));

	} while(index_judgement_next(&index_judgement));

	free(variables);
	index_formular_free(&holder);
	index_judgement_destroy(&index_judgement);
	free(buffer);
}

void print_truth_table(const struct formular *table) {
	fprint_truth_table(stdout, table);
}
