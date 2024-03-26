#include "transaction.h"

/**
* coinbase_create - create coinbase transaction
* @receiver: coin receiver
* @block_index: block index to which tx belongs
* Return: pointer to tx struct
*/
transaction_t *coinbase_create(EC_KEY const *receiver, uint32_t block_index)
{
	transaction_t *transaction;
	uint8_t pub[EC_PUB_LEN];

	/* Verificar si el receptor y la conversión de clave pública son exitosos */
	if (!receiver || !ec_to_pub(receiver, pub))
		return (NULL);

	/* Asignar memoria para la transacción */
	transaction = calloc(1, sizeof(*transaction));
	if (!transaction)
		return (NULL);

	/* Inicializar las listas de inputs y outputs de la transacción */
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->inputs || !transaction->outputs)
	{
		free(transaction);
		return (NULL);
	}

	/* Crear la transacción coinbase */
	if (!coinbase_create_continue(transaction, pub, block_index))
	{
		free(transaction);
		return (NULL);
	}

	/* Devolver la nueva transacción coinbase */
	return (transaction);
}

/**
* coinbase_create_continue - continues coinbase transaction creation
* @transaction: pointer to transaction struct
* @pub: public key buffer
* @block_index: block index to which tx belongs
* Return: 1 on success, 0 on failure
*/
int coinbase_create_continue(transaction_t *transaction, uint8_t *pub,
							uint32_t block_index)
{
	tx_out_t *output;
	tx_in_t *input;

	/* Asignar memoria para el output de la transacción */
	output = tx_out_create(COINBASE_AMOUNT, pub);
	if (!output)
		return (0);

	/* Asignar memoria para el input de la transacción */
	input = calloc(1, sizeof(*input));
	if (!input)
	{
		free(output);
		return (0);
	}

	memcpy(input->tx_out_hash, &block_index, 4);

	if (llist_add_node(transaction->inputs, input, ADD_NODE_REAR) ||
		llist_add_node(transaction->outputs, output, ADD_NODE_REAR))
	{
		free(output);
		free(input);
		llist_destroy(transaction->inputs, 0, NULL);
		llist_destroy(transaction->outputs, 0, NULL);
		return (0);
	}

	/* Calcular el hash de la transacción */
	if (!transaction_hash(transaction, transaction->id))
	{
		free(output);
		free(input);
		llist_destroy(transaction->inputs, 0, NULL);
		llist_destroy(transaction->outputs, 0, NULL);
		return (0);
	}

	/* Éxito en la creación de la transacción coinbase */
	return (1);
}
