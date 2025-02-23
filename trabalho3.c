/* ICSF13 - 2024-2 - TRABALHO 3
/* Alunos:
/* Arthur Dal Bem Nunes - 2697769
/* João Pedro Girelli - 2719819
/* Sergio Roncato Maccari - 2719851
/----------------------------------------------------------------------------/
/* Professores:
/* Bogdan T. Nassu                                                     */
/* Leyza E. B. Dorini
/============================================================================/

/* Função central do trabalho. */

#include "imagem.h"
#include "trabalho3.h"
#include <stdlib.h>
#include <stdio.h>

#define BRANCO 1
Rect* encontraFuros(Imagem1C* mask, int* n_furos)
{
    //Inicialização de variáveis.
    int i, j;
    int largura = mask->largura, altura = mask->altura;
    *n_furos = 0;

    //Aloca a matriz_checagem. Identifica pixels brancos e pretos.
    int existe_buraco = 0;
    int** matriz_checagem = (int**)malloc(altura * sizeof(int*));
    for (i = 0; i < altura; i++)
    {
        matriz_checagem[i] = (int*)malloc(largura * sizeof(int));
        for (j = 0; j < largura; j++)
        {
            if (mask->dados[i][j] >= 128)
            {
                matriz_checagem[i][j] = 1;
                existe_buraco = 1;
            }
            else
            {
                matriz_checagem[i][j] = 0;
            }
        }
    }

    // se não existirem pixels brancos na imagem, libera a memória alocada em matriz_checagem.
    if (existe_buraco == 0)
    {

        for (i = 0; i < altura; i++)
        {
            free(matriz_checagem[i]);
        }
        free(matriz_checagem);
        return NULL;
    }


    //Loop principal da função
    //flag_master vai definir se existe necessidade de analisar a matriz
    int flag_master = 1, symbol = 2;
    while(flag_master)
    {
        flag_master = 0;
        for(i=0; i<altura; i++)
        {
            for(j=0; j<largura; j++)
            {
                // verifica se existe um vizinho com símbolo
                if (matriz_checagem[i][j] == BRANCO)
                {
                    if ((i > 0 && matriz_checagem[i-1][j] == symbol) ||
                            (i < altura - 1 && matriz_checagem[i+1][j] == symbol) ||
                            (j > 0 && matriz_checagem[i][j-1] == symbol) ||
                            (j < largura - 1 && matriz_checagem[i][j+1] == symbol))
                    {
                        matriz_checagem[i][j] = symbol;
                        flag_master = 1;
                    }
                    // Manuseamento de bordas e cantos
                    if (i == 0 || j == 0 || i == altura - 1 || j == largura - 1)
                    {
                        if ((i > 0 && matriz_checagem[i-1][j] == symbol) ||
                                (i < altura - 1 && matriz_checagem[i+1][j] == symbol) ||
                                (j > 0 && matriz_checagem[i][j-1] == symbol) ||
                                (j < largura - 1 && matriz_checagem[i][j+1] == symbol))
                        {
                            matriz_checagem[i][j] = symbol;
                        }
                    }
                }
            }
        }
        /* se a verificacao em cima deu que não existe nenhum vizinho,
         deve atualizar o simbolo para o proximo e sinalizar que tem um vizinho*/
        int symbol_atualizado = 0;

        if (flag_master == 0)
        {
            for (i=0; i<altura && symbol_atualizado==0; i++)
            {
                for(j=0; j<largura && symbol_atualizado==0; j++)
                {
                    if(matriz_checagem[i][j] == BRANCO)
                    {
                        symbol++; // atualiza o symbol
                        matriz_checagem[i][j] = symbol;
                        flag_master = 1;
                        symbol_atualizado = 1;
                    }
                }
                if(flag_master == 1)
                {
                    symbol_atualizado = 1;
                }
            }
        }
    }
    /* conta n_furos a partir do número de simbolos(que começa como 2),
     por isso que está alocando dincamicamente o vetor rect só aqui*/
    *n_furos = symbol-1;
    Rect* vetor_de_retangulos = malloc((*n_furos) * sizeof(Rect));


    //Define as coordenadas dos retângulos
    int cont = 2;
    int min_x, max_x, min_y, max_y;
    while(cont <= symbol)
    { 
        //ajusta maximos e minimos
        min_x = largura, max_x = 0, min_y = altura, max_y = 0;
        for(i=0; i<altura ; i++)
        {
            for(j=0; j<largura ; j++)
            {
                if(matriz_checagem[i][j] == cont)
                {
                    if(i>max_y)
                    {
                        max_y = i;
                    }
                    if(i<min_y)
                    {
                        min_y = i;
                    }
                    if(j>max_x)
                    {
                        max_x = j;
                    }
                    if(j<min_x)
                    {
                        min_x = j;
                    }
                }
            }
        }
        //deferencia vetor_de_retangulos com coordenadas
        vetor_de_retangulos[cont-2].tl.x = min_x;
        vetor_de_retangulos[cont-2].tl.y = min_y;
        vetor_de_retangulos[cont-2].br.x = max_x;
        vetor_de_retangulos[cont-2].br.y = max_y;
        cont++;
    }


    //se vetor_de_retangulos for vazio, retorna NULL
    if (vetor_de_retangulos == NULL)
    {
        for (i = 0; i < altura; i++)
        {
            free(matriz_checagem[i]);
        }
        free(matriz_checagem);
        return NULL;
    }

    //liberia a memória alocada
    for (i = 0; i < altura; i++)
    {
        free(matriz_checagem[i]);
    }
    free(matriz_checagem);


    return vetor_de_retangulos;
}
