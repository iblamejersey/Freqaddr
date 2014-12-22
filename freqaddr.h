/*
 * CS 211: Computer Architecture: Fall 2013
 * Programming Assignment 1: Freqaddr
 * Author: Jeremy Balise
 * Instructor: Prof. Santosh Nagarakatte
 */

#include <stddef.h>
#define MAX_STRING_LEN 256

struct occurrence { /* Occurrence of a single address within the file */
	size_t address; /* 64-bit hexadecimal address */
	int frequency;  /* Number of the address's occurrences within the file */
	int order;      /* Position of the address within the file (used to maintain ordering property when sorting the addresses) */
};

/*
 * Comparison algorithm used for sorting non-unique addresses.
 * Parameters: two void pointers, a and b
 * Return value: integer value determining the sorting of the list elements
 */
int compareOccurrences(const void *a, const void *b) {
	
	const struct occurrence *occurrencePointer1 = a;
	const struct occurrence *occurrencePointer2 = b;
	
	if ((*occurrencePointer1).address > (*occurrencePointer2).address) return -1;
	if ((*occurrencePointer1).address == (*occurrencePointer2).address) {
		if ((*occurrencePointer1).order > (*occurrencePointer2).order) {
			return 1;
		}
		return -1;
	}
	if ((*occurrencePointer1).address < (*occurrencePointer2).address) return 1;
}

/*
 * Comparison function used for sorting the final list of unique addresses.
 * Parameters: two void pointers, a and b
 * Return value: integer value determining the sorting of the list elements
 */
int compareResults(const void *a, const void *b) {
	
	const struct occurrence *occurrencePointer1 = a;
	const struct occurrence *occurrencePointer2 = b;
	
	if ((*occurrencePointer1).frequency > (*occurrencePointer2).frequency) return -1;
	if ((*occurrencePointer1).frequency == (*occurrencePointer2).frequency) {
		if ((*occurrencePointer1).order > (*occurrencePointer2).order) {
			return 1;
		}
		return -1;
	}
	if ((*occurrencePointer1).frequency < (*occurrencePointer2).frequency) return 1;
}
