#include "estoque.h"

//Constroi um estoque vazio.
Estoque::Estoque()
{
    sem_init(&semaforo, 0, 1);

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

}

//Constroi um estoque cuja quantidade de cada ingrediente esta em um arquivo
//Path do arquivo dado no parametro
//O arquivo tem como padrao
//nome_ingrediente quantidade
Estoque::Estoque(std::string file_path)
{
    sem_init(&semaforo, 0, 1);

    FILE* arquivo_entrada;
    arquivo_entrada =  fopen(file_path.c_str(), "r");
    if (arquivo_entrada != NULL)
    {
        while (!feof(arquivo_entrada))
        {
            char ingrediente[21];
            int quantidade;
            fscanf(arquivo_entrada, "%s %d", ingrediente, &quantidade);

            if (!strcmp(ingrediente, "atum"))
                atum = quantidade;
            if (!strcmp(ingrediente, "azeitona"))
                azeitona = quantidade;
            if (!strcmp(ingrediente, "almondega"))
                almondega = quantidade;
            if (!strcmp(ingrediente, "alface"))
                alface = quantidade;
            if (!strcmp(ingrediente, "bacon"))
                bacon = quantidade;
            if (!strcmp(ingrediente, "batata_palha"))
                batata_palha = quantidade;
            if (!strcmp(ingrediente, "carne"))
                carne = quantidade;
            if (!strcmp(ingrediente, "frango"))
                frango = quantidade;
            if (!strcmp(ingrediente, "lombo_canadense"))
                lombo_canadense = quantidade;
            if (!strcmp(ingrediente, "milho"))
                milho = quantidade;
            if (!strcmp(ingrediente, "ovo"))
                ovo = quantidade;
            if (!strcmp(ingrediente, "pao"))
                pao = quantidade;
            if (!strcmp(ingrediente, "pao_arabe"))
                pao_arabe = quantidade;
            if (!strcmp(ingrediente, "pao_cachorro_quente"))
                pao_cachorro_quente = quantidade;
            if (!strcmp(ingrediente, "pao_hamburger"))
                pao_hamburger = quantidade;
            if (!strcmp(ingrediente, "peru"))
                peru = quantidade;
            if (!strcmp(ingrediente, "peperoni"))
                peperoni = quantidade;
            if (!strcmp(ingrediente, "peito_de_peru"))
                peito_de_peru = quantidade;
            if (!strcmp(ingrediente, "presunto"))
                presunto = quantidade;
            if (!strcmp(ingrediente, "pimentao"))
                pimentao = quantidade;
            if (!strcmp(ingrediente, "queijo"))
                queijo = quantidade;
            if (!strcmp(ingrediente, "salame"))
                salame = quantidade;
            if (!strcmp(ingrediente, "salsicha"))
                salsicha = quantidade;
            if (!strcmp(ingrediente, "tomate"))
                tomate = quantidade;

            if (!strcmp(ingrediente, "coca"))
                coca = quantidade;
            if (!strcmp(ingrediente, "guarana"))
                guarana = quantidade;
            if (!strcmp(ingrediente, "soda"))
                soda = quantidade;
            if (!strcmp(ingrediente, "suco_laranja"))
                suco_laranja = quantidade;
            if (!strcmp(ingrediente, "suco_uva"))
                suco_uva = quantidade;
            if (!strcmp(ingrediente, "iced_tea"))
                iced_tea = quantidade;

        }
    }
}

