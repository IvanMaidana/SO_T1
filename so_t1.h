typedef enum {
     ERR_OK = 0,
     ERR_MEM_END_INV,  // acesso a endereço de memória inválida
     ERR_ES_DISP_INV,  // acesso a dispositivo inexistente
     ERR_ES_OP_INV,    // operação inválida em dispositivo
     ERR_CPU_PARADA,    // CPU executou instrução PARA
     ERR_CPU_INSTR_INV, // tentativa de execução de instrução inexistente
} err_t;

//memoria
typedef int mem_t;

mem_t *mem_cria(int tam); //cria memoria
int mem_tam(mem_t *m);   // verifica o tamanho da memoria
err_t mem_le(mem_t *m, int endereco, int *pvalor); //le um valor da memoria
err_t mem_escreve(mem_t *m, int endereco, int valor);//escreve um valor da memoria
void mem_destroi(mem_t *m);  //libera memoria


//entrada e saida
typedef int es_t;

es_t *es_cria(); //cria o controlador de e/s
err_t es_le(es_t *es, int dispositivo, int *pvalor);//le um valor
err_t es_escreve(es_t *es, int dispositivo, int valor);//printa um valor no terminal


//cpu

typedef struct{
    int pc;  //contador de programas
    int a;     //acumulador
    int x;      //registrador auxiliar
    err_t modo;    // modo da cpu
    int cp;     // registrador complemento de parada
}cpu_estado_t;

typedef struct{
        cpu_estado_t estado; //representa o estado da cpu
        mem_t *mem;  // vetor que representa a memoria
        es_t es;    //controlador  de entrada e saida
}cpu_t;

cpu_t *cpu_cria(); //aloca a struct da cpu
cpu_estado_t *cpu_estado_cria(); //inicializa os valores do estado da cpu
cpu_estado_t *cpu_estado(cpu_t *cpu); //recebo a cpu e retorno o estado
void cpu_altera_estado(cpu_t *cpu, cpu_estado_t *estado); //altero o estado da cpu
void cpu_altera_memoria(cpu_t *cpu, mem_t *mem); //atribuo a memoria alocada a variavel que esta dentro da struct cpu
void cpu_altera_es(cpu_t *cpu, es_t *es); //atribuo a E/S alocada a variavel que esta dentro da struct cpu
err_t cpu_executa_1(cpu_t *cpu); //executa as instrucoes que estiverem na memoria
