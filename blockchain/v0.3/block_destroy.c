#include "blockchain.h"

/**
* block_destroy - Deletes an existing Block
* @block: Pointer to the Block to delete
*/
void block_destroy(block_t *block)
{
	/* Destroy the transaction list */
	llist_destroy(block->transactions, 1, (node_dtor_t)transaction_destroy);
	/* Free the block */
	free(block);
}