//Recebe um pedido por parametro, e primeiro verifica se todos os ingredientes
//deste pedido estao disponiveis
//Caso nao estejam, retorna false
//Caso estejam, retira do estoque e retorna true
bool Estoque::retira_ingredientes(Pedido ped)
{
    std::vector<Item> itens_pedido = ped.getItens();

    for (int i = 0; i < (int) itens_pedido.size(); i++)
    {
        if (itens_pedido[i].atum > atum)
            return false;
        if (itens_pedido[i].azeitona > azeitona)
            return false;
        if (itens_pedido[i].almondega > almondega )
            return false;
        if (itens_pedido[i].alface>alface)
            return false;
        if (itens_pedido[i].bacon>bacon)
            return false;
        if (itens_pedido[i].batata_palha>batata_palha)
            return false;
        if (itens_pedido[i].carne>carne)
            return false;
        if (itens_pedido[i].frango>frango)
            return false;
        if (itens_pedido[i].lombo_canadense>lombo_canadense)
            return false;
        if (itens_pedido[i].milho>milho)
            return false;
        if (itens_pedido[i].ovo>ovo)
            return false;
        if (itens_pedido[i].pao>pao)
            return false;
        if (itens_pedido[i].pao_arabe>pao_arabe)
            return false;
        if (itens_pedido[i].pao_cachorro_quente>pao_cachorro_quente)
            return false;
        if (itens_pedido[i].pao_hamburger>pao_hamburger)
            return false;
        if (itens_pedido[i].peru>peru)
            return false;
        if (itens_pedido[i].peperoni>peperoni)
            return false;
        if (itens_pedido[i].peito_de_peru>peito_de_peru)
            return false;
        if (itens_pedido[i].presunto>presunto)
            return false;
        if (itens_pedido[i].pimentao>pimentao)
            return false;
        if (itens_pedido[i].queijo>queijo)
            return false;
        if (itens_pedido[i].salame>salame)
            return false;
        if (itens_pedido[i].salsicha>salsicha)
            return false;
        if (itens_pedido[i].tomate>tomate)
            return false;

        if (itens_pedido[i].coca>coca)
            return false;
        if (itens_pedido[i].guarana>guarana)
            return false;
        if (itens_pedido[i].soda>soda)
            return false;
        if (itens_pedido[i].suco_laranja>suco_laranja)
            return false;
        if (itens_pedido[i].suco_uva>suco_uva)
            return false;
        if (itens_pedido[i].iced_tea>iced_tea)
            return false;
    }

    for (int i = 0; i < (int) itens_pedido.size(); i++)
    {
        atum -= itens_pedido[i].atum;
        azeitona -= itens_pedido[i].azeitona;
        almondega -= itens_pedido[i].almondega;
        alface -= itens_pedido[i].alface;
        bacon -= itens_pedido[i].bacon;
        batata_palha -= itens_pedido[i].batata_palha;
        carne -= itens_pedido[i].carne;
        frango -= itens_pedido[i].frango;
        lombo_canadense -= itens_pedido[i].lombo_canadense;
        milho -= itens_pedido[i].milho;
        ovo -= itens_pedido[i].ovo;
        pao -= itens_pedido[i].pao;
        pao_arabe -= itens_pedido[i].pao_arabe;
        pao_cachorro_quente -= itens_pedido[i].pao_cachorro_quente;
        pao_hamburger -= itens_pedido[i].pao_hamburger;
        peru -= itens_pedido[i].peru;
        peperoni -= itens_pedido[i].peperoni;
        peito_de_peru -= itens_pedido[i].peito_de_peru;
        presunto -= itens_pedido[i].presunto;
        pimentao -= itens_pedido[i].pimentao;
        queijo -= itens_pedido[i].queijo;
        salame -= itens_pedido[i].salame;
        salsicha -= itens_pedido[i].salsicha;
        tomate -= itens_pedido[i].tomate;

        coca -= itens_pedido[i].coca;
        guarana -= itens_pedido[i].guarana;
        soda -= itens_pedido[i].soda;
        suco_laranja -= itens_pedido[i].suco_laranja;
        suco_uva -= itens_pedido[i].suco_uva;
        iced_tea -= itens_pedido[i].iced_tea;

    }

    return true;
}

//Adiciona os ingredientes relacionados ao pedido passado por parametro
//ao estoque
void Estoque::adiciona_ingredientes(Pedido ped)
{
    std::vector<Item> itens_pedido = ped.getItens();

    for (int i = 0; i < (int) itens_pedido.size(); i++)
    {
        atum += itens_pedido[i].atum;
        azeitona += itens_pedido[i].azeitona;
        almondega += itens_pedido[i].almondega;
        alface += itens_pedido[i].alface;
        bacon += itens_pedido[i].bacon;
        batata_palha += itens_pedido[i].batata_palha;
        carne += itens_pedido[i].carne;
        frango += itens_pedido[i].frango;
        lombo_canadense += itens_pedido[i].lombo_canadense;
        milho += itens_pedido[i].milho;
        ovo += itens_pedido[i].ovo;
        pao += itens_pedido[i].pao;
        pao_arabe += itens_pedido[i].pao_arabe;
        pao_cachorro_quente += itens_pedido[i].pao_cachorro_quente;
        pao_hamburger += itens_pedido[i].pao_hamburger;
        peru += itens_pedido[i].peru;
        peperoni += itens_pedido[i].peperoni;
        peito_de_peru += itens_pedido[i].peito_de_peru;
        presunto += itens_pedido[i].presunto;
        pimentao += itens_pedido[i].pimentao;
        queijo += itens_pedido[i].queijo;
        salame += itens_pedido[i].salame;
        salsicha += itens_pedido[i].salsicha;
        tomate += itens_pedido[i].tomate;

        coca += itens_pedido[i].coca;
        guarana += itens_pedido[i].guarana;
        soda += itens_pedido[i].soda;
        suco_laranja += itens_pedido[i].suco_laranja;
        suco_uva += itens_pedido[i].suco_uva;
        iced_tea += itens_pedido[i].iced_tea;

    }
}

void Estoque::wait()
{
    sem_wait(&semaforo);
}

void Estoque::post()
{
    sem_post(&semaforo);
}
