#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <bits/posix_opt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "relatorio.h"
#include "clienteApp.h"

using namespace std;

bool encerra;
pthread_mutex_t mutex;
void *cria_comunicacao (void*);
void *recebe_dados (void*);

int main (void)
{
	pthread_t thread;
	
	encerra = false;
	
	pthread_mutex_init(&mutex, NULL);
	
	if (pthread_create (&thread, NULL, cria_comunicacao, NULL))
	{
		puts ("Impossível criar thread de comunicação. Encerrando a aplicação.\n");
		exit(1);
	}
	
	pthread_mutex_lock(&mutex);
	while (!encerra)
	{
		pthread_mutex_unlock(&mutex);
		
		if (getchar())
		{
			char c;
			printf ("Deseja sair? (s/n) ");
			scanf ("%c%*c", &c);
			if (c == 's' || c == 'S')
			{
				pthread_mutex_lock(&mutex);
				encerra = true;
				pthread_mutex_unlock(&mutex);
			}
		}
		
		pthread_mutex_lock(&mutex);
	}
}

void *cria_comunicacao (void *ptr)
{
	int socketID;
	sockaddr_in serv_addr;
	
	printf ("Aplicação Servidor da Matriz UFRJ Fast Food.\n\n");
	printf ("Para encerrar a aplicação, aperte enter.\n\n");
	
	socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0)
	{
		puts ("Impossível abrir canal de comunicação externa. Encerrando a aplicação.\n");
		exit(1);
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(2010);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(socketID, (const sockaddr*) &serv_addr, sizeof(serv_addr)))
	{
		puts ("Impossível abrir canal de comunicação externa. Encerrando a aplicação.\n");
		exit(1);
	}
	
	if (listen(socketID, (1<<16)) < 0)
	{
		puts ("Impossível receber conexões externas (listen). Encerrando a aplicação.\n");
		exit(1);
	}
	
	pthread_mutex_lock(&mutex);
	while (!encerra)
	{
		pthread_mutex_unlock(&mutex);
		Cliente *cliente = new Cliente();
		cliente->socketID = accept(socketID, (sockaddr*) &(cliente->addr), &(cliente->size));
		if (cliente->socketID < 0)
		{
			puts ("Impossível receber conexões externas (accept). Encerrando a aplicação.\n");
			exit(1);
		}
		if (pthread_create(&(cliente->thread), NULL, recebe_dados, cliente))
		{
			puts ("Impossível criar thread para tratar conexões externas. Ignorando conexão.\n");
		}
		
		pthread_mutex_lock(&mutex);
	}
	
	close(socketID);
	
	return NULL;
}

void *recebe_dados (void *ptr)
{
	Relatorio relatorio;
	Cliente cliente = *((Cliente*)ptr);
	
	puts ("Uma conexão foi estabelecida.\n");
	
	pthread_mutex_lock(&mutex);
	while (!encerra)
	{
		pthread_mutex_unlock(&mutex);
		read(cliente.socketID, &relatorio, sizeof(relatorio));
		if (relatorio.nome[0] == 0 && !relatorio.clientes_recebidos) break;
		
		char aux[128];
		
		string texto;
		texto += "Relatório mensal da franquia " + string(relatorio.nome) + ":\n";
		sprintf (aux, "%d", relatorio.clientes_recebidos);
		texto += "\tClientes recebidos: " + string(aux) + "\n";
		sprintf (aux, "%d", relatorio.clientes_sem_pedido);
		texto += "\tClientes não atendidos por falta de estoque: " + string(aux) + "\n";
		sprintf (aux, "%d", relatorio.clientes_sem_dinheiro);
		texto += "\tClientes não atendidos por falta de dinheiro: " + string(aux) + "\n";
		sprintf (aux, "%d", relatorio.clientes_atendidos);
		texto += "\tClientes efetivamente atendidos: " + string(aux) + "\n";
		sprintf (aux, "%.2f", relatorio.lucro);
		texto += "\tTotal de dinheiro recebido: " + string(aux) + "\n\n";
	
		printf ("%s", texto.c_str());
		
		pthread_mutex_lock(&mutex);
	}
	
	puts ("Uma conexão foi encerrada.\n");
	
	return NULL;
}

