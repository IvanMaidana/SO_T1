#include"SO.h"
#include<stdlib.h>
#include<stdio.h>

#define TAM 20 // o tamanho da memória, tambem o tamanho do vetor descritor de processos
int qual_processo=0;
int id=0;

// programa para executar na nossa CPU
int progr1[TAM] = { 2, 0, 7, 2, 10, 5, 17,    //  0      x=0; l=10
                    8, 20, 1,                 //  7 ali: print x
                    9, 8, 11, 17, 18, 7,      // 10      if ++x != l goto ali
                    1,                        // 16      FIM
                    0                         // 17 aqui tá o "l"
                  };

int progr2[TAM] = { 2, 0, 7, 2, 10, 5, 17,    //  0      x=0; l=10
                    8, 20, 1,                 //  7 ali: print x
                    9, 8, 11, 17, 18, 7,      // 10      if ++x != l goto ali
                    1,                        // 16      FIM
                    0                         // 17 aqui tá o "l"
                  };

struct so_t{
     cpu_t *cpu;
     rel *relo;
     descri_proces *descri;
};


struct tempo_pro{ // tempo do processo em cada estado   tempo de CPU, tempo de bloqueio, tempo em espera
    int temp_cpu; //tempo de cpu
    int bloq;     //tempo de bloqueado
    int espera;    //tempo de espera
};

struct descri_proces{ //descricao de processos
    cpu_estado_t *est_cpu; //estado do processo
    mem_t *mem;           // memoria do processo
    es_t *es;             //entrda e saida
    est_p est;            // enum que controla o estado do processo
    int id;               //identificacao de cda processo
    tempo_pro *temp;       //tempos em cada estado do processo
    int ultimo_rel;
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
    //es_t *es = es_cria();      // cria a e/s
    //cpu_altera_es(so->cpu, es);
    mem_t *mem = mem_cria(TAM);   //cria memoria da cpu
    cpu_altera_memoria(so->cpu, mem);
    cpu_estado_t *est = cpu_estado_cria();   //cria estado da cpu
    cpu_altera_estado(so->cpu, est);
    cria_despertadores(so->relo, 2, 4);

    //cria o descritor de processos e inicializa com o codigo de cada processo
    so->descri = cria_descritor(10); //cria o vetor descritor de processos
    inicializa_descritor(so->descri, 10);          //Inicializa o descritor com os processos
    recupera_contexto_processo(so);     //coloca o primeiro processo para executar
}


