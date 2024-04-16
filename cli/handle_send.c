#include "cli.h"

/**
* handle_send - handle the send command
* @amount: amount to send
* @receiver_address: receiver address
* @state: pointer to store EC key
* Return: 0 if success, -1 if failed
*/
int handle_send(int amount, char *receiver_address, state_t *state)
{
	uint8_t *pub;
	EC_KEY *receiver_key = NULL;
	transaction_t *tx = NULL;
	int i;

	pub = string_to_pub(receiver_address);

	if (!pub)
	{
		printf("Failed to convert receiver address\n");
		return (-1);
	}
	for (i = 0; i < (EC_PUB_LEN); i++)
		printf("%02x", pub[i]);
	printf("\n");

	receiver_key = ec_from_pub(pub);
	if (!receiver_key)
	{
		printf("Failed to create receiver key\n");
		free(pub);
		return (-1);
	}

	tx = transaction_create(state->wallet, receiver_key, amount,
							state->blockchain->unspent);
	if (tx)
		printf("Transaction created successfully\n");
	else
	{
		printf("Failed to create transaction\n");
		EC_KEY_free(receiver_key);
		free(pub);
		return (-1);
	}

	if ((transaction_is_valid(tx, state->blockchain->unspent)) == 1)
		printf("Transaction is valid\n");
	else
	{
		printf("Invalid transaction\n");
		transaction_destroy(tx);
		EC_KEY_free(receiver_key);
		free(pub);
		return (-1);
	}

	if (llist_add_node(state->tx_pool, tx, ADD_NODE_REAR) == -1)
	{
		printf("Failed to add transaction to pool\n");
		transaction_destroy(tx);
		EC_KEY_free(receiver_key);
		free(pub);
		return (-1);
	}
	printf("Transaction sent successfully\n");
	EC_KEY_free(receiver_key);
	free(pub);
	return (0);
}
