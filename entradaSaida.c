# include "entradaSaida.h"
#include <stdlib.h>
#include<stdio.h>


err_t verif_acesso(es_t *es, int dispositivo, acesso_t tipo){
  // só tem 2 dispositivos, teclado e video
  if (dispositivo != teclado && dispositivo != video) return ERR_ES_DISP_INV;
  // do teclado só dá para ler
  if (dispositivo == teclado && tipo != leitura) return ERR_ES_OP_INV;
  // o video é só para escrita
  if (dispositivo == video && tipo != escrita) return ERR_ES_OP_INV;
  return ERR_OK;
}



es_t *es_cria(){
    es_t *es = malloc(sizeof(*es));
    if (es == NULL){
            printf("Memoria insuficiente.\n");
            exit(1); // aborta o programa porque nao foi possivel alocar a memoria da nossa cpu
    }
    return es;
}

void *es_cria_escrita(es_t *es, char escrita[10]){
    int i=0;
    while(escrita[i] != '\0'){
        es->escrita[i] = escrita[i];
        i++;
    }
    FILE *file = fopen(es->escrita, "w"); //cria o arquivo de escrita  "no t4 trocar po "w"
    if (file == NULL){
            printf("Memoria insuficiente para alocar o arquivo de escrita.\n");
            exit(1);
    }
    fclose(file);
}

void *es_cria_leitura(es_t *es, char leitura[10]){
    es->leitura = fopen(leitura, "r");//abri o arquivo para escrita
        if(es->leitura == NULL){
           printf("Nao foi possivel abrir o arquivo\n");
        }
}


err_t es_le(es_t *es, int dispositivo, int *pvalor){   //o=teclado
    err_t err = verif_acesso(es, dispositivo, leitura);
    if(err == ERR_OK){
            int valor;
            fscanf(es->leitura, "%d", &valor);
            *pvalor = valor;
    }
    return err;
}

err_t es_escreve(es_t *es, int dispositivo, int valor){  //1=terminal

    err_t err = verif_acesso(es, dispositivo, escrita);
    if(err == ERR_OK){
        FILE *escreve = fopen(es->escrita, "a");//abri o arquivo para escrita
        if(escreve == NULL){
           printf("Nao foi possivel abrir o arquivo\n");
        }else{
            fprintf(escreve, "%d", valor);
        }
        fclose(escreve);
   }
   return err;
}
