#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/time.h>

#define HAM_DIST 2

int is_word(char *);
void test_dictionary(FILE *, FILE *);

#endif /* INPUT_H */
