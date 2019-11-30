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

void option_1_enter_parameters(int arr_of_sizes[]);
void option_2_read_cache(int arr_of_sizes[]);
void option_3_write_cache(int arr_of_sizes[]);
int size_is_0_or_not_pow_of_2(int size);

struct line
{
	int tag;
	int* line_block_ptr;
} *cache_ptr = NULL;

int* main_memory_ptr;


int main()
{
	int choice = 0;
	int arr_of_sizes[3] = { 0, 0, 0 };

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
			option_1_enter_parameters(arr_of_sizes);
		}
		if (choice == 2) {
			option_2_read_cache(arr_of_sizes);
		}
		if (choice == 3) {
			option_3_write_cache(arr_of_sizes);
		}

	} while (choice != 4);

	//free();
	printf("*** Memory Freed Up - Program Terminated Normally\n\n");
	return 0;
}


void
option_1_enter_parameters(int arr_of_sizes[])
{
	int main_memory_size = 0, cache_size = 0, block_size = 0, num_of_blocks_in_cache = 0;

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
		printf("Could not read user input for block size. Now exitting.\n");
		exit(0);
	}

	// Make sure all user input values are valid
	if (size_is_0_or_not_pow_of_2(main_memory_size)) {
		printf("*** Error - Main Memory Size is not a Power of 2\n\n");
		arr_of_sizes[0] = -1;
	}
	else if (size_is_0_or_not_pow_of_2(cache_size)) {
		printf("*** Error - Cache Size is not a Power of 2\n\n");
		arr_of_sizes[0] = -1;
	}
	else if (size_is_0_or_not_pow_of_2(block_size)) {
		printf("*** Error - Block Size is not a Power of 2\n\n");
		arr_of_sizes[0] = -1;
	}
	else if (block_size > cache_size) {
		printf("*** Error - Block size is larger than cache size\n");
		arr_of_sizes[0] = -1;
	}
	else {
		long cs = cache_size;
		while (cs > 0) {
			cs = cs - block_size;
		}
		if (cs != 0) {
			printf("*** Error - Cache Size is not a multiple of Block Size\n");
			arr_of_sizes[0] = -1;
		}
	}

	// If there were no errors in user input then allocate memory and return input data
	if (arr_of_sizes[0] != -1) {
		printf("*** All Input Parameters Accepted.\nStarting to Process Write/Read Requests\n");

		main_memory_ptr = (int*)malloc(main_memory_size * 2 * sizeof(int));
		int count = main_memory_size;
		for (int i = 0; i < main_memory_size; i++) {
			main_memory_ptr[i] = --count;
		}

		num_of_blocks_in_cache = (cache_size / block_size);
		cache_ptr = (struct line*)malloc(num_of_blocks_in_cache * sizeof(struct line));
		for (int i = 0; i < num_of_blocks_in_cache; i++) {
			cache_ptr[i].tag = -1;
			cache_ptr[i].line_block_ptr = NULL;
		}

		arr_of_sizes[0] = main_memory_size;	// num of lines(words) in main memory
		arr_of_sizes[1] = cache_size;					// num of blocks
		arr_of_sizes[2] = block_size;					// num of lines(words) in a block
	}

	return;
}


void
option_2_read_cache(int arr_of_sizes[])
{
	int cache_size = arr_of_sizes[1], cache_block_size = arr_of_sizes[2], line_in_cache = -1,
		total_lines_in_cache = 0, tag = -1, num_of_tags = 0, word = -1;
	int main_memory_address = -1, block_in_mm = -1, value = 0, total_blocks_in_mm = 0,
		mm_block_size = arr_of_sizes[2], mm_size = arr_of_sizes[0], block_address_in_mm = -1,
		num_of_words_in_mm_that_can_be_represented_by_cache_block = 0;

	printf("Enter Main Memory Address to Read: \n");
	if (!scanf("%d", &main_memory_address)) {
		printf("Could not read user input for main memory address.\n");
		return;
	}

	total_lines_in_cache = cache_size / cache_block_size;
	total_blocks_in_mm = mm_size / mm_block_size;
	num_of_tags = total_blocks_in_mm / (total_lines_in_cache);
	num_of_words_in_mm_that_can_be_represented_by_cache_block = (num_of_tags * cache_block_size);
	line_in_cache = main_memory_address / (num_of_words_in_mm_that_can_be_represented_by_cache_block);
	tag = ((main_memory_address) % (num_of_words_in_mm_that_can_be_represented_by_cache_block))
		/ (cache_block_size);
	block_in_mm = main_memory_address / cache_block_size;
	block_address_in_mm = block_in_mm * mm_block_size;
	word = main_memory_address % mm_block_size;
	//int num_of_bb = mm_size / bb_size;


	if (cache_ptr[line_in_cache].tag == tag) {
		printf("*** Cache Hit\n");
		value = cache_ptr[line_in_cache].line_block_ptr[word];
		printf("*** Word %d of Cache Line %d with Tag %d contains the Value %d\n",
			word, line_in_cache, tag, value);
	}
	else {
		printf("Read Miss - First Load Block from Memory!\n");
		value = main_memory_ptr[main_memory_address];
		cache_ptr[line_in_cache].tag = tag;
		if (cache_ptr[line_in_cache].line_block_ptr == NULL) {
			cache_ptr[line_in_cache].line_block_ptr = (int*)malloc(cache_block_size * sizeof(int));
		}
		for (int i = 0; i < cache_block_size; i++) {
			cache_ptr[line_in_cache].line_block_ptr[i] = main_memory_ptr[block_address_in_mm + i];
		}
		printf("*** Word %d of Cache Line %d with Tag %d contains the Value %d\n",
			word, line_in_cache, tag, value);
	}

	return;
}


