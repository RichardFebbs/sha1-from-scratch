#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint32_t rotleft(uint32_t x, unsigned char n) {
	return (x << n) | (x >> (32 - n));
}

void sha1(const uint8_t* msg, size_t msg_len, const uint8_t hash[20]);
