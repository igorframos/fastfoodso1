#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>
#include <bits/posix_opt.h>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <queue>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include "pedido.h"
#include "cliente.h"
#include "cozinheiro.h"
#include "entregador.h"
#include "caixa_pedido.h"
#include "caixa_pagamento.h"
#include "gerador_pedido.h"
#include "relata.h"
#include "gerencia.h"
#include "item.h"
#include "estoque.h"
#include "global.h"

#define MAX_CAIXAS_PEDIDO 5
#define MAX_CAIXAS_PAGAMENTO 3 
#define MAX_COZINHEIROS 5 
#define MAX_ENTREGADORES 3 

using namespace std;

string hostname;
char identificador[128];

int num_caixas_pedido;
int num_caixas_pagamento;
int num_cozinheiros;
int num_entregadores;
int at_caixa_pedido[5];
int at_caixa_pagamento[5];
int at_cozinheiro[5];
int at_entregador[5];

pthread_mutex_t relatorio_dia;

int fila_entrada;
int fila_pagamento[5];
int canal_conversa[5][2];
int canal_pagamento[5][2];
int fila_pedidos;
int fila_produtos;

Estoque guardados("estoque.txt");
Estoque separados;
Estoque almoxarifado;
sem_t semaforo_estoque;

Relatorio relatorio_mes;
sem_t semaforo_relatorio;

map<unsigned int,Pedido> pedido;

vector<string> conversa[30];

bool fim;

int dia;
int mes;

pthread_mutex_t mutex_IO;

