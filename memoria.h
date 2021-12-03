
#ifndef MEM_H
#define MEM_H

#include"err.h"
// memoria
typedef  struct  mem_t  mem_t ;

struct  mem_t {
  int tam;  // tamanho da memória
  int * mem;  // propria memoria
};

mem_t * mem_cria ( int tam); // cria memoria
int  mem_tam ( mem_t * m);   // verifica o tamanho da memoria
err_t  mem_le ( mem_t * m, int endereco, int * pvalor); // le um valor da memoria
err_t  mem_escreve ( mem_t * m, int endereco, int valor); // escreve um valor na memoria
void  mem_destroi ( mem_t * m);  // libera memoria

#endif // MEM_H
