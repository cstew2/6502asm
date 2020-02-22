#ifndef __SYMBOLS_H__
#define __SYMBOLS_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

//key, value pair inside symbol table
struct symbol{
	char *name;  /* hashable value */
	uint16_t value;
	bool undefined;
};

//structure definition
struct symbol_table{
	float load_factor;
	uint64_t filled;
	uint64_t size;
	struct symbol *table;
};

//PRNG state
uint64_t xstate;

//structure initialisation and termination
struct symbol_table symbol_table_init(float load_factor);
int symbol_table_term(struct symbol_table st);

//insert and search for (key,value) pairs
int symbol_table_insert(struct symbol_table st, char *name, uint16_t value, bool undefined);
int symbol_table_define(struct symbol_table st, char *name, uint16_t value);
uint16_t symbol_table_search(struct symbol_table st, char *name);
//recalculate hash table with a larger size
int symbol_table_rehash(struct symbol_table st);

//hashing functions for double hashing
uint64_t hash1(char *name, struct symbol_table st);
uint64_t hash2(char *name, struct symbol_table st);

//get a prime number bigger than n
uint64_t get_prime(uint64_t n);

//PRNG functions
uint64_t xorshift(void);
uint64_t xrand(uint64_t bound);
void xseed(uint64_t seed);

//integer power
uint64_t powi(uint64_t x, uint64_t n, uint64_t m);
	
#endif
