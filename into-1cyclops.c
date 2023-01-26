#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOL int
#define TRUE (1)
#define FALSE (0)


char *instructions =	"Stanford student, your goal is to get an A. "
						"This program expects a path to a python program that prints 'hello world' and then a newline (default print behaviour). "
						"The smaller your hello world code, the higher your score.\n"
						"Good luck!\n";

char *target = "hello world\n";

void failed(char * err);


BOOL file_prints_hello_world(char * path) {
	FILE *fp;
	char exec_path[0x1000] = {0};
	char output[0x100] = {0};

	sprintf(exec_path,"python3 %s", path);

	/* pretend we're executing the file in a sandbox :) */
	fp = popen(exec_path, "r");
	if (fp == NULL) {
		failed("couldn't run your file with python");
	}

	fgets(output, sizeof(output), fp);
	printf("your program output: %s\n", output);

	/* close */
	pclose(fp);

	if (strlen(output) != strlen(target)) {
		printf("your propgram output isn't right\n");
		return FALSE;
	}

	if (0 != strcmp(output, target)) {
		printf("incorrect program output.\n");
		return FALSE;
	}

	return TRUE;
}

void failed(char * err) {
	printf("failed because '%s'.\n", err);
	printf("Your grade: F\n");
	exit(1);
}

char size_to_letter_grade(int size) {
	printf("your file size: %d\n", size);

	if (size < 5) {
		return 'A';
	}

	if (size <= 15) {
		return 'B';
	}

	if (size <= 20) {
		return 'C';
	}

	if (size < 100) {
		return 'D';
	}

	return 'F';

}

char get_file_size(char * path) {
	FILE* fp = fopen(path, "r");
	
	if (fp == NULL) {
		failed("couldn't open file");
	}

	if (0 != fseek(fp, 0L, SEEK_END)) {
		failed("error fseek'ing to end of file");
		return 0;		
	}

	return ftell(fp);
}

char grade_file(char* path) {
	if (!file_prints_hello_world(path)) {
		failed("file doesn't print hello world");
	}

	return size_to_letter_grade(get_file_size(path));
}

int main(int argc, char *argv[]){
	printf("%s", instructions);

	if (argc != 2) {
		failed("didn't get path as argument to file");
	}

	char grade = grade_file(argv[1]);

	printf("Your grade: %c\n", grade);

	return 0;
}
