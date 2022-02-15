#include"UC.h"
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

struct uc_t {
    cpu_t *cpu;
    so_t *so;
    rel *relo;
};

uc_t *uc_cria(void){ //cria a unidade de controle
    uc_t *unidade = malloc(sizeof(*unidade));
    if (unidade == NULL){
            printf("Memoria insuficiente.\n");
            exit(1);
    }
    return unidade;
}

 void uc_altera_cpu(uc_t *uc, cpu_t *cpu){
     uc->cpu = cpu;
 }

void uc_altera_so(uc_t *uc, so_t *so){
    uc->so = so;
}

void uc_altera_relo(uc_t *uc, rel *relo){
    uc->relo = relo;
}

void imprime_estado(cpu_estado_t *est) //funcao que imprime o estado do processador
{
  printf("PC=%04d A=%06d X=%06d\n", est->pc, est->a, est->x);
}


err_t laco_controlador(uc_t *uc){
    err_t err;
    int tem;
    while (true){
        imprime_estado(cpu_estado(uc->cpu));
        err = cpu_executa_1(uc->cpu);
        passou_tempo(uc->relo);
        tem = 12;  //controlara o despertador
        while(tem != -1){
            if(err != ERR_OK){
               err = verifica_interrupcao(err, uc->so, -1);  //manda pro SO verificar a interrupcao
               while(err == ERR_CPU_PARADA){
                    passou_tempo(uc->relo);
                    tem = tem_despertador(uc->relo);
                    if(tem != -1){
                        err = verifica_interrupcao(err, uc->so, tem);
                    }
               }
               if(err != ERR_OK){           //verifica se SO conseguiu resolver a interrupcao
                    printf("Estado final:\n");
                    imprime_estado(cpu_estado(uc->cpu));
                    imprime_tempos(uc->so);
                    return err;
               }
            }
            tem = tem_despertador(uc->relo);
            if(tem == 11){
                imprime_estado(cpu_estado(uc->cpu));
                err = verifica_interrupcao(err, uc->so, -1);
                while(err == ERR_CPU_PARADA){
                   passou_tempo(uc->relo);
                   tem = tem_despertador(uc->relo);
                   if(tem != -1){
                        err = verifica_interrupcao(err, uc->so, tem);
                   }
                }
                if(err != ERR_OK){           //verifica se SO conseguiu resolver a interrupcao
                    printf("Estado final:\n");
                    imprime_estado(cpu_estado(uc->cpu));
                    imprime_tempos(uc->so);
                    return err;
                }
            }else if(tem>=0 && tem <10){
                imprime_estado(cpu_estado(uc->cpu));
                err = verifica_interrupcao(err, uc->so, tem); //desbloquear um processo tem = indice do processo no descritor
                if(err != ERR_OK){
                    printf("Estado final:\n");
                    imprime_estado(cpu_estado(uc->cpu));
                    imprime_tempos(uc->so);
                    return err;
                }
            }
        }
    }
    return err;
}

