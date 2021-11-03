#include<stdio.h>
#include <stdlib.h>

#include "so_t1.h"


cpu_estado_t *cpu_estado_cria(){
    cpu_estado_t *estado = (cpu_estado_t*)malloc(sizeof(cpu_estado_t));
    estado->pc = 0;
    estado->a = 0;
    estado->x = 0;
    estado->modo = 0;
    estado->cp = 0;
    return estado;
}

cpu_t *cpu_cria(){
    cpu_t *cpu = (cpu_t*)malloc(sizeof(cpu_t));
    return cpu;
}

cpu_estado_t *cpu_estado(cpu_t *cpu){
   // return cpu->estado;
}

void cpu_altera_estado(cpu_t *cpu, cpu_estado_t *estado){
    cpu->estado.pc = estado->pc;
    cpu->estado.a = estado->a;
    cpu->estado.x = estado->x;
    cpu->estado.modo = estado->modo;
    cpu->estado.cp = estado->cp;
}

void cpu_altera_memoria(cpu_t *cpu, mem_t *mem){
    cpu->mem = mem;
}

void cpu_altera_es(cpu_t *cpu, es_t *es){
    cpu->es = es;
}

err_t cpu_executa_1(cpu_t *cpu){
    printf("intrucao que vai ser executada %d\n", cpu->mem[cpu->estado.pc]);
    int a1;
    switch(cpu->mem[cpu->estado.pc]){
        case 0:
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 1:
            cpu->estado.modo = ERR_CPU_PARADA;
            cpu->estado.cp = cpu->estado.pc; //acerta o valor de cp
            return cpu->estado.modo;
        break;

        case 2://A=A1
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->estado.pc;
                return cpu->estado.modo;
            }
            cpu->estado.a = a1;
            cpu->estado.pc++;
            return cpu->estado.modo;

        break;

        case 3://A=mem[A1]
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
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 4://A=mem[A1+X]
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
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 5://mem[A1]=A
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
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 6:// mem[A1+X]=A
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
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 7://X=A
            cpu->estado.x = cpu->estado.a;
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 8://A=X
            cpu->estado.a = cpu->estado.x;
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 9://X++
            cpu->estado.x++;
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 10://A+=mem[A1]
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
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 11://A-=mem[A1]
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
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 12://A*=mem[A1]
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
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 13:// A/=mem[A1]
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
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 14://A%=mem[A1]
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
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 15://A=-A
            cpu->estado.a = cpu->estado.a - cpu->estado.a * 2;
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 16://PC=A1
            cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
            if(cpu->estado.modo != ERR_OK){      //verifica se deu algo de errado na funcao me_le
                cpu->estado.cp = cpu->estado.pc;//se tiver erro coloco o enderrco no cp(complemento de parada
                return cpu->estado.modo;
            }
            cpu->estado.pc = a1;
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 17://se A for 0, PC=A1
            if(cpu->estado.a == 0){
                cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);//pega o valor A1
                if(cpu->estado.modo != ERR_OK){      //verifica se deu algo de errado na funcao me_le
                    cpu->estado.cp = cpu->estado.pc;
                    return cpu->estado.modo;
                }
            }
            cpu->estado.a = a1;
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 18://se A n�o for 0, PC=A1
            if(cpu->estado.a != 0){
                cpu->estado.modo = mem_le(cpu->mem, cpu->estado.pc+1, &a1);
                if(cpu->estado.modo != ERR_OK){
                    cpu->estado.cp = cpu->estado.pc;
                    return cpu->estado.modo;
                }
            }
            cpu->estado.a = a1;
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 19://A=es[A1]
            cpu->es = cpu->mem[cpu->estado.pc+1];
            cpu->estado.modo = es_le(cpu->es, 0, &cpu->estado.a);
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->es;
                return cpu->estado.modo;
            }
            cpu->estado.pc++;
            return cpu->estado.modo;
        break;

        case 20://es[A1]=A
            cpu->es = cpu->mem[cpu->estado.pc+1];
            cpu->estado.modo = es_escreve(cpu->es, 1, cpu->estado.a);
            if(cpu->estado.modo != ERR_OK){
                cpu->estado.cp = cpu->es;
                return cpu->estado.modo;
            }
            cpu->estado.pc++;
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
