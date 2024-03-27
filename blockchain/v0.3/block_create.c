#include "blockchain.h"
#include <time.h>

/**
 * block_create - Creates a block structure and init it
 * @prev: pointer to the previous Block in chain
 * @data: points to memory area to duplicate in Block's data
 * @data_len: stores number of bytes to dup in data
 * -> constraint : must not overflow macro BLOCKCHAIN_DATA_MAX
 * otherwise BLOCKCHAIN_DATA_MAX must be duplicated instead
 *
 * -- Update Inserted --
 * Now init the Block's transaction list ot an empty linked list (l.32)
 *
 * Return: Pointer to newly allocated Block
 */

block_t *block_create(block_t const *prev, int8_t const *data,
				uint32_t data_len)
{
	block_t *new = calloc(1, sizeof(*new));

	if (!new)
		return (NULL);

	new->info.index = prev->info.index + 1;
	new->info.difficulty = new->info.nonce = 0;
	new->info.timestamp = time(NULL);
	memcpy(&(new->info.prev_hash), prev->hash, SHA256_DIGEST_LENGTH);
	memcpy(&(new->data.buffer), data, MIN(data_len, BLOCKCHAIN_DATA_MAX));
	new->data.len = MIN(data_len, BLOCKCHAIN_DATA_MAX);
	new->transactions = llist_create(MT_SUPPORT_FALSE);
	memset(&(new->hash), 0x0, SHA256_DIGEST_LENGTH);

	return (new);
}
