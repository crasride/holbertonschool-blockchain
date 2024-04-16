#include "cli.h"
#include "../blockchain/v0.3/transaction/transaction.h"

/**
* handle_exit - handle the exit command
* Return: 1
*/
int handle_exit(void)
{
	printf("Exiting CLI\n");
	return (1);
}

/**
* handle_wallet_create - handle the wallet_create command
* @ec_key: pointer to store EC key
* Return: 0 if success, -1 if failed
*/
int handle_wallet_create(state_t *state)
{
	state->wallet = ec_create();
	if (state->wallet)
	{
		printf("Create wallet (EC key)\n");
		return (0);
	}
	else
	{
		printf("Failed to create EC key\n");
		return (-1);
	}
}

/**
* handle_wallet_save - handle the wallet_save command
* @ec_key: EC key
* @path: path to save the key pair
* Return: 0 if success, -1 if failed
*/
int handle_wallet_save(state_t *state, char *path)
{
	if (!state->wallet)
	{
		printf("No EC key available\n");
		return (-1);
	}
	if (ec_save(state->wallet, path))
	{
		printf("Save wallet (EC key pair)\n");
		EC_KEY_free(state->wallet);
		return (0);
	}
	else
	{
		printf("Failed to save EC key\n");
		return (-1);
	}
}

/**
* handle_wallet_load - handle the wallet_load command
* @ec_key: pointer to store EC key
* @path: path to load the key pair
* Return: 0 if success, -1 if failed
*/
int handle_wallet_load(state_t *state, char *path)
{
	state->wallet = ec_load(path);
	if (state->wallet)
	{
		printf("Load wallet (EC key pair)\n");
		/* Print <base64 encoded data> Key pair */
		/* EC_KEY_print_fp(stdout, state->wallet, 0); */
		return (0);
	}
	else
	{
		printf("Failed to load EC key\n");
		if (state->wallet)
			EC_KEY_free(state->wallet);
		return (-1);
	}
}


/* ------------------------------ Mine ------------------------------------ */

/**
* invalid_tx - filter invalid transactions
* @node: transaction
* @arg: list of unspent transactions
* Return: 1 if node is invalid,0 otherwise
*
*/
static int invalid_tx(llist_node_t node, void *arg)
{
	transaction_t *transaction = node;
	llist_t *unspent = arg;

	return (!transaction_is_valid(transaction, unspent));
}

/**
* add_transaction - add a transaction to a block
* @node: transaction
* @idx: transaction index
* @arg: block in wich the transaction will be added
* Return: 0 on succes -1 otherwise
*/
static int add_transaction(llist_node_t node, unsigned int idx, void *arg)
{
	transaction_t *transaction = node;
	block_t *block = arg;
	(void)idx;

	return (llist_add_node(block->transactions, transaction, ADD_NODE_REAR));
}

/**
* mine - mine a block
* @state: cli state
* @block: block to mine
* @prev_block: previous block in the blockchain
* @coinbase_tx: coinbase transaction
* @argv: arguments
* Return: EXIT_FAILURE if call to a function fails
*         EXIT_SUCCESS otherwise
*/
static int mine(state_t *state, block_t *block, block_t *prev_block,
		transaction_t *coinbase_tx)
{
	int size, i;
	unspent_tx_out_t *utxo = NULL;

	size = llist_size(state->tx_pool);
	for (i = 0; i < size; i++)
		llist_remove_node(state->tx_pool, invalid_tx, state->blockchain->unspent, 1,
				(node_dtor_t)transaction_destroy);

	llist_for_each(state->tx_pool, add_transaction, block);
	block->info.difficulty = blockchain_difficulty(state->blockchain);
	llist_add_node(block->transactions, coinbase_tx, ADD_NODE_FRONT);
	block_mine(block);
	if ((block_is_valid(block, prev_block, state->blockchain->unspent)) != 0)
	{
		fprintf(stderr, "mine: Failed to mine a valid block\n");
		while (llist_pop(block->transactions))
			;
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}
	state->blockchain->unspent = update_unspent(state->tx_pool,
					block->hash, state->blockchain->unspent);
	utxo = unspent_tx_out_create(block->hash, coinbase_tx->id,
				llist_get_head(coinbase_tx->outputs));
	if (!utxo)
	{
		fprintf(stderr, "mine: failed to create unspent tx output\n");
		while (llist_pop(block->transactions))
			;
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return ((state->status = EXIT_FAILURE));
	}
	while (llist_pop(state->tx_pool))
		;
	llist_add_node(state->blockchain->chain, block, ADD_NODE_REAR);
	llist_add_node(state->blockchain->unspent, utxo, ADD_NODE_REAR);
	fprintf(stdout, "Block Successfuly mined\n");
	return ((state->status = EXIT_SUCCESS));
}

