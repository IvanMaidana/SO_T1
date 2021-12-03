#ifndef ES_H
#define ES_H

#include"err.h"

//entrada e saida
typedef struct es_t es_t;

struct es_t {
    char escrita[20]; //Nome do arquivo de escrita
    char leitura[20]; //Nome do arquivo de leitura
};

typedef enum { leitura, escrita } acesso_t;
typedef enum { teclado=0, video=1 } dispositivo_t;

err_t verif_acesso(es_t *es, int dispositivo, acesso_t tipo);

es_t *es_cria(char leitura[10], char escrita[10]);//cria os 2 arquivos de entrada e saida

err_t es_le(es_t *es, int dispositivo, int *pvalor);
err_t es_escreve(es_t *es, int dispositivo, int valor);

#endif // ES_H
