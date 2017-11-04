/*************************************************************************
* Title: (CIS265_13A) File Concatenator
* File: fact.c
* Author: James Eli
* Date: 9/10/2017
*
* Write a program named fcat that “concatenates” any number of files by
* writing them to standard output, one after the other, with no break
* between files. For example, the following command will display the files
* f1.c, f2.c, and f3.c on the screen:
*   fcat f1.c f2.c f3.c
* Fcat should issue an error message if any file can’t be opened. Hint:
* Since it has no more than one file open at a time, fcat needs only a
* single file pointer variable. Once it’s finished with a file, fcat can
* use the same variable when it opens the next file.
*
* Notes:
*  (1) Compiled with MS Visual Studio 2017 Community (v141), using C
*      language options.
*
* Submitted in partial fulfillment of the requirements of PCC CIS-265.
*************************************************************************
* Change Log:
*   09/10/2017: Initial release. JME
*   10/19/2017: Updated error reporting. JME
*************************************************************************/
#include <assert.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _MSC_VER
// C/C++ Preprocessor Definitions: _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996) 
#endif

static int cat(FILE *stream, char *s) {
	assert(stream != NULL);
	assert(s != NULL);

	// Reserve space to hold stream contents.
	char *buffer = calloc(1024, sizeof(char));
	if (buffer == NULL) 
		return(errno = ENOMEM); // Cannot allocate memory.
	
	long n; // Holds return value from fread.
	// Read and write...
	while ((n = fread(buffer, sizeof(char), 1, stream)) > 0)
		if (fwrite(buffer, strlen(buffer), 1, stdout) != 1 || n < 0) 
			break;
	
	free(buffer);
	return (errno);
}

void main(int argc, char *argv[]) {
	FILE *stream;

	_set_errno(0); // Reset error number.

	// Need at least one file on command line.
	if (argc >= 2) {
		// Repeat for all files on command line.
		for (int i = 1; i < argc; i++) {
			// Attempt to open file.
			if (fopen_s(&stream, argv[i], "r+t") == 0) {
				if (cat(stream, argv[i])) {
					fclose(stream);
					break;
				}
				fclose(stream);
			}
			else 
				break; // Open file failure.
		}
	}
	else
		fputs("\nUsage: fcat [file]...\n", stderr); // Display usage.

	if (errno) {
		fprintf(stderr, "\nEncountered error: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
		exit(EXIT_SUCCESS);
}
