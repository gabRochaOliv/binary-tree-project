#include "estoque.h"


struct medicamento {
    char nome[20];
    int codigo;
    float valor;
    int data[3];
};


struct arvore {
    Medicamento *m;
    Arvore *esquerda;
    Arvore *direita;
};

// Função que cria um novo medicamento
Medicamento *criaMedicamento(char* nome, int codigo, float valor, int *data_de_validade) {
    //Declara um ponteiro *novo do tipo Medicamento
    Medicamento *novo = (Medicamento*)malloc(sizeof(Medicamento));
    strcpy(novo->nome, nome);  // Copia o nome para o medicamento
    novo->codigo = codigo;
    novo->valor = valor;
    for (int i = 0; i < 3; i++) {
        novo->data[i] = data_de_validade[i];
    }
    return novo;
}


Arvore *criaArvore() {
    return NULL;
}

// Função que insere um medicamento na árvore de forma ordenada pelo código
Arvore *insereArvoreMedicamento(Arvore *a, Medicamento *m) {
    if (a == NULL) {
        // Aloca memória para um novo nó da arvore
        Arvore *novo = (Arvore*)malloc(sizeof(Arvore));
        novo->m = m;
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    // Ordena os medicamentos com base no código: à esquerda, menores; à direita, maiores
    if (m->codigo < a->m->codigo) {
        a->esquerda = insereArvoreMedicamento(a->esquerda, m);  // Insere à esquerda
    } else {
        a->direita = insereArvoreMedicamento(a->direita, m);    // Insere à direita
    }
    return a;
}

// Função que remove um medicamento da árvore, dado seu código
Arvore *retiraArvoreMedicamento(Arvore *a, int id_medicamento) {
    if (a == NULL)
        return NULL;

    // Procura o medicamento na subárvore esquerda ou direita
    if (a->m->codigo > id_medicamento)
        a->esquerda = retiraArvoreMedicamento(a->esquerda, id_medicamento);
    else if (a->m->codigo < id_medicamento)
        a->direita = retiraArvoreMedicamento(a->direita, id_medicamento);
    else {
        // Caso o nó a ser removido tenha sido encontrado:
        if (a->esquerda == NULL && a->direita == NULL) {
            // Caso 1: Nó é uma folha, apenas libera o nó
            free(a);
            a = NULL;
        } else if (a->esquerda == NULL) {
            // Caso 2: O nó tem apenas filho à direita
            Arvore *t = a;
            a = a->direita;
            free(t);
        } else if (a->direita == NULL) {
            // Caso 3: O nó tem apenas filho à esquerda
            Arvore *t = a;
            a = a->esquerda;
            free(t);
        } else {
            // Caso 4: O nó tem dois filhos, encontra o maior nó na subárvore esquerda
            Arvore *f = a->esquerda;
            while (f->direita != NULL) {
                f = f->direita;
            }
            // Substitui o código e os dados do nó a ser removido pelos do maior nó da subárvore esquerda
            a->m->codigo = f->m->codigo;
            f->m->codigo = id_medicamento;
            a->esquerda = retiraArvoreMedicamento(a->esquerda, id_medicamento);
        }
    }
    return a;
}

// Função que verifica se um medicamento está na árvore dado seu código
int verificaArvoreMedicamento(Arvore *a, int id_medicamento) {
    if (a == NULL)
        return 0;

    else if (a->m->codigo > id_medicamento)
        return verificaArvoreMedicamento(a->esquerda, id_medicamento);  // Busca à esquerda
    else if (a->m->codigo < id_medicamento)
        return verificaArvoreMedicamento(a->direita, id_medicamento);   // Busca à direita
    else
        return 1;  // Medicamento encontrado
}

// Função que verifica se há medicamentos vencidos na árvore
int VerificaArvoreValidade(FILE* fp, Arvore *a, int *dataDeValidade) {
    int encontrado = 0;
    if (a != NULL) {
        // Verifica medicamentos na subárvore esquerda
        // OU bit a bit com atribuição.
        encontrado |= VerificaArvoreValidade(fp, a->esquerda, dataDeValidade);

        // Compara a data de validade do medicamento atual
        if (a->m->data[2] < dataDeValidade[2] ||
            (a->m->data[2] == dataDeValidade[2] && a->m->data[1] < dataDeValidade[1]) ||
            (a->m->data[2] == dataDeValidade[2] && a->m->data[1] == dataDeValidade[1] && a->m->data[0] < dataDeValidade[0])) {
            fprintf(fp, "MEDICAMENTO %s %d VENCIDO\n", a->m->nome, a->m->codigo);  // Medicamento vencido
            encontrado = 1;
        }

        // Verifica medicamentos na subárvore direita
        // combina valores e guarda o resultado
        encontrado |= VerificaArvoreValidade(fp, a->direita, dataDeValidade);
    }
    return encontrado;
}

// Função que imprime todos os medicamentos da árvore em ordem crescente de código
void imprimeArvore(FILE *fp, Arvore *a) {
    if (a != NULL) {
        imprimeArvore(fp, a->esquerda);  // Percorre subárvore esquerda
        fprintf(fp, "MEDICAMENTO %s %d R$%.2f %d %d %d\n", a->m->nome, a->m->codigo, a->m->valor, a->m->data[0], a->m->data[1], a->m->data[2]);
        imprimeArvore(fp, a->direita);  // Percorre subárvore direita
    }
}

// Função que atualiza o preço de um medicamento dado seu código
void atualizaPrecoMedicamento(Arvore *a, int codigo, float novo_valor) {
    if (a == NULL) return;

    if (a->m->codigo == codigo) {
        a->m->valor = novo_valor;  // Atualiza o preço do medicamento
    } else if (codigo < a->m->codigo) {
        atualizaPrecoMedicamento(a->esquerda, codigo, novo_valor);  // Busca à esquerda
    } else {
        atualizaPrecoMedicamento(a->direita, codigo, novo_valor);   // Busca à direita
    }
}

// Função que libera toda a memória alocada pela árvore e pelos medicamentos
void liberaArvore(Arvore *a) {
    if (a != NULL) {
        liberaArvore(a->esquerda);
        liberaArvore(a->direita);
        free(a->m);
        free(a);
    }
}
