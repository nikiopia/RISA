#ifndef CL_STRLIB_C
#define CL_STRLIB_C

#include <stdio.h>
#include "cl-strlib.h"

#define MAX_ITER 100

/*!
 * @function	cl_strlen
 * @param		str			Source string
 * @result		Returns the length of a string
**/
int cl_strlen (char str[]) {
	if (!str) { return -1; }
	char* i = &str[0];
	// some protection against missing null terminators
	while (*i != '\0' && (i - &str[0] + 1) < MAX_ITER) {
		i++;
	}
	
	return (i - &str[0]);
}

/*!
 * @function	c1_substr
 * @discussion	Requires result string is already null-terminated,
 *				 0 -> success
 *				-1 -> source and/or result don't exist
 *				-2 -> starting index out of range
 * @param		source		Source string
 * @param		result		Where the substring is saved
 * @param		startIndex	Starting index of substring
 * @param		length		Length of substring
 * @result		A substring of the input string
**/
int cl_substr (char str[], char result[], int startIndex, int length) {
	if (!result || !str) { return -1; }
	if (startIndex < 0 || length < 0) { return -2; }
	int endIndex = startIndex + length - 1;
	
	char* i = &str[0];
	char* j = &result[0];
	
	while (*i != '\0' && (i - &str[0]) < startIndex) { i++; }
	
	if (*i == '\0') { return -2; }
	
	while (*i != '\0' && (i - &str[0]) >= startIndex && (i - &str[0]) <= endIndex) {
		if (*j != '\0') {
			*j = *i;
			j++;
		}
		i++;
	}
	
	return 0;
}

/*!
 * @function	cl_hstint
 * @discussion	Requires '0x' prefix on numbers, since
 *				they are hex values. Stops converting
 *				at first non hex digit
 *				 0 -> success
 *				-1 -> str and/or result don't exist
 *				-2 -> length error (length < 3)
 *				-3 -> missing 0x prefix
 * @param		str			Source string
 * @param		result		Converted value as an int
 * @result		Takes in a string of a hex number and returns
 *				by reference its equivalent value as an int
**/
int cl_hstint (char str[], int* result) {
	if (!str || !result) { return -1; } // existence check
	if (cl_strlen(str) < 3) { return -2; } // too small to hold information
	if (str[0] != '0' || str[1] != 'x') { return -3; } // no prefix
	
	*result = 0; // just to be safe
	char* i = &str[2];
	int keepLooping = 1;
	int value;
	while (*i != '\0' && keepLooping) {
		value = (*(int*) i) & 0xFF;
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
			// who knows
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

/*!
 * @function	cl_strtest
 * @discussion	Creates char[5] and tests each element
 *				for equality with the null byte, printing
 *				results as it tests
 * @result		Void
**/
void cl_strtest (void) {
	char test[5];
	for (int i = 0; i < 5; ++i) {
		printf("%d\n",test[i]=='\0');
	}
	
	return;
}
#endif