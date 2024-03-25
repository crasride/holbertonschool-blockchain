#include "transaction.h"

/**
 * tx_out_create - allocates and init a transaction output struct
 * @amount: the amount of transaction
 * @pub: Public key of the transaction receiver
 * Return: Pointer to the created tx output upon success, NULL if failure
 */

tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *tr_out = calloc(1, sizeof(*tr_out));

	if (!tr_out || !pub)
		return (NULL);
	tr_out->amount = amount;
	/* Ramdeck to `array decay` phenomenon */
	memcpy(tr_out->pub, pub, EC_PUB_LEN);

	if (!sha256((int8_t const *)tr_out, sizeof(uint32_t) + EC_PUB_LEN,
				tr_out->hash))
	{
		free(tr_out);
		return (NULL);
	}
	return (tr_out);
}

