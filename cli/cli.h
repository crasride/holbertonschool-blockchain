#ifndef _CLI_H_
#define _CLI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <curses.h>

#include "../crypto/hblk_crypto.h"
#include "../blockchain/v0.3/blockchain.h"
#include "../blockchain/v0.3/transaction/transaction.h"

#include <readline/readline.h>
#include <readline/history.h>

#define ADDRESS_MAX_LEN 42


/**
* struct state_s - client state structure
*
* @status: most recent executed command status
* @wallet: ec_keys
* @blockchain: blockchain
* @tx_pool: transaction pool
*/
typedef struct state_s
{
	int status;
	EC_KEY *wallet;
	blockchain_t *blockchain;
	llist_t *tx_pool;
} state_t;

/**
* struct user_s - user structure
* @username: username
* @wallet: wallet
*/
typedef struct user_s
{
	char *username;
	EC_KEY *wallet;
} user_t;

/**
* struct command_info_s - command info structure
* @command: command
* @description: description
*/
typedef struct command_info_s
{
	const char *command;
	const char *description;
} CommandInfo;


/* Prototypes Functions */
/* cli.c */
int find_command(char *cmd, char *arg1, char *arg2, state_t *state,
					char *receiver_address);
void print_logo(void);

/* handle_exit.c */
int handle_exit(void);

/* cleanup.c */
void cleanup(state_t *state);

/* mine.c */
int mine(state_t *state, block_t *block, block_t *prev_block,
		transaction_t *coinbase_tx);
int invalid_tx(llist_node_t node, void *arg);
int add_transaction(llist_node_t node, unsigned int idx, void *arg);

/* handle_mine.c */
int handle_mine(state_t *state);

/* handle_wallet_create */
int handle_wallet_create(state_t *state);

/* handle_wallet_save */
int handle_wallet_save(state_t *state, char *path);

/* handle_wallet_load */

int handle_wallet_load(state_t *state, char *path);

/* handle_load */
int handle_load(state_t *state, char *path);

/* handle_save */
int handle_save(state_t *state, char *path);

/* handle_info.c */
int handle_info(state_t *state);
char *bytes_to_hex(const uint8_t *bytes, size_t len);

/* handle_send.c */
int handle_send(int amount, char *receiver_address, state_t *state);
int str_to_key(char *str, EC_KEY **key);

/* handle_help.c */
void print_help(void);

/* handle_info_block.c */
int handle_info_block(state_t *state);
int print_transaction_info(llist_node_t node, unsigned int idx, void *arg);
int print_input_info(llist_node_t node, unsigned int idx, void *arg);
int print_output_info(llist_node_t node, unsigned int idx, void *arg);

/* unspent_list.c */
int print_unspent_tx_out_info(llist_node_t node, unsigned int idx, void *arg);
int generate_sorted_unspent_list(blockchain_t *blockchain);
int compares_unspent_tx_out(unspent_tx_out_t *first,
										unspent_tx_out_t *second, void *arg);
/* tools.c*/
void *llist_get_node_data(llist_node_t node);
uint8_t _get_endianness(void);
int sum_unspent(void *node, unsigned int idx, void *arg);
int check_string(char *address, uint8_t *pub, size_t len);
uint8_t *string_to_pub(char *address);




# endif /* _CLI_H_ */
