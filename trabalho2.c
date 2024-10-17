#include "estoque.h"

int main() {
    FILE* arq = fopen("entrada.txt", "r");
    FILE* arqExit = fopen("saida.txt", "w+");
    Arvore *ar;
    char funcao[50];
    char nome[20];
    int codigo;
    float valor;
    int data[3];

    ar = criaArvore();

    while (fscanf(arq, "%s", funcao) != EOF) {
        if (strcmp(funcao, "MEDICAMENTO") == 0) {
            fscanf(arq, "%s %d %f %d %d %d", nome, &codigo, &valor, &data[0], &data[1], &data[2]);
            Medicamento *novoMedicamento = criaMedicamento(nome, codigo, valor, data);
            ar = insereArvoreMedicamento(ar, novoMedicamento);
            fprintf(arqExit, "MEDICAMENTO %s %d ADICIONADO!\n", nome, codigo);
        } else if (strcmp(funcao, "RETIRA") == 0) {
            fscanf(arq, "%d", &codigo);
            ar = retiraArvoreMedicamento(ar, codigo);
            fprintf(arqExit, "MEDICAMENTO %d RETIRADO\n", codigo);
        } else if (strcmp(funcao, "IMPRIME_ARVORE") == 0) {
            imprimeArvore(arqExit, ar);
        } else if (strcmp(funcao, "ATUALIZA_PRECO") == 0) {
            fscanf(arq, "%d %f", &codigo, &valor);
            atualizaPrecoMedicamento(ar, codigo, valor);
            fprintf(arqExit, "PRECO ATUALIZADO %d %.1f\n", codigo, valor);
        } else if (strcmp(funcao, "VERIFICA_VALIDADE") == 0) {
            fscanf(arq, "%d %d %d", &data[0], &data[1], &data[2]);
            int resultado = VerificaArvoreValidade(arqExit, ar, data);
            if (resultado == 0) {
                fprintf(arqExit, "NENHUM MEDICAMENTO VENCIDO\n");
            }
        } else if (strcmp(funcao, "VERIFICA_ARVORE") == 0) {
            fscanf(arq, "%d", &codigo);
            int encontrado = verificaArvoreMedicamento(ar, codigo);
            if (encontrado) {
                fprintf(arqExit, "MEDICAMENTO %d ENCONTRADO\n", codigo);
            } else {
                fprintf(arqExit, "MEDICAMENTO %d NAO ENCONTRADO NA ARVORE\n", codigo);
            }
        }
    }

    fclose(arq);
    fclose(arqExit);
    liberaArvore(ar);

    return 0;
}
