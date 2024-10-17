#ifndef ESTOQUE_H_INCLUDED
#define ESTOQUE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct medicamento Medicamento;
typedef struct arvore Arvore;

Medicamento *criaMedicamento(char* nome, int codigo, float valor, int *data_de_validade);
Arvore *criaArvore();
Arvore *insereArvoreMedicamento(Arvore *a, Medicamento *m);
Arvore *retiraArvoreMedicamento(Arvore *a, int id_medicamento);
int verificaArvoreMedicamento(Arvore *a, int id_medicamento);
int verificaArvoreValidade(FILE *fp, Arvore *a, int *data);
void imprimeArvoreMedicamentos(Arvore *a);
void atualizaPrecoMedicamento(Arvore *a, int id_medicamento, float value);
void liberaArvore(Arvore *a);

#endif // ESTOQUE_H_INCLUDED
