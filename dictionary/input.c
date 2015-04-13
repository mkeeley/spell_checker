#include "input.h" 

typedef struct node {
	char 	letter;
	char	end_of_word;
	struct node *next;
	struct node *leaves;
} NODE;
	
static NODE *root = NULL;
static NODE *build_dict(char *);
static NODE *insert_letter(NODE **, char);
static NODE *find_letter(NODE *, char);
static void print_dictionary(NODE *, char *);
static void parse(char *);
int 	is_word(char *);
void	test_dictionary(FILE *, FILE *);

/* Function:	is_word()
 *
 * Description:	Determine if input string is a word
		in the dictionary - aka is it spelled
		correctly?
 */

int is_word(char *word) {
	char 	*c = word;
	NODE 	*curr = find_letter(root, *c),
		*prev = curr;
		
	c++;
	while(*c && (curr = find_letter(curr->leaves, *c))) {
		prev = curr;
		c++;
	}
	if(prev->end_of_word)
		return 1;
	return 0;
}

/* Function:	print_dictionary()
 *
 * Description:	Print all words in the dictionary
 */

static void print_dictionary(NODE *node, char *word) {
	int	len;

	if(node) {
		strcat(word, &node->letter);
		len = strlen(word);
		if(node->end_of_word)	
			printf("\t- %s\n", word);
		print_dictionary(node->leaves, word);
		word[len - 1] = '\0';
		print_dictionary(node->next, word);
	}
}
			
/* Function:	build_dict()
 *		
 * Description:	Insert all of the letters of the given
 *		word into the dictionary
 */

static NODE *build_dict(char *word) {
	char	*src = &word[1];
	NODE 	*temp;

	temp = insert_letter(&root, *word);
	if(!root)
		root = temp;

	while(*src) 
		temp = insert_letter(&temp->leaves, *src++);
	temp->end_of_word = 1;
	return root;
}

/* Function:	find_letter()
 * 
 * Description:	Given the root and letter, find the node
 *		containing the letter and return it. If
 *		not found, return NULL
 */ 

static NODE *find_letter(NODE *node, char letter) {
	while(node) {
		if(node->letter == letter) 
			return node;
		node = node->next;
	}
	return NULL;
}

/* Function: 	insert_letter()
 *
 * Description:	Insert the given letter into the 
 * 		tree. Return the head of the list
 */

static NODE *insert_letter(NODE **node, char letter) {
	NODE 	*new,
		*cur;

	if(!*node) {
		*node = malloc(sizeof(**node));
		(*node)->next = NULL;
		(*node)->leaves = NULL;
		(*node)->end_of_word = 0;
		(*node)->letter = letter;
		return *node;
	}

	for(cur = *node; cur->next != NULL && cur->letter != letter; cur = cur->next) ;
	if(cur->letter != letter) {
		new = malloc(sizeof(*new));
		new->next = cur->next;
		cur->next = new;
		new->leaves = NULL;
		new->end_of_word = 0;
		new->letter = letter;
		return new;
	}
	return cur;
}

/* Function: parse()
 *
 * Description: Replace all uppercase characters with its
 * 		lowercase counterpart.  Then remove all
 * 		all characters that are not a-z.
 */

static void parse(char *text) {
	char	*src,
		*dst;

	src = dst = text;
	while(*src) {
		*src = tolower(*src);
		if('a' <= *src && *src <= 'z') 
			*dst++ = *src;
		src++;
	}
	*dst = '\0';
}

/* Function: test_case()
 *
 * Description:	Build a dictionary and test text against
 * 		it. It is to be called by a test driver.
 */

void test_dictionary(FILE *in, FILE *check) {
	char	buf[64];

	while(fscanf(in, "%s", buf) != EOF) {
		parse(buf);
		root = build_dict(buf);
	}
	
	buf[0] = '\0';
	printf("printing dictionary:\n");
	print_dictionary(root, buf);

	printf("words from check file:\n");
	while(fscanf(check, "%s", buf) != EOF) {
		printf("\t- %s is ", buf);
		is_word(buf) ? printf("a word\n") : printf("not a word\n");
	}
}
