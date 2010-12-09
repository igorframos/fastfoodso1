#include "gerador_pedido.h"

#include <time.h>
#include <stdlib.h>
#include <string>

//Retorna uma string com o nome de um item. Aleatoriamente
std::string gera_item(void)
{
    int item = rand() % 19;

    if (item == 0)
        return "basicao";
    else if (item == 1)
        return "peruba";
    else if (item == 2)
        return "frango";
    else if (item == 3)
        return "italiano";
    else if (item == 4)
        return "ariel";
    else if (item == 5)
        return "americano";
    else if (item == 6)
        return "natural";
    else if (item == 7)
        return "cachorro_quente";
    else if (item == 8)
        return "cheeseburger";
    else if (item == 9)
        return "cheesebacon";
    else if (item == 10)
        return "cheesetudo";
    else if (item == 11)
        return "hamburger";
    else if (item == 12)
        return "canadense";
    else if (item == 13)
        return "coca-cola";
    else if (item == 14)
        return "guarana";
    else if (item == 15)
        return "soda";
    else if (item == 16)
        return "suco_laranja";
    else if (item == 17)
        return "suco_uva";
    else if (item == 18)
        return "iced_tea";

    return "";
}
