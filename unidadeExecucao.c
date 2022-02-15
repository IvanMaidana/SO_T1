#include "unidadeExecucao.h"
#include<stdlib.h>
#include<stdio.h>


cpu_estado_t *cpu_estado_cria(){
    cpu_estado_t *estado = (cpu_estado_t*)malloc(sizeof(cpu_estado_t));
    if(estado == NULL){
        printf("Nao foi possivel alocar memoria para o estado da cpu ou do processo\n");
        exit(1);
    }
    estado->pc = 0;
    estado->a = 0;
    estado->x = 0;
    estado->modo = ERR_OK;
    estado->cp = 0;
    return estado;
}

cpu_t *cpu_cria(){
    cpu_t *cpu = (cpu_t*)malloc(sizeof(cpu_t));
    return cpu;
}

cpu_estado_t *cpu_estado(cpu_t *cpu){
    return &cpu->estado;
}

void cpu_altera_memoria(cpu_t *cpu, mem_t *mem){
    cpu->mem = mem;
}

void cpu_altera_es(cpu_t *cpu, es_t *es){
    cpu->es = es;
}

err_t cpu_executa_1(cpu_t *cpu){
    printf("ESTA E AINSTRUCAO QUE STA SENDO EXECUTADA %d \n", cpu->mem->mem[cpu->estado.pc]);
    int a1;
    switch(cpu->mem->mem[cpu->estado.pc]){
        case 0:   // NOP
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 1:   //PARA
            cpu->estado.cp = cpu->estado.pc;
            return ERR_CPU_PRIV;
        break;

        case 2://A=A1   CARGI
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            cpu->estado.a = a1;
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 3://A=mem[A1]    CARGM
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            cpu->estado.modo = mem_le(cpu->mem, a1, &a1);  //pego o valor de a e atribuo a mem[a1+x]
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }else{
                cpu->estado.a = a1;
            }
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 4://A=mem[A1+X]   CARGX
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            int a2 = a1;
            a2 = a2 + cpu->estado.x;
            cpu->estado.modo = mem_le(cpu->mem, a2, &a1);  //pego o valor de a e atribuo a mem[a1+x]
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }else{
                cpu->estado.a = a1;
            }
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 5://mem[A1]=A   ARMM
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            cpu->estado.modo = mem_escreve(cpu->mem, a1, cpu->estado.a);  //pego o valor de a e atribuo a mem[a1+x]
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 6:// mem[A1+X]=A    ARMX
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            a1 += cpu->estado.x;
            cpu->estado.modo = mem_escreve(cpu->mem, a1, cpu->estado.a);  //pego o valor de a e atribuo a mem[a1+x]
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 7://X=A    MVAX
            cpu->estado.x = cpu->estado.a;
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 8://A=X    MVXA
            cpu->estado.a = cpu->estado.x;
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 9://X++   INCX
            cpu->estado.x++;
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 10://A+=mem[A1]     SOMA
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            cpu->estado.modo = mem_le(cpu->mem, a1, &a1);  //pego o valor de mem[a1]
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }else{
                cpu->estado.a += a1;               //faco a operacao
            }
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 11://A-=mem[A1]   SUB
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            cpu->estado.modo = mem_le(cpu->mem, a1, &a1);  //pego o valor de mem[a1]
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }else{
                int a2 = a1;
                cpu->estado.a -= a2;               //faco a operacao
            }
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 12://A*=mem[A1]   MULT
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            cpu->estado.modo = mem_le(cpu->mem, a1, &a1);  //pego o valor de mem[a1]
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }else{
                int a2 = a1;
                cpu->estado.a *= a2;               //faco a operacao
            }
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 13:// A/=mem[A1]    DIV
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            cpu->estado.modo = mem_le(cpu->mem, a1, &a1);  //pego o valor de mem[a1]
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }else{
                int a2 = a1;
                cpu->estado.a /= a2;               //faco a operacao
            }
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 14://A%=mem[A1]   RESTO
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            cpu->estado.modo = mem_le(cpu->mem, a1, &a1);  //pego o valor de mem[a1]
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }else{
                int a2 = a1;
                cpu->estado.a %= a2;               //faco a operacao
            }
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 15://A=-A   NEG
            cpu->estado.a = cpu->estado.a - cpu->estado.a * 2;
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 16://PC=A1    DESV
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){      //verifica se deu algo de errado na funcao me_le
                cpu->estado.cp = cpu->estado.pc;//se tiver erro coloco o enderrco no cp(complemento de parada
                return cpu->estado.modo;
            }
            cpu->estado.pc = a1;
            return cpu->estado.modo;
        break;

        case 17://se A for 0, PC=A1      DESVZ
            if(cpu->estado.a == 0){
                cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
                if(cpu->estado.modo != ERR_OK){      //verifica se deu algo de errado na funcao me_le
                    cpu->estado.cp = cpu->estado.pc;
                    return cpu->estado.modo;
                }else{
                    cpu->estado.pc = a1;
                    return cpu->estado.modo;
                }
            }
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 18://se A não for 0, PC=A1      DESVNZ
            if(cpu->estado.a != 0){
                cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);
                if(cpu->estado.modo != ERR_OK){
                    cpu->estado.cp = cpu->estado.pc;
                    return cpu->estado.modo;
                }else{
                    cpu->estado.pc = a1;
                    return cpu->estado.modo;
                }
            }
            cpu->estado.pc+=2;
            return cpu->estado.modo;
        break;

        case 19://A=es[A1]    LE
            cpu->estado.cp = cpu->estado.pc;
            cpu->estado.modo = ERR_CPU_PRIV;
            return cpu->estado.modo;
        break;

        case 20://es[A1]=A     ESCR
            cpu->estado.cp = cpu->estado.pc;
            cpu->estado.modo = ERR_CPU_PRIV;
            return cpu->estado.modo;
        break;

        case 21: //cria processo
            cpu->estado.cp = cpu->estado.pc;
            cpu->estado.modo = ERR_CPU_PRIV;
            return cpu->estado.modo;
        break;

        default:
            cpu->estado.modo = ERR_CPU_INSTR_INV;
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
            }
            return cpu->estado.modo;
    }
}

