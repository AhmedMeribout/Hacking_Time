#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define KEYSIZE 16

#ifdef __cplusplus
extern "C" {
#endif

unsigned char* generateKey(int timeSeed) {
    unsigned char *key = (unsigned char*)malloc(KEYSIZE * sizeof(unsigned char));
    if (key == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    srand(timeSeed);
    for (int i = 0; i < KEYSIZE; i++) {
        key[i] = rand() % 256;
    }
    return key;
}

#ifdef __cplusplus
}
#endif
