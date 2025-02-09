/*============================================================================*/
/* DETERMINA OS RET�NGULOS ENVOLVENTES DE BLOBS EM UMA M�SCARA BIN�RIA        */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu                                                     */
/*============================================================================*/

#ifndef __TRABALHO3_H
#define __TRABALHO3_H
#include <stdlib.h>
/*============================================================================*/

#include "imagem.h"

#define BRANCO 255
#define PRETO 0

/*============================================================================*/
/* Um tipo simples, usado para representar uma coordenada. */
/* Um tipo simples, usado para representar uma coordenada. */
typedef struct
{
    int x;
    int y;
} Coordenada;

/* Um tipo simples, usado para representar um ret�ngulo envolvente. */
typedef struct
{
    Coordenada tl; // top-left.
    Coordenada br; // bottom-right
} Rect;

/* Fun��o central do trabalho. */
Rect* encontraFuros(Imagem1C* mask, int* n_furos)
{
    int i, j;
    int largura = mask->largura, altura = mask->altura;
    *n_furos = 0;

    // Inicializa a matriz auxiliar para marcar as c�lulas processadas
    int** processado = (int**)malloc(altura * sizeof(int*));
    for (i = 0; i < altura; i++) {
        processado[i] = (int*)malloc(largura * sizeof(int));
        for (j = 0; j < largura; j++) {
            processado[i][j] = 0;  // Inicializa como n�o processado
        }
    }

    // Aloca o vetor de ret�ngulos dinamicamente (apenas um ret�ngulo)
    Rect* vetor_de_retangulos = NULL;

    // Agora, preenche o vetor de ret�ngulos com as coordenadas dos furos
    for (i = 0; i < altura; i++)
    {
        for (j = 0; j < largura; j++)
        {
            if (mask->dados[i][j] == BRANCO && processado[i][j] == 0)
            {
                // Encontra os limites do ret�ngulo
                int min_x = j, max_x = j, min_y = i, max_y = i;

                // Expande a largura do ret�ngulo (para a direita)
                while (max_x < largura && mask->dados[i][max_x] == BRANCO)
                {
                    max_x++;
                }

                // Expande a altura do ret�ngulo (para baixo)
                while (max_y < altura && mask->dados[max_y][j] == BRANCO)
                {
                    max_y++;
                }

                // Ajusta os limites para a esquerda e para cima
                while (min_x > 0 && mask->dados[i][min_x-1] == BRANCO) {
                    min_x--;
                }

                while (min_y > 0 && mask->dados[min_y-1][j] == BRANCO) {
                    min_y--;
                }

                // Armazena o ret�ngulo no vetor
                vetor_de_retangulos = (Rect*)realloc(vetor_de_retangulos, (*n_furos + 1) * sizeof(Rect));
                vetor_de_retangulos[*n_furos].tl.x = min_x;
                vetor_de_retangulos[*n_furos].tl.y = min_y;
                vetor_de_retangulos[*n_furos].br.x = max_x - 1; // max_x foi incrementado a mais
                vetor_de_retangulos[*n_furos].br.y = max_y - 1; // max_y foi incrementado a mais

                // Marca as c�lulas do ret�ngulo como processadas (sem alterar a imagem original)
                for (int y = min_y; y <= max_y; y++)
                {
                    for (int x = min_x; x <= max_x; x++)
                    {
                        processado[y][x] = 1; // Marca cada c�lula como processada
                    }
                }

                (*n_furos)++; // Incrementa o contador de ret�ngulos encontrados
            }
        }
    }

    // Libera a mem�ria da matriz auxiliar
    for (i = 0; i < altura; i++) {
        free(processado[i]);
    }
    free(processado);

    return vetor_de_retangulos; // Retorna o vetor com os ret�ngulos encontrados
}

/*============================================================================*/
#endif /* __TRABALHO3_H */
