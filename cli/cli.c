#include "cli.h"

/**
* find_command - find the command to execute
* @cmd: command
* @arg1: argument 1
* @arg2: argument 2
* @ec_key: EC key
* Return: 0 if ok, 1 if exit
*/
int find_command(char *cmd, char *arg1, char *arg2, EC_KEY **ec_key)
{
	if (strcmp(cmd, "exit") == 0)
	{
		return 1;
	}
	else if (strcmp(cmd, "wallet_create") == 0)
	{
		*ec_key = ec_create();
		if (*ec_key)
			printf("Create wallet (EC key)\n");
		else
			printf("Failed to create EC key\n");
	}
	else if (strcmp(cmd, "wallet_save") == 0)
	{
		if (!arg1)
			printf("Usage: wallet_save <path>\n");
		else
		{
			if (!*ec_key)
				printf("No ec_key variable\n");
			else
			{
				ec_save(*ec_key, arg1);
				printf("Save wallet (EC key pair)\n");
			}
		}
	}
	else if (strcmp(cmd, "wallet_load") == 0)
	{
		if (!arg1)
			printf("Usage: wallet_load <path>\n");
		else
		{
			*ec_key = ec_load(arg1);
			if (*ec_key)
				printf("Load wallet (EC key pair)\n");
			else
				printf("Failed to load EC key\n");
		}
	}
	else if (strcmp(cmd, "send") == 0)
	{
		if (!arg1 || !arg2)
		{
			printf("Usage: send <amount> <address>\n");
		}
		else
		{
			int amount = atoi(arg1);
			char *address = arg2;
			uint8_t pub[EC_PUB_LEN] = {0};
			EC_KEY *receiver = ec_from_pub(pub);
			transaction_t *tx = NULL;
			llist_t *transaction_pool = NULL;

			if (!receiver)
			{
				printf("Invalid receiver public key\n");
				return 0;
			}

			transaction_pool = llist_create(0); /* O llist_add_node */
			if (!transaction_pool)
			{
				printf("Failed to create transaction pool\n");
				return 0;
			}

			tx = transaction_create(*ec_key, receiver, amount, transaction_pool);
			if (tx)
			{
				if (transaction_is_valid(tx, llist_create(0)))
				{
					llist_add_node(transaction_pool, tx, ADD_NODE_REAR);
					printf("Transaction successfully added to pool\n");
				}
				else
				{
					printf("Invalid transaction\n");
					/* free_transaction(tx); */
				}
			}
			else
			{
				printf("Failed to create transaction\n");
			}
			llist_destroy(transaction_pool, 1, (node_dtor_t)transaction_destroy);
		}
	}
	else
	{
		printf("unknown command: \"%s\"\n", cmd);
	}

	return 0;
}



/**
* main - Entry point
* Return: 0 if ok, 1 if failed
*/
int main(void)
{
	EC_KEY *ec_key = NULL;
	ssize_t read;
	size_t len;
	int ret = 0;
	char *line = NULL, *cmd, *arg1, *arg2;

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

		ret = find_command(cmd, arg1, arg2, &ec_key);
		if (ret == 1)
			break;
	}
	if (line)
		free(line);
	return (0);
}
