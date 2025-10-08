// --- BIBLIOTECAS NECESSÁRIAS ---
#include <stdio.h>      // Para funções de entrada e saída, como printf e scanf
#include <stdlib.h>     // Para alocação de memória (malloc), números aleatórios (rand, srand) e exit
#include <string.h>     // Para manipulação de strings, como strcpy e strcmp
#include <time.h>       // Para inicializar a semente de números aleatórios com o tempo atual (time)

// --- DEFINIÇÕES GLOBAIS ---
// Usamos #define para criar constantes de tempo de compilação.
// Isso é necessário para definir o tamanho de vetores que são inicializados na declaração.
#define TOTAL_TERRITORIOS 5
#define TOTAL_MISSOES 5

// --- ESTRUTURAS DE DADOS ---
// A struct 'Territorio' agrupa todas as informações de uma única área do mapa.
typedef struct {
    char nome[30];  // Nome do território
    char cor[10];   // Cor do jogador que domina o território
    int tropas;     // Quantidade de exércitos no território
} Territorio;

// --- PROTÓTIPOS DAS FUNÇÕES ---
// Declarar as funções aqui permite que a 'main' as chame antes de suas implementações.
void inicializarTerritorios(Territorio* mapa, int tamanho);
void exibirMapa(const Territorio* mapa, int tamanho);
void atribuirMissao(char** destino, const char* missoes[], int totalMissoes);
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho, const char* corJogador);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa, char* missaoJogador);
void exibirMissao(const char* missao);

