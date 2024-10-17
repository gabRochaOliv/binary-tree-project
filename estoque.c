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

// Fun��o que cria um novo medicamento
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

// Fun��o que insere um medicamento na �rvore de forma ordenada pelo c�digo
Arvore *insereArvoreMedicamento(Arvore *a, Medicamento *m) {
    if (a == NULL) {
        // Aloca mem�ria para um novo n� da arvore
        Arvore *novo = (Arvore*)malloc(sizeof(Arvore));
        novo->m = m;
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    // Ordena os medicamentos com base no c�digo: � esquerda, menores; � direita, maiores
    if (m->codigo < a->m->codigo) {
        a->esquerda = insereArvoreMedicamento(a->esquerda, m);  // Insere � esquerda
    } else {
        a->direita = insereArvoreMedicamento(a->direita, m);    // Insere � direita
    }
    return a;
}

// Fun��o que remove um medicamento da �rvore, dado seu c�digo
Arvore *retiraArvoreMedicamento(Arvore *a, int id_medicamento) {
    if (a == NULL)
        return NULL;

    // Procura o medicamento na sub�rvore esquerda ou direita
    if (a->m->codigo > id_medicamento)
        a->esquerda = retiraArvoreMedicamento(a->esquerda, id_medicamento);
    else if (a->m->codigo < id_medicamento)
        a->direita = retiraArvoreMedicamento(a->direita, id_medicamento);
    else {
        // Caso o n� a ser removido tenha sido encontrado:
        if (a->esquerda == NULL && a->direita == NULL) {
            // Caso 1: N� � uma folha, apenas libera o n�
            free(a);
            a = NULL;
        } else if (a->esquerda == NULL) {
            // Caso 2: O n� tem apenas filho � direita
            Arvore *t = a;
            a = a->direita;
            free(t);
        } else if (a->direita == NULL) {
            // Caso 3: O n� tem apenas filho � esquerda
            Arvore *t = a;
            a = a->esquerda;
            free(t);
        } else {
            // Caso 4: O n� tem dois filhos, encontra o maior n� na sub�rvore esquerda
            Arvore *f = a->esquerda;
            while (f->direita != NULL) {
                f = f->direita;
            }
            // Substitui o c�digo e os dados do n� a ser removido pelos do maior n� da sub�rvore esquerda
            a->m->codigo = f->m->codigo;
            f->m->codigo = id_medicamento;
            a->esquerda = retiraArvoreMedicamento(a->esquerda, id_medicamento);
        }
    }
    return a;
}

// Fun��o que verifica se um medicamento est� na �rvore dado seu c�digo
int verificaArvoreMedicamento(Arvore *a, int id_medicamento) {
    if (a == NULL)
        return 0;

    else if (a->m->codigo > id_medicamento)
        return verificaArvoreMedicamento(a->esquerda, id_medicamento);  // Busca � esquerda
    else if (a->m->codigo < id_medicamento)
        return verificaArvoreMedicamento(a->direita, id_medicamento);   // Busca � direita
    else
        return 1;  // Medicamento encontrado
}

// Fun��o que verifica se h� medicamentos vencidos na �rvore
int VerificaArvoreValidade(FILE* fp, Arvore *a, int *dataDeValidade) {
    int encontrado = 0;
    if (a != NULL) {
        // Verifica medicamentos na sub�rvore esquerda
        // OU bit a bit com atribui��o.
        encontrado |= VerificaArvoreValidade(fp, a->esquerda, dataDeValidade);

        // Compara a data de validade do medicamento atual
        if (a->m->data[2] < dataDeValidade[2] ||
            (a->m->data[2] == dataDeValidade[2] && a->m->data[1] < dataDeValidade[1]) ||
            (a->m->data[2] == dataDeValidade[2] && a->m->data[1] == dataDeValidade[1] && a->m->data[0] < dataDeValidade[0])) {
            fprintf(fp, "MEDICAMENTO %s %d VENCIDO\n", a->m->nome, a->m->codigo);  // Medicamento vencido
            encontrado = 1;
        }

        // Verifica medicamentos na sub�rvore direita
        // combina valores e guarda o resultado
        encontrado |= VerificaArvoreValidade(fp, a->direita, dataDeValidade);
    }
    return encontrado;
}

// Fun��o que imprime todos os medicamentos da �rvore em ordem crescente de c�digo
void imprimeArvore(FILE *fp, Arvore *a) {
    if (a != NULL) {
        imprimeArvore(fp, a->esquerda);  // Percorre sub�rvore esquerda
        fprintf(fp, "MEDICAMENTO %s %d R$%.2f %d %d %d\n", a->m->nome, a->m->codigo, a->m->valor, a->m->data[0], a->m->data[1], a->m->data[2]);
        imprimeArvore(fp, a->direita);  // Percorre sub�rvore direita
    }
}

// Fun��o que atualiza o pre�o de um medicamento dado seu c�digo
void atualizaPrecoMedicamento(Arvore *a, int codigo, float novo_valor) {
    if (a == NULL) return;

    if (a->m->codigo == codigo) {
        a->m->valor = novo_valor;  // Atualiza o pre�o do medicamento
    } else if (codigo < a->m->codigo) {
        atualizaPrecoMedicamento(a->esquerda, codigo, novo_valor);  // Busca � esquerda
    } else {
        atualizaPrecoMedicamento(a->direita, codigo, novo_valor);   // Busca � direita
    }
}

// Fun��o que libera toda a mem�ria alocada pela �rvore e pelos medicamentos
void liberaArvore(Arvore *a) {
    if (a != NULL) {
        liberaArvore(a->esquerda);
        liberaArvore(a->direita);
        free(a->m);
        free(a);
    }
}
