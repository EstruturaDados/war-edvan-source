#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Para malloc, free, rand, srand
#include <time.h>   // Para time()

// --- Constantes Globais---
#define NUM_TERRITORIOS 5
#define TAM_STRING 30

// --- Definição da Estrutura (Struct) ---
struct Territorio {
    char nome[TAM_STRING];
    char cor[TAM_STRING];
    int numeroTropas;
};

// --- Protótipos das Funções ---
void limparBufferEntrada();
void exibirMapa(struct Territorio *mapa, int numTerritorios);
void simularAtaque(struct Territorio *atacante, struct Territorio *defensor);

// --- Função Principal ---
int main() {
    // Agora, mapaMundi é um ponteiro que receberá o endereço da memória alocada.
    struct Territorio *mapaMundi;

    // Semente para a geração de números aleatórios.
    srand(time(NULL));

    // Alocação dinâmica de memória para o mapa.
    mapaMundi = (struct Territorio*) malloc(NUM_TERRITORIOS * sizeof(struct Territorio));

    // Verificação de segurança: checa se a memória foi alocada com sucesso.
    if (mapaMundi == NULL) {
        printf("Erro: Falha ao alocar memoria para o mapa.\n");
        return 1; // Retorna 1 para indicar um erro.
    }

    printf("--- WAR ESTRUTURADO - CADASTRO INICIAL ---\n\n");
    printf("Vamos cadastrar os %d territorios iniciais do nosso mundo.\n", NUM_TERRITORIOS);
    printf("--------------------------------------------------\n");

    // --- Laço de Repetição para Cadastrar os Territórios ---
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Cadastrando Territorio %d ---\n", i + 1);

        printf("Nome do Territorio: ");
        fgets(mapaMundi[i].nome, TAM_STRING, stdin);

        printf("Cor do Exercito: ");
        fgets(mapaMundi[i].cor, TAM_STRING, stdin);

        printf("Numero de Tropas: ");
        scanf("%d", &mapaMundi[i].numeroTropas);
        limparBufferEntrada();

        // Remove o caractere '\n' que o fgets captura no final das strings
        mapaMundi[i].nome[strcspn(mapaMundi[i].nome, "\n")] = '\0';
        mapaMundi[i].cor[strcspn(mapaMundi[i].cor, "\n")] = '\0';
    }

    printf("\nCadastro inicial concluido com sucesso!\n\n");
    
    int idAtacante, idDefensor;

    // --- Loop Principal do Jogo (Turnos) ---
    do {
        // Exibe o estado atual do mapa no início de cada turno.
        exibirMapa(mapaMundi, NUM_TERRITORIOS);

        printf("--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): ", NUM_TERRITORIOS);
        scanf("%d", &idAtacante);

        if (idAtacante == 0) {
            break; // Sai do loop se o usuário digitar 0.
        }

        printf("Escolha o territorio defensor (1 a %d): ", NUM_TERRITORIOS);
        scanf("%d", &idDefensor);
        limparBufferEntrada();

        // Validação das escolhas
        if (idAtacante < 1 || idAtacante > NUM_TERRITORIOS || idDefensor < 1 || idDefensor > NUM_TERRITORIOS) {
            printf("\nOpcao invalida! Territorio nao existe.\n");
        } else if (idAtacante == idDefensor) {
            printf("\nUm territorio nao pode atacar a si mesmo!\n");
        } else if (mapaMundi[idAtacante - 1].numeroTropas <= 1) {
            printf("\nNao e possivel atacar! O territorio precisa ter mais de 1 tropa para atacar.\n");
        } else {
            // Chama a função de simulação de ataque, passando os ponteiros para os territórios corretos.
            // Usamos "-1" para converter o ID (1-5) para o índice do array (0-4).
            simularAtaque(&mapaMundi[idAtacante - 1], &mapaMundi[idDefensor - 1]);
        }

        printf("\nPressione Enter para continuar para o proximo turno...");
        getchar();
        printf("\n\n");

    } while (1); // Loop infinito que só é quebrado com o 'break'.

    printf("\nJogo encerrado!\n");

    // Libera a memória alocada dinamicamente antes de encerrar o programa.
    free(mapaMundi);
    printf("Memoria do mapa liberada com sucesso.\n");

    return 0;
}

// --- Função para limpar o buffer de entrada ---
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Função para exibir o estado atual do mapa ---
void exibirMapa(struct Territorio *mapa, int numTerritorios) {
    printf("\n====================================\n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("====================================\n");

    for (int i = 0; i < numTerritorios; i++) {
        printf("%d. %s (Exercito %s, Tropas: %d)\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].numeroTropas);
    }
    printf("====================================\n\n");
}

// --- Função que simula a batalha entre dois territórios ---
void simularAtaque(struct Territorio *atacante, struct Territorio *defensor) {
    printf("\n--- RESULTADO DA BATALHA ---\n");
    
    // Simula o lançamento de um dado de 6 lados para cada um.
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoAtaque);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoDefesa);

    // Compara os resultados dos dados
    if (dadoAtaque > dadoDefesa) {
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
        defensor->numeroTropas--;

        // Verifica se o território foi conquistado
        if (defensor->numeroTropas == 0) {
            printf("CONQUISTA! O territorio %s foi dominado pelo Exercito %s!\n", defensor->nome, atacante->cor);
            strcpy(defensor->cor, atacante->cor); // Copia a cor do atacante para o defensor
            defensor->numeroTropas = 1; // O atacante move 1 tropa para o novo território
            atacante->numeroTropas--;
        }
    } else {
        printf("VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n");
        atacante->numeroTropas--;
    }
}