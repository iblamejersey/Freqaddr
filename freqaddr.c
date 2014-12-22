/*
 * CS 211: Computer Architecture: Fall 2013
 * Programming Assignment 1: Freqaddr
 * Author: Jeremy Balise
 * Instructor: Prof. Santosh Nagarakatte
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freqaddr.h"

int main(int argc, char **argv) {
	
	/* Check input command for validity/proper formatting. If the input is found to be invalid for any reason, generate an appropriate error message. */
	
	if (argc != 4) {
		fprintf(stderr, "Error: Program must be given three input arguments.\n");
		return -1;
	}
	
	if (strcmp(argv[1], "-n") != 0) {
		fprintf(stderr, "Error: First input argument must be \"-n\".\n");
		return -1;
	}
	
	char *end;
	int number_of_entries = strtol(argv[2], &end, 10);
	if (*end || number_of_entries < 0) {
		fprintf(stderr, "Error: Second input argument must be an integer greater than or equal to zero.\n");
		return -1;
	}
	
	/* Copy only the first 255 characters of the third argument (and the escape sequence) in order to prevent buffer overflow. */
	
	char filename[MAX_STRING_LEN];
	strncpy(filename, argv[3], MAX_STRING_LEN);
	
	/* Attempt to open the file. Generate an error message if the file cannot be opened. */
	
	FILE *fp = fopen(filename, "r"); 
	if (fp == NULL) {
		fprintf(stderr, "Error: Could not open the specified file. Third input argument must be the name of an existing file.\n");
		return -1;
	}
	
	/* Determine the number of addresses (number of lines) in the file. */
	
	size_t currentAddress;
	int numLines = 0;
	
	while(fscanf(fp, "%Ix", &currentAddress) != EOF) {
		numLines++;
	}
	
	rewind(fp);
	
	/* Check to see if the file is empty. */
	
	if (numLines == 0) {
		fprintf(stderr, "Error: specified file is empty.");
		return -1;
	}
		 
	/* Create an array of all addresses within the file. */
	
	struct occurrence *occurrenceList = (struct occurrence*)malloc(sizeof(struct occurrence) * numLines);
	
	int i;
	for (i = 0; i < numLines; i++) {
		/* Check to see if any of the addresses are invalid. */
		if (fscanf(fp, "%Ix", &currentAddress) == EOF) {
			fprintf(stderr, "Error: specified file contains one or more invalid addresses.");
			return -1;
		}
		occurrenceList[i].address = currentAddress;
		occurrenceList[i].frequency = 1; /* All frequencies are initialized to 1 and changed later, if necessary. */
		occurrenceList[i].order = i + 1; /* Order numbers will range from 1 to <numLines>. */
	}
	
	/*
	 * Sort the list of all addresses according to hexadecimal number size and order number. At the end of this function call, the list is sorted in
	 * decreasing order by hexadecimal number. If multiple hexadecimal numbers are equal, the one with the smallest order number appears first in the 
	 * sorted list.
	 */
	
	qsort(occurrenceList, numLines, sizeof(struct occurrence), compareOccurrences);
	
	/* Determine the frequency of each unique address and store each unique address into an array of unique addresses. */
	
	struct occurrence *result = (struct occurrence*)malloc(sizeof(struct occurrence) * numLines);
	
	int firstOccurrenceIndex = 0, resultIndex = 0;
	
	if (numLines == 1) {
		result[resultIndex].address = occurrenceList[firstOccurrenceIndex].address;
		result[resultIndex].order = occurrenceList[firstOccurrenceIndex].order;
		result[resultIndex].frequency = occurrenceList[firstOccurrenceIndex].frequency;
	}
	
	int j;
	for (j = 1; j < numLines; j++) {
		if (occurrenceList[j-1].address == occurrenceList[j].address) {
			occurrenceList[firstOccurrenceIndex].frequency++;
			occurrenceList[j].frequency = 0;
		} else {
			result[resultIndex].address = occurrenceList[firstOccurrenceIndex].address;
			result[resultIndex].order = occurrenceList[firstOccurrenceIndex].order;
			result[resultIndex].frequency = occurrenceList[firstOccurrenceIndex].frequency;
			resultIndex++;
			firstOccurrenceIndex = j;
		}
		if ((j + 1) == numLines) {
			result[resultIndex].address = occurrenceList[firstOccurrenceIndex].address;
			result[resultIndex].order = occurrenceList[firstOccurrenceIndex].order;
			result[resultIndex].frequency = occurrenceList[firstOccurrenceIndex].frequency;
		}
	}
	
	/*
	 * Sort the list of unique addresses by decreasing frequency. If the frequency of multiple addresses is the same, the addresses are sorted in increasing
	 * order by order number so that the address that appeared first in the file appears first in the list, the address that appeared second in the file
	 * appears second in the list, and so on.
	 */
	
	qsort(result, resultIndex+1, sizeof(struct occurrence), compareResults);
	
	 /* Print out the address/frequency pairs according to the number specified by the input command. */
	
	if (number_of_entries > resultIndex + 1) {
		fprintf(stderr, "Error: Number of frequent addresses is less than the number of entries specified.");
		return -1;
	}
	
	if (number_of_entries == 0) { /* Print the whole list. */
		int k;
		for (k = 0; k <= resultIndex; k++) {
			printf("0x%Ix :%d\n", result[k].address, result[k].frequency);
		}
	} else { /* Print specified number of entries. */
		int l;
		for (l = 0; l < number_of_entries; l++) {
			printf("0x%Ix :%d\n", result[l].address, result[l].frequency);
		}
	}
	
	/* Free the allocated memory and exit the program. */
	
	free(occurrenceList); free(result); fclose(fp); return 0;
}
