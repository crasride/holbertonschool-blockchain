#include "transaction.h"

/**
* find_unspent - finds an unspent transaction output in a list
* @node: current node
* @hash: hash to compare
* Return: 0 if match, 1 if not
**/
int find_unspent(llist_node_t node, void *hash)
{
	unspent_tx_out_t *unspent = (unspent_tx_out_t *)node;

	return (!memcmp(unspent->out.hash, hash, sizeof(unspent->out.hash)));
}

/**
* update_unspent - updates the list of unspent transaction outputs
* @transactions: list of transactions
* @block_hash: hash of the Block containing the transaction @tx_id
* @all_unspent: list of all unspent transaction outputs
* Return: updated list of unspent transaction outputs
**/
llist_t *update_unspent(llist_t *transactions,
				uint8_t block_hash[SHA256_DIGEST_LENGTH], llist_t *all_unspent)
{
	int i, j, buffer, size;
	transaction_t *transaction;
	tx_in_t *in;
	tx_out_t *out;
	unspent_tx_out_t *unspent;

	if (!transactions || !block_hash || !all_unspent)
		return (all_unspent);

	for (i = 0, size = llist_size(transactions); i < size; i++)
	{
		transaction = llist_get_node_at(transactions, i);
		for (j = 0, buffer = llist_size(transaction->inputs); j < buffer; j++)
		{
			in = llist_get_node_at(transaction->inputs, j);
			llist_remove_node(all_unspent, find_unspent, in->tx_out_hash, 1, NULL);
		}
		for (j = 0, buffer = llist_size(transaction->outputs); j < buffer; j++)
		{
			out = llist_get_node_at(transaction->outputs, j);
			unspent = unspent_tx_out_create(block_hash, transaction->id, out);
			llist_add_node(all_unspent, unspent, ADD_NODE_REAR);
		}
	}

	return (all_unspent);
}
