#ifndef UC_H
#define UC_H

#include"SO.h"
#include"relogio.h"
#include "unidadeExecucao.h"

typedef struct uc_t uc_t;

uc_t *uc_cria(void);
void uc_altera_cpu(uc_t *uc, cpu_t *cpu);
void uc_altera_so(uc_t *uc, so_t *so);
void uc_altera_relo(uc_t *uc, rel *relo);
err_t laco_controlador(uc_t *uc);


#endif // UC_H_INCLUDED
