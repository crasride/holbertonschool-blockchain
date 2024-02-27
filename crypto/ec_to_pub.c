#include "hblk_crypto.h"

/**
* ec_to_pub - Extracts the public key from an EC_KEY structure
*
* @key: Pointer to the EC_KEY structure to retrieve the public key from
* @pub: Address at which to store the extracted public key
*
* Return: Pointer to pub on success, NULL on failure
*/
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_POINT *point;

	/* Check if key is NULL */
	if (!key)
		return (NULL);

	/* Get the public key point from the EC_KEY structure */
	point = EC_KEY_get0_public_key(key);
	if (!point)
		return (NULL);

	/* Convert the EC_POINT to octet string representation of the public key */
	if (EC_POINT_point2oct(
	/* EC_KEY_get0_group - get the EC_GROUP structure of an EC_KEY object */
			EC_KEY_get0_group(key),
			/* point - the EC_POINT to be converted */
			point,
	/* EC_POINT_CONVERSION_UNCOMPRESSED - the point is represented as (x,y) */
			POINT_CONVERSION_UNCOMPRESSED,
	/* pub - the buffer where the octet string representation is stored */
			pub,
			/* EC_PUB_LEN - the maximum number of bytes to write to buf */
			EC_PUB_LEN,
			/* NULL - the standard conversion form is used */
			NULL) == 0)
		return (NULL);

	return (pub);
}
