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

#include "imagem.h"
#include "trabalho3.h"
#include <stdlib.h>
#include <stdio.h>

#define BRANCO 1

/* Função central do trabalho. */

#include "imagem.h"
#include "trabalho3.h"
#include <stdlib.h>
#include <stdio.h>

#define BRANCO 1

/* Função central do trabalho. */

#include "imagem.h"
#include "trabalho3.h"
#include <stdlib.h>
#include <stdio.h>

#define BRANCO 1
Rect* encontraFuros(Imagem1C* mask, int* n_furos)
{

    int i, j;
    int largura = mask->largura, altura = mask->altura;
    *n_furos = 0;

    int counter = 0; // essa variavel aqui foi orignalmente colocada por mim, mudada de nome pelo João e agora eu nao sei o que ela ta fazendo
    // ela era bem essencial quando imaginava-se que n_furos era o numero de pixels brancos, e nao os de furos da imagem
    //agora que a gente entende n_furos talvez esse n_counter seja redundante e ate mesmo prejudicial
    // prestar atenção nas linhas 58 (counter++)

    // Inicializa a matriz de checagem que verificara mais a frente se os numeros ja fazem parte de um retangulo(objetivo nao repetir a formação de reatngulo)
    int** matriz_checagem = (int**)malloc(altura * sizeof(int*));
    for (i = 0; i < altura; i++)
    {
        matriz_checagem[i] = (int*)malloc(largura * sizeof(int));
        for (j = 0; j < largura; j++)
        {
            if (mask->dados[i][j] >= 128)
            {
                matriz_checagem[i][j] = 1;
                counter++; // counter conta os pixels brancos
            }
            else
            {
                matriz_checagem[i][j] = 0;
            }
        }
    }

    //Deixei o comentario antigo(linha 70) abaixo para mostrar que parece que counter não faz sentido agora que a gente entende n_furos
    //Por favor reler o enunciado e analisar
    // SERIO MESMO ANALISE DESDE A PARTE DE CIMA ATE ESSE IF PRA VER SE FAZ SENTIDO.


    //De acordo com o enunciado, se o numero de furos for 0 a func retorna NULL. Nesse caso, da free na matriz pra verificação criada anteriormente (COMENTARIO ANTIGO)
    if (counter == 0) // tenho 70% de certeza que esse looping, caso ele faça sentido, está no local errado.
    {
        // se não tiver pixels brancos da free em matriz checagem e finaliza a func(????)
        for (i = 0; i < altura; i++)
        {
            free(matriz_checagem[i]);
        }
        free(matriz_checagem);
        return; //nao deveria retornar NULL
    }

//Essa parte do codigo é o looping principal, divididos em uma llop que preenche a amtriz_checahem com simbolos
// e outro pega e manda pro vetor as coordenadas dos retangulos
// Loop  a seguir preenche a matriz checagem com simbolos

    int availablePath = 1, symbol = 2; // symbol tem que ser 2 pois foi delimitado na matriz_checagem que 1 é branco,
// porém pela lógica do looping o primeiro simbolo posto será 3. ISSO EH IMPORTANTE NA HORA DE DETERMINAR COORDENADAS DOS RETANGULOS.
// pelo amor de deus mudar nome das variáveis
    while(availablePath)
    {
        availablePath = 0; // sinceramente por favor leiam o código para entender quando availablePath é 0 e 1
        for(i=0; i<altura; i++)
        {
            for(j=0; j<largura; j++)
            {
                // chega se existe um vizinho com símbolo
                if (matriz_checagem[i][j] == BRANCO)
                {// eu separei os casos de cada borda/canto aqui também
                    if ((i > 0 && matriz_checagem[i-1][j] == symbol) ||
                        (i < altura - 1 && matriz_checagem[i+1][j] == symbol) ||
                        (j > 0 && matriz_checagem[i][j-1] == symbol) ||
                        (j < largura - 1 && matriz_checagem[i][j+1] == symbol))
                    {
                        matriz_checagem[i][j] = symbol;
                        availablePath = 1;
                    }
                    // manuseamento de bordas e cantos
                    if (i == 0 || j == 0 || i == altura - 1 || j == largura - 1) 
                    { // eu separei os casos de cada borda/canto aqui também
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
        // se a verificacao em cima deu que não exisge nenhum vizinho, deve atualizar o simbolo para o proximo e sinalizar que tem um vizinho availablePath = 1
        if (availablePath == 0)
        {
            for (i=0; i<altura; i++)
            {
                for(j=0; j<largura; j++)
                {
                    if(matriz_checagem[i][j] == BRANCO)
                    {
                        symbol++; // se voce fazer o teste de mesa percebe que o primeiro simbolo sera 3, isso eh importante la embaixo pro cont+1
                        matriz_checagem[i][j] = symbol;
                        availablePath = 1;
                        break; //break pois já definiu o próximo simbolo e atribuiu ele para um indice da amtriz (break no loop interno, contador j)
                    }
                }
                if(availablePath == 1)  // break no loop externo, contador i
                {

                    break;
                }
            }
// sinceramente se vocês conseguirem fazer esse porra funcionar sem o break a minha vida será muito mais feliz
        }


    }




    *n_furos = symbol-1; // conta n_furos a partir do número de simbolos, por isso que está alocando dincamicamente o vetor rect só aqui
    Rect* vetor_de_retangulos = malloc((*n_furos) * sizeof(Rect));

    int cont = 1; // meio que é 1 por intuição mas funciona
    int min_x, max_x, min_y, max_y;
    while(cont < symbol)  // vai analisar até ser 1 menor que o símbolo
    {
        min_x = largura, max_x = 0, min_y = altura, max_y = 0;
        for(i=0; i<altura ; i++)
        {
            for(j=0; j<largura ; j++)
            {
                if(matriz_checagem[i][j] == cont+1)  // esse cont +1 eh o trabalho de muita intuicao, 30 minutos printando a matriz_checagem, e muito go horse
                {
                    // em retrospectiva é porque o a condição de parada do looping é enquanto cont for menor que symbol
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
        cont++; // isso daqui é muita intuição e muito go horse, foi o segredo para parar de dar segfault (não sei porque, por favor descubram e me digam)
        // se você colocar cont ++ após a definição dos minimos e maximos do vetor da segfault (no momento desisti de entender o motivo, talvez no futuro eu entenda ou
        // ou vocês me ecpliquem
        vetor_de_retangulos[cont-2].tl.x = min_x;
        vetor_de_retangulos[cont-2].tl.y = min_y;
        vetor_de_retangulos[cont-2].br.x = max_x;
        vetor_de_retangulos[cont-2].br.y = max_y;
    }


//condicional necessária de acordo com o enunciado do trabalho
    if (vetor_de_retangulos == NULL)
    {
        for (i = 0; i < altura; i++)
        {
            free(matriz_checagem[i]);
        }
        free(matriz_checagem);
        return NULL;
    }

    for (i = 0; i < altura; i++)
    {
        free(matriz_checagem[i]);
    }
    free(matriz_checagem);

    printf("\n%d\n%d\n%d\n%d\n\n", min_x, min_y, max_x, max_y);

    return vetor_de_retangulos; //its over, we made it(barely)
}
