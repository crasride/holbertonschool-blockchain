#include "hblk_crypto.h"

/**
* ec_save - Saves an EC key pair on disk
*
* @key: Pointer to the EC key pair to be saved
* @folder: Path to the folder in which to save the keys
*
* Return: 1 on success, 0 on failure
*/
int ec_save(EC_KEY *key, char const *folder)
{
	char path_priv[BUFSIZ];
	char path_pub[BUFSIZ];

	/* Check if key or folder is NULL */
	if (!key || !folder)
		return (0);
	/* Create the paths for private and public key files */
	sprintf(path_priv, "%s/key.pem", folder);
	sprintf(path_pub, "%s/key_pub.pem", folder);
	/* Ensure the folder exists */
	if (access(folder, F_OK) == -1)
	{
		if (mkdir(folder, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) == -1)
		{
			perror("mkdir");
			return (0);
		}
	}
	/* Save private key to file */
	FILE *priv_file = fopen(path_priv, "w");

	if (!priv_file)
	{
		perror("fopen");
		return (0);
	}
	if (PEM_write_ECPrivateKey(priv_file, key, NULL, NULL, 0, NULL, NULL) == 0)
	{
		fprintf(stderr, "Failed to write private key to file\n");
		fclose(priv_file);
		return (0);
	}

	fclose(priv_file);
	/* Save public key to file */
	FILE *pub_file = fopen(path_pub, "w");

	if (!pub_file)
	{
		perror("fopen");
		return (0);
	}
	if (PEM_write_EC_PUBKEY(pub_file, key) == 0)
	{
		fprintf(stderr, "Failed to write public key to file\n");
		fclose(pub_file);
		return (0);
	}
	fclose(pub_file);

	return (1);
}
