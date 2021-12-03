#include "relogio.h"
#include<stdlib.h>
#include<stdio.h>

struct desp{
    int incremento;
    int despertador;
};

struct rel{
    int relo;
    desp desp[50];
};


rel *cria_rel(void){
    rel *relo = malloc(sizeof(*relo));
    if(relo == NULL){
        printf("Memoria insuficiente.\n");
        exit(1);
    }
    inic_despe(relo);
    relo->relo = 0;  //inicializa o relogio
    return relo;
}

void inic_despe(rel *relo){
    for(int i=0; i<50; i++){
        relo->desp[i].despertador = -20;     //inicializa os despertadores com um valor imposivel
    }
}

void cria_despertadores(rel *relo, int inicio, int periodo){
    int i=0, controle=0;
    while(i<50 && controle == 0){
        if(relo->desp[i].despertador == -20){
            relo->desp[i].incremento = periodo;
            relo->desp[i].despertador = inicio;
            controle = 1;
        }
        i++;
    }
}

void passou_tempo(rel *relo){
    relo->relo += 1;
}

int que_horas_sao(rel *relo){
    return relo->relo;
}

void imprime_relogio(rel *relo){
    printf("valor do relogio %d\n", relo->relo);
}

int tem_despertador(rel *relo){
    for(int i=0; i<50; i++){
        if(relo->desp[i].despertador == relo->relo){
            relo->desp[i].despertador += relo->desp[i].incremento;
            return 1;  //tem despretador
        }
    }
    return 0;  //nao tem despertador
}
