#ifndef CL_STRLIB_C
#define CL_STRLIB_C

#include <stdio.h>
#include "cl-strlib.h"

#define MAX_ITER 100

int cl_strlen (char str[], int* result) {
	if (!str) { return -1; } // existence check
	char* i = &str[0];
	// MAX_ITER for some protection against missing null terminators ('\0')
	while (*i != '\0' && (i - &str[0] + 1) < MAX_ITER) {
		i++;
	}
	
	if (*i != '\0' && (i - &str[0]) == MAX_ITER) {
		// the token case when it is stopped by MAX_ITER
		*result = MAX_ITER;
		return -1;
	}
	
	*result = (i - &str[0]);
	return 0;
}

int cl_substr (char str[], char result[], int startIndex, int length) {
	if (!result || !str) { return -1; }
	if (startIndex < 0 || length < 0) { return -2; }
	int endIndex = startIndex + length - 1;
	
	char* i = &str[0];
	char* j = &result[0];
	
	// get to startIndex
	while (*i != '\0' && (i - &str[0]) < startIndex) {
		i++;
	}
	
	if (*i == '\0') { return -2; } // couldn't get there
	
	// (i - &str[0]) is the current index
	while (*i != '\0' && (i - &str[0]) >= startIndex && (i - &str[0]) <= endIndex) {
		if (*j != '\0') {
			*j = *i;
			j++;
		}
		i++;
	}
	
	return 0;
}

int cl_hstint (char str[], int* result) {
	if (!str || !result) { return -1; } // existence check
	if (cl_strlen(str) < 3) { return -2; } // too small to hold information
	if (str[0] != '0' || str[1] != 'x') { return -3; } // no prefix
	
	*result = 0; // just to be safe
	char* i = &str[2];
	int keepLooping = 1;
	int value;
	while (*i != '\0' && keepLooping) {
		value = (*(int*) i) & 0xFF; // get character at i as if it's an int
		if (value >= 48 && value <= 57) {
			// 0-9
			value = value - 48;
		} else if (value >= 97 && value <= 102) {
			// a-f
			value = value - 87;
		} else if (value >= 65 && value <= 70) {
			// A-F
			value = value - 55;
		} else {
			// who knows what this is, dont do anything and stop
			value = -1;
			keepLooping = 0;
		}
		if (value != -1) {
			*result = *result << 4;
			*result = *result + value;
		}
		i++;
	}
	
	return 0;
}

void cl_strtest (void) {
	char test[5];
	for (int i = 0; i < 5; ++i) {
		printf("%d\n",test[i]=='\0');
	}
	
	return;
}
#endif