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

es_t *es_cria(char leitura[10], char escrita[10]){
    es_t *es = malloc(sizeof(*es));
    if (es == NULL){
            printf("Memoria insuficiente.\n");
            exit(1); // aborta o programa porque nao foi possivel alocar a memoria da nossa cpu
    }
    int i=0;
    while(leitura[i] != '\0'){
        es->leitura[i] = leitura[i];
        es->escrita[i] = escrita[i];
        i++;
    }

    FILE *file = fopen(es->escrita, "a"); //cria o arquivo de escrita
    if (file == NULL){
            printf("Memoria insuficiente para alocar o arquivo de escrita.\n");
            exit(1);
    }
    FILE *files = fopen(es->leitura, "a"); // cria o arquivo de leitura
    if (files == NULL){
            printf("Memoria insuficiente para alocar o arquivo de leitura.\n");
            exit(1);
    }

    fclose(file);
    fclose(files);
    return es;
 }

err_t es_le(es_t *es, int dispositivo, int *pvalor){   //o=teclado

    err_t err = verif_acesso(es, dispositivo, leitura);
    if(err == ERR_OK){
        FILE *ler = fopen(es->leitura, "a");//abri o arquivo para escrita
        if(ler == NULL){
           printf("Nao foi possivel abrir o arquivo\n");
        }else{
            int valor;
            scanf("%d", &valor);
            *pvalor = valor;
            fprintf(ler, "%d", valor);
            fclose(ler);
        }
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
