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


#include "../crypto/hblk_crypto.h"
#include "../blockchain/v0.3/blockchain.h"
#include "../blockchain/v0.3/transaction/transaction.h"

/* Prototypes Functions */
int handle_exit(void);
int handle_wallet_create(EC_KEY **ec_key);
int handle_wallet_save(EC_KEY *ec_key, char *path);
int handle_wallet_load(EC_KEY **ec_key, char *path);


#endif /* _CLI_H_ */
