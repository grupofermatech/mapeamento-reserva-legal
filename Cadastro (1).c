#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

#define MAX_RESERVAS 100


struct ReservaLegal {
    char nome[50];
    float area;
    char regiao[50];
};

struct Pessoa {
    char nome[50];
    char endereco[100];
    float hectares;
};

struct ReservaLegal reservas[MAX_RESERVAS]; // Array de reservas legais
int numReservas = 0; // N�mero de reservas cadastradas


void cadastrarReservaLegal() {
    struct ReservaLegal reserva;

    printf("Nome da reserva legal: ");
    fgets(reserva.nome, sizeof(reserva.nome), stdin);

    printf("�rea da reserva legal: ");
    scanf("%f", &reserva.area);

    printf("Regi�o onde se encontra a reserva legal: ");
    getchar(); // Limpar o caractere de nova linha residual

    char regiao[50];
    fgets(regiao, sizeof(regiao), stdin);

    // Converter a string para letras min�sculas
    int i = 0;
    while (regiao[i] != '\0' && regiao[i] != '\n') {
        regiao[i] = tolower(regiao[i]);
        i++;
    }

    // Verificar a regi�o v�lida
    if (strcmp(regiao, "florestas\n") == 0 ||
        strcmp(regiao, "cerrado\n") == 0 ||
        strcmp(regiao, "campos gerais\n") == 0) {
        strcpy(reserva.regiao, regiao);
    } else {
        printf("Regi�o inv�lida. Op��es dispon�veis: Florestas, Cerrado, Campos gerais.\n");
        return;
    }

    FILE *arquivoReserva = fopen("reservas.dat", "ab");
    if (arquivoReserva == NULL) {
        printf("Erro ao abrir o arquivo de reservas.\n");
        return;
    }

    fwrite(&reserva, sizeof(struct ReservaLegal), 1, arquivoReserva);
    fclose(arquivoReserva);

    reservas[numReservas] = reserva; // Registrar a reserva no array

    printf("Dados da reserva legal cadastrados com sucesso!\n");
    numReservas++; // Incrementar o contador de reservas

    int numPessoas;
    printf("\n\nQuantas pessoas voc� deseja cadastrar? ");
    scanf("%d", &numPessoas);

    FILE *arquivoPessoas = fopen("pessoas.dat", "ab");
    if (arquivoPessoas == NULL) {
        printf("Erro ao abrir o arquivo de pessoas.\n");
        return;
    }

    float areaReservaAtual = 0.0;  // Armazena a �rea total de reserva legal atualmente cadastrada

    i = 0;
    while (i < numPessoas) {
        struct Pessoa pessoa;

        getchar(); // Limpar o caractere de nova linha residual

        printf("\nInforma��es da pessoa %d:\n", i+1);

        printf("Nome completo: ");
        fgets(pessoa.nome, sizeof(pessoa.nome), stdin);

        printf("Endere�o: ");
        fgets(pessoa.endereco, sizeof(pessoa.endereco), stdin);

        printf("Quantidade de hectares: ");
        scanf("%f", &pessoa.hectares);

        // Verifica��o da �rea de reserva legal
        float areaReservaMinima;

        if (strcmp(reserva.regiao, "florestas") == 0) {
            areaReservaMinima = 0.8 * pessoa.hectares;
        } else if (strcmp(reserva.regiao, "cerrado\n") == 0) {
			areaReservaMinima = 0.35 * pessoa.hectares;
			} else if (strcmp(reserva.regiao, "campos gerais\n") == 0) {
			areaReservaMinima = 0.2 * pessoa.hectares;
			}

     if (areaReservaAtual + areaReservaMinima <= reserva.area) {
        fwrite(&pessoa, sizeof(struct Pessoa), 1, arquivoPessoas);
        areaReservaAtual += areaReservaMinima;
    } else {
        printf("N�o � poss�vel adicionar essa pessoa � reserva legal. Limite de �rea excedido.\n");
    }

    i++; // Incrementar o contador
}

fclose(arquivoPessoas);

printf("Dados das pessoas cadastrados com sucesso!\n");
getch();
system("cls");
}

