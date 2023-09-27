#ifndef CL_STRLIB_H
#define CL_STRLIB_H

#include "cl-strlib.c"

/*!
 * @function	cl_strlen
 * @discussion	 0 -> success
 *				-1 -> stopped by MAX_ITER
 * @param		str			Source string
 * @param		result		Integer length returned by reference
 * @result		Returns the length of a string
**/
int cl_strlen(char* str);

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
int cl_substr(char* str, char* result, int startIndex, int length);

/*!
 * @function	cl_hstint
 * @discussion	Requires '0x' prefix on numbers, since
 *				they are hex values. Stops converting
 *				at first non hex digit after the prefix
 *				 0 -> success
 *				-1 -> str and/or result don't exist
 *				-2 -> length error (length < 3)
 *				-3 -> missing 0x prefix
 * @param		str			Source string
 * @param		result		Converted value as an int
 * @result		Takes in a string of a hex number and returns
 *				by reference its equivalent value as an int
**/
int cl_hstint(char* str, int* result);

/*!
 * @function	cl_strtest
 * @discussion	Creates char[5] and tests each element
 *				for equality with the null byte, printing
 *				results as it tests
 * @result		Void
**/
void cl_strtest(void);
#endif