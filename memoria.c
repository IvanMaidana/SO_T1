#include <stdio.h>
#include <stdlib.h>

#include"so_t1.h"


//retorna o tamanho da memoria
int mem_tam(mem_t *m){
    int contador=0;
    for(int i=0; m[i] != -1; i++){
        contador++;
    }
    return contador;
}

//cria a memoria
mem_t *mem_cria(int tam){
    if(tam > 0){
        mem_t *m = (mem_t*)malloc(tam * sizeof(mem_t)) ;
        if (m == NULL){
            printf("Memoria insuficiente.\n");
            exit(1); // aborta o programa porque nao foi possivel alocar a memoria da nossa cpu
        }
        m[tam] = -1;                      //defino como -1 o final da memoria(vetor)
        for(int i=0; i<tam; i++){
            m[i]=0;                    //inicializa com 0 a memoria
        }
        return m;
    }else{
        return 0;
    }
}

//mando o enderco e me retorna o que tem neste endereco da memoria
err_t mem_le(mem_t *m, int endereco, int *pvalor){
    int tam = mem_tam(m);
    if(endereco > tam || endereco < 0){
        return ERR_MEM_END_INV;  //esta tentando acessar um indice invalido da memoria
    }else{
        *pvalor = m[endereco];
        return ERR_OK;
    }
}

//mando o valor e o endereco e escreve na memoria
err_t mem_escreve(mem_t *m, int endereco, int valor){
    int tam = mem_tam(m);
    if(endereco < 0 || endereco > tam){
         return ERR_MEM_END_INV;      //esta tentando acessar um indice invalido da memoria
    }else{
        m[endereco] = valor;
        return ERR_OK;
    }
}

void mem_destroi(mem_t *m){
    free(m);
}

