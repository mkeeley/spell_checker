#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>

#define HAM_DIST 2

typedef struct node {
	char 	letter;
	char	end_of_word;
	struct node *next;
	struct node *leaves;
} NODE;
	
NODE *build_dict(NODE *, char *);
void test_dictionary(FILE *, FILE *);

#endif /* INPUT_H */
