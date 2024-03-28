#include "blockchain.h"
#include "./provided/endianness.h"

uint8_t _get_endianness(void);

/**
* write_transactions - serialize the transactions to the file descriptor "fd"
* @transactions: Points to Linked list of transactions
* @fd: file descriptor
* Return: Nothing
* Transactions are serialized as a table described:
* -- Transaction ID (hash)
* -- Number of tx inputs
*		|__ list of tx inputs -- input "node" * 169 bytes
* -- Number of tx outputs
*		|__ list of tx outputs -- output "node" * 101 bytes
* -- Then jump back to `blockchain_serialize` to finally fwrite the utxo's
*/
void write_transactions(llist_t *transactions, int fd)
{
	int i, j, tx_in, tx_out;
	transaction_t *t_node;
	tx_in_t *in_node;
	tx_out_t *out_node;

	for (i = 0; i < llist_size(transactions); i++)
	{
		t_node = llist_get_node_at(transactions, i);
		tx_in = llist_size(t_node->inputs);
		tx_out = llist_size(t_node->outputs);
		write(fd, &t_node->id, SHA256_DIGEST_LENGTH);
		write(fd, &tx_in, 4);
		write(fd, &tx_out, 4);
		/* Now list of tx_inputs => 169 times number of inputs */
		for (j = 0; j < tx_in; j++)
		{
			in_node = llist_get_node_at(t_node->inputs, j);
			write(fd, in_node, 169);
		}
		/* Same for list of outputs => 101 times number of outputs */
		for (j = 0; j < tx_out; j++)
		{
			out_node = llist_get_node_at(t_node->outputs, j);
			write(fd, out_node, 101);
		}
	}
}

/**
* init_block_header - initializes a block_header struct
* @header: Pointer to block_header struct
*/
void init_block_header(block_header_t *header)
{
	if (!header)
		return;

	memcpy(header->magic, "HBLK", 4);
	memcpy(header->version, "0.3", 3);
	header->endian = _get_endianness();
	header->blocks = 0;
	header->unspent = 0;
}

/**
* blockchain_serialize - serializes a Blockchain into a file
* @blockchain: Points to the blockchain to serialize
* @path: Contains the path to a file to serialize Blockchain into
*        must be overwritten if pointing to an existing file
* Return: 0 on Success, -1 upon Failure
*/
int blockchain_serialize(blockchain_t const *blockchain,
						char const *path)
{
	int fd = -1;
	uint32_t i;
	block_header_t header;
	block_t *block;
	unspent_tx_out_t *unspent_node;
	int tx_size = 0;

	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd < 0 || !blockchain || !path)
		return (-1);

	init_block_header(&header);
	header.blocks = llist_size(blockchain->chain);
	header.unspent = llist_size(blockchain->unspent);
	write(fd, &header, sizeof(header));

	for (i = 0; i < header.blocks; i++)
	{
		block = llist_get_node_at(blockchain->chain, i);
		write(fd, block, sizeof(block->info));
		write(fd, &block->data.len, sizeof(block->data.len));
		write(fd, block->data.buffer, block->data.len);
		write(fd, &block->hash, SHA256_DIGEST_LENGTH);
		/* If we are on genesis block */
		if (block->info.index == 0)
			tx_size = -1;
		else
			tx_size = llist_size(block->transactions);
		write(fd, &tx_size, 4);
		write_transactions(block->transactions, fd);
	}
	/* Unspent tx output serialized contiguously too */
	/* First one right after the last serialized Block */
	for (i = 0; i < header.unspent; i++)
	{
		unspent_node = llist_get_node_at(blockchain->unspent, i);
		write(fd, unspent_node, 165);
	}
	close(fd);
	return (0);
}
