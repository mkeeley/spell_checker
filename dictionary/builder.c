#include "input.h"
#include "builder.h"

#include <errno.h>
#define THREADS 2

static FILE*	src;

static void *construct(void *arg) {
	NODE 	*node = (NODE *) arg;
	int	total = 0;
	char 	*buffer = NULL;
	char	*token;
	char	word[64];
	char	delim[3] = " \n";
	char	buf[64];
	size_t	size;

	while(fscanf(src, "%s", buf) != EOF) {
		parse(buf);
		node = build_dict(node, buf);
		total++;
	}	
/*
	while(getline(&buffer, &size, src) != -1) {
		token = strtok(buffer, delim);
		while(token != NULL) {
			parse(token);
			node = build_dict(node, token);
			total++;
			token = strtok(NULL, delim);
		}	
	}
*/
	printf("thread: inserted %d words\n", total);
	pthread_exit(NULL);
}

void spawn_threads(FILE* in) {
	int	i;
	pthread_t pid[THREADS];
	NODE 	*nodes[THREADS] = {NULL};

	src = in;
	printf("splitting threads\n");
	for(i = 0; i < THREADS; i++) 
		pthread_create(&pid[i], NULL, construct, (void *) nodes[i]);
	for(i = 0; i < THREADS; i++)
		pthread_join(pid[i], NULL);
	printf("joining threads\n");
}
