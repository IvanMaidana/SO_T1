#ifndef RELOGIO
#define RELOGIO

typedef struct rel rel;
typedef struct desp desp;

rel *cria_rel(void);
void passou_tempo(rel *relo);    //incrementa o valor do relogio
int que_horas_sao(rel *relo);    //retorna o valor do relogio
void imprime_relogio(rel *relo); //imprime o valro do relogio
void inic_desper(rel *relo);     //inicializa os valores do despertador
void cria_despertadores(rel *relo, int inicio, int periodo); //cria o despertador periodico
int tem_despertador(rel *relo);  //verifica se possui algum despertador

#endif // RELOGIO_H_INCLUDED