// --- FUNÇÃO PRINCIPAL ---
// O ponto de entrada do programa.
int main() {
    // Inicializa a semente para a geração de números aleatórios.
    // Fazer isso apenas uma vez garante que os números (ex: dados) sejam diferentes a cada execução.
    srand(time(NULL));

    // Constante que define a cor do jogador principal.
    const char* COR_JOGADOR = "Azul";

    // Vetor de ponteiros para strings (char*). Armazena as missões disponíveis no jogo.
    const char* missoes_disponiveis[TOTAL_MISSOES] = {
        "Conquistar 3 territorios",
        "Destruir o exercito Verde",
        "Conquistar 2 territorios na mesma rodada",
        "Eliminar todas as tropas da cor Vermelha",
        "Manter 5 tropas em 2 territorios diferentes"
    };

    // --- ALOCAÇÃO DINÂMICA DE MEMÓRIA ---
    // Aloca memória no "heap" para o vetor de territórios. O tamanho é calculado em tempo de execução.
    Territorio* mapa = (Territorio*) malloc(TOTAL_TERRITORIOS * sizeof(Territorio));
    // O ponteiro para a missão do jogador começa como NULL. A memória será alocada na função atribuirMissao.
    char* missaoJogador1 = NULL;

    // É fundamental verificar se a alocação de memória foi bem-sucedida.
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa!\n");
        return 1; // Retorna 1 para indicar um erro.
    }

    // --- INICIALIZAÇÃO DO JOGO ---
    inicializarTerritorios(mapa, TOTAL_TERRITORIOS);
    atribuirMissao(&missaoJogador1, missoes_disponiveis, TOTAL_MISSOES);

    // Exibe a mensagem de boas-vindas e a missão sorteada para o jogador.
    printf("================ BEM-VINDO AO WAR ESTRUTURADO ================\n");
    exibirMissao(missaoJogador1);
    printf("==============================================================\n\n");

    int opcao;
    int vitoria = 0; // Flag para controlar se o jogador venceu. 0 = não, 1 = sim.

    // --- LOOP PRINCIPAL DO JOGO ---
    // O loop 'do-while' executa pelo menos uma vez e continua enquanto a opção não for 0.
    do {
        exibirMapa(mapa, TOTAL_TERRITORIOS);
        printf("\n--- MENU DE ACOES ---\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missao\n");
        printf("0 - Sair\n");
        printf("Escolha sua acao: ");
        scanf("%d", &opcao);
        printf("\n");

        // O 'switch' direciona o fluxo do programa com base na escolha do jogador.
        switch (opcao) {
            case 1: { // Lógica de ataque
                int idAtacante, idDefensor;
                printf("Digite o ID do territorio atacante: ");
                scanf("%d", &idAtacante);
                printf("Digite o ID do territorio defensor: ");
                scanf("%d", &idDefensor);

                // Validação para garantir que os IDs são válidos e diferentes.
                if (idAtacante >= 0 && idAtacante < TOTAL_TERRITORIOS &&
                    idDefensor >= 0 && idDefensor < TOTAL_TERRITORIOS &&
                    idAtacante != idDefensor) {
                    
                    // Verifica se o território de ataque pertence ao jogador.
                    if (strcmp(mapa[idAtacante].cor, COR_JOGADOR) == 0) {
                        char corDefensorAntes[10];
                        strcpy(corDefensorAntes, mapa[idDefensor].cor);

                        atacar(&mapa[idAtacante], &mapa[idDefensor]);

                        // Se a cor do defensor mudou após o ataque, a conquista foi bem-sucedida.
                        if (strcmp(corDefensorAntes, mapa[idDefensor].cor) != 0) {
                            printf("\nVerificando missao apos conquista...\n");
                            // Após uma conquista, verifica se a missão foi cumprida.
                            if (verificarMissao(missaoJogador1, mapa, TOTAL_TERRITORIOS, COR_JOGADOR)) {
                                vitoria = 1; // Define a flag de vitória.
                                opcao = 0;   // Força a saída do loop do jogo.
                            }
                        }
                    } else {
                        printf("\nAcao invalida! Voce so pode atacar com um territorio da sua cor (%s).\n", COR_JOGADOR);
                    }
                } else {
                    printf("\nSelecao de territorios invalida!\n");
                }
                break;
            }
            case 2: // Apenas exibe a missão e seu status atual.
                exibirMissao(missaoJogador1);
                if (verificarMissao(missaoJogador1, mapa, TOTAL_TERRITORIOS, COR_JOGADOR)) {
                     printf("--> STATUS: Missao Cumprida! Voce pode declarar vitoria!\n");
                } else {
                     printf("--> STATUS: Missao ainda nao cumprida.\n");
                }
                break;
            case 0: // Sai do jogo.
                printf("Saindo do jogo...\n");
                break;
            default: // Trata qualquer outra entrada inválida.
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
        printf("\n--------------------------------------------------------------\n");
    } while (opcao != 0);
    
    // Se a flag 'vitoria' foi ativada, exibe a mensagem de vitória.
    if (vitoria) {
        printf("\n***************************************************\n");
        printf("*** PARABENS! Voce cumpriu sua missao e venceu! ***\n");
        printf("***************************************************\n");
        exibirMapa(mapa, TOTAL_TERRITORIOS);
    }

    // --- LIBERAÇÃO DE MEMÓRIA ---
    // Chama a função para liberar toda a memória alocada dinamicamente com 'malloc'.
    liberarMemoria(mapa, missaoJogador1);

    return 0; // Retorna 0 para indicar que o programa terminou com sucesso.
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES AUXILIARES ---

void inicializarTerritorios(Territorio* mapa, int tamanho) {
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].cor, "Verde");
    mapa[0].tropas = 5;

    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].cor, "Azul");
    mapa[1].tropas = 3;

    strcpy(mapa[2].nome, "Peru");
    strcpy(mapa[2].cor, "Vermelha");
    mapa[2].tropas = 2;

    strcpy(mapa[3].nome, "Colombia");
    strcpy(mapa[3].cor, "Verde");
    mapa[3].tropas = 4;

    strcpy(mapa[4].nome, "Chile");
    strcpy(mapa[4].cor, "Azul");
    mapa[4].tropas = 6;
}


// Itera sobre o mapa e imprime o estado de cada território.


