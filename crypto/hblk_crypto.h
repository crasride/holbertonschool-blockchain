#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
/*Add Function Tasks 2*/
#define EC_PUB_LEN 65

/*Function Tasks 0*/
uint8_t *sha256(int8_t const *s, size_t len,
		uint8_t digest[SHA256_DIGEST_LENGTH]);

/*Function Tasks 1*/
EC_KEY *ec_create(void);

/*Function Tasks 2*/
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);

#endif /* _CRYPTO_H_ */
