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
void 	hamming(char *, char *, int, int, NODE *);

/* Function:	is_word()
 *
 * Description:	Determine if string resides in the
 *		dictionary - is word spelled correctly?
 */

int is_word(char *word) {
	char 	*c = &word[1];
	NODE 	*curr = find_letter(root, *word),
		*prev = curr;

	if(!curr)
		return 0;
	while(*c && (curr = find_letter(curr->leaves, *c))) {
		prev = curr;
		c++;
	}
	if(!prev->end_of_word || !curr)
		return 0;
	return 1;
}

/* Function:	print_dictionary()
 *
 * Description:	Print all words in the dictionary
 */

static void print_dictionary(NODE *node, char *word) {
	int	len;

	if(node) {
		len = strlen(word);
		strncat(word, &node->letter, 1);
		if(node->end_of_word)	
			printf("\t- %s\n", word);
		print_dictionary(node->leaves, word);
		word[len] = '\0';
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
	char	buf[64],
		suggestion[64];
	int	tot = 0,
		errors = 0;

	while(fscanf(in, "%s", buf) != EOF) {
		parse(buf);
		root = build_dict(buf);
		tot++;
	}	

	printf("%d words inserted\n", tot);
	memset(buf, 0, sizeof(buf));
	memset(suggestion, 0, sizeof(suggestion));
	tot = 0;

	// temp, print all words
	//print_dictionary(root, buf);
	//memset(buf, 0, sizeof(buf));

	while(fscanf(check, "%s", buf) != EOF) {
		parse(buf);
		if(!is_word(buf)) {
			errors++;
			printf("- %s not found\n", buf);
			printf("suggestions:\n");
			hamming(buf, suggestion, 0, strlen(buf), root);
		}
		tot++;
	}
	printf("%d words searched\n", tot);
	printf("%d errors found\n", errors);
}

// hamming distance for mispelled words of similar length (+/- 2)

void hamming(char *wrong, char *new, int curr, int org_len, NODE *node) {
	int 	len;

	if(node && *wrong && curr < HAM_DIST) {
		len = strlen(new);
		strncat(new, &node->letter, 1);
		if(node->letter != *wrong) {
			if(node->end_of_word && len + 1 == org_len && curr + 1 < HAM_DIST) 
				printf("\t%s\n", new);
			hamming(wrong + 1, new, curr + 1, org_len, node->leaves);
		}
		else {
			if(node->end_of_word && len + 1 == org_len && curr < HAM_DIST) 
				printf("\t%s\n", new);
			hamming(wrong + 1, new, curr, org_len, node->leaves);
		}
		new[len] = '\0';
		hamming(wrong, new, curr, org_len, node->next);
	}
}
