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

#ifndef __TRABALHO3_H
#define __TRABALHO3_H
#include <stdlib.h>
/*============================================================================*/

#include "imagem.h"

#define BRANCO 255
#define PRETO 0

/*============================================================================*/

typedef struct
{
    int x;
    int y;
} Coordenada;

/* Um tipo simples, usado para representar um retângulo envolvente. */
typedef struct
{
    Coordenada tl; // top-left.
    Coordenada br; // bottom-right
} Rect;

/* Função central do trabalho. */
Rect* encontraFuros(Imagem1C* mask, int* n_furos)
{
    int i, j;
    int largura = mask->largura, altura = mask->altura;
    *n_furos = 0;

    // Inicializa a matriz de checagem que verificara mais a frente se os numeros ja fazem parte de um retangulo(objetivo nao repetir a formação de reatngulo)
    int** matriz_checagem = (int**)malloc(altura * sizeof(int*));
    for (i = 0; i < altura; i++)
    {
        matriz_checagem[i] = (int*)malloc(largura * sizeof(int));
        for (j = 0; j < largura; j++)
        {
            matriz_checagem[i][j] = 0;
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
    for (i = 0; i < altura; i++)
    {
        for (j = 0; j < largura; j++)
        {
            if (mask->dados[i][j] == BRANCO && matriz_checagem[i][j] == 0)
            {
                // Definição dos limites do retangulo
                int min_x = j, max_x = j, min_y = i, max_y = i;

                //Talvez seria legal ou necessário colocar um condicional dentro de cada while para que ele nao extrapole os limites da imagem

                // Aumenta o limite para a direita
                while (mask->dados[i][max_x] == BRANCO)
                {
                    max_x++;
                }

                // Aumenta o limite para a esquerda
                while (mask->dados[max_y][j] == BRANCO)
                {
                    max_y++;
                }

                // // Aumenta o limite para baixo. Não é necessário aumentar o limite para cima devido como o loop percorre a amtrizICSF12
                while (mask->dados[i][min_x - 1] == BRANCO)
                {
                    min_x--;
                }

                while (mask->dados[min_y - 1][j] == BRANCO)
                {
                    min_y--;
                }

                vetor_de_retangulos[cont_retangulo].tl.x = min_x;
                vetor_de_retangulos[cont_retangulo].tl.y = min_y;
                vetor_de_retangulos[cont_retangulo].br.x = max_x - 1;
                vetor_de_retangulos[cont_retangulo].br.y = max_y - 1;

                for (int y = min_y; y <= max_y; y++)
                {
                    for (int x = min_x; x <= max_x; x++)
                    {
                        matriz_checagem[y][x] = 1;
                    }
                }

                cont_retangulo++;
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

#endif /* __TRABALHO3_H */
