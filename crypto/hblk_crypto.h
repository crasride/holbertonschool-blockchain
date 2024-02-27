#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <openssl/pem.h>
#include <openssl/sha.h>


/*Function Tasks 0*/
uint8_t *sha256(int8_t const *s, size_t len,
		uint8_t digest[SHA256_DIGEST_LENGTH]);

#endif /* _CRYPTO_H_ */
