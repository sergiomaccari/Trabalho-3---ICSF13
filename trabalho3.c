/*============================================================================*/
/* ICSF13 - 2024-2 - TRABALHO 3
/* Alunos:
/* Arthur Dal Bem Nunes -
/* João Pedro Girelli -
/* Sergio Roncato Maccari -
/*----------------------------------------------------------------------------*/
/* Professores:
/* Bogdan T. Nassu                                                     */
/* Leyza E. B. Dorini
/*============================================================================*/

#include "imagem.h"
#include "trabalho3.h"

/* Função central do trabalho. */
Rect* encontraFuros(Imagem1C* mask, int* n_furos)
{
    int i, j;
    int min_x = 0, max_x = 0, min_y = 0, max_y = 0;
    int largura = mask->largura, altura = mask->altura;
    *n_furos = 0;

    // Inicializa a matriz de checagem que verificara mais a frente se os numeros ja fazem parte de um retangulo(objetivo nao repetir a formação de reatngulo)
    int** matriz_checagem = (int**)malloc(altura * sizeof(int*));
    for (i = 0; i < altura; i++)
    {
        matriz_checagem[i] = (int*)malloc(largura * sizeof(int));
        for (j = 0; j < largura; j++)
        {
            matriz_checagem[i][j] = mask->dados[i][j];
        }
    }

    // Contagem de furos na imagem
    for (i = 0; i < altura; i++)
    {
        for (j = 0; j < largura; j++)
        {
            if (mask->dados[i][j] == BRANCO && matriz_checagem[i][j] == 0)
            {
                (*n_furos)++;
            }
        }
    }

    //De acordo com o enunciado, se o numero de furos for 0 a func retorna NULL. Nesse caso, da free na matriz pra verificação criada anteriormente
    if (*n_furos == 0)
    {
        for (i = 0; i < altura; i++)
        {
            free(matriz_checagem[i]);
        }
        free(matriz_checagem);
        return NULL;
    }

    // Alocação dinamica do vetor de retangulos, de acordo com o tamanho n_furos como dita o enunciado
    Rect* vetor_de_retangulos = (Rect*)malloc(*n_furos * sizeof(Rect));

    if (vetor_de_retangulos == NULL)
    {
        for (i = 0; i < altura; i++)
        {
            free(matriz_checagem[i]);
        }
        free(matriz_checagem);
        return NULL;
    }

    // Preenche o vetor de retangulos
    int cont_retangulo = 0;
    int availablePath = 1, symbol = 2, symbolAttrib = 0; lref,cref,lfinal,cdir,cesq;
    while(avaliablePath){
        for (i = 0; i < altura; i++)
        {
            for (j = 0; j < largura; j++)
            {
                if (matriz_checagem[i][j] && !symbolAttrib){
                    matriz_checagem[i][j] = symbol;
                    symbol++;
                    symbolAttrib = 1;
                    lref = i;
                    cref = j;
                }
                // vetorretangulo[symbol-2].cord.mmx

                if(matriz_checagem[i + 1][j]){
                    matriz_checagem[i + 1][j] = symbol;

                }
                if(matriz_checagem[i][j + 1]){
                    matriz_checagem[i + 1][j] = symbol;
                    cdir++;
                }
                if(matriz_checagem[i][j - 1]){
                    matriz_checagem[i + 1][j] = symbol;
                    cesq++;
                }
                if(matriz_checagem[i - 1][j]){
                    matriz_checagem[i + 1][j] = symbol;
                    lfinal++;
                }
                //coordenadas que seriam mandadas para o retangulo!, de preferencia deixar no ultimo loop do furo que ta sendo usado;
                //ponto minimo = lref, cref- cesq;
                //ponto maximo = lref+lfnial. cref+cdis
                else{
                    symbolAttrib = 0;
                }
            }
        }
    }

    for (i = 0; i < altura; i++)
    {
        free(matriz_checagem[i]);
    }
    free(matriz_checagem);

    return vetor_de_retangulos;
}

