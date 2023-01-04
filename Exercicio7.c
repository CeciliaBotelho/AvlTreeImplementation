#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define TAM_BUFFER (sizeof(int) * 4 + 20 * sizeof(char))
#define CONTADOR (*(int*)(pBuffer))
#define OPCAO (*(int*)(pBuffer + sizeof(int)))
#define NUM_PESSOAS (*(int *)(pBuffer + 2 * sizeof(int)))
#define CONTADOR2 (*(int *)(pBuffer + 3 * sizeof(int)))
#define NOME_BUFFER (char *)(pBuffer + 4 * sizeof(int))


// Cecilia Botelho
typedef struct
{
    char nome[20];
    int telefone;
    int idade;
} Pessoa;

Pessoa pessoas[10];


// tiEstudante maximo[10] = {};

int adicionar_nom(void *pBuffer)
{
    printf("Nome da pessoa:");
    scanf("%s", pessoas [NUM_PESSOAS].nome);

    printf("Numero do telefone:");
    scanf("%d", &pessoas [NUM_PESSOAS].telefone);

    printf("idade:");
    scanf("%d", &pessoas [NUM_PESSOAS].idade);


    return (0);
}

int mostrar_nom(void *pBuffer)
{

    if (NUM_PESSOAS == 0)
    {
        printf("\n Nao ha nomes cadastrados\n");
        return (1);
    }

    printf("\n Agenda :\n");

    for (CONTADOR = 0; CONTADOR < NUM_PESSOAS; CONTADOR++)
    {
        printf("\n Nome da pessoa = %s \n", pessoas [CONTADOR].nome);
        printf("\n telefone da pessoa = %d \n", pessoas [CONTADOR].telefone);
        printf("\n idade da pessoa = %d \n", pessoas [CONTADOR].idade);
    }
    return (0);
}

void *remover_nom(void *pBuffer)
{
    printf("Insira o nome a ser removido da lista: ");
    scanf("%s", NOME_BUFFER );
    for (CONTADOR = 0; CONTADOR < NUM_PESSOAS; CONTADOR++)
    {
        if (strstr(NOME_BUFFER, pessoas[CONTADOR].nome))
        {
            for (CONTADOR2 = CONTADOR; CONTADOR2 < NUM_PESSOAS-1; CONTADOR2++) 
               {
                   //USEI O FOR PARA PUXAR AS PESSOAS PARA DESLOCAR UM, A PARTIR DE ONDE ACHEI A PESSOA
                strcpy(pessoas[CONTADOR2].nome,pessoas[CONTADOR2+1].nome);
                pessoas[CONTADOR2].idade=pessoas[CONTADOR2+1].idade;
                pessoas[CONTADOR2].telefone= pessoas[CONTADOR2+1].telefone;
                }
        }
    }
    return (0);
}

void *buscar_nom(void *pBuffer)
{

    printf("Insira o nome a ser buscado na lista: ");
    scanf("%s", NOME_BUFFER );
    for (CONTADOR = 0; CONTADOR < NUM_PESSOAS; CONTADOR++)
    {
        if (strstr(NOME_BUFFER, pessoas[CONTADOR].nome))
        {
            printf("\n Nome da pessoa = %s \n", pessoas [CONTADOR].nome);
            printf("\n telefone da pessoa = %d \n", pessoas [CONTADOR].telefone);
            printf("\n idade da pessoa = %d \n", pessoas [CONTADOR].idade);
        return (0);
        }
    }

    printf("Não existe esse nome");
    return (0);
}

int main()
{
    void *pBuffer = malloc(TAM_BUFFER); // declaro o ponteiro pbuffer e aloco um espaço de tamanho TAMBUFFER

    if (!pBuffer)
    {
        printf("faltou memória\n");
        return -1;
    }

    NUM_PESSOAS = 0;
    OPCAO = 1;

    while (OPCAO != 0)
    {
        printf("1. incluir dados de uma pessoa\n");
        printf("2. apagar dados de uma pessoa\n");
        printf("3. Listar dados obtidos\n");
        printf("4. buscar dados de uma pessoa\n");
        printf("0. sair\n");

        scanf("%d", &OPCAO);
        getc(stdin);

        switch (OPCAO)
        {
        case 1:
            adicionar_nom(pBuffer);
            NUM_PESSOAS++;
            break;

        case 2:
            remover_nom(pBuffer);
            NUM_PESSOAS--;
            break;

        case 3:
            mostrar_nom(pBuffer);
            break;

        case 4:
            buscar_nom(pBuffer);
            break;

        case 0:
            printf("Saindo do programa");
            break;
        }
    }
    free(pBuffer);
    return 0;
}