descri_proces *cria_descritor(int tam){ //cria o descritor

    descri_proces *des = (descri_proces*)malloc(tam * sizeof(descri_proces));//cria o vetor de processos
    if(des == NULL){
        printf("Nao foi possivel alocar espaco na memoria par ao descritor \n");
        exit(1);
    }
    for(int i=0; i<tam; i++){
        des[i].est = LIVRE;
        des[i].est_cpu = cpu_estado_cria();
        des[i].mem = mem_cria(TAM);
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


void inicializa_descritor(descri_proces *des, int tam){
    int cont=0; //ele controla o while
    char nome_escr[10] = "escrita";  //são os nomes bases dos arquivos
    char nome_leit[10] = "leitura";
    char nome_escr1[20];   //são os nomes já alterados
    char nome_leit1[20];
    while(tam > cont){
        switch(cont){
        case 0:
            sprintf(nome_escr1, "%s_%d.txt", nome_escr, cont);//monta o nome dos arquivos de e/s
            sprintf(nome_leit1, "%s_%d.txt", nome_leit, cont);
            des[0].es = es_cria(nome_leit1, nome_escr1);
            inicializa_processo(des, cont, progr1);
            cont++;
            break;
        case 1:
            sprintf(nome_escr1, "%s_%d.txt", nome_escr, cont);
            sprintf(nome_leit1, "%s_%d.txt", nome_leit, cont);
            des[1].es = es_cria(nome_leit1, nome_escr1);
            inicializa_processo(des, cont, progr2);
            cont++;
            break;
        case 2:
            cont++;
            break;
        case 3:
            cont++;
            break;
        case 4:
            cont++;
            break;
        case 5:
            cont++;
            break;
        case 6:
            cont++;
            break;
        case 7:
            cont++;
            break;
        case 8:
            cont++;
            break;
        case 9:
            cont++;
            break;
        default:
            printf("Não tem mais espaço na memoria para processos\n");

        }
    }
}

void inicializa_processo(descri_proces *des, int i, int progr[]){
    des[i].est = PRONTO;
    des[i].est_cpu = cpu_estado_cria();
    des[i].id = id;
    id++;
    des[i].temp->bloq = 0;
    des[i].temp->espera = 0;
    des[i].temp->temp_cpu = 0;
    des[i].ultimo_rel = 0;
    des[i].mem = mem_cria(TAM);
    for(int j = 0; j < TAM; j++){
        if (mem_escreve(des[i].mem, j, progr[j]) != ERR_OK) {
            printf("Erro de memoria, endereco %d\n", j);
            exit(1);
        }
    }
}


err_t verifica_interrupcao(err_t err, so_t *so){
    if (err == ERR_CPU_PRIV){
        if(so->cpu->mem->mem[so->cpu->estado.cp] == 20){ //ESCR
            so->cpu->estado.modo = es_escreve(so->descri[qual_processo].es, so->cpu->mem->mem[so->cpu->estado.pc+1], so->cpu->estado.a);
            if(so->cpu->estado.modo != ERR_OK){
                so->cpu->estado.cp = 1;
                so->descri[qual_processo].est = MORTO;
            }
            so->cpu->estado.pc += 2;
            so->cpu->estado.modo = ERR_OK;
            so->descri[qual_processo].est = PRONTO;
            salva_contexto_processo(so);
        }else if(so->cpu->mem->mem[so->cpu->estado.cp] == 19){
            so->cpu->estado.modo = es_le(so->descri[qual_processo].es, so->cpu->mem->mem[so->cpu->estado.pc+1], &so->cpu->estado.a);
            if(so->cpu->estado.modo != ERR_OK){
                so->cpu->estado.cp = 0;
                so->descri[qual_processo].est = MORTO;
            }
            so->cpu->estado.pc += 2;
            so->cpu->estado.modo = ERR_OK;
            so->descri[qual_processo].est = PRONTO;
            salva_contexto_processo(so);
        }else if(so->cpu->mem->mem[so->cpu->estado.cp] == 1){
            so->descri[qual_processo].est = MORTO;
            salva_contexto_processo(so);
        }
        int certo = escalonador(so);
        if(certo == 0){
            printf("Nao foi possivel escalonar nem um processo\n");
            return err;
        }
        return ERR_OK;
    }else if (err == ERR_OK){
        so->descri[qual_processo].est = PRONTO;
        salva_contexto_processo(so);
        int certo = escalonador(so);
        if(certo == 0){
            printf("Nao foi possivel escalonar nem um processo2\n");
            return err;
        }
    }
    return err;
}


int escalonador(so_t *so){
    int tam = 10;
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
    }
    return 0;
}

void salva_contexto_processo(so_t *so){  //copia da cpu para o descritor de processos
    so->descri[qual_processo].es = so->cpu->es;
    so->descri[qual_processo].mem = so->cpu->mem;
    salva_no_descritor(so->descri[qual_processo].est_cpu ,so->cpu);
    so->descri[qual_processo].temp->temp_cpu += que_horas_sao(so->relo) - so->descri[qual_processo].ultimo_rel;//contagem de tempo processo em execucao
    so->descri[qual_processo].ultimo_rel = que_horas_sao(so->relo);
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
    so->descri[qual_processo].est = EXECUCAO; //muda o estado do porcessador;
    so->descri[qual_processo].temp->espera += que_horas_sao(so->relo) - so->descri[qual_processo].ultimo_rel;//contagem de tempo processo em espera
    so->descri[qual_processo].ultimo_rel = que_horas_sao(so->relo);
}


void cpu_altera_estado(cpu_t *cpu, cpu_estado_t *estado){
    cpu->estado.pc = estado->pc;
    cpu->estado.a = estado->a;
    cpu->estado.x = estado->x;
    cpu->estado.modo = estado->modo;
    cpu->estado.cp = estado->cp;
}
