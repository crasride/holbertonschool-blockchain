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
	block_t *new_block = malloc(sizeof(block_t));

	if (!new_block)
		return (NULL);

	new_block->info.index = prev->info.index + 1;
	new_block->info.difficulty = 0;
	new_block->info.nonce = 0;
	new_block->info.timestamp = time(NULL);
	memset(new_block->info.prev_hash, 0, SHA256_DIGEST_LENGTH);

	if (prev)
		memcpy(new_block->info.prev_hash, prev->hash, SHA256_DIGEST_LENGTH);

	if (data_len > BLOCKCHAIN_DATA_MAX)
		data_len = BLOCKCHAIN_DATA_MAX;

	memcpy(new_block->data.buffer, data, data_len);
	new_block->data.len = data_len;

	memset(new_block->hash, 0, SHA256_DIGEST_LENGTH);

	return (new_block);
}


