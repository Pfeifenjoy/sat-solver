#include "sat-solver/judgement.h"
#include "assert.h"

void test_verum_judgement() {
	//make
	struct formular *formular = make_verum_formular();

	//test
	struct judgement judgement;
	judgement_init(&judgement);
	assert(judgement_eval(&judgement, formular) == true);
	
	//free
	formular_free(formular);
	judgement_destroy(&judgement);
}

void test_falsum_judgement() {
	//make
	struct formular *formular = make_falsum_formular();

	//test
	struct judgement judgement;
	judgement_init(&judgement);
	assert(judgement_eval(&judgement, formular) == false);
	
	//free
	formular_free(formular);
	judgement_destroy(&judgement);
}

void test_literal_judgement() {
	//make
	struct formular *formular = make_literal_formular("a");

	//test
	struct judgement judgement;
	judgement_init(&judgement);
	judgement_set(&judgement, "a", false);
	assert(judgement_eval(&judgement, formular) == false);
	judgement_set(&judgement, "a", true);
	assert(judgement_eval(&judgement, formular) == true);
	
	//free
	formular_free(formular);
	judgement_destroy(&judgement);
}

void test_neg_judgement() {
	//make
	struct formular *inner = make_literal_formular("a");
	struct formular *formular = make_neg_formular(inner);

	//test
	struct judgement judgement;
	judgement_init(&judgement);
	judgement_set(&judgement, "a", false);
	assert(judgement_eval(&judgement, formular) == true);
	judgement_set(&judgement, "a", true);
	assert(judgement_eval(&judgement, formular) == false);
	
	//free
	formular_free(formular);
	judgement_destroy(&judgement);
}

void test_and_judgement() {
	//make
	struct formular *left = make_literal_formular("a");
	struct formular *right = make_literal_formular("b");
	struct formular *formular = make_and_formular(left, right);

	//test
	struct judgement judgement;
	judgement_init(&judgement);
	judgement_set(&judgement, "a", false);
	judgement_set(&judgement, "b", false);
	assert(judgement_eval(&judgement, formular) == false);
	judgement_set(&judgement, "a", true);
	judgement_set(&judgement, "b", false);
	assert(judgement_eval(&judgement, formular) == false);
	judgement_set(&judgement, "a", false);
	judgement_set(&judgement, "b", true);
	assert(judgement_eval(&judgement, formular) == false);
	judgement_set(&judgement, "a", true);
	judgement_set(&judgement, "b", true);
	assert(judgement_eval(&judgement, formular) == true);
	
	//free
	formular_free(formular);
	judgement_destroy(&judgement);
}

void test_or_judgement() {
	//make
	struct formular *left = make_literal_formular("a");
	struct formular *right = make_literal_formular("b");
	struct formular *formular = make_or_formular(left, right);

	//test
	struct judgement judgement;
	judgement_init(&judgement);
	judgement_set(&judgement, "a", false);
	judgement_set(&judgement, "b", false);
	assert(judgement_eval(&judgement, formular) == false);
	judgement_set(&judgement, "a", true);
	judgement_set(&judgement, "b", false);
	assert(judgement_eval(&judgement, formular) == true);
	judgement_set(&judgement, "a", false);
	judgement_set(&judgement, "b", true);
	assert(judgement_eval(&judgement, formular) == true);
	judgement_set(&judgement, "a", true);
	judgement_set(&judgement, "b", true);
	assert(judgement_eval(&judgement, formular) == true);
	
	//free
	formular_free(formular);
	judgement_destroy(&judgement);
}

void test_basic_judgement() {
	test_verum_judgement();
	test_falsum_judgement();
	test_literal_judgement();
	test_neg_judgement();
	test_and_judgement();
	test_or_judgement();
}

void test_verum_index_judgement() {
	//make
	struct formular *formular = make_verum_formular();
	struct index_formular_holder holder;
	make_index_formular(&holder, formular);


	//test
	struct index_judgement index_judgement;
	index_judgement_init(&index_judgement, &holder);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == true);
	
	//free
	formular_free(formular);
	index_formular_free(&holder);
}

void test_falsum_index_judgement() {
	//make
	struct formular *formular = make_falsum_formular();
	struct index_formular_holder holder;
	make_index_formular(&holder, formular);


	//test
	struct index_judgement index_judgement;
	index_judgement_init(&index_judgement, &holder);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == false);
	
	//free
	formular_free(formular);
	index_formular_free(&holder);
}

void test_literal_index_judgement() {
	//make
	struct formular *formular = make_literal_formular("a");
	struct index_formular_holder holder;
	make_index_formular(&holder, formular);


	//test
	struct index_judgement index_judgement;
	index_judgement_init(&index_judgement, &holder);
	index_judgement_set(&index_judgement, 0, true);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == true);
	index_judgement_set(&index_judgement, 0, false);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == false);
	
	//free
	formular_free(formular);
	index_formular_free(&holder);
}

void test_neg_index_judgement() {
	//make
	struct formular *inner = make_literal_formular("a");
	struct formular *formular = make_neg_formular(inner);
	struct index_formular_holder holder;
	make_index_formular(&holder, formular);


	//test
	struct index_judgement index_judgement;
	index_judgement_init(&index_judgement, &holder);
	index_judgement_set(&index_judgement, 0, true);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == false);
	index_judgement_set(&index_judgement, 0, false);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == true);
	
	//free
	formular_free(formular);
	index_formular_free(&holder);
}
void test_and_index_judgement() {
	//make
	struct formular *left = make_literal_formular("a");
	struct formular *right = make_literal_formular("b");
	struct formular *formular = make_and_formular(left, right);
	struct index_formular_holder holder;
	make_index_formular(&holder, formular);


	//test
	struct index_judgement index_judgement;
	index_judgement_init(&index_judgement, &holder);
	index_judgement_set(&index_judgement, 0, false);
	index_judgement_set(&index_judgement, 1, false);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == false);
	index_judgement_set(&index_judgement, 0, true);
	index_judgement_set(&index_judgement, 1, false);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == false);
	index_judgement_set(&index_judgement, 0, false);
	index_judgement_set(&index_judgement, 1, true);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == false);
	index_judgement_set(&index_judgement, 0, true);
	index_judgement_set(&index_judgement, 1, true);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == true);
	
	//free
	formular_free(formular);
	index_formular_free(&holder);
}
void test_or_index_judgement() {
	//make
	struct formular *left = make_literal_formular("a");
	struct formular *right = make_literal_formular("b");
	struct formular *formular = make_or_formular(left, right);
	struct index_formular_holder holder;
	make_index_formular(&holder, formular);


	//test
	struct index_judgement index_judgement;
	index_judgement_init(&index_judgement, &holder);
	index_judgement_set(&index_judgement, 0, false);
	index_judgement_set(&index_judgement, 1, false);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == false);
	index_judgement_set(&index_judgement, 0, true);
	index_judgement_set(&index_judgement, 1, false);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == true);
	index_judgement_set(&index_judgement, 0, false);
	index_judgement_set(&index_judgement, 1, true);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == true);
	index_judgement_set(&index_judgement, 0, true);
	index_judgement_set(&index_judgement, 1, true);
	assert(index_judgement_eval(&index_judgement, holder.formulars) == true);
	
	//free
	formular_free(formular);
	index_formular_free(&holder);
}

void test_index_judgement() {
	test_verum_index_judgement();
	test_falsum_index_judgement();
	test_literal_index_judgement();
	test_neg_index_judgement();
	test_and_index_judgement();
	test_or_index_judgement();
}

int main(const int argc, const char **argv) {
	test_basic_judgement();
	test_index_judgement();
}
