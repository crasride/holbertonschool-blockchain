#include "blockchain.h"

/**
* block_mine - Mines a Block to insert it in the Blockchain
* @block: Pointer to the Block to be mined
*/
void block_mine(block_t *block)
{
	uint64_t nonce = 0;
	uint8_t hash[SHA256_DIGEST_LENGTH];

	while (1)
	{
		block->info.nonce = nonce;

		block_hash(block, hash);

		if (hash_matches_difficulty(hash, block->info.difficulty))
			break;
		nonce++;
	}

	memcpy(block->hash, hash, SHA256_DIGEST_LENGTH);
}
