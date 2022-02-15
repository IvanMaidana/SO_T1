#include"SO.h"
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#define TAM 100 // o tamanho da memória, tambem o tamanho do vetor descritor de processos
int qual_processo=0;
int id=0;

struct programa{
    int tam_mem;   //tamanho que a processo vai usar
    int *conteudo; // conteudo do processo
    bool leitura;  // vai utilizar leitura
    bool escrita;   //vai usar escrita
    char *leituraa;     //ponteiro para o arquivo de leitura
    int tempoE;
    int tempoL;
};
programa vetor_programas[10];  //declaracao do vetor de programas


int inicializa_vet_programas(){
    //////////////////////////////////////Programa Init///////////////////////////////////////////////////////////////
    int progr0[34] = { 2, 0, 7, 2, 3, 5, 31, 21, 1, 9,
                       8, 11, 31, 18, 7, 2, 0, 7, 2, 2,
                       5, 31, 21, 2, 9, 8, 11, 31, 18, 22,
                       1, 0};
    vetor_programas[0].tam_mem = 34 ;
    vetor_programas[0].conteudo = (int*)malloc(vetor_programas[0].tam_mem * sizeof(int));//cria o vetor de processos
    if(vetor_programas[0].conteudo == NULL){
        printf("Nao foi possivel alocar espaco na memoria para o vetor de programas \n");
        exit(1);
    }
    for(int i=0; i<vetor_programas[0].tam_mem; i++){
        vetor_programas[0].conteudo[i] = progr0[i];
    }
    vetor_programas[0].leitura=1;
    vetor_programas[0].leitura=1;
    vetor_programas[0].leituraa = "Leitura_0.txt";
    vetor_programas[0].tempoE = 2;
    vetor_programas[0].tempoL = 2;

    /*/////////////////////////////////////////Segundo programa///////////////////////////////////////////////////////*/
    int progr1[18] = {2, 0, 7, 2, 10, 5, 17, 8, 20, 1, 9, 8, 11, 17, 18, 7, 1, 0};
    vetor_programas[1].tam_mem = 18;
    vetor_programas[1].conteudo = (int*)malloc(vetor_programas[1].tam_mem * sizeof(int));//cria o vetor de processos
    if(vetor_programas[1].conteudo == NULL){
        printf("Nao foi possivel alocar espaco na memoria para o vetor de programas \n");
        exit(1);
    }
    for(int i=0; i<vetor_programas[1].tam_mem; i++){
        vetor_programas[1].conteudo[i] = progr1[i];
    }
    vetor_programas[1].leitura=1;
    vetor_programas[1].leitura=1;
    vetor_programas[1].leituraa = "Leitura_1.txt";
    vetor_programas[1].tempoE = 3;
    vetor_programas[0].tempoL = 3;
    /*/////////////////////////////////////////Terceiro programa///////////////////////////////////////////////////////*/
    int progr2[18] = {2, 0, 7, 2, 10, 5, 17, 8, 20, 1, 9, 8, 11, 17, 18, 7, 1, 0};
    vetor_programas[2].tam_mem = 18;
    vetor_programas[2].conteudo = (int*)malloc(vetor_programas[2].tam_mem * sizeof(int));//cria o vetor de processos
    if(vetor_programas[2].conteudo == NULL){
        printf("Nao foi possivel alocar espaco na memoria para o vetor de programas \n");
        exit(1);
    }
    for(int i=0; i<vetor_programas[2].tam_mem; i++){
        vetor_programas[2].conteudo[i] = progr2[i];
    }
    vetor_programas[2].leitura=1;
    vetor_programas[2].leitura=1;
    vetor_programas[2].leituraa = "Leitura_2.txt";
    vetor_programas[2].tempoE = 4;
    vetor_programas[0].tempoL = 4;
    return 0;
}


struct so_t{
     cpu_t *cpu;
     rel *relo;
     descri_proces *descri;
};


struct tempo_pro{ // tempo do processo em cada estado   tempo de CPU, tempo de bloqueio, tempo em espera
    int execucao; //tempo de cpu
    int bloq;     //tempo de bloqueado
    int pronto;    //tempo de espera
    int quantos_prontos;
    int trocas_de_estado;
    int ultimo_relogio;
    int tempo_da_ultima_execucao;
};

struct descri_proces{ //descricao de processos
    cpu_estado_t *est_cpu; //estado da cpu
    mem_t *mem;           // memoria do processo
    es_t *es;             //entrada e saida
    est_p est;            // enum que controla o estado do processo
    int id;               //identificacao de cda processo
    tempo_pro *temp;       //tempos em cada estado do processo
    int temp_bloq_e;
    int temp_bloq_l;
};


