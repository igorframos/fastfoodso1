#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>
#include <bits/posix_opt.h>

#define SEM 4					// Número de semáforos.
#define VAR 10					// Tamanho do vetor resposta[].
#define STACK_SIZE (1<<14)		// Tamanho da pilha de cada thread.

int m, n;						// Tamanho da matriz. (n = linhas, m = colunas)
int **mat;						// Ponteiro para que a matriz seja armazenada.
pthread_mutex_t semaforo[SEM];	// Vetor com semáforos de exclusão mútua.
long long resposta[VAR];		// Vetor que armazenará a resposta final.
/*
	Vetor resposta guarda:
		resposta[0]: maior valor					semaforo[0]
		resposta[1]: índice i do maior valor		semaforo[0]
		resposta[2]: índice j do maior valor		semaforo[0]
		resposta[3]: menor valor					semaforo[1]
		resposta[4]: índice i do menor valor		semaforo[1]
		resposta[5]: índice j do menor valor		semaforo[1]
		resposta[6]: maior produto					semaforo[2]
		resposta[7]: linha com o maior produto		semaforo[2]
		resposta[8]: menor produto					semaforo[3]
		resposta[9]: linha com o menor produto		semaforo[4]
*/

// Cabeçalho da função que calcula o produto interno de uma linha.
void *prod_interno (void*);

int main (void)
{
	void **ptr;				// Ponteiro para guardar as pilhas das threads e permitir a posterior liberação da memória.
	pthread_t *thread;		// Ponteiro para guardar um vetor de threads.
	pthread_attr_t attr;	// Atributos de uma thread qualquer. Serve para modificar local e tamanho da pilha.

	srand(time(NULL));
	
	// Inicializa os atributos da thread com o padrão.
	if (pthread_attr_init(&attr))
	{
		puts("Falha na definição dos atributos da thread.\n");
		exit(1);
	}
	
	while (1)
	{
		scanf ("%d %d", &n, &m);
		if (!n && !m) break;
		
		// Inicializa o vetor de respostas com valores altos para o mínimo e baixos para o máximo.
		resposta[0] = -1;
		resposta[3] = 0x3f3f3f3f;
		resposta[6] = -1;
		resposta[8] = 0x3f3f3f3f;
		
		// Alocação de memória.
		puts ("Alocando memória.");
		bool erro = false;
		
		// Alocação da matriz.
		mat = (int**) malloc (n * sizeof(int*));
		if (!mat)
		{
			puts ("Falha na alocação de memória.\n");
			continue;
		}
		for (int i = 0; i < n; i++)
		{
			mat[i] = (int*) malloc (m * sizeof(int));
			if (!mat[i])
			{
				erro = true;
				puts("Falha na alocação de memória.\n");
				break;
			}
		}
		if (erro) continue;
		
		// Alocação do vetor de threads.
		thread = (pthread_t*) malloc (n * sizeof(thread));
		if (!thread)
		{
			puts ("Falha na alocação de memória.\n");
			continue;
		}
		
		// Alocação das pilhas que serão dadas a cada thread.
		ptr = (void**) malloc (n * sizeof(void*));
		if (!ptr)
		{
			puts ("Falha na alocação de memória.\n");
			continue;
		}
		for (int i = 0; i < n; i++)
		{
			// Aloca uma área de pilha para a thread i.
			ptr[i] = malloc (STACK_SIZE);
			if (!ptr[i])
			{
				puts ("Falha na alocação da pilha da thread.");
				erro = true;
				break;
			}
		}
		if (erro) break;
		
		// Preparação dos semáforos. Utilizando os semáforos da própria pthread.h.
		puts ("Preparando semáforos.");
		for (int i = 0; i < SEM; i++)
		{
			semaforo[i] = (pthread_mutex_t) PTHREAD_MUTEX_INITIALIZER;
		}
		if (erro) continue;
		
		// Gera a matriz aleatória em um único fluxo.
		puts ("Gerando matriz aleatória.\n");
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				mat[i][j] = rand() % 10;
				printf ("%d ", mat[i][j]);
			}
			printf ("\n");
		}
		printf ("\n");
		
		// Fase de cálculo. Gera uma thread por linha e executa o cálculo.
		puts ("Computando.\n");
		time_t start = time(NULL);
		
		for (int i = 0; i < n; i++)
		{
			int ret;
			
			// Determina os atributos para que a área de pilha alocada seja utilizada.
			if (pthread_attr_setstack(&attr, ptr[i], STACK_SIZE))
			{
				puts ("Falha ao modificar os atributos da thread.");
				puts ("Tentando com a próxima linha.\n");
				continue;
			}
			
			// Cria a thread com a pilha recém-alocada. A nova thread executará a função prod_interno.
			ret = pthread_create(&thread[i], &attr, prod_interno,  (void*) i);
		}
		
		// A thread principal espera todas as outras terminarem.
		for (int i = 0; i < n; i++)
		{
			pthread_join(thread[i], NULL);
		}
		
		// Imprime o resultado encontrado.
		printf ("Tempo: %d\n", (int) (time(NULL) - start));
		printf ("Maior valor na posição (%lld, %lld): %lld.\n", resposta[1], resposta[2], resposta[0]);
		printf ("Menor valor na posição (%lld, %lld): %lld.\n", resposta[4], resposta[5], resposta[3]);
		printf ("Maior produto interno na linha %lld: %lld.\n", resposta[7], resposta[6]);
		printf ("Menor produto interno na linha %lld: %lld.\n", resposta[9], resposta[8]);
		printf ("\n");
		
		// Libera a memória utilizada.
		free(thread);
		for (int i = 0; i < n; i++)
		{
			free(mat[i]);
			free(ptr[i]);
		}
		free(mat);
		free(ptr);
	}

	return 0;
}