/**
* handle_mine - mine a block
* @state: cli state
* Return: 2 if number of arguments are wrong,
*         EXIT_FAILURE if call to a function fails
*         EXIT_SUCCESS otherwise
*/
int handle_mine(state_t *state)
{
	int8_t block_data[1024] = {0};

	block_t *block = NULL;
	block_t *prev_block = llist_get_tail(state->blockchain->chain);
	transaction_t *coinbase_tx = NULL;

	if (!state->wallet)
	{
		fprintf(stderr, "mine: No wallet available\n");
		return (-1);
	}
	block = block_create(prev_block, block_data, 1024);
	if (!block)
	{
		fprintf(stderr, "mine: failed to create block\n");
		return (-1);
	}

	coinbase_tx = coinbase_create(state->wallet, block->info.index);
	if (!coinbase_tx)
	{
		fprintf(stderr, "mine: failed to create coinbase transaction\n");
		block_destroy(block);
		return (-1);
	}
	if (!coinbase_is_valid(coinbase_tx, block->info.index))
	{
		fprintf(stderr,
			"mine: failed to create valid coinbase transaction\n");
		transaction_destroy(coinbase_tx);
		block_destroy(block);
		return (-1);
	}

	return (mine(state, block, prev_block, coinbase_tx));
}

/* -------------------------End Mine------------------------------------------ */
/**
* handle_save - save blockchain to a file
* @state: cli state
* @path: path to save the blockchain
* Return: 2 if number of arguments are wrong,
*         EXIT_FAILURE if call to a function fails
*         EXIT_SUCCESS otherwise
*/
int handle_save(state_t *state, char *path)
{
	if (blockchain_serialize(state->blockchain, path) == -1)
	{
		fprintf(stdout, "Failed to save blockchain in file %s\n",
			path);
		return (-1);
	}

	fprintf(stdout, "Saved blockchain to %s\n", path);
	return (0);
}

/* ------------------------Load--------------------------------------------- */
/**
* handle_load - load the blockchain from a file
* @state: cli state
* @path: path to load the blockchain
* Return: 2 if number of arguments are wrong,
*         EXIT_FAILURE if call to a function fails
*         EXIT_SUCCESS otherwise
*/
int handle_load(state_t *state, char *path)
{
	blockchain_t *blockchain = NULL;

	blockchain = blockchain_deserialize(path);

	if (!blockchain)
	{
		fprintf(stdout, "Failed to load blockchain from %s\n",
			path);
		return (-1);
	}
	blockchain_destroy(state->blockchain);
	state->blockchain = blockchain;
	fprintf(stdout, "Loaded blockchain from %s\n", path);
	return (0);
}

/* ------------------------End Load--------------------------------------------- */
static int sum_unspent(void *node, unsigned int idx, void *arg)
{
	unspent_tx_out_t *unspent_tx_out = node;
	uint32_t *total = arg;
	(void)idx;
	*total += unspent_tx_out->out.amount;
	return (0);
}


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
		printf("Number of pending transactions in transaction pool: %u\n", llist_size(tx_pool));
	}
	else
	{
		printf("Error: Transaction pool is NULL\n");
	}
	printf("Coins: %u\n", coins);

	return (0);
}


