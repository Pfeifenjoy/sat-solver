#include "sat-solver/formular.h"
#include "sat-solver/truth-table.h"
#include "sat-solver/judgement.h"

void test_literal_formular() {
	struct formular *formular = make_literal_formular("a");

	print_truth_table(formular);

	formular_free(formular);
}
void test_neg_formular() {
	struct formular *inner = make_literal_formular("a");
	struct formular *formular = make_neg_formular(inner);

	print_truth_table(formular);

	formular_free(formular);
}
void test_and_formular() {
	struct formular *left = make_literal_formular("a");
	struct formular *right = make_literal_formular("b");
	struct formular *formular = make_and_formular(left, right);
	print_truth_table(formular);

	formular_free(formular);
}
void test_or_formular() {
	struct formular *left = make_literal_formular("a");
	struct formular *right = make_literal_formular("b");
	struct formular *formular = make_or_formular(left, right);

	print_truth_table(formular);

	formular_free(formular);
}

void test_three_var() {
	struct formular *left = make_literal_formular("a");
	struct formular *right = make_literal_formular("b");
	struct formular *left2 = make_and_formular(left, right);
	struct formular *right2 = make_literal_formular("c");
	struct formular *formular = make_or_formular(left2, right2);

	print_truth_table(formular);

	formular_free(formular);

}

int main(const int argc, const char **argv) {
	test_literal_formular();
	test_neg_formular();
	test_and_formular();
	test_or_formular();
	test_three_var();
	return 0;
}
