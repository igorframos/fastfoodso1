#ifndef _ESTOQUE_H_
#define _ESTOQUE_H_

#include <stdio.h>
#include <string>
#include <string.h>
#include <semaphore.h>
#include "pedido.h"
#include "item.h"

class Estoque
{
    public:
        sem_t semaforo;

        int atum;
        int azeitona;
        int almondega;
        int alface;
        int bacon;
        int batata_palha;
        int carne;
        int frango;
        int lombo_canadense;
        int milho;
        int ovo;
        int pao;
        int pao_arabe;
        int pao_cachorro_quente;
        int pao_hamburger;
        int peru;
        int peperoni;
        int peito_de_peru;
        int presunto;
        int pimentao;
        int queijo;
        int salame;
        int salsicha;
        int tomate;

        int coca;
        int guarana;
        int soda;
        int suco_laranja;
        int suco_uva;
        int iced_tea;

    public:
        Estoque(std::string);
        Estoque();
        bool retira_ingredientes(Pedido);
        void adiciona_ingredientes(Pedido);
        void wait();
        void post();

};

#endif
