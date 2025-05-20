#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

uint32_t rotleft(uint32_t x, unsigned char n);
void sha1(const uint8_t* msg, size_t msg_len, const uint8_t hash[20]);
