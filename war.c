#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// --- Constante Globais---
#define NUM_TERRITORIOS 5
#define TAM_STRING 30

// --- Definição da Estrutura (Struct) ---
struct Territorio{
    char nome[TAM_STRING];
    char cor [TAM_STRING];
    int numeroTropas;

};
// --- Função para limpar o buffer de entrada---
void limparBufferEntrada(){
    int c;
    while (c = getchar() != '\n' && c != EOF);
};

// --- Função Principal ---
int main(){
    struct Territorio mapaMundi[NUM_TERRITORIOS];

    printf("--- WAR ESTRUTURA - CADASTRO INICIAL---\n\n");
    printf("Vamos cadastrando os %d territorio iniciais nosso mundo.\n", NUM_TERRITORIOS);
    printf("--------------------------------------------------\n");

// --- Laço de Repetição para Cadastrar os Territorios ---
for (int i = 0; i < NUM_TERRITORIOS; i++){
    printf("\n--- Cadastrando Territorio %d ---\n ",i + 1);

    printf("Nome do Territorio: ");
    fgets(mapaMundi[i].nome, TAM_STRING, stdin);

    printf("Cor do Exercito (ex: Azul, Verde): ");
    fgets(mapaMundi[i].cor, TAM_STRING, stdin);

    printf("Numeros de Tropas ");
    scanf("%d", &mapaMundi[i].numeroTropas);
    limparBufferEntrada();

// Remove o caractere '\n' que o fget captura no final das strings
    mapaMundi[i].nome[strcspn(mapaMundi[i].nome, "\n")] = '\0';
    mapaMundi[i].cor[strcspn(mapaMundi[i].cor, "\n")] = '\0';
        
    }
    printf("\nCadastro inicial concluido com sucesso!\n");
    printf("\n====================================\n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("====================================\n");

    // --- Laço de Repetição para Exibir os Territórios Cadastrados ---
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\nTERRITORIO %d:\n", i + 1);
        printf("- Nome: %s\n", mapaMundi[i].nome);
        printf("- Dominado por: Exercito %s\n", mapaMundi[i].cor);
        printf("- Tropas: %d\n", mapaMundi[i].numeroTropas);
    }
    
    printf("\n====================================\n");

    return 0; 

}