void visualizarReservasLegais() {
    printf("Reservas Legais Cadastradas:\n");
    int i;
    for (i = 0; i < numReservas; i++) {
        printf("Reserva %d:\n", i + 1);
        printf("Nome: %s", reservas[i].nome);
        printf("�rea: %.2f\n", reservas[i].area);
        printf("Regi�o: %s\n", reservas[i].regiao);
        printf("--------------------------------\n");
    }

    FILE *arquivoReservas = fopen("reservas.dat", "rb");
    if (arquivoReservas == NULL) {
        printf("Erro ao abrir o arquivo de reservas.\n");
        return;
    }

    struct ReservaLegal reserva;
    while (fread(&reserva, sizeof(struct ReservaLegal), 1, arquivoReservas)) {
        int reservaIndex = numReservas;
        printf("\nReserva %d:\n", reservaIndex + 1);
        printf("Nome: %s", reserva.nome);
        printf("�rea: %.2f\n", reserva.area);
        printf("Regi�o: %s\n", reserva.regiao);
        printf("--------------------------------\n");
        numReservas++; // Incrementar o contador de reservas
    }

    fclose(arquivoReservas);

    FILE *arquivoPessoas = fopen("pessoas.dat", "rb");
    if (arquivoPessoas == NULL) {
        printf("Erro ao abrir o arquivo de pessoas.\n");
        return;
    }

    struct Pessoa pessoa;
    while (fread(&pessoa, sizeof(struct Pessoa), 1, arquivoPessoas)) {
        printf("Pessoa:\n");
        printf("Nome: %s", pessoa.nome);
        printf("Endere�o: %s", pessoa.endereco);
        printf("Quantidade de hectares: %.2f\n", pessoa.hectares);
        printf("--------------------------------\n");
    }

    fclose(arquivoPessoas);

    // Verificar se a tecla "Esc" foi pressionada
    if (getch() == 27) {
        printf("Programa encerrado pelo usu�rio.\n");
        exit(0);
    }
}



void adicionarPessoaReserva() {
    int numReserva;
    printf("Digite o n�mero da reserva em que deseja adicionar uma pessoa: ");
    scanf("%d", &numReserva);
    getchar(); // Limpar o caractere de nova linha residual

    if (numReserva <= 0 || numReserva > numReservas) {
        printf("N�mero de reserva inv�lido.\n");
        return;
    }

    FILE *arquivoPessoas = fopen("pessoas.dat", "ab");
    if (arquivoPessoas == NULL) {
        printf("Erro ao abrir o arquivo de pessoas.\n");
        return;
    }

    struct ReservaLegal reserva = reservas[numReserva - 1];
    float areaReservaMinima;

    if (strcmp(reserva.regiao, "florestas") == 0) {
        areaReservaMinima = 0.8 * reserva.area;
    } else if (strcmp(reserva.regiao, "cerrado\n") == 0) {
        areaReservaMinima = 0.35 * reserva.area;
    } else if (strcmp(reserva.regiao, "campos gerais\n") == 0) {
        areaReservaMinima = 0.2 * reserva.area;
    }

    float areaReservaAtual = 0.0;

    struct Pessoa pessoa;

    printf("\nInforma��es da pessoa:\n");

    printf("Nome completo: ");
    fgets(pessoa.nome, sizeof(pessoa.nome), stdin);

    printf("Endere�o: ");
    fgets(pessoa.endereco, sizeof(pessoa.endereco), stdin);

    printf("Quantidade de hectares: ");
    scanf("%f", &pessoa.hectares);
    getchar(); // Limpar o caractere de nova linha residual

    // Verificar se � poss�vel adicionar a pessoa � reserva
    if (areaReservaAtual + areaReservaMinima <= reserva.area) {
        // Escrever a pessoa no arquivo de pessoas
        fwrite(&pessoa, sizeof(struct Pessoa), 1, arquivoPessoas);
        areaReservaAtual += areaReservaMinima;

        // Atualizar o arquivo de reservas com a nova �rea
        FILE *arquivoReservas = fopen("reservas.dat", "rb+");
        if (arquivoReservas == NULL) {
            printf("Erro ao abrir o arquivo de reservas.\n");
            return;
        }

        fseek(arquivoReservas, (numReserva - 1) * sizeof(struct ReservaLegal), SEEK_SET);
        fwrite(&reserva, sizeof(struct ReservaLegal), 1, arquivoReservas);

        fclose(arquivoReservas);

        printf("Pessoa adicionada � reserva com sucesso!\n");
    } else {
        printf("N�o � poss�vel adicionar essa pessoa � reserva legal. Limite de �rea excedido.\n");
    }

    fclose(arquivoPessoas);
}


int main() {
    int opcao;
    setlocale(LC_ALL, "Portuguese");

    do {
        numReservas = 0; // Redefinir o n�mero de reservas
        memset(reservas, 0, sizeof(reservas)); // Zerar o array de reservas

        system("cls");
        printf("Menu:\n");
        printf("1. Cadastrar reserva legal\n");
        printf("2. Visualizar reservas legais cadastradas\n");
        printf("3. Adicionar pessoa a uma reserva legal existente\n");
        printf("4. Sair\n");
        printf("Op��o: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o caractere de nova linha residual

        switch (opcao) {
            case 1:
                cadastrarReservaLegal();
                break;
            case 2:
                visualizarReservasLegais();
                break;
            case 3:
                adicionarPessoaReserva();
                break;
            case 4:
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Op��o inv�lida. Por favor, tente novamente.\n");
        }
    } while (opcao != 4);

    return 0;
}
