typedef enum {
     ERR_OK = 0,
     ERR_MEM_END_INV,  // acesso a endere�o de mem�ria inv�lida
     ERR_ES_DISP_INV,  // acesso a dispositivo inexistente
     ERR_ES_OP_INV,    // opera��o inv�lida em dispositivo
     ERR_CPU_PARADA,    // CPU executou instru��o PARA
     ERR_CPU_INSTR_INV, // tentativa de execu��o de instru��o inexistente
} err_t;