/**
* find_command - find the command to execute
* @cmd: command
* @arg1: argument 1
* @arg2: argument 2
* @ec_key: pointer to store EC key
* Return: 0 if ok, 1 if exit
*/
int find_command(char *cmd, char *arg1, char *arg2, state_t *state, char *receiver_address)
{
	if (strcmp(cmd, "exit") == 0)
		return (handle_exit());
	else if (strcmp(cmd, "wallet_create") == 0)
	{
		if (arg1)
		{
			printf("Usage: wallet_create\n");
			printf("Too many arguments\n");
			return (-1);
		}
		else
			return (handle_wallet_create(state));
	}
	else if (strcmp(cmd, "wallet_save") == 0)
	{
		if (!arg1)
		{
			printf("Usage: wallet_save <path>\n");
			return (-1);
		}
		else
			return (handle_wallet_save(state, arg1));
	}
	else if (strcmp(cmd, "wallet_load") == 0)
	{
		if (!arg1)
		{
			printf("Usage: wallet_load <path>\n");
			return (-1);
		}
		else
			return (handle_wallet_load(state, arg1));
	}
	else if (strcmp(cmd, "mine") == 0)
	{
		if (arg1)
		{
			printf("Usage: mine\n");
			printf("Too many arguments\n");
			return (-1);
		}
		else
			return (handle_mine(state));
	}
	else if (strcmp(cmd, "send") == 0)
	{
		if (!arg1 || !arg2)
		{
			printf("Usage: send <amount> <address>\n");
			return (-1);
		}
		else
		{
			int amount = atoi(arg1);
			strncpy(receiver_address, arg2, (EC_PUB_LEN * 2) + 1);
			return (handle_send(amount, receiver_address, state));
		}
	}
	else if (strcmp(cmd, "info") == 0)
		return handle_info(state);
	else if (strcmp(cmd, "load") == 0)
	{
		if (!arg1)
		{
			printf("Usage: load <path>\n");
			return (-1);
		}
		else
			return (handle_load(state, arg1));
	}
	else if (strcmp(cmd, "save") == 0)
	{
		if (!arg1)
		{
			printf("Usage: save <path>/<file>\n");
			return (-1);
		}
		else
			return (handle_save(state, arg1));
	}
	else
	{
		printf("unknown command: \"%s\"\n", cmd);
		return (-1);
	}
}

/**
* _get_endianness - Get current endianness
*
* Return: 1 for little endian, 2 for big endian, 0 for unknown
*/
uint8_t _get_endianness(void)
{
	_endian_t order = { 1 };

	if (order.bytes[0] == 1)
		return (1); /* Little endian */
	if (order.bytes[0] == 0)
		return (2); /* Big endian */

	return (0); /* Should not happen */
}

/**
* string_to_pub - converts a EC_KEY public key in a lowercase or
* uppercase hex string to a byte array
*
* @address: public key of EC_PUB_LEN bytes, in hex
*
* Return: byte array on success, or NULL on failure
*/
uint8_t *string_to_pub(char *address)
{
	uint8_t *pub;
	unsigned int scan;
	size_t i, j, len, upper, case_detected;

	if (!address)
	{
		fprintf(stderr, "string_to_pub: NULL parameter\n");
		return (NULL);
	}
	pub = calloc(EC_PUB_LEN, sizeof(uint8_t));
	if (!pub)
	{
		fprintf(stderr, "string_to_pub: calloc failure\n");
		return (NULL);
	}
	len = strlen(address);
	if (len < (EC_PUB_LEN * 2))
	{
		free(pub);
		return (NULL);
	}

	for (i = 0, case_detected = 0; i < len; i++)
	{
		if (isdigit(address[i]))
			continue;
		else if (address[i] >= 'a' && address[i] <= 'f')
		{
			if (!case_detected)
			{
				upper = 0;
				case_detected = 1;
			}
			if (upper)
			{
				free(pub);
				return (NULL);
			}
		}
		else if (address[i] >= 'A' && address[i] <= 'F')
		{
			if (!case_detected)
			{
				upper = 1;
				case_detected = 1;
			}
			if (!upper)
			{
				free(pub);
				return (NULL);
			}
		}
		else
		{
			free(pub);
			return (NULL);
		}
	}

	for (i = 0, j = 0; i < EC_PUB_LEN; i++, j += 2)
	{
		if (sscanf(address + j, upper ? "%02X" : "%02x", &scan) == -1)
		{
			perror("string_to_pub: sscanf");
			free(pub);
			return (NULL);
		}
		pub[i] = scan;
	}

	return (pub);
}

