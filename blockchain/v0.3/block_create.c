#include "blockchain.h"

/**
 * block_create - Creates a Block structure and initializes it
 * @prev: Pointer to the previous Block in the Blockchain
 * @data: Points to a memory area to duplicate in the Block’s data
 * @data_len: Number of bytes to duplicate in data
 *
 * Return: A pointer to the allocated Block
 */
block_t *block_create(block_t const *prev, int8_t const *data,
					uint32_t data_len)
{
	block_t *block; /* Block to create */
	uint32_t max_len; /* Maximum length of data to copy */

	/* Check if the previous block and data pointers are NULL */
	if (!prev || !data)
		return (NULL);

	/* Allocate memory for the new block */
	block = calloc(1, sizeof(*block));
	if (!block)
		return (NULL);

	/* Initialize the Block's transaction list to an empty linked list */
	block->transactions = llist_create(MT_SUPPORT_FALSE);
	if (!block->transactions)
	{
		free(block);
		return (NULL);
	}

	/* Set the block's information */
	memcpy(block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);
	block->info.index = prev->info.index + 1;
	block->info.timestamp = (uint64_t)time(NULL);

	/* Copy the provided data to the block's data */
	max_len = data_len > BLOCKCHAIN_DATA_MAX ? BLOCKCHAIN_DATA_MAX : data_len;
	memcpy(block->data.buffer, data, max_len);
	block->data.len = max_len;

	return (block);
}
