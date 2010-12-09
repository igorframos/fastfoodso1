#ifndef _RELATORIO_H
#define _RELATORIO_H

struct Relatorio
{
	char nome[128];
	int clientes_recebidos;
	int clientes_sem_pedido;
	int clientes_sem_dinheiro;
	int clientes_atendidos;
	float lucro;
};

#endif