// Write a value to the cache and  memory (write back). If the memory block is in the cache then
// its considered a cache hit so the value to written to cache and then to mem. If the memory
// block isn't in cache then the value will be written to mem and the block will be copied to cache
void
option_3_write_cache(int arr_of_sizes[])
{
	int cache_size = arr_of_sizes[1], cache_block_size = arr_of_sizes[2], line_in_cache = -1,
		total_lines_in_cache = 0, tag = -1, num_of_tags = 0, word = -1;
	int main_memory_address = -1, block_in_mm = -1, value = 0, total_blocks_in_mm = 0,
		mm_block_size = arr_of_sizes[2], mm_size = arr_of_sizes[0], block_address_in_mm = -1,
		num_of_words_in_mm_that_can_be_represented_by_cache_block = 0;

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

	total_lines_in_cache = cache_size / cache_block_size;
	total_blocks_in_mm = mm_size / mm_block_size;
	num_of_tags = total_blocks_in_mm / (total_lines_in_cache);
	num_of_words_in_mm_that_can_be_represented_by_cache_block = (num_of_tags * cache_block_size);
	line_in_cache = main_memory_address / (num_of_words_in_mm_that_can_be_represented_by_cache_block);
	tag = ((main_memory_address) % (num_of_words_in_mm_that_can_be_represented_by_cache_block))
				/ (cache_block_size);
	block_in_mm = main_memory_address / cache_block_size;
	block_address_in_mm = block_in_mm * mm_block_size;
	word = main_memory_address % mm_block_size;
	//int num_of_bb = mm_size / bb_size;

	if (cache_ptr[line_in_cache].tag == tag) {
		printf("*** Cache Hit\n");
		cache_ptr[line_in_cache].line_block_ptr[word] = value;
		main_memory_ptr[main_memory_address] = value;
		printf("*** Word %d of Cache Line %d with Tag %d contains the Value %d\n",
			word, line_in_cache, tag, value);
	}
	else {
		printf("*** Write Miss - First load block from memory\n");
		main_memory_ptr[main_memory_address] = value;
		cache_ptr[line_in_cache].tag = tag;
		if (cache_ptr[line_in_cache].line_block_ptr == NULL) {
			cache_ptr[line_in_cache].line_block_ptr = (int*)malloc(cache_block_size * sizeof(int));
		}
		for (int i = 0; i < cache_block_size; i++) {
			cache_ptr[line_in_cache].line_block_ptr[i] = main_memory_ptr[block_address_in_mm + i];
		}
		printf("*** Word %d of Cache Line %d with Tag %d contains the Value %d\n",
			word, line_in_cache, tag, value);
	}

	return;
}


int size_is_0_or_not_pow_of_2(int size) {
	if (size == 0) {
		return (1);
	}

	double log2OfSize = log2(size);
	int wholeNumOfSize = (int)log2OfSize;

	if (log2OfSize - wholeNumOfSize == 0) {
		return (0);
	}
	else {
		return (1);
	}
}

// OLD CODE

/*block_num_in_cache = main_memory_address / cache_size;
	tag = main_memory_address % cache_block_size;
	total_lines_in_cache = cache_size / cache_block_size;
	total_blocks_in_mm = mm_size / mm_block_size;
	num_of_tags = total_blocks_in_mm / (total_lines_in_cache);
	num_of_words_in_mm_that_can_be_represented_by_cache_block = (num_of_tags * cache_block_size);
	line_in_cache = main_memory_address / (num_of_words_in_mm_that_can_be_represented_by_cache_block);*/