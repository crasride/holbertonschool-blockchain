#include "blockchain.h"

/**
* block_destroy - Deletes an existing Block
* @block: Pointer to the Block to delete
*/
void block_destroy(block_t *block)
{
	/* Check if the block pointer is NULL */
	if (!block)
		return;
	/* Destroy the block's transactions */
	llist_destroy(block->transactions, 1, (node_dtor_t)transaction_destroy);
	free(block);
}
