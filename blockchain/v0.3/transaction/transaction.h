#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include "../../../crypto/hblk_crypto.h"
#include <llist.h>
#define EC_PUB_LEN 65


/**
* struct transaction_s - Transaction structure
*
* @id:      Transaction ID. A hash of all the inputs and outputs.
*           Prevents further alteration of the transaction.
* @inputs:  List of `tx_in_t *`. Transaction inputs
* @outputs: List of `tx_out_t *`. Transaction outputs
*/
typedef struct transaction_s
{
	uint8_t     id[SHA256_DIGEST_LENGTH];
	llist_t     *inputs;
	llist_t     *outputs;
} transaction_t;

/**
* struct tx_out_s - Transaction output
*
* @amount: Amount received
* @pub:    Receiver's public address
* @hash:   Hash of @amount and @pub. Serves as output ID
*/
typedef struct tx_out_s
{
	uint32_t    amount;
	uint8_t     pub[EC_PUB_LEN];
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} tx_out_t;

/**
* struct tx_in_s - Transaction input
*
* Description: A transaction input always refers to a previous
* transaction output. The only exception is for a Coinbase transaction, that
* adds new coins to ciruclation.
*
* @block_hash:  Hash of the Block containing the transaction @tx_id
* @tx_id:       ID of the transaction containing @tx_out_hash
* @tx_out_hash: Hash of the referenced transaction output
* @sig:         Signature. Prevents anyone from altering the content of the
*               transaction. The transaction input is signed by the receiver
*               of the referenced transaction output, using their private key
*/
typedef struct tx_in_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	uint8_t     tx_out_hash[SHA256_DIGEST_LENGTH];
	sig_t       sig;
} tx_in_t;

/**
* struct unspent_tx_out_s - Unspent transaction output
*
* Description: This structure helps identify transaction outputs that were not
* used in any transaction input yet, making them "available".
*
* @block_hash: Hash of the Block containing the transaction @tx_id
* @tx_id:      ID of the transaction containing @out
* @out:        Copy of the referenced transaction output
*/
typedef struct unspent_tx_out_s
{
	uint8_t     block_hash[SHA256_DIGEST_LENGTH];
	uint8_t     tx_id[SHA256_DIGEST_LENGTH];
	tx_out_t    out;
} unspent_tx_out_t;

/* Protypes Functions Porject Blockchain - Transactions */

/* Functions Task 0*/
tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN]);

#endif /* _TRANSACTION_H_ */
