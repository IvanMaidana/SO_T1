#ifndef SO_H
#define SO_H

#include"unidadeExecucao.h"
#include"relogio.h"

typedef struct so_t so_t; //defini tudoq ue tem dentro na struct do SO

typedef struct descri_proces descri_proces; //defini o que tem dentro da struct do processo
typedef struct tempo_pro tempo_pro; //guarda os tempos em cada estado do processo


so_t *so_cria();
void so_inicializa(so_t *so, cpu_t *cpu, rel *relo);
descri_proces *cria_descritor(int tam); //o tam define o tamanho do descritor
tempo_pro *temp_cria();
void inicializa_descritor(descri_proces *des, int tam);
void inicializa_processo(descri_proces *des, int i, int progr[]);

err_t verifica_interrupcao(err_t err, so_t *so);
int escalonador(so_t *so);

void salva_contexto_processo(so_t *so);  //copia o estado do processo para o descritor do processo
void salva_no_descritor(cpu_estado_t *estado, cpu_t *cpu);

void recupera_contexto_processo(so_t *so);  // copia do descritor de processos em execucao para a CPU
void cpu_altera_estado(cpu_t *cpu, cpu_estado_t *estado);

#endif // SO_H_INCLUDED
