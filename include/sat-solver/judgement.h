#ifndef JUDGEMENT_H_KX2RUWIV
#define JUDGEMENT_H_KX2RUWIV

#include "formular.h"
#include "trie.h"

struct judgement {
	struct trie *trie;
};

bool eval(struct judgement *, struct formular *);

#endif /* end of include guard: JUDGEMENT_H_KX2RUWIV */
