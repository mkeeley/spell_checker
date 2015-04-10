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
static void print_dictionary(NODE *, char *);
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

static void print_dictionary(NODE *node, char *word) {
	int 	i;
	int	len;

	if(node) {
		strcat(word, &node->letter);
		len = strlen(word);
		if(node->end_of_word)	
			printf("%s\n", word);
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
	
	if(argc != 3) {
		printf("./prog <dictionary> <test file>\n");
		exit(1);
	}

	f = fopen(argv[1], "r");
	while(fscanf(f, "%s", buf) == 1) {
		parse(buf);
		root = build_dict(buf);
	}
	buf[0] = '\0';
	print_dictionary(root, buf);
	printf("\n");

	fclose(f);
	f = fopen(argv[2], "r");
	while(fscanf(f, "%s", buf) != -1) {
		printf("%s is ", buf);
		is_word(buf) ? printf("a word\n") : printf("not a word\n");
	}
	fclose(f);
	return 1;
}
