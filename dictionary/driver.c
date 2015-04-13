#include "input.h"
#define TEST_PATH "../tests/"

void drive_time(char *arg) {
	struct timeval start, end;
	FILE 	*f_src,
		*f_in,
		*f_check;
	char 	in[64],
		check[64],
		path_in[128],
		path_check[128];

	f_src = fopen(arg, "r");
	while(fscanf(f_src, "%s %s", in, check) != EOF) {
		char path_in[128] = TEST_PATH;
		char path_check[128] = TEST_PATH;

		strcat(path_in, in);
		strcat(path_check, check);
		printf("testing: %s against %s\n", path_check, path_in);
		
		f_in = fopen(path_in, "r");
		f_check = fopen(path_check, "r");
	
		gettimeofday(&start, NULL);
		test_dictionary(f_in, f_check);
		gettimeofday(&end, NULL);
		printf("time (microsec): %ld\n", ((end.tv_sec * 1000000 + end.tv_usec) \
			- (start.tv_sec * 1000000 + start.tv_usec)));
		fclose(f_in);
		fclose(f_check);
	}
	fclose(f_src);
}

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("./prog <master list>\n");
		exit(1);
	}
	drive_time(argv[1]);
	return 1;
}
