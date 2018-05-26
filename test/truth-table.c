#include "sat-solver/formular.h"
#include "sat-solver/truth-table.h"
#include "sat-solver/judgement.h"
#include "logging/logging.h"
#include "assert.h"

void check_result(const struct formular *formular, char *expected_result_path) {
	FILE *expected_result = fopen(expected_result_path, "r");

	if(expected_result == NULL) {
		error("Could not open: \"%s\"", expected_result_path);
		exit(1);
	}

	FILE *result = tmpfile();

	if(result == NULL) {
		error("Could not create temporary file.");
		exit(1);
	}

	fprint_truth_table(result, formular);
	rewind(result);

	char ch1 = getc(expected_result);
	char ch2 = getc(result);
 
	while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) {
		ch1 = getc(expected_result);
		ch2 = getc(result);
	}

	assert(ch1 == ch2);

	fclose(expected_result);
	fclose(result);
}

void test_literal_formular() {
	struct formular *formular = make_literal_formular("a");
	check_result(formular, "literal");
	formular_free(formular);
}

void test_neg_formular() {
	struct formular *inner = make_literal_formular("a");
	struct formular *formular = make_neg_formular(inner);

	check_result(formular, "neg");

	formular_free(formular);
}

void test_and_formular() {
	struct formular *left = make_literal_formular("a");
	struct formular *right = make_literal_formular("b");
	struct formular *formular = make_and_formular(left, right);

	check_result(formular, "and");

	formular_free(formular);
}

void test_or_formular() {
	struct formular *left = make_literal_formular("a");
	struct formular *right = make_literal_formular("b");
	struct formular *formular = make_or_formular(left, right);

	check_result(formular, "or");

	formular_free(formular);
}

void test_three_var() {
	struct formular *left = make_literal_formular("a");
	struct formular *right = make_literal_formular("b");
	struct formular *left2 = make_and_formular(left, right);
	struct formular *right2 = make_literal_formular("c");
	struct formular *formular = make_or_formular(left2, right2);

	check_result(formular, "three-vars");

	formular_free(formular);

}

int main(const int argc, const char **argv) {
	test_literal_formular();
	test_neg_formular();
	test_and_formular();
	test_or_formular();
	test_three_var();
	return EXIT_SUCCESS;
}


