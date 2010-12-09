#include "cliente.h"
#include "gerador_pedido.h"

void *cliente (void *ptr)
{
	msg_t msg;
	char nome[MAX_MSG_SIZE];
	char pedidos[8][MAX_MSG_SIZE];
	int itens_pedido;
	float dinheiro;
	int cx;
	char temp[256];
	Pedido copia_pedido;
	
    //Gera ID do cliente, e gera pedido aleatoriamente
	sprintf (nome, "%u", (unsigned int)pthread_self());
	itens_pedido = rand() % 7 + 1;
	dinheiro = rand() % 51 + (rand() % 100)/100.0;
    for (int i = 0; i < itens_pedido; i++)
    {
    	sprintf(pedidos[i], "%s", gera_item().c_str());
    }

	// Entra na fila de entrada
	if (msgrcv(fila_entrada, &msg, MAX_MSG_SIZE, 1, 0) < 0) //Espera um caixa de pedido chamar
	{
		puts ("Não ouço o que este caixa fala! Vou embora deste restaurante!\n");
		pthread_exit(NULL);
	}
	sscanf (msg.mtext, "%d", &cx); //Recebe id do caixa
	
	
	// Se identifica para o caixa de pedido (para ser colocado no map o pedido) e espera a resposta
	strcpy(msg.mtext, nome); //Envia id do cliente para o caixa de pedido
	sprintf (temp, "Olá, %d, sou %u.\n", cx+1, (unsigned int)pthread_self());
	conversa[cx].push_back(temp);
	if (msgsnd(canal_conversa[cx][1], &msg, MAX_MSG_SIZE, 0) < 0) //Envia id do cliente para o caixa de pedido
	{
		puts ("Este caixa é surdo! Vou embora deste restaurante!\n");
		pthread_exit(NULL);
	}
	if (msgrcv(canal_conversa[cx][0], &msg, MAX_MSG_SIZE, 1, 0) < 0) //Espera confirmacao do caixa de pedido
	{
		puts ("Não ouço o que este caixa fala! Vou embora deste restaurante!\n");
		pthread_exit(NULL);
	}
	
	// Faz o pedido
	sprintf (temp, "Eu gostaria de:\n");
	conversa[cx].push_back(temp);
	for (int i = 0; i < itens_pedido; i++)
	{
		sprintf (temp, "\tUm(a) %s\n", pedidos[i]);
		conversa[cx].push_back(temp);
		strcpy(msg.mtext, pedidos[i]);
		if (msgsnd(canal_conversa[cx][1], &msg, MAX_MSG_SIZE, 0) < 0) //Envia todos os pedidos
		{
			puts ("Este caixa é surdo! Vou embora deste restaurante!\n");
			pthread_exit(NULL);
		}
	}
	strcpy(msg.mtext, "so");
	if (msgsnd(canal_conversa[cx][1], &msg, MAX_MSG_SIZE, 0) < 0) //Indica termino dos pedidos
	{
		puts ("Este caixa é surdo! Vou embora deste restaurante!\n");
		pthread_exit(NULL);
	}

    //Espera confirmacao do pedido
	if (msgrcv(canal_conversa[cx][0], &msg, MAX_MSG_SIZE, 1, 0) < 0)
	{
		puts ("Não ouço o que este caixa fala! Vou embora deste restaurante!\n");
		pthread_exit(NULL);
	}
	if (!strcmp(msg.mtext, "ingredientes em falta")) //Caso nao tenha ingredientes, cancela thread
	{
		sprintf (temp, "Como pode faltar um ingrediente? Vou embora deste restaurante!\n\n");
		conversa[cx].push_back(temp);
		pthread_exit(NULL);
	}
	
	// Entra na fila de um caixa de pagamento, aleatoriamente
	cx = rand() % num_caixas_pagamento;
	if (msgrcv(fila_pagamento[cx], &msg, MAX_MSG_SIZE, 1, 0) < 0) //Espera ser chamado pelo caixa de pagamento
	{
		puts ("Não ouço o que este caixa fala! Vou embora deste restaurante!\n");
		pthread_exit(NULL);
	}

    //Manda identificacao do cliente para o caixa de pagamento
    sprintf (temp, "Olá, sou %s.\n", nome);
		conversa[cx+num_caixas_pedido].push_back(temp);
	strcpy(msg.mtext, nome);
	if (msgsnd(canal_pagamento[cx][1], &msg, MAX_MSG_SIZE, 0) < 0)
	{
		puts ("Este caixa é surdo! Vou embora deste restaurante!\n");
		pthread_exit(NULL);
	}

    //Recebe o preco do pedido
	if (msgrcv(canal_pagamento[cx][0], &msg, MAX_MSG_SIZE, 1, 0) < 0)
	{
		puts ("Não ouço o que este caixa fala! Vou embora deste restaurante!\n");
		pthread_exit(NULL);
	}
	float preco;
	sscanf (msg.mtext, "%f", &preco);
	if (dinheiro < preco) //Verifica se tem dinheiro para pagar
	{
		sprintf (temp, "Este restaurante é muito caro! Vou embora daqui!\n");
		conversa[cx+num_caixas_pedido].push_back(temp);
		strcpy(msg.mtext, "nao");
		if (msgsnd(canal_pagamento[cx][1], &msg, MAX_MSG_SIZE, 0) < 0) //Envia aviso ao caixa de pagamento que
                                                        //o cliente nao pode pagar
		{
			puts ("Este caixa é surdo! Vou embora deste restaurante!\n");
		}
		pthread_exit(NULL);
		
	}
	else
	{
		sprintf (temp, "Ótimo! Aqui está o dinheiro!.\n");
		conversa[cx+num_caixas_pedido].push_back(temp);
		strcpy(msg.mtext, "sim");
		if (msgsnd(canal_pagamento[cx][1], &msg, MAX_MSG_SIZE, 0) < 0) //Envia aviso ao caixa de pagamento que
                                                           //o cliente pode pagar
		{
			puts ("Este caixa é surdo! Vou embora deste restaurante!\n");
            pthread_exit(NULL);
		}
	}
	
	// Paga
	dinheiro -= preco;
	
	// Espera seu pedido ser liberado e recebe
    pedido[(unsigned int)pthread_self()].wait();

    printf("Cliente %u recebeu pedido.\n", (unsigned int)pthread_self());

	// Vai embora	
	return NULL;
}

