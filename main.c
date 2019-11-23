/***************************************************************************************************
Brandon Cobb
Programming Assignment 4: Cache Simulation
Comp 222 - Fall 2019
Meeting Time: 0930 - 1045
November 13, 2019
This program...
***************************************************************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

int* option_1_enter_parameters();
void option_2_read_cache();
void option_3_write_cache();

struct line
{
	int tag;
	int *line_block_ptr;
} *cache_ptr = NULL;


int main()
{
	int choice = 0, block_size = 0;

	do {
		printf("\nMain Menu - Main Memory to Cache Memory Mapping\n"
			   "------------------------------------------------\n"
			   "1 Enter Configuration Parameters\n"
			   "2 Read from Cache\n"
			   "3 Write to Cache\n"
		   	   "4 Quit Program\n");
		if (!scanf("%d", &choice)) {
			printf("Count not read user input. Please enter a number 1-4. Now exitting.\n");
			exit(0);
		}

		if (choice == 1) {
			option_1_enter_parameters();
		}
		if (choice == 2) {
			option_2_read_cache();
		}
		if (choice == 3) {
			option_3_write_cache();
		}

	} while (choice != 4);

	//free();
	printf("*** Memory Freed Up - Program Terminated Normally\n\n");
	return 0;
}


int*
option_1_enter_parameters()
{
	int arr_of_sizes[3];
	long main_memory_size = 0, cache_size = 0, block_size = 0;

	// Allow user to enter parameters
	printf("Enter main memory size (words): \n");
	if (!scanf("%d", &main_memory_size)) {
		printf("Count not read user input for main memory. Now exitting.\n");
		exit(0);
	}

	printf("Enter cache size (words): \n");
	if (!scanf("%d", &cache_size)) {
		printf("Count not read user input for cache size. Now exitting.\n");
		exit(0);
	}

	printf("Enter block size (words/block): \n");
	if (!scanf("%d", &block_size)) {
		printf("*** Error - Cache Size is not a Power of 2\n\n");
		arr_of_sizes[0] = NULL;
	}

	// Make sure all user input values are valid
	if (size_is_0_or_not_pow_of_2(main_memory_size)) {
		printf("*** Error - Main Memory Size is not a Power of 2\n\n");
		arr_of_sizes[0] = NULL;
	}

	if (size_is_0_or_not_pow_of_2(cache_size)) {
		printf("*** Error - Cache Size is not a Power of 2\n\n");
		arr_of_sizes[0] = NULL;
	}

	if (size_is_0_or_not_pow_of_2(block_size)) {
		printf("*** Error - Block Size is not a Power of 2\n\n");
		arr_of_sizes[0] = NULL;
	}

	if (block_size > cache_size) {
		printf("*** Error - Block size is larger than cache size\n");
		arr_of_sizes[0] = NULL;
	}
	
	long cs = cache_size;
	while (cs > 0) {
		cs = cs - block_size;
	}
	if (cs != 0) {
		printf("*** Error - Cache Size is not a multiple of Block Size\n");
		arr_of_sizes[0] = NULL;
	}

	//if (arr_of_sizes[0] != NULL) {
	//	printf("*** All Input Parameters Accepted.  Starting to Process Write/Read Requests");
	//}

	int* main_memory_ptr = malloc(main_memory_size * sizeof(int));
	for (int i = 0; i < main_memory_size; i++) {
		main_memory_ptr[i] = --main_memory_size;
	}

	cache_ptr = (struct line*)malloc(cache_size * block_size * sizeof(struct line));
	for (int i = 0; i < cache_size; i++) {
		cache_ptr[i].tag = -1;
		cache_ptr[i].line_block_ptr = NULL;
	}

	arr_of_sizes[0] = main_memory_size;
	arr_of_sizes[1] = cache_size;
	arr_of_sizes[2] = block_size;

	return arr_of_sizes;
}


void
option_2_read_cache()
{
	int main_memory_address = 0;

	printf("Enter Main Memory Address to Read: \n");
	if (!scanf("%d", &main_memory_address)) {
		printf("Could not read user input for main memory address.\n");
		return;
	}



	return;
}


void
option_3_write_cache()
{
	int main_memory_address = 0, value = 0;

	printf("Enter Main Memory Address to Write: \n");
	if (!scanf("%d", &main_memory_address)) {
		printf("Could not read user input for main memory address.\n");
		return;
	}

	printf("Enter Value to Write: \n");
	if (!scanf("%d", &value)) {
		printf("Could not read user input for value to write.\n");
		return;
	}

	return;
}


int size_is_0_or_not_pow_of_2(long size) {
	if (size == 0) {
		return (1);
	}

	for (int i = 0; size >= 0; i++) {
		if (size == 0) {
			return(0);
		}
		if (size < 0) {
			return(1);
		}
		size -= pow(size, i);
	}
}