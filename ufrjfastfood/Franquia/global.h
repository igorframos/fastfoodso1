#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <string>
#include "pedido.h"
#include "estoque.h"
#include "relatorio.h"

extern std::string hostname;

extern char identificador[128];

extern int num_caixas_pedido;
extern int num_caixas_pagamento;
extern int num_cozinheiros;
extern int num_entregadores;
extern int at_caixa_pedido[5];
extern int at_caixa_pagamento[5];
extern int at_cozinheiro[5];
extern int at_entregador[5];

extern pthread_mutex_t relatorio_dia;

extern Estoque guardados;
extern Estoque separados;
extern Estoque almoxarifado;

extern int fila_entrada;
extern int canal_conversa[5][2];
extern int fila_pagamento[5];
extern int canal_pagamento[5][2];
extern int fila_pedidos;
extern int fila_produtos;

extern Relatorio relatorio_mes;
extern sem_t semaforo_relatorio;

extern std::map<unsigned int,Pedido> pedido;

extern std::vector<std::string> conversa[30];

extern bool fim;

extern int dia;
extern int mes;

extern pthread_mutex_t mutex_IO;

#endif
