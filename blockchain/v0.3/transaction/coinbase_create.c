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
	tx_out_t *output;
	tx_in_t *input;
	uint8_t pub[EC_PUB_LEN];

	/* Verificar si el receptor y la conversión de clave pública son exitosos */
	if (!receiver || !ec_to_pub(receiver, pub))
		return (NULL);

	/* Asignar memoria para la transacción */
	transaction = calloc(1, sizeof(*transaction));
	if (!transaction)
		return (NULL);

	/* Asignar memoria para el output de la transacción */
	output = tx_out_create(COINBASE_AMOUNT, pub);
	if (!output)
	{
		free(transaction);
		return (NULL);
	}

	/* Asignar memoria para el input de la transacción */
	input = calloc(1, sizeof(*input));
	if (!input)
	{
		free(transaction);
		free(output);
		return (NULL);
	}

	/* Copiar los primeros 4 bytes del índice de bloque en el hash del output del input */
	memcpy(input->tx_out_hash, &block_index, 4);

	/* Inicializar las listas de inputs y outputs de la transacción */
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	if (!transaction->inputs || !transaction->outputs)
	{
		free(transaction);
		free(output);
		free(input);
		return (NULL);
	}

	/* Agregar el input y el output a las listas correspondientes de la transacción */
	if (llist_add_node(transaction->inputs, input, ADD_NODE_REAR) ||
		llist_add_node(transaction->outputs, output, ADD_NODE_REAR))
	{
		free(transaction);
		free(output);
		free(input);
		llist_destroy(transaction->inputs, 0, NULL);
		llist_destroy(transaction->outputs, 0, NULL);
		return (NULL);
	}

	/* Calcular el hash de la transacción */
	if (!transaction_hash(transaction, transaction->id))
	{
		free(transaction);
		free(output);
		free(input);
		llist_destroy(transaction->inputs, 0, NULL);
		llist_destroy(transaction->outputs, 0, NULL);
		return (NULL);
	}

	/* Devolver la nueva transacción coinbase */
	return (transaction);
}
