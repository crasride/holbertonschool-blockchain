#include "transaction.h"

/**
* coinbase_create - create coinbase transaction
* @receiver: pub key of a receiver
* @block_index: index of block of this transaction
* Return: a new coinbase transaction
*/
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *transaction; /* New transaction */
	tx_out_t *output; /* New transaction output */
	tx_in_t *input; /* New transaction input */
	uint8_t pub[EC_PUB_LEN]; /* Buffer for public key */

	/* Verificar que el índice de bloque no sea negativo */
	if (block_index == 0)
	{
		fprintf(stderr, "Error: Invalid block index\n");
		return (NULL);
	}

	/* Check if the public key conversion is successful */
	if (!ec_to_pub(receiver, pub))
		return (NULL);
	/* Create a new transaction output with the amount of COINBASE_AMOUNT */
	output = tx_out_create(COINBASE_AMOUNT, pub);
	if (!output)
		return (NULL);
	/* Allocate memory for the transaction input and check if it is successful */
	input = calloc(1, sizeof(*input));
	if (!input)
		return (free(output), NULL);
	/* Copy the block index to the first 4 bytes of the transaction output hash */
	memcpy(input->tx_out_hash, &block_index, sizeof(block_index));
	/* Allocate memory for the transaction and check if it is successful */
	transaction = calloc(1, sizeof(*transaction));
	if (!transaction)
		return (free(input), free(output), NULL);
	/* Initialize transaction input and output lists */
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	/* Add the input and output to the corresponding transaction lists*/
	llist_add_node(transaction->inputs, input, ADD_NODE_REAR);
	llist_add_node(transaction->outputs, output, ADD_NODE_REAR);
	/* Calculate transaction hash */
	transaction_hash(transaction, transaction->id);
	/* Return the new coinbase transaction */
	return (transaction);
}
