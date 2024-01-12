/* File       : SHA256.c
 * Description: C implementation of SHA-256 (256-bit Secure Hash Algorithm).
 *
 * Author     : AugFJTan
 * Last Modified 18 Feb 2018 06:50 PM
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "constants.h"
#include "cSha.h"

int derive_k(uint64_t l);
uint32_t right_rotate(uint32_t x, int n) { return (x >> n) | (x << (32-n)); }
uint32_t s0(uint32_t x) { return right_rotate(x, 7) ^ right_rotate(x, 18) ^ (x >> 3); }
uint32_t s1(uint32_t x) { return right_rotate(x, 17) ^ right_rotate(x, 19) ^ (x >> 10); }
uint32_t S0(uint32_t x) { return right_rotate(x, 2) ^ right_rotate(x, 13) ^ right_rotate(x, 22); }
uint32_t S1(uint32_t x) { return right_rotate(x, 6) ^ right_rotate(x, 11) ^ right_rotate(x, 25); }
uint32_t ch(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (~x & z); }
uint32_t maj(uint32_t x, uint32_t y, uint32_t z) { return (x & y) ^ (x & z) ^ (y & z); }

int main(int argc, char *argv[])
{


	/* Variables */
	char *raw_message, *default_message = "abcd";
	int chunks, i, j, m, n, k;
	uint64_t l, message_length;
	uint32_t temp1, temp2;
	uint32_t a, b, c, d, e, f, g, h;
	uint32_t **bit_array, w[64];
    uint32_t hash[8];

	raw_message = (argc == 1) ? default_message : argv[1];
    printf("raw_message = %d\n",CONVERT_TO_INT(raw_message));

	/* Pre-processing */
	l = strlen(raw_message) * sizeof(char) * 8;  /* Assuming ASCII encoding */
	k = derive_k(l);

	/* Allocate memory for each 512-bit chunk (16 32-bit values) */
	message_length = l + 1 + k + 64;
	chunks = message_length / 512;

	bit_array = (uint32_t**)calloc(chunks, sizeof(uint32_t*));

	for(i=0; i < chunks; i++)
		bit_array[i] = (uint32_t*)calloc(16, sizeof(uint32_t));

	/* Assign values */
	i = j = n = 0;
	m = 3;

	while(raw_message[n] != '\0')
	{
		bit_array[i][j] |= raw_message[n] << m * 8;
        printf("bit array at i and j %u\n", bit_array[i][j]  );

		if(m > 0) m--;
		else
		{
			m = 3;

			if(j < 16) j++;
			else
			{
				j = 0;
				i++;
			}
		}

		n++;
	}

	/* Append '1'-bit */
	bit_array[i][j] |= 0x80 << m * 8;
    printf("m = %d\n", m);
    /* Append length as 64-bit big endian value */
    bit_array[chunks-1][14] = (uint32_t)(l >> 32);
    bit_array[chunks-1][15] = (uint32_t)(l & 0xffffffff);
    int z = 0;
    for(z = 0; z < 16; z++)
        printf("z bits bit array at i and j %u\n", bit_array[i][z] );

	/* Process message */
	for(j=0; j<chunks; j++)
	{
		for(i=0; i<16; i++) {
            w[i] = bit_array[j][i];
            printf("w[i=%d] = %u\n", i, w[i]);
        }

		for(i=16; i<64; i++) {
            w[i] = w[i - 16] + s0(w[i - 15]) + w[i - 7] + s1(w[i - 2]);
            printf("w[i=%d] = %u\n", i, w[i]);
        }


        a = H[0];
		b = H[1];
		c = H[2];
		d = H[3];
		e = H[4];
		f = H[5];
		g = H[6];
		h = H[7];

		for(i=0; i<64; i++)
		{
			temp1 = h + S1(e) + ch(e, f, g) + K[i] + w[i];
			temp2 = S0(a) + maj(a, b, c);

			h = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}

		hash[0] = H[0] + a;
		hash[1] = H[1] + b;
		hash[2] = H[2] + c;
		hash[3] = H[3] + d;
		hash[4] = H[4] + e;
		hash[5] = H[5] + f;
		hash[6] = H[6] + g;
		hash[7] = H[7] + h;
	}

	/* Free memory */
	for(i=0; i < chunks; i++)
		free(bit_array[i]);

	free(bit_array);

	/* Display the final hash */
	printf("Message: \"%s\"\n", raw_message);
	printf("Hash: ");

	for(i=0; i<8; i++)
		printf("%08x", hash[i]);

	printf("\n");
    printf("sizeof(uint32_t) = %lu\n", sizeof(uint32_t));

	return 0;
}

int derive_k(uint64_t l)
{
	int remainder = (l + 1 + 64) % 512;

	if(remainder == 0) /* Total message length is a multiple of 512 */
		return 0;
	else
		return 512 - remainder;
}
