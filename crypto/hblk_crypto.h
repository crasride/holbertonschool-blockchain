#ifndef _CRYPTO_H_
#define _CRYPTO_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include <openssl/pem.h>
#include <openssl/sha.h>


/*Function Tasks 0*/
uint8_t *sha256(int8_t const *s, size_t len,
		uint8_t digest[SHA256_DIGEST_LENGTH]);

/*Function Tasks 1*/
EC_KEY *ec_create(void);

/*Function Tasks 2*/
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN]);

/*Function Tasks 3*/
EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN]);

/*Function Tasks 4*/
int ec_save(EC_KEY *key, char const *folder);

/*Function Tasks 5*/
EC_KEY *ec_load(char const *folder);

/*Function Tasks 6*/
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg, size_t msglen,
		sig_t *sig);

/*Function Tasks 7*/
int ec_verify(EC_KEY const *key, uint8_t const *msg, size_t msglen,
		sig_t const *sig);

#endif /* _CRYPTO_H_ */
