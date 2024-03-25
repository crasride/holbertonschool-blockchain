#include "transaction.h"

/**
* tx_out_create - Creates a transaction output structure
* @amount: Amount in the transaction output
* @pub: Receiver's public address
*
* Return: A pointer to the allocated transaction output
*/
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *output;

	if (pub == NULL)
		return (NULL);

	output = malloc(sizeof(tx_out_t));
	if (output == NULL)
		return (NULL);

	output->amount = amount;
	memcpy(output->pub, pub, EC_PUB_LEN);

	/* Compute the hash of the transaction output */
	if (!sha256((const int8_t *)output, sizeof(tx_out_t), output->hash))
	{
		free(output);
		return (NULL);
	}

	return (output);
}
