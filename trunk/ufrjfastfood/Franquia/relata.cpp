#include "relata.h"

void *relata (void *ptr)
{
	hostent *server;
	int socketID, porta;
	Relatorio relatorio;
	sockaddr_in serv_addr;
	
	socketID = socket(AF_INET, SOCK_STREAM, 0);
	if (socketID < 0)
	{
		puts ("Impossível abrir canal de comunicação externa. Não serão enviados relatórios.\n");
		pthread_exit(NULL);
	}
	
	porta = 2010;
	
	server = gethostbyname(hostname.c_str());
	printf ("O host é: %s. O server vale: %p\n\n", hostname.c_str(), server);
	if (!server)
	{
		puts ("Não foi possível encontrar o servidor pelo nome dado. Não serão enviados relatórios.\n");
		pthread_exit(NULL);
	}
	
	bzero (&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(porta);
	
	if (connect(socketID,(const struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0)
	{
		printf ("Impossível conectar ao servidor. Não serão enviados relatórios.\n");
		pthread_exit(NULL);
	}
	
	while (!fim)
	{
		sleep(mes);
		if (fim) break;
		
		sem_wait(&semaforo_relatorio);
			relatorio = relatorio_mes;
		
			relatorio_mes.clientes_recebidos = 0;
			relatorio_mes.clientes_sem_pedido = 0;
			relatorio_mes.clientes_sem_dinheiro = 0;
			relatorio_mes.clientes_atendidos = 0;
			relatorio_mes.lucro = 0;
		sem_post(&semaforo_relatorio);
	
		if (write(socketID, &relatorio, sizeof(relatorio)) < 0)
		{
			puts ("Impossível enviar mensagem para o servidor. Relatório deste mês não poderá ser enviado.\n");
		}
	}
	
	memset (&relatorio, 0, sizeof(relatorio));
	write(socketID, &relatorio, sizeof(relatorio));
	
	close(socketID);
	
	return NULL;
}

