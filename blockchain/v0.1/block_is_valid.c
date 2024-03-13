#include "blockchain.h"

/**
* cleanup_hash - Clears a hash by setting all its bytes to 0
* @hash: Pointer to the hash to clear
* @hash_len: Length of the hash in bytes
*/
void cleanup_hash(uint8_t *hash, size_t hash_len)
{
	memset(hash, 0, hash_len);
}


/**
* block_is_valid - Verifies if a block is valid
* @block: Pointer to the block to check
* @prev_block: Pointer to the previous block in the blockchain
*
* Return: 1 if the block is valid, otherwise 0
*/
int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t hash_verify[SHA256_DIGEST_LENGTH] = {0};
	block_t const genesis = GENESIS_BLOCK;

	if (!block || (!prev_block && block->info.index != 0))
		return (1);

	if (block->info.index == 0)
		return (memcmp(block, &genesis, sizeof(genesis)));

	if (block->info.index != prev_block->info.index + 1)
		return (1);

	if (block_hash(prev_block, hash_verify) ||
		memcmp(hash_verify, prev_block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);

	if (memcmp(prev_block->hash, block->info.prev_hash,
													SHA256_DIGEST_LENGTH) != 0)
		return (1);

	cleanup_hash(hash_verify, SHA256_DIGEST_LENGTH);

	if (block_hash(block, hash_verify) == NULL ||
		memcmp(hash_verify, block->hash, SHA256_DIGEST_LENGTH) != 0)
		return (1);

	if (block->data.len > BLOCKCHAIN_DATA_MAX)
		return (1);

	return (0);
}
