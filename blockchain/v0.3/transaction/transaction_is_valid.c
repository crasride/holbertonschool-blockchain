#include "transaction.h"

/**
* cmp_tr_ins - filter unspent transactions
* choose only trx which belong to args[0]
* @node: current node
* @idx: index of @node
* @args: arguments:
* args[0] = llist_t *tx_inputs
* args[1] = uint32_t balance
* args[2] = uint32_t idx
* args[3] = uint8_t tx_id[SHA256_DIGEST_LENGTH]
* Return: 0 on success, 1 on failure
*/
int cmp_tr_ins(llist_node_t node, unsigned int idx, void *args)
{
	unspent_tx_out_t *unspent = node;
	void **ptr = args;
	uint32_t *tx_in_idx = ptr[2], *balance = ptr[1];
	llist_t *list = ptr[0];
	tx_in_t *tx_in = llist_get_node_at(list, *tx_in_idx);
	EC_KEY *key;

	(void)idx;
	if (!tx_in)
		return (1);
	if (!memcmp(unspent->out.hash, tx_in->tx_out_hash, SHA256_DIGEST_LENGTH))
	{
		*tx_in_idx += 1;
		*balance += unspent->out.amount;
		key = ec_from_pub(unspent->out.pub);
		if (!key)
			return (1);
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
* check_amounts - check if input transaction and output transactions match
* @node: current node
* @idx: index of @node
* @arg: balance
* Return: 0 on success, 1 on failure
*/
int check_amounts(llist_node_t node, unsigned int idx, void *arg)
{
	uint32_t *balance = arg;
	tx_out_t *out = node;

	(void)idx;
	if (*balance < out->amount)
	{
		return (1);
	}
	*balance -= out->amount;
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
	void *args[4];
	uint32_t idx = 0;
	uint32_t total_input = 0;

	if (!transaction || !all_unspent)
		return (0);
	transaction_hash(transaction, hash_buf);
	if (memcmp(transaction->id, hash_buf, SHA256_DIGEST_LENGTH))
	{
		return (0);
	}
	args[0] = transaction->inputs, args[1] = &total_input;
	args[2] = &idx, args[3] = (void *)&transaction->id;
	if (llist_for_each(all_unspent, cmp_tr_ins, args)
		|| idx != (uint32_t)llist_size(transaction->inputs))
	{
		return (0);
	}
	if (llist_for_each(transaction->outputs, check_amounts, &total_input)
		|| total_input != 0)
	{
		return (0);
	}
	return (1);
}

