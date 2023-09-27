#include <stdio.h>

int main (int argc, char* argv[]) {
	FILE* file_ptr;
	file_ptr = fopen("readtest.txt", "r");
	
	if (!file_ptr) {
		printf("Cannot open file.\n");
		return 0;
	}
	
	char line[80];
	while (fgets(line, 80, file_ptr)) {
		printf("%s", line); //file already has a line-feed at end of each line
	}
	
	fclose(file_ptr);
	
	return 0;
}