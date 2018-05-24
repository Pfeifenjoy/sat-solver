#ifndef TRUTH_TABLE_H_THUNDNXZ
#define TRUTH_TABLE_H_THUNDNXZ

#include "formular.h"
#include "stdio.h"

struct truth_table {
	//formular
	struct formular *formular;

	//configuration
	char *current_configuration;
	size_t configuration_length;

	//variables
	char **variables;
	size_t variables_length;
};

void get_truth_table(struct truth_table *, struct formular *);
void truth_table_destruct(struct truth_table *);
void reset_configuration(struct truth_table *);
void print_truth_table(FILE *, struct truth_table*);

#endif /* end of include guard: TRUTH_TABLE_H_THUNDNXZ */
