/*
	O que pretendo fazer?
	
	Um cliente é uma thread. Uma fila de clientes é implementada com semáforos.
	Um pedido é uma instância de uma struct/classe.
	Caixas, cozinheiros e entregadores são threads.
	
	Alocação dinâmica dos vetores de threads. Cliente feito com vector e com modificação da área de pilha.
	Cozinheiros, caixas e entregadores não precisam de pilha em local diferente.

	Número de caixas e outros tipos de funcionários pode ser um parâmetro de entrada.

	Comunicação entre cliente e caixa se dá por mensagem ou, caso necessário, por uma variável controlada por um semáforo.
	Essa variável funciona assim:
		Um caixa tem uma posição de memória onde vai procurar um pedido
		Fica aguardando o semáforo com o pedido
		Quando libera o semáforo ele vê e processa o pedido

	Controle de estoque: se não estiver disponível o cliente deve decidir entre fazer outro pedido ou ir embora
	(Ir embora = finalizar thread deste cliente)

	Caixa processa pedido e envia consumidor para fila de pagamento

	Cliente escolhe uma das filas segundo algum critério a ser definido

	Caixa verifica se o pagamento irá ocorrer e devolve os ingredientes reservados caso o cliente não pague
	Caixa recebe pagamento e coloca o pedido do cliente na fila

	Map associando cliente (thread ID) a um semáforo

	Entregadores liberam o semáforo do cliente cujo pedido ficou pronto
*/

#include <stdio.h>
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

struct pedido_t
{
	float preco;
};

void *cliente (void*);
void *cozinheiro (void*);
void *entregador (void*);
void *caixa_pedido (void*);
void *caixa_pagamento (void*);

map<pthread_t,pthread_mutex_t> espera_pedidos;

/*
	Função main terá os papeis de:
		- Inicializar tudo que precisa ser inicializado globalmente
		- Lançar threads
	
	Acho que não há mais nenhum papel da main. O resto pode ser sempre uma thread a executar.
	Só é preciso escolher bem como a main vai esperar alguma thread ou algo assim.
	Na prática, a main é o simulador.
*/
int main (void)
{
	srand(time(NULL));
	
	// Receber lista de ingredientes e sanduíches e refrigerantes
	// Receber número de caixas, cozinheiros e entregadores
	// Iniciar threads de caixas, cozinheiros e entregadores
	// Ficar produzindo clientes para a loja
	// Criar thread de envio de relatórios periodicamente
	
	return 0;
}

void *cliente (void *ptr)
{
	printf("The ID of this thread is: %u\n", (unsigned int)pthread_self());
	// Pega dados do cliente de algum lugar ou gera aleatoriamente
	// Entra na fila
	// Faz o pedido
	// Entra na fila de um caixa de pagamento
	// Dirige-se ao caixa de pagamento
	// Paga
	// Espera seu pedido ser liberado
	// Recebe seu pedido
	// Vai embora
	
	// Pega dados do cliente de algum lugar ou gera aleatoriamente
	// Entra na fila
	// Faz o pedido
	// Recebe informação de que o pedido não pode ser feito
	// Decide alterar o pedido
	// Retorna à fase de fazer pedido
	
	// Pega dados do cliente de algum lugar ou gera aleatoriamente
	// Entra na fila
	// Faz o pedido
	// Recebe informação de que o pedido não pode ser feito
	// Decide cancelar o pedido
	// Vai embora
	
	return NULL;
}

void *cozinheiro (void *ptr)
{
	// Recebe pedido dos caixas
	// Produz comida
	// Passa para o entregador a comida produzida

	return NULL;
}

void *entregador (void *ptr)
{
	// Recebe comida produzida pelos cozinheiros
	// Chama o cliente correspondente
	// Entrega para o cliente
	
	return NULL;
}

void *caixa_pedido (void *ptr)
{
	/*
		Pode imprimir uma "conversa" com o cliente. Acho que ia ficar muito maneiro se cada caixa tivesse um arquivo de log
		com momento de começo do atendimento e com o cliente atendido, além de uma conversinha simulada do tipo:
		Caixa: "Bem vindo ao UFRJ Fast Food. Qual o seu nome?"
		Cliente: Responde com uma dentre algumas frases possíveis informando o seu TID
		Caixa: "Qual o seu pedido, $TID?"
		Cliente: Responde com o pedido. Enumera uma lista de itens e o caixa vai registrando. Fica combinada uma forma padrão de encerrar a lista.
		
		Se o pedido puder ser atendido:
			Caixa: "Seu pedido foi registrado com sucesso. Dirija-se a um dos caixas para efetuar o pagamento. A UFRJ Fast Food agradece a preferência."
		
		Se o pedido não puder ser atendido:
			Caixa: "Infelizmente o ingrediente X está em falta e não poderemos produzir o seu pedido. Gostaria de alterar algum dos itens de seu pedido?"
			Se cliente responde sim voltamos para o "Qual o seu pedido, $TID?"
			Se cliente responde não Caixa diz: "Pedimos desculpas pelo inconveniente. Esperamos que você retorne em uma outra ocasião."
	*/
	
	// Recebe um cliente
	// Recebe o pedido do cliente
	// Verifica disponibilidade dos ingredientes
	// Reserva os ingredientes
	// Encaminha cliente para caixa de pagamento
	// Aguarda próximo cliente
	
	// Recebe um cliente
	// Recebe o pedido do cliente
	// Verifica disponibilidade dos ingredientes
	// Comunica ao cliente que ingredientes não estão disponíveis
	// Recebe do cliente indicação de que o pedido será alterado
	// Retorna à fase de receber pedido
	
	// Recebe um cliente
	// Recebe o pedido do cliente
	// Verifica disponibilidade dos ingredientes
	// Comunica ao cliente que ingredientes não estão disponíveis
	// Recebe do cliente indicação de que o pedido será cancelado
	// Aguarda próximo cliente
	
	return NULL;
}

void *caixa_pagamento (void *ptr)
{
	// Recebe um cliente
	// Verifica o valor do pedido dele
	// Recebe e registra recebimento do dinheiro
	// Passa o pedido do cliente para a cozinha
	// Aguarda novo cliente
	
	// Recebe um cliente
	// Verifica o valor do pedido dele
	// Cliente não tem dinheiro e vai embora
	// Devolve ingredientes reservados para o estoque
	// Aguarda novo cliente
	
	return NULL;
}

