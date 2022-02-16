#ifndef ES_H
#define ES_H

#include"err.h"
#include<stdio.h>

//entrada e saida
typedef struct es_t es_t;

struct es_t {
    char escrita[20]; //Nome do arquivo de escrita
    FILE *leitura;
    //char leitura[20]; //Nome do arquivo de leitura
};

typedef enum { leitura, escrita } acesso_t;
typedef enum { teclado=0, video=1 } dispositivo_t;

err_t verif_acesso(es_t *es, int dispositivo, acesso_t tipo);

es_t *es_cria();//cria a struct de entrada e saida
void *es_cria_escrita(es_t *es, char escrita[10]);  //vai criar o arquivo onde vai ser escrito
void *es_cria_leitura(es_t *es, char leitura[10]); //o arquivo ja foi criado so vai passar o nome

err_t es_le(es_t *es, int dispositivo, int *pvalor);
err_t es_escreve(es_t *es, int dispositivo, int valor);

#endif // ES_H
