#ifndef _PEDIDO_H_
#define _PEDIDO_H_

#include <semaphore.h>
#include <vector>

#include "item.h"

class Pedido
{
    private:
    std::vector<Item> itens;
    float preco;
    sem_t semaforo;

    public:
    Pedido();
    void adiciona_item(Item i);
    float getPreco();
    std::vector<Item> getItens();
    void wait();
    void post();

};

#endif
