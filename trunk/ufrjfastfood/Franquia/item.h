#ifndef _ITEM_H_ 
#define _ITEM_H_

#include <string>

class Item
{
    public:
        std::string nome;
        float preco;
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
        Item( std::string );
        std::string getNome();
        float getPreco();
};

#endif