so_t *so_cria(){
    so_t *so = malloc(sizeof(*so));
    if(so == NULL){
        printf("Nao foi possivel alocar espaco \n");
        exit(1);
    }
    return so;
}

void so_inicializa(so_t *so, cpu_t *cpu, rel *relo){
    so->relo = relo;
    so->cpu = cpu;
    mem_t *mem = mem_cria(TAM);   //cria memoria da cpu
    cpu_altera_memoria(so->cpu, mem);
    cpu_estado_t *est = cpu_estado_cria();   //cria estado da cpu
    cpu_altera_estado(so->cpu, est);
    cria_despertadores(so->relo, 2, 4, -1, true);  //Inicialização do despertador (inicio, intervalo, qual_processo, periodico)

    //cria o descritor de processos e inicializa com o codigo de cada processo
    so->descri = cria_descritor(10, 0); //cria o vetor descritor de processos tamanho=10 indice=0
    inicializa_descritor(so);          //Inicializa o descritor com os processos
    recupera_contexto_processo(so);     //coloca o primeiro processo para executar
}


descri_proces *cria_descritor(int tam, int indice){ //cria o descritor

    descri_proces *des = (descri_proces*)malloc(tam * sizeof(descri_proces));//cria o vetor de processos
    if(des == NULL){
        printf("Nao foi possivel alocar espaco na memoria para o descritor \n");
        exit(1);
    }
    for(int i=0; i<tam; i++){
        des[i].est = LIVRE;
        des[i].est_cpu = cpu_estado_cria();
        des[i].temp = temp_cria();
    }
    return des;
}

tempo_pro *temp_cria(){
    tempo_pro *temp = (tempo_pro*)malloc(sizeof(tempo_pro));//cria o vetor de processos
    if(temp == NULL){
        printf("Nao foi possivel alocar espaco para struct tempo \n");
        exit(1);
    }
    return temp;
}


void inicializa_descritor(so_t *so){
    int cont=0;
    char nome_escr[10] = "escrita";  //são os nomes bases dos arquivos
    char nome_leit[10] = "leitura";
    char nome_escr1[20];   //são os nomes já alterados
    char nome_leit1[20];

    sprintf(nome_escr1, "%s_%d.txt", nome_escr, cont);//monta o nome dos arquivos de e/s
    sprintf(nome_leit1, "%s_%d.txt", nome_leit, cont);
    so->descri[0].es = es_cria(nome_leit1, nome_escr1);
    inicializa_processo(so, cont, vetor_programas[0].conteudo, cont);
}

void alimenta_descritor(so_t *so, int indice){
    char nome_escr[10] = "escrita";  //são os nomes bases dos arquivos
    char nome_leit[10] = "leitura";
    char nome_escr1[20];   //são os nomes já alterados
    char nome_leit1[20];

    sprintf(nome_escr1, "%s_%d.txt", nome_escr, indice);//monta o nome dos arquivos de e/s
    sprintf(nome_leit1, "%s_%d.txt", nome_leit, indice);
    for(int i=0; i<10; i++){
        if(so->descri[i].est == LIVRE){
            so->descri[i].es = es_cria(nome_leit1, nome_escr1);
            inicializa_processo(so, i, vetor_programas[indice].conteudo, indice);
            i=10;
        }
    }
}

void inicializa_processo(so_t *so, int i, int progr[], int indice){
    troca_estado(so, PRONTO, i);
    so->descri[i].mem = mem_cria(vetor_programas[indice].tam_mem);
    so->descri[i].id = id;
    id++;
    so->descri[i].temp->bloq = 0;
    so->descri[i].temp->pronto = 0;
    so->descri[i].temp->execucao= 0;
    so->descri[i].temp->quantos_prontos = 0;
    so->descri[i].temp->trocas_de_estado = 0;
    so->descri[i].temp->ultimo_relogio = 0;
    so->descri[i].temp->tempo_da_ultima_execucao = 0;
    so->descri[i].temp_bloq_e = vetor_programas[indice].tempoE;
    so->descri[i].temp_bloq_l = vetor_programas[indice].tempoL;
    for(int j = 0; j < vetor_programas[indice].tam_mem; j++){   //carrego o programa para o descritor
        if (mem_escreve(so->descri[i].mem, j, progr[j]) != ERR_OK) {
            printf("Erro de memoria, endereco %d\n", j);
            exit(1);
        }
    }
}


