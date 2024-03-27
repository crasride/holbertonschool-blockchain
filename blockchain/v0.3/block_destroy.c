#include "blockchain.h"

/**
* block_destroy - Deletes an existing Block
* @block: Pointer to the Block to delete
*/
void block_destroy(block_t *block)
{
	/* Check if the block pointer is NULL */
	if (block)
		free(block);
}
