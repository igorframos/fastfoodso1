/**
	Caixa de Pagamento
	Função thread que recebe um cliente.
**/
#include "caixa_pagamento.h"

void *caixa_pagamento (void *ptr)
{
	int id = (int) ptr;
	msg_t msg;
	unsigned int nome_cliente;
	char temp[256];
	
	while (1)
	{
		msg.mtype = 1;
		
		conversa[id + num_caixas_pedido].clear();
		
		// Recebe um cliente
		sprintf (temp, "Caixa de pagamento (%d) livre para operação.\n", id + 1);
		conversa[id + num_caixas_pedido].push_back(temp);
		if (msgsnd(fila_pagamento[id], &msg, MAX_MSG_SIZE, 0) < 0) //Chama um novo cliente
		{
			puts ("Esses clientes me deixam louco! Eu me demito!\n");
			pthread_exit(NULL);
		}
		
        	//Recebe identificacao do cliente
		if (msgrcv(canal_pagamento[id][1], &msg, MAX_MSG_SIZE, 1, 0) < 0)
		{
			puts ("Esses clientes me deixam louco! Eu me demito!\n");
			pthread_exit(NULL);
		}
		sscanf (msg.mtext, "%u", &nome_cliente);
	        sprintf(temp, "Olá, %s. ", msg.mtext);
	        conversa[id + num_caixas_pedido].push_back(temp);
		
		// Verifica o valor do pedido dele
		sprintf (msg.mtext, "%.2f", pedido[nome_cliente].getPreco());
		sprintf (temp, "O seu pedido irá custar %.2f. \n", pedido[nome_cliente].getPreco());
		conversa[id + num_caixas_pedido].push_back(temp);
		if (msgsnd(canal_pagamento[id][0], &msg, MAX_MSG_SIZE, 0) < 0) //Manda o preco do pedido
		{
			puts ("Esses clientes me deixam louco! Eu me demito!\n");
			pthread_exit(NULL);
		}
		
		// Recebe e registra recebimento do dinheiro ou aviso do cliente de que ele não pode pagar
		if (msgrcv(canal_pagamento[id][1], &msg, MAX_MSG_SIZE, 1, 0) < 0)
		{
			puts ("Esses clientes me deixam louco! Eu me demito!\n");
			pthread_exit(NULL);
		}
		if (!strcmp(msg.mtext, "nao")) //Caso o cliente nao possa pagar
		{ // Devolver ingredientes separados para guardados
		    separados.wait();
		        separados.retira_ingredientes(pedido[nome_cliente]);
		    separados.post();
		    guardados.wait();
		        guardados.adiciona_ingredientes(pedido[nome_cliente]);
		    guardados.post();
		    pedido.erase(pedido.find(nome_cliente));
		}

		/*sprintf(temp, "O cliente %s pode pagar.\n", msg.mtext);
		conversa[id + num_caixas_pedido].push_back(temp);*/
		sprintf(temp, "Obrigado. Volte sempre!\n");
		conversa[id + num_caixas_pedido].push_back(temp);

		//Imprime toda conversa do cliente com o caixa
		pthread_mutex_lock(&mutex_IO);
		for (int i = 0; i < (int) conversa[id + num_caixas_pedido].size(); i++)
		{
			printf ("%s", conversa[id + num_caixas_pedido][i].c_str());
		}
		printf ("\n");
		pthread_mutex_unlock(&mutex_IO);

		// Passa o pedido do cliente para a cozinha
		if (!strcmp(msg.mtext, "sim"))
		{
		    sprintf(msg.mtext, "%u", nome_cliente); //Passa a id do cliente para o cozinheiro. 
		        //Assim ele pode ir no map pedido e verificar o pedido
		    if (msgsnd(fila_pedidos, &msg, MAX_MSG_SIZE, 0) < 0)
		    {
		        puts("Esses cozinheiros não recebem o pedido! Eu me demito!\n");
		        pthread_exit(NULL);
		    }

		    //Contabiliza no relatorio
		    sem_wait(&semaforo_relatorio);
		    	relatorio_mes.clientes_atendidos++;
		        relatorio_mes.lucro += pedido[nome_cliente].getPreco();
		    sem_post(&semaforo_relatorio);
			
			pthread_mutex_lock(&relatorio_dia);
				at_caixa_pagamento[id]++;
			pthread_mutex_unlock(&relatorio_dia);

		    //Contabiliza no almoxarifado
		    almoxarifado.wait();
		    	almoxarifado.adiciona_ingredientes(pedido[nome_cliente]);
		    almoxarifado.post();
		}
		else
		{
			sem_wait(&semaforo_relatorio);
				relatorio_mes.clientes_sem_dinheiro++;
			sem_post(&semaforo_relatorio);
			
			pthread_mutex_lock(&relatorio_dia);
				at_caixa_pagamento[id]++;
			pthread_mutex_unlock(&relatorio_dia);
		}
	}
	
	return NULL;
}
