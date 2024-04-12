#include "cli.h"


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
int handle_wallet_create(EC_KEY **ec_key)
{
	*ec_key = ec_create();
	if (*ec_key)
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
int handle_wallet_save(EC_KEY *ec_key, char *path)
{
	if (!ec_key)
	{
		printf("No EC key available\n");
		return (-1);
	}
	if (ec_save(ec_key, path))
	{
		printf("Save wallet (EC key pair)\n");
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

int handle_wallet_load(EC_KEY **ec_key, char *path)
{
	*ec_key = ec_load(path);

	if (*ec_key)
	{
		printf("Load wallet (EC key pair)\n");
		return (0);
	}
	else
	{
		printf("Failed to load EC key\n");
		return (-1);
		if (*ec_key)
		EC_KEY_free(*ec_key);
	}
}

/**
* find_command - find the command to execute
* @cmd: command
* @arg1: argument 1
* @arg2: argument 2
* @ec_key: pointer to store EC key
* Return: 0 if ok, 1 if exit
*/
int find_command(char *cmd, char *arg1, char *arg2, EC_KEY **ec_key)
{
	if (strcmp(cmd, "exit") == 0)
	{
		return (handle_exit());
	}
	else if (strcmp(cmd, "wallet_create") == 0)
	{
		return (handle_wallet_create(ec_key));
	}
	else if (strcmp(cmd, "wallet_save") == 0)
	{
		if (!arg1)
		{
			printf("Usage: wallet_save <path>\n");
			return (-1);
		}
		else
		{
			return (handle_wallet_save(*ec_key, arg1));
		}
	}
	else if (strcmp(cmd, "wallet_load") == 0)
	{
		if (!arg1)
		{
			printf("Usage: wallet_load <path>\n");
			return (-1);
		}
		else
		{
			return (handle_wallet_load(ec_key, arg1));
		}
	}
	else
	{
		printf("unknown command: \"%s\"\n", cmd);
		return (-1);
	}
	(void)arg2;
}

/**
* cleanup - free allocated memory before exiting
* @ec_key: EC key pointer
*/
void cleanup(EC_KEY *ec_key)
{
	if (ec_key)
		EC_KEY_free(ec_key);
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
	cleanup(ec_key);

	return (ret != 0);
}
