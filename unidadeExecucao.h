#ifndef CPU_H
#define CPU_H


#include"err.h"
#include"memoria.h"
#include"entradaSaida.h"

// cpu

typedef  struct {
    int pc;  // contador de programas
    int a;     // acumulador
    int x;      // registrador auxiliar
    err_t modo;    // modo da cpu
    int cp;     // registrador complemento de parada
} cpu_estado_t ;

typedef  struct {
        cpu_estado_t estado; // representa o estado da cpu
        mem_t * mem;  // vetor que representa uma memória
        es_t * es;    // driver de entrada e saida
} cpu_t ;

cpu_t * cpu_cria (); // aloca a struct da cpu
cpu_estado_t * cpu_estado_cria (); // inicializa os valores do estado da cpu
cpu_estado_t * cpu_estado ( cpu_t * cpu); // recebo a cpu e retorno o estado

void  cpu_altera_memoria ( cpu_t * cpu, mem_t * mem); // atribuo a memoria alocada a variavel que esta dentro da struct cpu
void  cpu_altera_es ( cpu_t * cpu, es_t * es); // atribuo a E / S alocada a variavel que esta dentro da struct cpu

err_t  cpu_executa_1 ( cpu_t * cpu); // executa como instrucoes que pagam na memoria

#endif // CPU_H
