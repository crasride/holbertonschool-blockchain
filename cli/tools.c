#include "cli.h"

/**
* _get_endianness - Get current endianness
*
* Return: 1 for little endian, 2 for big endian, 0 for unknown
*/
uint8_t _get_endianness(void)
{
	_endian_t order = { 1 };

	if (order.bytes[0] == 1)
		return (1); /* Little endian */
	if (order.bytes[0] == 0)
		return (2); /* Big endian */

	return (0); /* Should not happen */
}

/**
* sum_unspent - sum unspent transaction outputs
* @node: unspent transaction output
* @idx: index of the node
* @arg: pointer to the total
* Return: 0
*/
int sum_unspent(void *node, unsigned int idx, void *arg)
{
	unspent_tx_out_t *unspent_tx_out = node;
	uint32_t *total = arg;
	(void)idx;
	*total += unspent_tx_out->out.amount;
	return (0);
}

/**
* string_to_pub - converts a EC_KEY public key in a lowercase or
* uppercase hex string to a byte array
*
* @address: public key of EC_PUB_LEN bytes, in hex
*
* Return: byte array on success, or NULL on failure
*/
uint8_t *string_to_pub(char *address)
{
	uint8_t *pub;
	unsigned int scan;
	size_t i, j, len, upper, case_detected;

	if (!address)
	{
		fprintf(stderr, "string_to_pub: NULL parameter\n");
		return (NULL);
	}
	pub = calloc(EC_PUB_LEN, sizeof(uint8_t));
	if (!pub)
	{
		fprintf(stderr, "string_to_pub: calloc failure\n");
		return (NULL);
	}
	len = strlen(address);
	if (len < (EC_PUB_LEN * 2))
	{
		free(pub);
		return (NULL);
	}

	for (i = 0, case_detected = 0; i < len; i++)
	{
		if (isdigit(address[i]))
			continue;
		else if (address[i] >= 'a' && address[i] <= 'f')
		{
			if (!case_detected)
			{
				upper = 0;
				case_detected = 1;
			}
			if (upper)
			{
				free(pub);
				return (NULL);
			}
		}
		else if (address[i] >= 'A' && address[i] <= 'F')
		{
			if (!case_detected)
			{
				upper = 1;
				case_detected = 1;
			}
			if (!upper)
			{
				free(pub);
				return (NULL);
			}
		}
		else
		{
			free(pub);
			return (NULL);
		}
	}

	for (i = 0, j = 0; i < EC_PUB_LEN; i++, j += 2)
	{
		if (sscanf(address + j, upper ? "%02X" : "%02x", &scan) == -1)
		{
			perror("string_to_pub: sscanf");
			free(pub);
			return (NULL);
		}
		pub[i] = scan;
	}

	return (pub);
}
