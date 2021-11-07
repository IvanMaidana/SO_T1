#include<stdio.h>
#include <stdlib.h>

#include"so_t1.h"

struct es_t {
  // não tem muito o que colocar aqui, os dispositivos estão hard-coded abaixo
};


err_t verif_acesso(es_t *es, int dispositivo, acesso_t tipo){
  // só tem 2 dispositivos, teclado e video
  if (dispositivo != teclado && dispositivo != video) return ERR_ES_DISP_INV;
  // do teclado só dá para ler
  if (dispositivo == teclado && tipo != leitura) return ERR_ES_OP_INV;
  // o video é só para escrita
  if (dispositivo == video && tipo != escrita) return ERR_ES_OP_INV;
  return ERR_OK;
}

es_t *es_cria(void){
    es_t *controlador = malloc(sizeof(*controlador));
    if (controlador == NULL){
            printf("Memoria insuficiente.\n");
            exit(1); // aborta o programa porque nao foi possivel alocar a memoria da nossa cpu
    }
    return controlador;
}


err_t es_le(es_t *es, int dispositivo, int *pvalor){   //o=teclado

    err_t err = verif_acesso(es, dispositivo, leitura);
    if(err == ERR_OK){
        int valor;
        printf("Digite um numro inteiro\n");
        scanf("%d", &valor);
        *pvalor = valor;
    }
    return err;
}


err_t es_escreve(es_t *es, int dispositivo, int valor){  //1=terminal

    err_t err = verif_acesso(es, dispositivo, escrita);
    if(err == ERR_OK){
        printf("VALOR DA ESCRITA: %d\n", valor);
    }
    return err;
}