/**
* handle_send - handle the send command
* @amount: amount to send
* @receiver_address: receiver address
* @sender_key: sender key
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
		return -1;
	}
	for (i = 0; i < (EC_PUB_LEN); i++)
		printf("%02x", pub[i]);
	printf("\n");

	receiver_key = ec_from_pub(pub);
	if (!receiver_key)
	{
		printf("Failed to create receiver key\n");
		free(pub);
		return -1;
	}

	tx = transaction_create(state->wallet, receiver_key, amount, state->blockchain->unspent);
	if (tx)
	{
		printf("Transaction created successfully\n");
	}
	else
	{
		printf("Failed to create transaction\n");
		EC_KEY_free(receiver_key);
		free(pub);
		return (-1);
	}

	if ((transaction_is_valid(tx, state->blockchain->unspent)) == 1)
	{
		printf("Transaction is valid\n");
	}
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
		return -1;
	}

	printf("Transaction sent successfully\n");

	EC_KEY_free(receiver_key);
	free(pub);
	return 0;
}

/**
* cleanup - free allocated memory before exiting
* @ec_key: EC key pointer
*/
void cleanup(state_t *state)
{
	if (state)
	{
		if (state->wallet)
			EC_KEY_free(state->wallet);
		if (state->tx_pool)
			llist_destroy(state->tx_pool, 1, (node_dtor_t)transaction_destroy);
		if (state->blockchain)
			blockchain_destroy(state->blockchain);
		free(state);
	}
}

/**
* main - Entry point
* Return: 0 if ok, 1 if failed
*/
int main(void)
{
	ssize_t read;
	size_t len;
	int ret = 0;
	char *line = NULL, *cmd, *arg1, *arg2, receiver_address[EC_PUB_LEN * 2];
	state_t *state = NULL;
	blockchain_t *blockchain = blockchain_create();
	llist_t *tx_pool = llist_create(MT_SUPPORT_FALSE);

	if (!blockchain)
	{
		fprintf(stderr, "Failed to create blockchain\n");
		return (1);
	}
	if (!tx_pool)
	{
		fprintf(stderr, "Failed to create list transaction pool\n");
		blockchain_destroy(blockchain);
		return (1);
	}
	state = calloc(1, sizeof(state_t));
	if (!state)
	{
		fprintf(stderr, "Failed to allocate memory for state\n");
		blockchain_destroy(blockchain);
		llist_destroy(tx_pool, 1, NULL);
		return (1);
	}
	state->blockchain = blockchain;
	state->tx_pool = tx_pool;
	state->status = 0;
	state->wallet = NULL;

	while (1)
	{
		printf("CLI prompt> ");
		read = getline(&line, &len, stdin);
		if (read == -1)
			break;
		if (read <= 1)
			continue;
		line = strtok(line, "\n");
		cmd = strtok(line, " ");
		arg1 = strtok(NULL, " ");
		arg2 = strtok(NULL, " ");
		/* remember to try with & for state */
		ret = find_command(cmd, arg1, arg2, state, receiver_address);
		if (ret == 1)
			break;
	}
	if (line)
		free(line);
	cleanup(state);
	return (ret != 0);
}
