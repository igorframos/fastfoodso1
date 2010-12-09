#include "entregador.h"

void *entregador (void *ptr)
{
    int id = (int) ptr;
    msg_t msg;
    unsigned int nome_cliente;
    char temp[256];

	while (1)
	{
        msg.mtype = 1;
        
        conversa[id + num_caixas_pedido + num_caixas_pagamento + num_cozinheiros].clear();

        //Recebe um pedido
        sprintf(temp, "Entregador (%d) livre.\n", id + 1);
        conversa[id + num_caixas_pedido + num_caixas_pagamento + num_cozinheiros].push_back(temp);
        if (msgrcv(fila_produtos, &msg, MAX_MSG_SIZE, 1, 0) < 0) //Recebe id do cliente.
        {
            puts("Esses cozinheiros não me entregam os pedidos! Eu me demito!\n");
            pthread_exit(NULL);
        }
        sscanf(msg.mtext, "%u", &nome_cliente);
			
		pthread_mutex_lock(&relatorio_dia);
			at_entregador[id]++;
		pthread_mutex_unlock(&relatorio_dia);
        
        //Libera pedido para o cliente
        sprintf(temp, "Entregador (%d) liberou o pedido para o cliente %u.\n\n", id + 1, nome_cliente);
        conversa[id + num_caixas_pedido + num_caixas_pagamento + num_cozinheiros].push_back(temp);
        pedido[nome_cliente].post();

        //Imprime os dados sobre a ação do entregador
        pthread_mutex_lock(&mutex_IO);
		for (int i = 0; i < (int) conversa[id + num_caixas_pedido + num_caixas_pagamento + num_cozinheiros].size(); i++)
		{
			printf ("%s", conversa[id + num_caixas_pedido + num_caixas_pagamento + num_cozinheiros][i].c_str());
		}
		printf ("\n");
		pthread_mutex_unlock(&mutex_IO);
		
		pedido.erase(nome_cliente);
    }


	return NULL;
}
