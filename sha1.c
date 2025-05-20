#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint32_t rotleft(uint32_t x, unsigned char n) {
	return (x << n) | (x >> (32 - n));
}

void sha1(const uint8_t* msg, size_t msg_len, uint8_t hash[20]) {
    /*
     * msg: The input string as a pointer to a const uint8_t array
     * msg_len: The length of the input string
     * hash: The output buffer containing the 20 bytes hash value
     */

    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;
    /* 32-bit values that represent the initial state of the hash */

    size_t new_len = msg_len;
    while (new_len % 64 != 0) {
	    new_len++;
    }
    /* The above block of code gets the length required for
     * the string to be congruent to 448(mod 512)
     */

    uint8_t* padded_msg = calloc(new_len, 1);
    /* The required length is allocated to the padded array 
     * All bytes are initialised to 0x0.
     */
    memcpy(padded_msg, msg, msg_len);
    /*
     * msg_len bytes are copied from msg to padded_msg
     */
    padded_msg[msg_len] = 0x80;
    /* A 1-bit(in the form of the 00000001 byte) is appended next to the
     * the last byte copied to the padded array.
     */

    uint64_t bit_len = msg_len * 8;

    padded_msg[new_len - 8] = (bit_len >> 56) & 0xFF;
    padded_msg[new_len - 7] = (bit_len >> 48) & 0xFF;
    padded_msg[new_len - 6] = (bit_len >> 40) & 0xFF;
    padded_msg[new_len - 5] = (bit_len >> 32) & 0xFF;
    padded_msg[new_len - 4] = (bit_len >> 24) & 0xFF;
    padded_msg[new_len - 3] = (bit_len >> 16) & 0xFF;
    padded_msg[new_len - 2] = (bit_len >> 8) & 0xFF;
    padded_msg[new_len - 1] = bit_len & 0xFF;

    /* The length of the original string in bits is obtained and stored
     * in a 64 bit(8 byte) unsigned integer which is split into single bytes
     * so it can be stored in the 8-bit(1 byte) padded array by shifting
     * the bits and is appended to the padded array by shifting the bits
     * to move the desired byte to the right most position
     * and masking the result with 0xFF (255 in decimal 11111111 in binary)
     * to make sure the stored value is an 8-bit integer
     */


    char blocks = new_len / 64;;
    /* Gets the amount of 64-byte blocks in the padded array */

    for (char c = 0; c < blocks; c++) {
        uint32_t words[80];
        for (int i = 0; i < 16; i++) {
            words[i] = (uint32_t)padded_msg[i * 4] << 24;
            words[i] |= (uint32_t)padded_msg[i * 4 + 1] << 16;
            words[i] |= (uint32_t)padded_msg[i * 4 + 2] << 8;
            words[i] |= (uint32_t)padded_msg[i * 4 + 3];
        }
        /* 👆 An array of type 32-bit unsigned integer is initalized
         * and the previous array is divided into 16 words of 32-bits
         * Ex.:
         * The hex values of a,b,c,and d are 0x61, 0x62, 0x63, and 0x64 respectively
         * Keep in mind 0x61 is the same as 0x00000061
         * Assuming a,b,c, and d make up the 1st 4 bytes of the padded array...
         * 
         * words[0] = (uint32_t)padded_msg[0] << 24;
         * words[0] |= (uint32_t)padded_msg[1] << 16;
         * words[0] |= (uint32_t)padded_msg[2] << 8;
         * words[0] |= (uint32_t)padded_msg[3];
         *
         * Which means...
         *
         * words[0] = (uint32_t) 0x00000061 << 24  =>  0x61000000
         * words[0] |= (uint32_t) 0x00000062 << 16 =>  0x00620000
         * words[0] |= (uint32_t) 0x00000063 << 8  =>  0x00006300
         * words[0] |= (uint32_t) 0x00000064       =>  0x00000064
         *                                            ------------
         * Thus,  words[0]                         =>  0x61626364
         *
         * This iteration carries on 16 times per 512-bit(64 byte block)
         * in the padded array.
         */

        for (int i = 16; i < 80; i++) {
            words[i] = rotleft((words[i-3] ^ words[i-8] ^ words[i-14] ^ words[i-16]), 1);
        }
        /*
         * The 16 32-bit words are expanded into 80 words using xor(exclusive OR)
         * and the rotleft function which left-shifts an unsigned 32-bit int
         * and wraps the shifted bit to the end of the integer
         */

        // print_words(words, 80);
        uint32_t a = h0, b = h1, c = h2, d = h3, e = h4;
        uint32_t f, k;
        for (int i = 0; i < 80; i++) {
            if ( i >= 0 & i <= 19) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            } else if ( i >= 20 & i <= 39) {
                f = (b ^ c ^ d);
                k = 0x6ED9EBA1;
            } else if (i >= 40 & i <= 59) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else {
                f = (b ^ c ^ d);
                k = 0xCA62C1D6;
            }

            uint32_t temp = rotleft(a, 5) + f + e + words[i] + k;
            e = d;
            d = c;
            c = rotleft(b, 30);
            b = a;
            a = temp;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    // printf("%#X\n", h0);
    // printf("%#X\n", h1);
    // printf("%#X\n", h2);
    // printf("%#X\n", h3);
    // printf("%#X\n", h4);

    uint32_t parts[5] = {h0, h1, h2, h3, h4};

    for ( int i = 0; i < 5; i++) {
        hash[i*4 + 0] = (parts[i] >> 24) & 0xFF;
        hash[i*4 + 1] = (parts[i] >> 16) & 0xFF;
        hash[i*4 + 2] = (parts[i] >> 8) & 0xFF;
        hash[i*4 + 3] = (parts[i] >> 0) & 0xFF;
    }

    free(padded_msg);
}
