#include "hblk_crypto.h"


/**
* ec_create - Creates a new EC key pair using the secp256k1 curve
*
* Return: Pointer to an EC_KEY structure (containing public and private keys)
* NULL on failure
*/
EC_KEY *ec_create(void)
{
	EC_KEY *key;
	/* EC_KEY_new_by_curve_create a new EC_KEY structure using a named curve */
	key = EC_KEY_new_by_curve_name(NID_secp256k1);
	/* EC_KEY_new_by_curve_name - create a new EC_KEY structure */
	if (!key)
	{
		fprintf(stderr, "Failed to create EC key\n");
		return (NULL);
	}
	/* EC_KEY_generate_key - generate a new public and private key */
	if (!EC_KEY_generate_key(key))
	{
		fprintf(stderr, "Failed to generate EC key pair\n");
		EC_KEY_free(key);
		return (NULL);
	}

	return (key);
}

