#include "input.h" 

typedef struct node {
	char 	letter;
	char	end_of_word;
	struct node *next;
	struct node *leaves;
} NODE;
	
static NODE *root = NULL;
static NODE *build_dict(char *);
static NODE *insert_letter(NODE *, char);
static NODE *find_letter(NODE *, char);
static void print_dictionary(NODE *);
static void parse(char *);
int 	is_word(char *);

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

static void print_dictionary(NODE *node) {
	int 	i;

	if(node) {
		printf("%c", node->letter);
		if(node->end_of_word)	
			printf("*");
		if(node->leaves)
			print_dictionary(node->leaves);
		if(node->next) {
			printf("\n");
			print_dictionary(node->next);
		}
	}
}
			
/* Function:	build_dict()
 *		
 * Description:	Insert all of the letters of the given
 *		word into the dictionary
 */

static NODE *build_dict(char *word) {
	char	*src = word;
	NODE 	*temp;

	temp = insert_letter(root, *src);
	if(!root)
		root = temp;
	temp = find_letter(root, *src);
	src++;

	while(*src) {
		temp->leaves = insert_letter(temp->leaves, *src);
		temp = find_letter(temp->leaves, *src);
		src++;
	}
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

static NODE *insert_letter(NODE *node, char letter) {
	NODE 	*new;
	NODE	*cur;

	if(!node) {
		node = malloc(sizeof(*node));
		node->next = NULL;
		node->leaves = NULL;
		node->end_of_word = 0;
		node->letter = letter;
		return node;
	}

	for(cur = node; cur->next != NULL && cur->letter != letter; cur = cur->next) ;
	if(cur->letter != letter) {
		new = malloc(sizeof(*new));
		new->next = cur->next;
		cur->next = new;
		new->leaves = NULL;
		new->end_of_word = 0;
		new->letter = letter;
	}
	return node;
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

int main(int argc, char **argv) {
	FILE	*f;
	char	buf[128];
	size_t	len = 0;
	
	f = fopen(argv[1], "r");
	while(fscanf(f, "%s", buf) == 1) {
		parse(buf);
		root = build_dict(buf);
	}
	print_dictionary(root);
	printf("\n");
	fclose(f);

	return 1;
}
