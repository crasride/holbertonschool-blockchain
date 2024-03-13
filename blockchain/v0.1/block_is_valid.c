#include "blockchain.h"

/**
* block_is_valid - Verifies if a block is valid
* @block: Pointer to the block to check
* @prev_block: Pointer to the previous block in the blockchain
*
* Return: 1 if the block is valid, otherwise 0
*/
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	block_t genesis = GENESIS_BLOCK;

	if (block == NULL)
		return (0);

	if (prev_block == NULL)
	{
		if (block->info.index != 0)
			return (0);

		if (memcmp(block, &genesis, sizeof(block_t)) != 0)
			return (0);
	}
	else
	{
		if (block->info.index != prev_block->info.index + 1 ||
			memcmp(block->info.prev_hash, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
			return (0);
	}

	if (block_hash(block, NULL) == NULL ||
		memcmp(block->hash, block->info.hash, SHA256_DIGEST_LENGTH) != 0)
		return (0);

	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (0);

	return (1);
}
