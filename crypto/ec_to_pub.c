#include "hblk_crypto.h"

/**
 * ec_to_pub - extracts the public key from an EC_KEY opaque structure
 * @key: a pointer to the EC_KEY structure
 * @pub: the address at which to store the extracted public key
 * Return: a pointer to pub, NULL on failure,
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_GROUP *group;
	const EC_POINT *point;
	size_t len;

	if (!key || !pub)
		return (NULL);

	group = EC_KEY_get0_group(key);
	point = EC_KEY_get0_public_key(key);
	if (!group || !point)
		return (NULL);
	len = EC_POINT_point2oct(group, point,
			POINT_CONVERSION_UNCOMPRESSED, pub, EC_PUB_LEN, NULL);
	if (len != EC_PUB_LEN)
		return (NULL);
	return (pub);
}
