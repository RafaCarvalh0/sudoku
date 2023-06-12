#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define NUM_THREADS 12

typedef struct
{
    int **matriz;
    int L;  // linha
    int C;  // coluna
    int SL; // sub-linha
    int SC; // sub-coluna
    int inicio;
    int fim;
    bool repeticaoEncontrada;
} VerificacaoArgs;

bool verificaRepeticao(int **matriz, int linha, int coluna, int valor, int L, int C, int SL, int SC)
{
    // verifica linha
    for (int j = 0; j < C; j++)
    {
        if (j != coluna && matriz[linha][j] == valor)
        {
            return true;
        }
    }

    // verifica coluna
    for (int i = 0; i < L; i++)
    {
        if (i != linha && matriz[i][coluna] == valor)
        {
            return true;
        }
    }

    // verifica subgrade
    int inicioLinha = (linha / SL) * SL;
    int inicioColuna = (coluna / SC) * SC;
    for (int i = inicioLinha; i < inicioLinha + SL; i++)
    {
        for (int j = inicioColuna; j < inicioColuna + SC; j++)
        {
            if (i != linha && j != coluna && matriz[i][j] == valor)
            {
                return true;
            }
        }
    }

    return false;
}

void *verificaLinhas(void *args)
{
    VerificacaoArgs *threadArgs = (VerificacaoArgs *)args;
    int **matriz = threadArgs->matriz;
    int L = threadArgs->L;
    int C = threadArgs->C;
    int SL = threadArgs->SL;
    int SC = threadArgs->SC;
    int inicio = threadArgs->inicio;
    int fim = threadArgs->fim;

    for (int i = inicio; i < fim; i++)
    {
        for (int j = 0; j < C; j++)
        {
            int valor = matriz[i][j];
            if (verificaRepeticao(matriz, i, j, valor, L, C, SL, SC))
            {
                threadArgs->repeticaoEncontrada = true;
                return NULL;
            }
        }
    }

    return NULL;
}

void *verificaColunas(void *args)
{
    VerificacaoArgs *threadArgs = (VerificacaoArgs *)args;
    int **matriz = threadArgs->matriz;
    int L = threadArgs->L;
    int C = threadArgs->C;
    int SL = threadArgs->SL;
    int SC = threadArgs->SC;
    int inicio = threadArgs->inicio;
    int fim = threadArgs->fim;

    for (int j = inicio; j < fim; j++)
    {
        for (int i = 0; i < L; i++)
        {
            int valor = matriz[i][j];
            if (verificaRepeticao(matriz, i, j, valor, L, C, SL, SC))
            {
                threadArgs->repeticaoEncontrada = true;
                return NULL;
            }
        }
    }

    return NULL;
}

void *verificaSubgrades(void *args)
{
    VerificacaoArgs *threadArgs = (VerificacaoArgs *)args;
    int **matriz = threadArgs->matriz;
    int L = threadArgs->L;
    int C = threadArgs->C;
    int SL = threadArgs->SL;
    int SC = threadArgs->SC;
    int inicio = threadArgs->inicio;
    int fim = threadArgs->fim;

    for (int k = inicio; k < fim; k++)
    {
        int subgrade = k;
        int inicioLinha = (subgrade / (C / SC)) * SL;
        int inicioColuna = (subgrade % (C / SC)) * SC;
        for (int i = inicioLinha; i < inicioLinha + SL; i++)
        {
            for (int j = inicioColuna; j < inicioColuna + SC; j++)
            {
                int valor = matriz[i][j];
                if (verificaRepeticao(matriz, i, j, valor, L, C, SL, SC))
                {
                    threadArgs->repeticaoEncontrada = true;
                    return NULL;
                }
            }
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    FILE *arquivo;
    int L, C, SL, SC;

    arquivo = fopen(argv[1], "r");

    // tratamento de erro 2
    if (arquivo == NULL)
    {
        printf("Error to read file\n");
        return 1;
    }
    // tratamento de erro 1
    else if (argc > 2)
    {
        printf("Invalid number of parameters\n");
        return 1;
    }

    fscanf(arquivo, "%dx%d", &L, &C);
    fscanf(arquivo, "%dx%d", &SL, &SC);

    // matriz para armazenar os valores
    int **matriz = (int **)malloc(L * sizeof(int *));
    for (int i = 0; i < L; i++)
    {
        matriz[i] = (int *)malloc(C * sizeof(int));
    }

    // leitura do arquivo e preencher matriz
    for (int i = 0; i < L; i++)
    {
        for (int j = 0; j < C; j++)
        {
            fscanf(arquivo, "%d", &matriz[i][j]);
            // tratamento de erro 3
            if (matriz[i][j] == 0 || matriz[i][j] < 1)
            {
                printf("File out of format\n");
                return 1;
            }
            // tratamento de erro 3
            if (L != C || SL * SC != L)
            {
                printf("File out of format\n");
                return 1;
            }
            // tratamento de erro 3
            if (matriz[i][j] > L)
            {
                printf("File out of format\n");
                return 1;
            }
        }
    }
    fclose(arquivo);

    pthread_t threads[NUM_THREADS];
    VerificacaoArgs threadArgs[NUM_THREADS];
    bool repeticaoEncontrada = false;

    // verifica linha
    for (int i = 0; i < 4; i++)
    {
        threadArgs[i].matriz = matriz;
        threadArgs[i].L = L;
        threadArgs[i].C = C;
        threadArgs[i].SL = SL;
        threadArgs[i].SC = SC;
        threadArgs[i].inicio = i * (L / 4);
        threadArgs[i].fim = (i + 1) * (L / 4);
        threadArgs[i].repeticaoEncontrada = false;

        pthread_create(&threads[i], NULL, verificaLinhas, (void *)&threadArgs[i]);
    }

    // verifica coluna
    for (int i = 4; i < 8; i++)
    {
        threadArgs[i].matriz = matriz;
        threadArgs[i].L = L;
        threadArgs[i].C = C;
        threadArgs[i].SL = SL;
        threadArgs[i].SC = SC;
        threadArgs[i].inicio = (i - 4) * (C / 4);
        threadArgs[i].fim = ((i - 4) + 1) * (C / 4);
        threadArgs[i].repeticaoEncontrada = false;

        pthread_create(&threads[i], NULL, verificaColunas, (void *)&threadArgs[i]);
    }

    // verifica subgrade
    for (int i = 8; i < 12; i++)
    {
        threadArgs[i].matriz = matriz;
        threadArgs[i].L = L;
        threadArgs[i].C = C;
        threadArgs[i].SL = SL;
        threadArgs[i].SC = SC;
        threadArgs[i].inicio = i - 8;
        threadArgs[i].fim = i - 7;
        threadArgs[i].repeticaoEncontrada = false;

        pthread_create(&threads[i], NULL, verificaSubgrades, (void *)&threadArgs[i]);
    }

    // espera pela finalização das threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);

        if (threadArgs[i].repeticaoEncontrada)
        {
            repeticaoEncontrada = true;
            break;
        }
    }

    // print no arquivo .out
    FILE *arquivo_saida = fopen("sudoku_rac.out", "w");

    if (repeticaoEncontrada)
    {
        // printf("FAIL\n");
        fprintf(arquivo_saida, "FAIL\n");
    }
    else
    {
        // printf("SUCCESS\n");
        fprintf(arquivo_saida, "SUCCESS\n");
    }

    for (int i = 0; i < L; i++)
    {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}