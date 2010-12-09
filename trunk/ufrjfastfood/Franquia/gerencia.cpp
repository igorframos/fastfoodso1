#include "gerencia.h"

void *gerencia (void *ptr)
{
	while (1)
	{
		sleep(dia);
		
		pthread_mutex_lock(&mutex_IO);
		pthread_mutex_lock(&relatorio_dia);
		printf ("Relatório diário da franquia %s:\n", relatorio_mes.nome);
		for (int i = 0; i < num_caixas_pedido; i++)
		{
			printf ("\tCaixa de pedido %d: %d clientes atendidos.\n", i, at_caixa_pedido[i]);
			at_caixa_pedido[i] = 0;
		}
		for (int i = 0; i < num_caixas_pagamento; i++)
		{
			printf ("\tCaixa de pagamento %d: %d clientes atendidos.\n", i, at_caixa_pagamento[i]);
			at_caixa_pagamento[i] = 0;
		}
		for (int i = 0; i < num_cozinheiros; i++)
		{
			printf ("\tCozinheiro %d: %d pedidos preparados.\n", i, at_cozinheiro[i]);
			at_cozinheiro[i] = 0;
		}
		for (int i = 0; i < num_entregadores; i++)
		{
			printf ("\tEntregador %d: %d pedidos entregues.\n", i, at_entregador[i]);
			at_entregador[i] = 0;
		}
		pthread_mutex_unlock(&relatorio_dia);
		printf ("\n");
		pthread_mutex_unlock(&mutex_IO);
	}
	
	return NULL;
}

