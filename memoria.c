# include "memoria.h"
#include <stdbool.h>
#include<stdlib.h>
#include<stdio.h>

//retorna o tamanho da memoria
int mem_tam(mem_t *m){
   return m->tam;
}

//cria a memoria
mem_t *mem_cria(int tam){
    mem_t *m = (mem_t*)malloc(sizeof(mem_t)) ;
    if (m == NULL){
        printf("Memoria insuficiente.\n");
        exit(1); // aborta o programa porque nao foi possivel alocar a memoria da nossa cpu
    }
    m->tam = tam;
    m->mem = malloc(tam*sizeof(*m->mem));
    if(m->mem == NULL){
        free(m);
        m = NULL;
        printf("Nao foi possivel alocar a memoria\n");
    }
    return m;
}

//mando o enderco e me retorna o que tem neste endereco da memoria
err_t mem_le(mem_t *m, int endereco, int *pvalor){

    if(endereco > m->tam || endereco < 0){
        return ERR_MEM_END_INV;  //esta tentando acessar um indice invalido da memoria
    }else{
        *pvalor = m->mem[endereco];
    }
    return ERR_OK;
}

//mando o valor e o endereco e escreve na memoria
err_t mem_escreve(mem_t *m, int endereco, int valor){

    if(endereco < 0 || endereco > m->tam){
         return ERR_MEM_END_INV;      //esta tentando acessar um indice invalido da memoria
    }else{
        m->mem[endereco] = valor;
    }
    return ERR_OK;
}

void mem_destroi(mem_t *m){
    free(m);
    m = NULL;
}

