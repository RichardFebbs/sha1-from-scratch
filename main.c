#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "sha1.h"


int main(int argc, char** argv) {
    if ( argc < 2) {
        printf("You must pass an argument main\n./main.c <ARG>\n");
        return 1;
    }
    const char* msg = argv[1];
    size_t msg_len = strlen(msg);
    uint8_t hash[20];
    sha1((const uint8_t*)msg, msg_len, hash);
    for (int i = 0; i < 20; i++) {
        printf("%x", hash[i]);
    }
    return 0;
}
