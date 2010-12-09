#include "pedido.h"



Pedido::Pedido()
{
    sem_init(&semaforo, 0, 0);
    preco = 0;
}

void Pedido::adiciona_item(Item i)
{
    itens.push_back(i);
    preco += i.getPreco();
}

float Pedido::getPreco()
{
    return preco;
}

std::vector<Item> Pedido::getItens()
{
    return itens;
}

void Pedido::wait()
{
    sem_wait(&semaforo);
}    

void Pedido::post()
{
    sem_post(&semaforo);
}
