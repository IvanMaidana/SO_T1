#include<stdio.h>
#include <stdlib.h>

#include"so_t1.h"

es_t *es_cria(){
    es_t *controlador = (es_t*)malloc(sizeof(es_t));
    controlador = 0;
    return controlador;
}


err_t es_le(es_t *es, int dispositivo, int *pvalor){   //o=teclado    es = 0 ler
    if(dispositivo != 0){
        return ERR_ES_DISP_INV;  //dispositivo errado
    }else if (es != 0){
        return ERR_ES_OP_INV;   //operacao invalida(nao e para ler)
    }else{
        int valor;
        scanf("%d", &valor);
        *pvalor = valor;
        return ERR_OK;
    }
}


err_t es_escreve(es_t *es, int dispositivo, int valor){  //1=terminal    es = 1 escrever
    if(dispositivo != 1){
         return ERR_ES_DISP_INV; //dispositivo errado
    }else if (es != 1){
        return ERR_ES_OP_INV;   //operacao invalida(nao e printar na tela)
    }else{
        printf("%d\n", valor);
        return ERR_OK;
    }
}
