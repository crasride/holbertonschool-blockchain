#include "hblk_crypto.h"

/**
 * ec_load - Loads an EC key pair from disk
 *
 * @folder: Path to the folder from which to load the keys
 *
 * Return: Pointer to the created EC key pair on success, or NULL on failure
 */
EC_KEY *ec_load(char const *folder)
{
	char path_priv[BUFSIZ];
	char path_pub[BUFSIZ];
	FILE *priv_file, *pub_file;
	EC_KEY *key;

	if (!folder)
		return (NULL);

	sprintf(path_priv, "%s/key.pem", folder);
	sprintf(path_pub, "%s/key_pub.pem", folder);
	priv_file = fopen(path_priv, "r");
	if (!priv_file)
	{
		perror("fopen");
		return (NULL);
	}
	key = PEM_read_ECPrivateKey(priv_file, NULL, NULL, NULL);
	fclose(priv_file);
	if (!key)
	{
		fprintf(stderr, "Failed to read private key from file\n");
		return (NULL);
	}
	pub_file = fopen(path_pub, "r");
	if (!pub_file)
	{
		perror("fopen");
		EC_KEY_free(key);
		return (NULL);
	}
	if (PEM_read_EC_PUBKEY(pub_file, &key, NULL, NULL) == 0)
	{
		fprintf(stderr, "Failed to read public key from file\n");
		fclose(pub_file);
		EC_KEY_free(key);
		return (NULL);
	}
	fclose(pub_file);

	return (key);
}
