#include "caixa_pedido.h"

void *caixa_pedido (void *ptr)
{
	int id = (int) ptr;
	msg_t msg;
	unsigned int nome_cliente;
	char temp[256];
	
	while (1)
	{		
		msg.mtype = 1;
		
		conversa[id].clear();
		
		printf ("Caixa de pedido (%d), Esperando receber cliente\n\n", id+1);
		
		// Recebe um cliente
		sprintf(msg.mtext, "%d", id); //Manda id do caixa para o cliente
		sprintf (temp, "Olá. Sou %d. Bem vindo ao UFRJ Fast Food. Qual é o seu nome?\n", id+1);
		conversa[id].push_back(temp);
		if (msgsnd(fila_entrada, &msg, MAX_MSG_SIZE, 0) < 0) //Manda id do caixa para o cliente
		{
			puts ("Clientes me deixam louco! Eu me demito!\n");
			pthread_exit(NULL);
		}
			
		pthread_mutex_lock(&relatorio_dia);
			at_caixa_pedido[id]++;
		pthread_mutex_unlock(&relatorio_dia);
		
		if (msgrcv(canal_conversa[id][1], &msg, MAX_MSG_SIZE, 1, 0) < 0) //Espera receber id do cliente
		{
			puts ("Clientes me deixam louco! Eu me demito!\n");
			pthread_exit(NULL);
		}
		printf ("Caixa de pedido (%d), Recebi um cliente\n", id+1);
		sprintf (temp, "Bem vindo, %s. O que você gostaria de pedir?\n", msg.mtext);
		conversa[id].push_back(temp);
		sscanf (msg.mtext, "%u", &nome_cliente); //Copia id do cliente
		
		strcpy(msg.mtext, "ok");
		if (msgsnd(canal_conversa[id][0], &msg, MAX_MSG_SIZE, 0) < 0) //Confirma que recebeu o id do cliente
		{
			puts ("Clientes me deixam louco! Eu me demito!\n");
			pthread_exit(NULL);
		}
		
		// Recebe o pedido do cliente
        Pedido pedido_cliente; //Pedido do cliente
		while (1) //Recebe pedidos, ate que o cliente diga que eh "so"
		{
			if (msgrcv(canal_conversa[id][1], &msg, MAX_MSG_SIZE, 1, 0) < 0)
			{
				puts ("Clientes me deixam louco! Eu me demito!\n");
				pthread_exit(NULL);
			}
		
			if (!strcmp(msg.mtext, "so")) break;

		    //Cria o item do pedido
		    Item item(msg.mtext);

		    //Adiciona item ao pedido
		    pedido_cliente.adiciona_item(item);
		}

		// Verifica disponibilidade dos ingredientes e separa os ingredientes
        guardados.wait();
            bool ha_ingredientes = guardados.retira_ingredientes(pedido_cliente);
        guardados.post();

		// Reserva os ingredientes ou avisa que não há como produzir o pedido
		if (ha_ingredientes)
		{
            //Reserva os ingredientes.
            separados.wait();
                separados.adiciona_ingredientes(pedido_cliente);
            separados.post();

            //Confirma pedido
			strcpy(msg.mtext, "ok");
			sprintf (temp, "Seu pedido foi confirmado, %u. Por favor, dirija-se a um dos caixas para efetuar o pagamento.\n", nome_cliente);
			conversa[id].push_back(temp);
			if (msgsnd(canal_conversa[id][0], &msg, MAX_MSG_SIZE, 0) < 0) //Confirma pedido
			{
				puts ("Clientes me deixam louco! Eu me demito!\n");
				pthread_exit(NULL);
			}
			
			//Adiciona pedido do cliente ao map de pedidos
	        pedido[nome_cliente] = pedido_cliente;
		}
        else
        {
            //Avisa que os ingredientes estao em falta.
            strcpy(msg.mtext, "ingredientes em falta");
            sprintf(temp, "Sinto muito, %u, nao temos ingredientes.\n", nome_cliente);
            conversa[id].push_back(temp);
            if (msgsnd(canal_conversa[id][0], &msg, MAX_MSG_SIZE, 0) < 0) //Cancela pedido
            {
                puts ("Clientes me deixam louco! Eu me demito!\n");
                pthread_exit(NULL);
            }
			
            //Contabiliza no relatorio clientes nao satisfeitos
            //por falta de ingredientes
			sem_wait(&semaforo_relatorio);
				relatorio_mes.clientes_sem_pedido++;
			sem_post(&semaforo_relatorio);
        }
		
        //Imprime toda conversa do cliente com o caixa
        pthread_mutex_lock(&mutex_IO);
		for (int i = 0; i < (int) conversa[id].size(); i++)
		{
			printf ("%s", conversa[id][i].c_str());
		}
		printf ("\n");
		pthread_mutex_unlock(&mutex_IO);
	}
	
	return NULL;
}


