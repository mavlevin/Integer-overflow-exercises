#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOL int
#define TRUE (1)
#define FALSE (0)


char *instructions =	"Stanford student, your goal is to NOT get a F. "
						"This program expects a the number of programs you want to grade that print 'hello world'."
						" It then calculates a score for all the files together. "
						"The smaller your hello world code, the higher your score.\nYOU ARE NOW IN HARD MODE. 8)\n"
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

char* size_to_letter_grade(int size) {
	printf("your file size: %d\n", size);
	if (size > 100) {
		return "D";
	}

	if (size >= 20) {
		return "C";
	}

	if (size > 15) {
		return "B";
	}

	if (size >= 0) {
		return "A (not A+)";
	}

	return "A+";
}

int get_file_size(char * path) {
	FILE* fp = fopen(path, "r");
	
	if (fp == NULL) {
		printf("failed opening '%s'\n", path);
		failed("couldn't open file");
	}

	if (0 != fseek(fp, 0L, SEEK_END)) {
		failed("error fseek'ing to end of file");
		return 0;		
	}

	int rv = ftell(fp);
	fclose(fp);
	if (rv < 2 || rv > 1000) {
		printf("hacker detected and averted! Why don't you use a simple print hello world :P\n");
		return 1;
	}
	return rv;
}

char * ask_user_path() {
	return "example_hello.py";
}

long long sum_arr(long long * arr, long long arr_size) {
	if (arr_size < 0) {
		// not sure what to do
		return -1;
	}
	long long sum = 0;
	for (long long i = 0; i < arr_size; ++i)
	{
		sum += arr[i];
	}
	return sum;
}

char* grade_files(int file_count_to_grade) {
	char * file_grade_array;
	int bytes_per_file_to_grade = 0x100; // make sure hacker can't overflow!
	file_grade_array = malloc(file_count_to_grade * bytes_per_file_to_grade);
	if (NULL == file_grade_array) {
		failed("failed to allocate buffer big enough for grades");
	}

	for (int i = 0; i < file_count_to_grade; ++i)
	{
		// ask for file name to grade
		char * current_graded_file_path = ask_user_path();
		file_grade_array[i] = get_file_size(current_graded_file_path);
	}

	// calculate grade according to sum + number of files
	long long sum;
	sum = sum_arr(file_grade_array, file_count_to_grade);
	long long final_score;
	final_score = sum + file_count_to_grade;

	return size_to_letter_grade(final_score);
}

int main(int argc, char *argv[]){
	printf("%s", instructions);

	printf("\nThere are 2 ways to solve this challenge. Make sure to find both before conitnuing.\n");
	if (argc != 2) {
		failed("didn't get number as argument to file");
	}

	int files_to_grade = atoi(argv[1]);
	if (files_to_grade < 1) {
		failed("need the number of files to grade to be at least 1");
	}

	char *grade = grade_files(files_to_grade);

	printf("Your grade: %s\n", grade);

	return 0;
}
