#include "relogio.h"
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

struct desp{
    int despertador;
    int intervalo;
    int qual_processo;
    bool Periodico;
};

struct rel{
    int relo;
    desp desp[500];
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
    for(int i=0; i<500; i++){
        relo->desp[i].despertador = -20;     //inicializa os despertadores com um valor imposivel
    }
}

void cria_despertadores(rel *relo, int inicio, int intervalo, int qual_processo, bool periodico){//essa variavel vai controlar se o despertador e periodico

    for(int i=0; i<500; i++){
        if(relo->desp[i].despertador == -20){
            relo->desp[i].intervalo = intervalo;
            relo->desp[i].despertador = inicio;
            relo->desp[i].qual_processo = qual_processo;
            if(periodico == true){
                relo->desp[i].Periodico = true;
            }
        i = 500;
        }
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
    for(int i=0; i<500; i++){
        if(relo->desp[i].despertador == relo->relo){
            if(relo->desp[i].qual_processo >= 0 && relo->desp[i].qual_processo < 10){
                relo->desp[i].despertador = -50;
                return relo->desp[i].qual_processo;    //tem despertador e para desbloquear um processo
            }
            if(relo->desp[i].Periodico == true){
                relo->desp[i].despertador += relo->desp[i].intervalo;
            }
            return 11;  //tem despretador
        }
    }
    return -1;  //nao tem despertador
}