// Calcula o produto interno de uma linha e guarda o que for necessário no vetor resposta.
void *prod_interno (void *arg)
{
	int maior[2] = {-1, 0};				// Guarda o maior valor da linha e sua posição.
	int menor[2] = {0x3f3f3f3f, 0};		// Guarda o menor valor da linha e sua posição.
	long long ans = 0;					// Guarda o produto interno da linha.
	int i = (int) arg;					// Determina de qual linha se trata a chamada atual.
	
	for (int j = 0; j < m; j++)
	{
		// Percorre a linha somando o quadrado de cada componente ao produto interno.
		ans += mat[i][j] * mat[i][j];
		
		if (mat[i][j] > maior[0])
		{
			maior[0] = mat[i][j];
			maior[1] = j;
		}
		
		if (mat[i][j] < menor[0])
		{
			menor[0] = mat[i][j];
			menor[1] = j;
		}
	}
	
	// Verifica se o maior da linha é o maior da tabela até o momento e guarda caso seja.
	pthread_mutex_lock(&semaforo[0]);
		if (maior[0] > resposta[0])
		{
			resposta[0] = maior[0];
			resposta[1] = i;
			resposta[2] = maior[1];
		}
	pthread_mutex_unlock(&semaforo[0]);
	
	// Verifica se o menor da linha é o menor da tabela até o momento e guarda caso seja.
	pthread_mutex_lock(&semaforo[1]);
		if (menor[0] < resposta[0])
		{
			resposta[3] = menor[0];
			resposta[4] = i;
			resposta[5] = menor[1];
		}
	pthread_mutex_unlock(&semaforo[1]);
	
	// Testa se o produto interno desta linha é o maior dentre os já calculados. Caso seja, guarda.
	pthread_mutex_lock(&semaforo[2]);
		if (ans > resposta[6])
		{
			resposta[6] = ans;
			resposta[7] = i;
		}
	pthread_mutex_unlock(&semaforo[2]);
	
	// Testa se o produto interno desta linha é o menor dentre os já calculados. Caso seja, guarda.
	pthread_mutex_lock(&semaforo[3]);
		if (ans < resposta[8])
		{
			resposta[8] = ans;
			resposta[9] = i;
		}
	pthread_mutex_unlock(&semaforo[3]);
	
	return NULL;
}