err_t verifica_interrupcao(err_t err, so_t *so, int processo){
    if (err == ERR_CPU_PRIV){
        if(so->cpu->mem->mem[so->cpu->estado.cp] == 20){ //ESCR
            so->cpu->estado.modo = es_escreve(so->descri[qual_processo].es, so->cpu->mem->mem[so->cpu->estado.pc+1], so->cpu->estado.a);
            if(so->cpu->estado.modo != ERR_OK){
                so->cpu->estado.cp = 1;
                troca_estado(so, MORTO, qual_processo);
            }
            so->cpu->estado.pc += 2;
            so->cpu->estado.modo = ERR_OK;
            troca_estado(so, BLOQUEADO, qual_processo);
            cria_despertadores(so->relo, que_horas_sao(so->relo)+so->descri[qual_processo].temp_bloq_e, 0, qual_processo, false);
            salva_contexto_processo(so);
        }else if(so->cpu->mem->mem[so->cpu->estado.cp] == 19){ //LER
            so->cpu->estado.modo = es_le(so->descri[qual_processo].es, so->cpu->mem->mem[so->cpu->estado.pc+1], &so->cpu->estado.a);
            if(so->cpu->estado.modo != ERR_OK){
                so->cpu->estado.cp = 0;
                troca_estado(so, MORTO, qual_processo);
            }
            so->cpu->estado.pc += 2;
            so->cpu->estado.modo = ERR_OK;
            troca_estado(so, BLOQUEADO, qual_processo);
            cria_despertadores(so->relo, que_horas_sao(so->relo)+so->descri[qual_processo].temp_bloq_l, 0, qual_processo, false);
            salva_contexto_processo(so);
        }else if(so->cpu->mem->mem[so->cpu->estado.cp] == 1){
            troca_estado(so, MORTO, qual_processo);
            salva_contexto_processo(so);
        }else if(so->cpu->mem->mem[so->cpu->estado.cp] == 21){
            printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%VALOR QUE ESTA NA MEMORIA  %d \n", so->descri->mem->mem[so->cpu->estado.pc+1]);
            alimenta_descritor(so, so->descri->mem->mem[so->cpu->estado.pc+1]);
            so->cpu->estado.pc += 2;
            so->cpu->estado.modo = ERR_OK;
            troca_estado(so, PRONTO, qual_processo);
            salva_contexto_processo(so);
        }
        int certo = escalonador(so);
        if(certo == -1){
            printf("Nao tem processo em estado de pronto \n");
            so->cpu->estado.modo = ERR_CPU_PARADA;
            return ERR_CPU_PARADA;
        }else if(certo == 0){
            printf("Nao foi possivel escalonar nem um processo\n");
            return err;
        }
        return ERR_OK;
    }else if (err == ERR_CPU_PARADA || err == ERR_OK){
        if(processo >= 0 && processo <10){
            troca_estado(so, PRONTO, processo);
        }
        troca_estado(so, PRONTO, qual_processo);
        salva_contexto_processo(so);
        so->cpu->estado.modo = ERR_OK;
        err = ERR_OK;
        int certo = escalonador(so);
        if(certo == -1){
            printf("Não tem processo em estado de pronto \n");
            so->cpu->estado.modo = ERR_CPU_PARADA;
            return ERR_CPU_PARADA;
        }else if(certo == 0){
            printf("Nao foi possivel escalonar nem um processo2\n");
            return err;
        }
    }
    return err;
}


int escalonador(so_t *so){
    int tam = 10;
    int verifica_se_tem_bloqueado = 0;
    int roda = qual_processo+1;
    int controle = 0;
    while(controle < tam){
        if(so->descri[roda].est == PRONTO){
            qual_processo = roda;
            recupera_contexto_processo(so);
            return 1;
        }else{
            if(roda >= 9){
                roda=0;
            }else{
                roda++;
            }
         controle++;
        }
        if(so->descri[roda].est == BLOQUEADO){
            verifica_se_tem_bloqueado++;
        }
    }
    if(verifica_se_tem_bloqueado > 0){ //siguinifca que nem todo os processos estao mortos
        return -1;                    //mas nao tem nem um em estaod de pronto.
    }
    return 0;
}

