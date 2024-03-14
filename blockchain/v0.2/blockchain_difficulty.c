#include "blockchain.h"


/**
* blockchain_difficulty - get difficulty of the next block in the blockchain
* @blockchain: pointer to the blockchain
*
* Return: difficulty of the next block in the blockchain
*/
uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *last_block, *adjusted_block;
	uint64_t expected_time, actual_time;
	uint32_t new_difficulty;

	if (!blockchain)
		return (0);

	last_block = llist_get_tail(blockchain->chain);
	if (!last_block)
		return (0);

	if (last_block->info.index == 0 ||
		last_block->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL)
		return (last_block->info.difficulty);

	adjusted_block = llist_get_node_at(blockchain->chain,
		llist_size(blockchain->chain) - DIFFICULTY_ADJUSTMENT_INTERVAL);
	if (!adjusted_block)
		return (0);

	expected_time = DIFFICULTY_ADJUSTMENT_INTERVAL * BLOCK_GENERATION_INTERVAL;
	actual_time = last_block->info.timestamp - adjusted_block->info.timestamp;

	if (actual_time < expected_time / 2)
		new_difficulty = last_block->info.difficulty + 1;
	else if (actual_time > expected_time * 2)
		new_difficulty = last_block->info.difficulty > 0 ?
			last_block->info.difficulty - 1 : 0;
	else
		new_difficulty = last_block->info.difficulty;

	return (new_difficulty);
}
