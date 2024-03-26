#include "transaction.h"

/**
* select_unspent_transaction - Selects unspent transactions that match the
* given transaction inputs.
* @node: Current node representing an unspent transaction output.
* @idx: Index of the current node (unused).
* @args: Arguments passed to the function:
* args[0] = Pointer to the list of transaction inputs (llist_t *tx_inputs).
* args[1] = Pointer to the current balance (uint32_t *balance).
* args[2] = Pointer to the index the current transaction input (uint32_t *idx).
* args[3] = Transaction ID (uint8_t tx_id[SHA256_DIGEST_LENGTH]).
* Return: 0 on success (matching unspent transaction found), 1 on failure
* (no match found or error).
*/
int select_unspent_transaction(llist_node_t node, unsigned int idx, void *args)
{
	unspent_tx_out_t *unspent = node; /* Unspent transaction output */
	void **ptr = args;/* Array of arguments */
	/* Pointer to the index of the current transaction input */
	uint32_t *tx_in_idx = ptr[2];
	uint32_t *balance = ptr[1]; /*Pointer to the current balance */
	llist_t *list = ptr[0]; /* List of transaction inputs */
	/*Current transaction input */
	tx_in_t *tx_in = llist_get_node_at(list, *tx_in_idx);
	EC_KEY *key; /* Public key */
	/**
	* printf("ptr[0] (transaction entry list): %p\n", ptr[0]);
	* printf("ptr[1] (Balance): %u\n", *(uint32_t *)ptr[1]);
	* printf("ptr[2] (input transaction id): %u\n", *(uint32_t *)ptr[2]);
	* printf("ptr[3] (transaction ID): %p\n", ptr[3]);
	*/
	(void)idx;/* Avoid the warning "unused parameter" */
	/* Check if the unspent transaction matches the transaction input */
	if (!tx_in)
		return (1);

	if (!memcmp(unspent->out.hash, tx_in->tx_out_hash, SHA256_DIGEST_LENGTH))
	{
		/* Increment the index and balance */
		*tx_in_idx += 1;
		/* Update the balance */
		*balance += unspent->out.amount;
		/* Create a new transaction input */
		key = ec_from_pub(unspent->out.pub);
		/* Check if the public key is valid */
		if (!key)
			return (1);
		/* Verify the signature of the transaction input */
		if (!ec_verify(key, ptr[3], SHA256_DIGEST_LENGTH, &tx_in->sig))
		{
			EC_KEY_free(key);
			return (1);
		}
		EC_KEY_free(key);
	}
	return (0);
}

/**
* verify_output_amounts - Verifies if the total output amounts are valid.
* @node: Pointer to the current node containing the transaction output.
* @idx: Index of the current node (unused).
* @arg: Pointer to the remaining balance to be checked.
* Return: 0 on success (valid output amounts), 1 on failure (invalid amounts).
*/
int verify_output_amounts(llist_node_t node, unsigned int idx, void *arg)
{
	/* Cast the argument to a pointer to the remaining balance */
	uint32_t *remaining_balance = arg;
	/* Cast the node to a pointer to the transaction output */
	tx_out_t *output = node;

	(void)idx;/* Avoid the warning "unused parameter" */

	/* Check if the remaining balance is less than the output amount */
	if (*remaining_balance < output->amount)
		return (1);

	/* Deduct the output amount from the remaining balance */
	*remaining_balance -= output->amount;

	/* Output amount is valid, continue processing */
	return (0);
}

/**
* transaction_is_valid - validate transaction
* @transaction: transaction to be validated
* @all_unspent: unpspent outputs
* Return: 1 if valid, 0 otherwise
*/
int transaction_is_valid(transaction_t const *transaction,
							llist_t *all_unspent)
{
	uint8_t hash_buf[SHA256_DIGEST_LENGTH];
	void *args[4]; /* Array of arguments */
	uint32_t idx = 0; /* Index of the current transaction input */
	uint32_t input_amount = 0; /* Total input amount */

	if (!transaction || !all_unspent)
		return (0);
	/* Compute the hash of the transaction */
	transaction_hash(transaction, hash_buf);
	/* Compare the computed hash with the transaction ID */
	if (memcmp(transaction->id, hash_buf, SHA256_DIGEST_LENGTH))
	{
		return (0);
	}
	/* Select unspent transactions that match the transaction inputs */
	args[0] = transaction->inputs, args[1] = &input_amount;
	args[2] = &idx, args[3] = (void *)&transaction->id;
	/* Verify the transaction inputs */
	if (llist_for_each(all_unspent, select_unspent_transaction, args)
		|| idx != (uint32_t)llist_size(transaction->inputs))
	{
		return (0);
	}
	/* Verify the transaction outputs */
	if (llist_for_each(transaction->outputs, verify_output_amounts, &input_amount)
		|| input_amount != 0)
	{
		return (0);
	}
	return (1);
}