int main (int argc, char** argv)
{
	srand(time(NULL));
	
	fim = false;
	
	pthread_mutex_init(&mutex_IO, NULL);
	pthread_mutex_init(&relatorio_dia, NULL);

	// Receber lista de ingredientes e sanduíches e refrigerantes
    // Essa parte está na criacao dos estoques. Que é recebido atraves
    // Do arquivo "estoque.txt"
    sem_init(&semaforo_estoque, 0, 1);
    sem_init(&semaforo_relatorio, 0, 1);
    
    relatorio_mes.clientes_recebidos = 0;
	relatorio_mes.clientes_sem_pedido = 0;
	relatorio_mes.clientes_sem_dinheiro = 0;
	relatorio_mes.clientes_atendidos = 0;
	relatorio_mes.lucro = 0;
	memset (at_caixa_pedido, 0, sizeof(at_caixa_pedido));
	memset (at_caixa_pagamento, 0, sizeof(at_caixa_pagamento));
	memset (at_cozinheiro, 0, sizeof(at_cozinheiro));
	memset (at_entregador, 0, sizeof(at_entregador));
    
    // Receber identificador da franquia
    do
    {
		printf ("Determine o identificador da franquia: (Não é permitido identificador vazio)\n");
		fgets(relatorio_mes.nome, 128, stdin);
		relatorio_mes.nome[strlen(relatorio_mes.nome) - 1] = 0;
	} while (!relatorio_mes.nome[0]);

	// Receber número de caixas, cozinheiros e entregadores
    do
    {
        printf("Quantos serao os caixas para pedidos? (no minimo 3, no maximo 5)\n");
        scanf("%d", &num_caixas_pedido);
    }while (num_caixas_pedido < 3 || num_caixas_pedido > 5);
    
    do
    {
        printf("Quantos serao os caixas para pagamento? (no minimo 1, no maximo 3)\n");
        scanf("%d", &num_caixas_pagamento);
    }while (num_caixas_pagamento < 1 || num_caixas_pagamento > 3);
    
    do
    {
        printf("Quantos serao os cozinheiros? (no minimo 3, no maximo 5)\n");
        scanf("%d", &num_cozinheiros);
    }while (num_cozinheiros < 3 || num_cozinheiros > 5);
    
    do
    {
        printf("Quantos serao os entregadores? (no minimo 2, no maximo 3)\n");
        scanf("%d", &num_entregadores);
    }while (num_entregadores < 2 || num_entregadores > 3);
    
    // Requisita endereço do servidor
    printf ("Digite o endereço do servidor:\n");
    char tmp[128];
    scanf ("%s", tmp);
    hostname = string(tmp);
    cout << "O nome do host é: " << hostname << "\n\n";
    
    printf ("Quantos segundos deve durar um dia? ");
    scanf ("%d", &dia);
    printf ("Quantos segundos deve durar um mês? ");
    scanf ("%d%*c", &mes);
    printf ("\n");
    
    printf ("Para enviar um novo cliente pressione <enter>.\n");
    printf ("Para encerrar a aplicação, digite qualquer caractere válido e pressione <enter>.\n\n");
    

    //--------Iniciar canais de troca de mensagem----------
    key_t key = IPC_PRIVATE;
	int msgflg = IPC_CREAT | IPC_EXCL | 0666;
	
    //Inicia fila de entrada
	if((fila_entrada = msgget(key, msgflg)) < 0)
	{
		puts("Não foi possível abrir uma message queue. Abortando.\n");
		exit(1);
	}
	
    //Inicia canais dos caixas de pedido
	for (int i = 0; i < num_caixas_pedido; i++)
	{
		if((canal_conversa[i][0] = msgget(key, msgflg)) < 0)
		{
			puts("Não foi possível abrir uma message queue. Abortando.\n");
			exit(1);
		}
		if((canal_conversa[i][1] = msgget(key, msgflg)) < 0)
		{
			puts("Não foi possível abrir uma message queue. Abortando.\n");
			exit(1);
		}
	}
	
    //Inicia canais dos caixas de pagamento
	for (int i = 0; i < num_caixas_pagamento; i++)
	{
		if((fila_pagamento[i] = msgget(key, msgflg)) < 0) // ???
		{
			puts("Não foi possível abrir uma message queue. Abortando.\n");
			exit(1);
		}
		if((canal_pagamento[i][0] = msgget(key, msgflg)) < 0)
		{
			puts("Não foi possível abrir uma message queue. Abortando.\n");
			exit(1);
		}
		if((canal_pagamento[i][1] = msgget(key, msgflg)) < 0)
		{
			puts("Não foi possível abrir uma message queue. Abortando.\n");
			exit(1);
		}
	}

    //Inicia canais dos cozinheiros
	if((fila_pedidos = msgget(key, msgflg)) < 0)
	{
		puts("Não foi possível abrir uma message queue. Abortando.\n");
		exit(1);
	}

    //Inicia fila de produtos
    if((fila_produtos = msgget(key, msgflg)) < 0)
    {
        puts("Não foi possível abrir uma message queue. Abortando.\n");
        exit(1);
    }
    
	// Iniciar threads de caixas, cozinheiros e entregadores
    pthread_t   threadid_caixas_pedido[MAX_CAIXAS_PEDIDO],
                threadid_caixas_pagamento[MAX_CAIXAS_PAGAMENTO],
                threadid_cozinheiros[MAX_COZINHEIROS],
                threadid_entregadores[MAX_ENTREGADORES],
                threadid_relatorio_mes,
                threadid_relatorio_dia;

    for (int i = 0; i < num_caixas_pedido; i++)
        if (pthread_create( &threadid_caixas_pedido[i], NULL, caixa_pedido, (void*) i ) != 0) //Sou mto tr00. XD
            exit(1);

    for (int i = 0; i < num_caixas_pagamento; i++)
        if (pthread_create( &threadid_caixas_pagamento[i], NULL, caixa_pagamento, (void*) i ) != 0)
            exit(1);

    for (int i = 0; i < num_cozinheiros; i++)
        if (pthread_create( &threadid_cozinheiros[i], NULL, cozinheiro, (void*) i ) != 0)
            exit(1);

    for (int i = 0; i < num_entregadores; i++)
        if (pthread_create( &threadid_entregadores[i], NULL, entregador, (void*) i) != 0)
            exit(1);
    
	// Criar thread de envio de relatórios periodicamente
    if (pthread_create(&threadid_relatorio_mes, NULL, relata, NULL))
    {
    	puts ("Não será possível enviar relatórios, pois não foi possível criar uma thread.\n");
    }
    
    // Criar thread de relatórios diários
    if (pthread_create(&threadid_relatorio_dia, NULL, gerencia, NULL))
    {
    	puts ("Não será possível exibir relatórios diários, pois não foi possível criar uma thread.\n");
    }

	// Ficar produzindo clientes para a loja
	pthread_t threadid_cliente;
	
	while (getchar() == '\n')
	{
		sem_wait(&semaforo_relatorio);
		relatorio_mes.clientes_recebidos++;
		sem_post(&semaforo_relatorio);
		
		pthread_create(&threadid_cliente, NULL, cliente, NULL);
	}
	
	fim = true;
	
	puts ("Encerrando aplicação. Por favor, aguarde.\n");
	pthread_join(threadid_relatorio_mes, NULL);
	
	return 0;
}


