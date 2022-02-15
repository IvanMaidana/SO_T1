#include "unidadeExecucao.h"
#include "UC.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int main(){

    // variaveis que representam o computador
    uc_t *uc = uc_cria();  //Unidade de controle
    so_t *so = so_cria();  // SO
    rel *relo = cria_rel(); // relogio
    cpu_t *cpu = cpu_cria(); //cria cpu

    inicializa_vet_programas(); // Vai inicializar o vetor de programas
    so_inicializa(so, cpu, relo);// inicializa o so/cpu/descritor de processos


    //inicializa a uc
    uc_altera_cpu(uc, cpu);
    uc_altera_so(uc, so);
    uc_altera_relo(uc, relo);

    //inicializa a execucao dos processos
    laco_controlador(uc);
}
