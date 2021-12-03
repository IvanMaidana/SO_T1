#ifndef ERR_H
#define ERR_H

typedef enum {
     ERR_OK = 0,
     ERR_MEM_END_INV,  // acesso a endere�o de mem�ria inv�lida
     ERR_ES_DISP_INV,  // acesso a dispositivo inexistente
     ERR_ES_OP_INV,    // opera��o inv�lida em dispositivo
     ERR_CPU_PARADA,    // CPU executou instru��o PARA
     ERR_CPU_INSTR_INV, // tentativa de execu��o de instru��o inexistente
     ERR_CPU_PRIV
} err_t;

typedef enum {
    PRONTO = 0, //ESPERANDO A CPU
    EXECUCAO,  //ESTA EM EXECUACAO NA CPU
    BLOQUEADO, //T3
    MORTO,    //O PROCESSO NAO PODE SER EXECUATADO
    LIVRE, //REPRESENTA QUE A POSSICAO DO VETOR ESTA LIVRE
} est_p;

#endif // ERR_H