void exibirMapa(const Territorio* mapa, int tamanho) {
    printf("--- ESTADO ATUAL DO MAPA ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("ID: %d | Territorio: %-10s | Cor: %-10s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}


 //Sorteia uma missão, aloca memória para ela e a copia para o jogador.
 // destino É um ponteiro para ponteiro (char**), pois precisamos modificar
 //o endereço para o qual o ponteiro original 'missaoJogador1' aponta.
 
void atribuirMissao(char** destino, const char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    
    // Aloca memória suficiente para guardar a string da missão sorteada (+1 para o caractere nulo '\0').
    *destino = (char*) malloc(strlen(missoes[indiceSorteado]) + 1);
    
    // Se a alocação falhar, o programa é encerrado para evitar erros piores.
    if (*destino == NULL) {
        printf("Erro ao alocar memoria para a missao!\n");
        exit(1);
    }
    
    // Copia a string da missão do vetor de missões para a memória recém-alocada.
    strcpy(*destino, missoes[indiceSorteado]);
}

 //Apenas imprime a missão atual do jogador.
 
void exibirMissao(const char* missao) {
    if (missao != NULL) {
        printf("Sua Missao: %s\n", missao);
    } else {
        printf("Nenhuma missao atribuida.\n");
    }
}

  //Simula a batalha entre dois territórios usando dados aleatórios.
 //Ponteiro para o território que ataca. Seus dados podem ser modificados.
 //Ponteiro para o território que defende. Seus dados podem ser modificados.
 
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\nAtaque de %s (%d tropas) contra %s (%d tropas)!\n",
           atacante->nome, atacante->tropas, defensor->nome, defensor->tropas);
    
    // Simula a rolagem de um dado de 6 faces para cada um.
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("Dado Atacante: %d | Dado Defensor: %d\n", dadoAtacante, dadoDefensor);

    // Lógica da batalha: o maior dado vence.
    if (dadoAtacante > dadoDefensor) {
        printf(">> VITORIA do atacante! %s conquista %s.\n", atacante->nome, defensor->nome);
        strcpy(defensor->cor, atacante->cor); // O defensor agora pertence ao atacante.
        defensor->tropas = atacante->tropas / 2; // Metade das tropas se move para o novo território.
        atacante->tropas -= defensor->tropas; // O atacante perde as tropas que se moveram.
        if (atacante->tropas == 0) atacante->tropas = 1; // Garante que o território de origem não fique vazio.
    } else {
        printf(">> DERROTA do atacante! %s perdeu uma tropa.\n", atacante->nome);
        atacante->tropas--;
        if (atacante->tropas < 1) { // O atacante não pode ter menos de 1 tropa.
            atacante->tropas = 1;
        }
    }
}


  //Contém a lógica para verificar se o objetivo de uma missão foi alcançado.
  //1 (verdadeiro) se a missão foi cumprida, 0 (falso) caso contrário.
 
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho, const char* corJogador) {
    // Compara a string da missão do jogador com as missões conhecidas.
    if (strcmp(missao, "Conquistar 3 territorios") == 0) {
        int contador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                contador++;
            }
        }
        return contador >= 3; // Retorna verdadeiro se o jogador tiver 3 ou mais territórios.
    }

    if (strcmp(missao, "Destruir o exercito Verde") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Verde") == 0) {
                return 0; // Se encontrar qualquer território Verde, a missão não foi cumprida.
            }
        }
        return 1; // Se o loop terminar, significa que não há mais territórios Verdes.
    }
    
    if (strcmp(missao, "Eliminar todas as tropas da cor Vermelha") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelha") == 0) {
                return 0; // Se encontrar qualquer território Vermelho, a missão não foi cumprida.
            }
        }
        return 1; // Se o loop terminar, não há mais territórios Vermelhos.
    }

    // Se a missão não corresponder a nenhuma lógica implementada, retorna falso.
    return 0;
}

//Libera a memória alocada com 'malloc' para evitar vazamento de memória (memory leak).
 
void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    printf("Liberando memoria...\n");
    free(mapa);            // Libera a memória do vetor de territórios.
    mapa = NULL;           // Boa prática: aponta o ponteiro para NULL após liberar.
    free(missaoJogador);   // Libera a memória da string da missão.
    missaoJogador = NULL;  // Aponta o ponteiro para NULL.
    printf("Memoria liberada com sucesso!\n");
}