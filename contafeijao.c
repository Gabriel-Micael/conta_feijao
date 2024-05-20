/*=============================================================
 *           UNIFAL = Universidade Federal de Alfenas.
 *             BACHARELADO EM CIENCIA DA COMPUTACAO.
 * Trabalho..: Contador de feijoes
 * Professor.: Luiz Eduardo da Silva
 * Aluno.....: Gabriel Micael Henrique
 * Data......: 20/05/2024
 *=============================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/imagelib.h"

// bean1.pgm = 41 componentes
// bean2.pgm = 68 componentes
// bean3.pgm = 96 componentes
// bean4.pgm = 123 componentes
// bean5.pgm = 10 componentes
// bean6.pgm = 20 componentes
// bean7.pgm = 30 componentes


// Função para limiarizar uma imagem PBM
image limiarizar_pbm(image In, int limiar)
{
    image Out = img_clone(In);
    for (int i = 0; i < In->nc * In->nr; i++)
    {
        if (In->px[i] >= limiar)
        {
            Out->px[i] = 0;
        }
        else
        {
            Out->px[i] = 1;
        }
    }
    return Out;
}

int find(int parent[], int i)
{
    while (parent[i] != i)
        i = parent[i];
    return i;
}

// função para unir 2 pixels com rótulos diferentes do mesmo objeto em apenas um rótulo
void Union(int parent[], int i, int j)
{
    int x = find(parent, i);
    int y = find(parent, j);
    parent[y] = x;
}


int quantidadeRotulos(int *parent, int numLabel)
{
    int qtdcomponentes = 0;
    int pertence;
    for (int i = 1; i <= numLabel; i++)
    {
        pertence = 0;
        for (int j = 0; j <= numLabel; j++)
        {
            if (i == parent[j])
            {
                pertence = 1;
                break;
            }
        }
        if (pertence)
        {
            qtdcomponentes++;
        }
    }
    return qtdcomponentes;
}

int label(image In)
{
    int nr = In->nr;
    int nc = In->nc;
    int *px = In->px;
    int numLabel = 0;
    int *parent = (int *)malloc(nc * nr * sizeof(int));
    for (int i = 0; i < nr * nc; i++)
        parent[i] = i;
    for (int i = 1; i < nr; i++)
        for (int j = 1; j < nc; j++)
        {
            int p = px[i * nc + j];
            int r = px[(i - 1) * nc + j];
            int t = px[i * nc + j - 1];
            if (p != 0)
            {
                if (r == 0 && t == 0)
                    px[i * nc + j] = ++numLabel;
                if (r != 0 && t == 0)
                    px[i * nc + j] = r;
                if (r == 0 && t != 0)
                    px[i * nc + j] = t;
                if (r != 0 && t != 0 && t == r)
                    px[i * nc + j] = r;
                if (r != 0 && t != 0 && t != r)
                {
                    px[i * nc + j] = t;
                    Union(parent, r, t);
                }
            }
        }
    for (int i = 0; i < nr * nc; i++)
        In->px[i] = find(parent, In->px[i]);
    In->ml = numLabel;
    return quantidadeRotulos(parent, numLabel);
}

void msg(char *s)
{
    printf("\nLabelling");
    printf("\n-------------------------------");
    printf("\nUso:  %s  nome-imagem[.pgm] \n\n", s);
    printf("    nome-imagem[.pgm] é o nome do arquivo da imagem \n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int nc, nr, ml, tp = GRAY, quantidadeDeComponentes;
    char nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    if (argc < 2)
    {
        msg(argv[0]);
    }

    img_name(argv[1], nameIn, nameOut, tp, tp - 1);
    //-- read image
    In = img_get(nameIn, tp);
    //-- transformation
    Out = limiarizar_pbm(In, 94);
    //-- save image
    img_put(Out, nameOut, tp - 1);

    quantidadeDeComponentes = label(Out);
    printf("#componentes= %2.0d\n", quantidadeDeComponentes);

    img_free(In);
    img_free(Out);
    return 0;
}
