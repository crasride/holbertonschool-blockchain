#include "hblk_crypto.h"

/**
 * ec_to_pub - Extrae la clave pública de una estructura EC_KEY
 *
 * @key: Puntero a la estructura EC_KEY para extraer la clave pública
 * @pub: Dirección en la que almacenar la clave pública extraída
 *
 * Return: Puntero a pub en caso de éxito, NULL en caso de fallo
 */
uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	const EC_GROUP *group;
	const EC_POINT *point;

	/*Check if key or pub is NULL*/
	if (!key || !pub)
		return (NULL);

	/*Get the group and public key point*/
	group = EC_KEY_get0_group(key);
	point = EC_KEY_get0_public_key(key);

	/*Check whether group or point is NULL*/
	if (!group || !point)
		return (NULL);

	/*Convert public key point to uncompressed octet string*/
	if (!EC_POINT_point2oct(group, point, POINT_CONVERSION_UNCOMPRESSED, pub,
				EC_PUB_LEN, NULL))
		return (NULL);
	return (pub);
}