int escalonador_com_prioridade(so_t *so){
    int tam = 10;
    int verifica_se_tem_bloqueado = 0;
    int controle = 0;
    int mais_prioritario = 1000;
    for(int i=0; i<10; i++){
        if(so->descri[i].est == PRONTO){  //pegar o primeiro candidato se escalonado
            mais_prioritario = i;
            controle = i+1;
            i=10;
        }
    }
    while(controle < tam){
        if(so->descri[controle].est == PRONTO){
            if(so->descri[mais_prioritario].temp->tempo_da_ultima_execucao > so->descri[controle].temp->tempo_da_ultima_execucao){ //descobrir qual tem a prioridade maior
                mais_prioritario = controle;
            }
        }else if(so->descri[controle].est == BLOQUEADO){
            verifica_se_tem_bloqueado++;
        }
        controle++;
    }
    if(mais_prioritario >= 0 && mais_prioritario < 10){
        qual_processo = mais_prioritario;
        recupera_contexto_processo(so);
        return 1;
    }
    if(verifica_se_tem_bloqueado > 0){ //siguinifca que nem todo os processos estao mortos
        return -1;                    //mas nao tem nem um em estaod de pronto.
    }
    return 0;
}

void salva_contexto_processo(so_t *so){  //copia da cpu para o descritor de processos
    so->descri[qual_processo].es = so->cpu->es;
    so->descri[qual_processo].mem = so->cpu->mem;
    salva_no_descritor(so->descri[qual_processo].est_cpu ,so->cpu);
}

void salva_no_descritor(cpu_estado_t *estado, cpu_t *cpu){
    estado->pc = cpu->estado.pc;
    estado->a = cpu->estado.a;
    estado->x = cpu->estado.x;
    estado->modo = cpu->estado.modo;
    estado->cp = cpu->estado.cp;
}

void recupera_contexto_processo(so_t *so){  // copia do descritor do processo em execução para a CPU
    so->cpu->es = so->descri[qual_processo].es;
    so->cpu->mem = so->descri[qual_processo].mem;
    cpu_altera_estado(so->cpu, so->descri[qual_processo].est_cpu);
    troca_estado(so, EXECUCAO, qual_processo);
}


void cpu_altera_estado(cpu_t *cpu, cpu_estado_t *estado){
    cpu->estado.pc = estado->pc;
    cpu->estado.a = estado->a;
    cpu->estado.x = estado->x;
    cpu->estado.modo = estado->modo;
    cpu->estado.cp = estado->cp;
}

 void troca_estado(so_t *so, est_p novo_estado, int indice){ // troca o estado de 'proc' para 'estado_novo' (em_exec, pronto, bloqueado, morto)
     est_p estado_velho = so->descri[indice].est;

     int temp_agora = que_horas_sao(so->relo);

     int  tempo_no_estado = temp_agora - so->descri[indice].temp->ultimo_relogio;

     so->descri[indice].est = novo_estado;
     so->descri[indice].temp->ultimo_relogio = temp_agora;
     so->descri[indice].temp->trocas_de_estado++;

     if(tempo_no_estado > 0){
        if(estado_velho == PRONTO){
            so->descri[indice].temp->quantos_prontos++;
            so->descri[indice].temp->pronto += tempo_no_estado;
        }else if(estado_velho == BLOQUEADO){
            so->descri[indice].temp->bloq += tempo_no_estado;
        }else if(estado_velho == EXECUCAO){
            so->descri[indice].temp->tempo_da_ultima_execucao = tempo_no_estado;
            so->descri[indice].temp->execucao += tempo_no_estado;
        }
     }
 }


 void imprime_tempos(so_t *so){
     printf("Vai imprimir os tempos\n");
     for(int i=0; i<10 ; i++){
        if(so->descri[i].est != LIVRE){
            printf("\n");
            printf("PROCESSO %d \n", i);
            printf("PRONTO no processo %d\n", so->descri[i].temp->pronto);
            printf("QUANTOS VEZ O PROCESSO ESTEVE NO PRONTO % d \n", so->descri[i].temp->quantos_prontos);
            printf("BLOQUEADO no processo %d \n", so->descri[i].temp->bloq);
            printf("EXECUCAO no processo %d   \n", so->descri[i].temp->execucao);
            printf("QUANTAS TROCAS DE PROCESSO %d \n", so->descri[i].temp->trocas_de_estado);
            float media = so->descri[i].temp->pronto / so->descri[i].temp->quantos_prontos;
            printf("TEMPO DE RESPOSTA MEDIO %.2f \n", media);
            printf("TEMPO DE RETORNO %d \n", so->descri[i].temp->bloq + so->descri[i].temp->execucao + so->descri[i].temp->pronto);
        }
     }
     printf("Tempo total de relogio %d \n", que_horas_sao(so->relo));
 }
