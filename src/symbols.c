#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "symbols.h"

//prime number
static const uint64_t INITIAL_SIZE = 67;
uint64_t xstate;

struct symbol_table symbol_table_init(float load_factor)
{
	xseed(time(NULL));

	struct symbol_table st;
	st.size = INITIAL_SIZE;
	st.table = malloc(sizeof(struct symbol) * st.size);
	st.filled = 0;
	st.load_factor = load_factor;
	return st;
}

int symbol_table_term(struct symbol_table st)
{
	st.size = 0;
	free(st.table);
	return 0;
}

int symbol_table_insert(struct symbol_table st, char *name, uint16_t value, bool undefined)
{
	if(st.filled > st.size * st.load_factor) {
		symbol_table_rehash(st);
	}
	
	uint64_t h1 = hash1(name, st);
	uint64_t h2 = hash2(name, st);

	uint64_t h = 0;
	for(int i=0; i < 3; i++) {
		h = h1 + (i*h2) % st.size;

		if(st.table[h].name == NULL) {
			struct symbol s;
			s.name = malloc(sizeof(char) * strlen(name) + 1);
			strcpy(s.name, name);
			s.value = value;
			s.undefined = undefined;
			
			st.table[h] = s;
		        return 0;
		}
	}
	
	return -1;
}

int symbol_table_define(struct symbol_table st, char *name, uint16_t value)
{
	uint64_t h1 = hash1(name, st);
	uint64_t h2 = hash2(name, st);

	uint64_t h = 0;
	for(int i=0; i < 3; i++) {
		h = h1 + (i*h2) % st.size;
		
		if(st.table[h].name == name) {
			st.table[h].value = value;
			st.table[h].undefined = true;
			return 0;
		}
	}

	return -1;	
}

uint16_t symbol_table_search(struct symbol_table st, char *name)
{
	uint64_t h1 = hash1(name, st);
	uint64_t h2 = hash2(name, st);

	uint64_t h = 0;
	for(int i=0; i < 3; i++) {
		h = h1 + (i*h2) % st.size;
		
		if(st.table[h].name == name) {
			return st.table[h].value;
		}
	}

	return -1;	
}

int symbol_table_rehash(struct symbol_table st)
{
	struct symbol_table temp;
	temp.size = st.size;
	temp.table = st.table;

	st.size = get_prime(temp.size*2);
	st.table = malloc(sizeof(uint16_t) * st.size);
	
	for(size_t i=0; i < temp.size; i++) {
		if(temp.table[i].name != NULL) {
			symbol_table_insert(st, temp.table[i].name, temp.table[i].value, false);
		}
	}

	free(temp.table);
	
	return 0;
}

uint64_t hash1(char *name, struct symbol_table st)
{
	uint64_t h = 0;
	uint64_t a = 31;
	for(uint64_t i=0; i < strlen(name); i++) {
		h = ((h*a) + name[i]) % st.size;
	}
	return h;
}

uint64_t hash2(char *name, struct symbol_table st)
{
	uint64_t h = 0;
	uint64_t a = 31;
	for(uint64_t i=0; i < strlen(name); i++) {
		h = ((h*a) + name[i]) % st.size-1;
	}
	return 1 + h;	
}

uint64_t get_prime(uint64_t n)
{
	uint64_t prime = n;
	//check size
	if(n < 3) {
		return 3;
	}
	
	//check if even
        n = ((n % 2) == 0) ? n+1 : n;

	//find prime integer bigger than n but smaller than 2^64-1
	while(prime < 9223372036854775806) {
		//Miller-Rabin primality test
		
		//factor out powers of 2 from n-1
		uint64_t d = prime-1;
		uint64_t r = 0;
		while(d % 2 == 0) {
			r++;
			d = d/2;
		}

		//find fermat witnesses of j's primality
		int k = 10;
		for(int i=0; i < k; i++) {
			uint64_t a = xrand(prime-4)+2 % n;
			uint64_t x  = powi(a, d, n);
			if(x != 1 && x != prime-1) {
				break;
			}
			x = powi(x, 2, prime);
			if(x != prime-1) {
				break;
			}
		}
		prime+=2;
	}
	
	return prime;
}

uint64_t xorshift(void)
{
	uint64_t u = xstate;
	u ^= u << 12;
	u ^= u << 25;
	u ^= u >> 27;
	xstate = u;
	return u * 0x2545F4914F6CDD1D;
}

uint64_t xrand(uint64_t bound)
{
	return xorshift() % bound;
}

void xseed(uint64_t seed)
{
	xstate = seed;
}

uint64_t powi(uint64_t n, uint64_t x, uint64_t m)
{
	if(n == 0) {
		return 1;
	}

	uint64_t y = 1;
	while(n > 1) {
		if(n % 2 == 0) {
			x = (x * x) % m;
			n = n/2;
		}
		else {
			y = (x * y) % m;
			x = (x * x) % m;
			n = (n-1) / 2;
		}
	}
	return (x * y) % m;
}
