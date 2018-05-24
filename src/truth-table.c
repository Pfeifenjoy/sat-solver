#include "sat-solver/truth-table.h"
#include "sat-solver/string-util.h"
#include "stdbool.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

#define CHAR_SIZE (sizeof(char) * 8)

bool variable_exists(char *variable, char **variables, size_t variable_length) {
	size_t i;
	for(i = 0; i < variable_length; ++i) {
		if(variables[i] == variable) {
			return true;
		}
	}
	return false;
}

size_t count_max_variables(struct formular *formular) {
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

void get_variables(
		struct formular *formular,
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
			get_variables(formular->value.binary.left, variables, variable_length);
			get_variables(formular->value.binary.right, variables, variable_length);
			break;
		case NEG_FORMULAR:
			get_variables(formular->value.neg, variables, variable_length);
			break;
	}
}

void init_variables(struct truth_table *table) {
	size_t max_variables = count_max_variables(table->formular);
	table->variables = (char **) malloc(max_variables * sizeof(char *));
	get_variables(table->formular, table->variables, &table->variables_length);
	table->variables =
		(char **) realloc(table->variables, table->variables_length * sizeof(char *));
}

void init_configuration(struct truth_table *table) {
	table->configuration_length = table->variables_length / CHAR_SIZE;
	if(table->variables_length % CHAR_SIZE != 0) {
		++table->configuration_length;
	}
	table->current_configuration = (char *) malloc(table->configuration_length * CHAR_SIZE);
}

void get_truth_table(struct truth_table *table, struct formular *formular) {
	table->formular = formular;
	init_variables(table);
	init_configuration(table);
}

bool truth_table_increment(struct truth_table *table) {
	//increment sub configurations
	//thus the first n - 1 variables starting from the back
	size_t i;
	for(i = 0; i < table->configuration_length - 1; ++i) {
		if((++table->current_configuration[table->configuration_length - 1 - i]) != 0) {
			goto DONE;
		}
	}

	//the last (n) sub configuration must be treated seperatly
	unsigned char offset = table->variables_length % CHAR_SIZE;
	char mask = (1 << offset) - 1;
	table->current_configuration[0] = ((++table->current_configuration[0]) & mask);

	//check if increment is still possible
	return table->current_configuration[0] != 0;

DONE:;
		 //can still increment
		 return true;
}

void reset_configuration(struct truth_table *table) {
	size_t i;
	for(i = 0; i < table->configuration_length; ++i) {
		table->current_configuration[i] = 0;
	}
}

bool get_variable_value(struct truth_table *table, size_t index) {
	size_t i = index / CHAR_SIZE;
	char mask = 1 << (index % CHAR_SIZE);
	return (table->current_configuration[i] & mask) > 0;
}

size_t header_size(struct truth_table *table) {
	size_t size = 0;
	size_t i;
	for(i = 0; i < table->variables_length; ++i) {
		size += 2; // "| "
		size += strlen(table->variables[i]);
		size += 1; // " "
	}
	size += 2; // "| "
	size += stringify_get_length(table->formular);
	size += 3; // " |\n"
	size += 1; // zero byte
	return size;
}

void generate_header(struct truth_table *table, char *buffer) {
	size_t i;
	for(i = 0; i < table->variables_length; ++i) {
		buffer = append(buffer, "| ");
		buffer = append(buffer, table->variables[i]);
		buffer = append(buffer, " ");
	}
	buffer = append(buffer, "| ");
	char *formular_text = stringify(table->formular);
	buffer = append(buffer, formular_text);
	free(formular_text);
	buffer = append(buffer, " |\n");
	*buffer = 0;
}

void generate_bar(struct truth_table *table, char *buffer) {
	size_t i;
	for(i = 0; i < table->variables_length; ++i) {
		buffer = append(buffer, "|-");
		buffer = print_n(buffer, '-', strlen(table->variables[i]));
		buffer = append(buffer, "-");
	}
	buffer = append(buffer, "|-");
	buffer = print_n(buffer, '-', stringify_get_length(table->formular));
	buffer = append(buffer, "-|\n");
	*buffer = 0;
}

void print_truth_table(FILE *file, struct truth_table *table) {
	reset_configuration(table);
	size_t buffer_size = header_size(table);
	char *buffer = (char *) malloc(buffer_size * sizeof(char));
	generate_header(table, buffer);
	printf("%s", buffer);
	generate_bar(table, buffer);
	printf("%s", buffer);

	do {
		size_t i;
		for(i = 0; i < table->variables_length; ++i) {
			printf("| ");
			unsigned char j;
			for(j = 0; j < strlen(table->variables[i]) - 1; ++j) {
				printf(" ");
			}
			printf("%d ", get_variable_value(table, i));
		}
		printf("|\n");
	} while(truth_table_increment(table));
}
