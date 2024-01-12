//
// Created by f0rio2 on 1/10/24.
//

#ifndef CSHA_CSHA_H
#define CSHA_CSHA_H

#include <stdint.h>
#include <stdio.h>

#define ROTR(x, n) ((x >> n) | (x << (32 - n)))

#define SIGMA0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3))
#define SIGMA1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10))

#define CAPSIGMA0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define CAPSIGMA1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))

#define CH(x, y, z) ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

typedef struct  {
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
} bytes_block; ;

#define CONVERT_TO_INT(data) ((data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3]))

void sort_file(const char *filename);
void set(char *arr[], int size, int* uniqueCount);

#endif //CSHA_CSHA_H
