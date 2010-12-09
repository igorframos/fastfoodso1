#include "item.h"

//Dado o nome do item, seta os ingredientes desse item
Item::Item(std::string n)
{
    nome = n;

    preco = 0;

    atum = 0;
    azeitona = 0;
    almondega = 0;
    alface = 0;
    bacon = 0;
    batata_palha = 0;
    carne = 0;
    frango = 0;
    lombo_canadense = 0;
    milho = 0;
    ovo = 0;
    pao = 0;
    pao_arabe = 0;
    pao_cachorro_quente = 0;
    pao_hamburger = 0;
    peru = 0;
    peperoni = 0;
    peito_de_peru = 0;
    presunto = 0;
    pimentao = 0;
    queijo = 0;
    salame = 0;
    salsicha = 0;
    tomate = 0;

    coca = 0;
    guarana = 0;
    soda = 0;
    suco_laranja = 0;
    suco_uva = 0;
    iced_tea = 0;

    if (nome.compare("basicao") == 0)
    {
        preco = 5.00;

        pao = 1;
        presunto = 1;
        queijo = 1;
        alface = 1;
        tomate = 1;
    }
    else if (nome.compare("peruba") == 0)
    {
        preco = 6.50;

        pao = 1;
        peru = 1;
        presunto = 1;
        bacon = 1;
    }
    else if (nome.compare("frango") == 0)
    {
        preco = 6.00;

        pao = 1;
        frango = 1;
        alface = 1;
        tomate = 1;
    }
    else if (nome.compare("italiano") == 0)
    {
        preco = 6.00;

        pao = 1;
        salame = 1;
        peperoni = 1;
        presunto = 1;
    }
    else if (nome.compare("ariel") == 0)
    {
        preco = 6.50;

        pao = 1;
        atum = 1;
        azeitona = 1;
    }
    else if (nome.compare("americano") == 0)
    {
        preco = 5.00;

        pao = 1;
        almondega = 1;
    }
    else if (nome.compare("natural") == 0)
    {
        preco = 5.00;

        pao_arabe = 1;
        alface = 1;
        tomate = 1;
        peito_de_peru = 1;
        queijo = 1;
    }
    else if (nome.compare("cachorro_quente") == 0)
    {
        preco = 5.00;

        pao_cachorro_quente = 1;
        salsicha = 1;
        tomate = 1;
        pimentao = 1;
        milho = 1;
        batata_palha = 1;
    }
    else if (nome.compare("cheeseburger") == 0)
    {
        preco = 6.00;

        pao_hamburger = 1;
        carne = 1;
        queijo = 1;
        batata_palha = 1;
    }
    else if (nome.compare("cheesebacon") == 0)
    {
        preco = 6.50;

        pao_hamburger = 1;
        carne = 1;
        bacon = 1;
        queijo = 1;
    }
    else if (nome.compare("cheesetudo") == 0)
    {
        preco = 7.00;

        pao_hamburger = 1;
        carne = 1;
        queijo = 1;
        bacon = 1;
        ovo = 1;
    }
    else if (nome.compare("hamburger") == 0)
    {
        preco = 5.00;

        pao_hamburger = 1;
        carne = 1;
    }
    else if (nome.compare("canadense") == 0)
    {
        preco = 5.00;

        pao = 1;
        lombo_canadense = 1;
        alface = 1;
        tomate = 1;
    }
    else if (nome.compare("coca-cola") == 0)
    {
        preco = 2.50;

        coca = 1;
    }
    else if (nome.compare("guarana") == 0)
    {
        preco = 2.50;

        guarana = 1;
    }
    else if (nome.compare("soda") == 0)
    {
        preco = 2.50;

        soda = 1;
    }
    else if (nome.compare("suco_laranja") == 0)
    {
        preco = 2.00;

        suco_laranja = 1;
    }
    else if (nome.compare("suco_uva") == 0)
    {
        preco = 2.00;

        suco_uva = 1;
    }
    else if (nome.compare("iced_tea") == 0)
    {
        preco = 3.00;

        iced_tea = 1;
    }
}

std::string Item::getNome(void)
{
    return nome;
}

float Item::getPreco()
{
    return preco;
}
