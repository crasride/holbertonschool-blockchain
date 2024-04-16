#include "cli.h"

/**
* handle_info - handle the info command
* @state: CLI state
* Return: 0 if success, -1 if failed
*/

int handle_info(state_t *state)
{
	blockchain_t *blockchain = state->blockchain;
	llist_t *unspent = blockchain->unspent;
	llist_t *tx_pool = state->tx_pool;
	uint32_t coins = 0;

	if (!state || !blockchain)
	{
		printf("Error: State or Blockchain is NULL\n");
		return (-1);
	}

	if (!state->blockchain)
	{
		printf("Error: Blockchain is NULL\n");
		return (-1);
	}

	llist_for_each(state->blockchain->unspent, sum_unspent, &coins);

	printf("Number of Blocks in Blockchain: %u\n", llist_size(blockchain->chain));
	printf("Number of unspent transaction outputs: %u\n", llist_size(unspent));

	if (tx_pool)
	{
		printf("Transaction pool is available\n");
		printf("Number of pending transactions in transaction pool: %u\n",
				llist_size(tx_pool));
	}
	else
	{
		printf("Error: Transaction pool is NULL\n");
	}
	printf("Coins: %u\n", coins);

	return (0);
}
