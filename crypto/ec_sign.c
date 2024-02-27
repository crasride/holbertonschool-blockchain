#include "hblk_crypto.h"

/**
* ec_sign - Sign a given set of bytes using a given EC_KEY private key
*
* @key: Pointer to the EC_KEY structure containing the private key
* @msg: Pointer to the bytes to be signed
* @msglen: Length of the message
* @sig: Pointer to the sig_t structure to store the signature
*
* Return: Pointer to the signature buffer upon success, or NULL upon failure
*/
uint8_t *ec_sign(EC_KEY const *key, uint8_t const *msg,
		size_t msglen, sig_t *sig)
{

	uint32_t siglen = 0;

	if (!key || !msg || !sig)
		return (NULL);

	memset(sig->sig, 0, sizeof(sig->sig));

	if (ECDSA_sign(0, msg, msglen, sig->sig, &siglen, (EC_KEY *)key) != 1)
		return (NULL);

	sig->len = siglen;
	return (sig->sig);
}



