/*=============================================================
*           UNIFAL = Universidade Federal de Alfenas.
*             BACHARELADO EM CIENCIA DA COMPUTACAO.
* Trabalho..: Contagem de feijoes
* Professor.: Luiz Eduardo da Silva
* Aluno.....: Gabriel Micael Henrique
* Data......: 20/05/2024
*=============================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/imagelib.h"

// Função para limiarizar uma imagem PBM
void limiarizar_pbm(int *In, int *Out, int largura, int altura, int limiar) {
    for (int i = 0; i < altura * largura; i++){
            if (In[i] >= limiar) {
                Out[i] = 1;
            } else {
                Out[i] = 0;
            }
        printf("Repetição %d", i);
    }
    printf("Saiu da repetição");
}


int find(int parent[], int i)
{
    while (parent[i] != i)
        i = parent[i];
    return i;
}

//função para unir 2 pixels com rótulos diferentes do mesmo objeto em apenas um rótulo
void Union(int parent[], int i, int j)
{
    int x = find(parent, i);
    int y = find(parent, j);
    parent[y] = x;
}

void label(image In)
{
    int nr = In->nr;
    int nc = In->nc;
    int *px = In->px;
    int numLabel = 0;
    int parent[1000];
    for (int i = 0; i < 1000; i++)
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
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int nc, nr, ml, tp = 2;
    char nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    //-- define input/output file name
    img_name(argv[1], nameIn, nameOut, tp, tp);
    //-- read image
    In = img_get(nameIn, tp);
    //-- transformation
    limiarizar_pbm(In->px,Out->px,In->nc,In->nr,50);
    //-- save image
    img_put(Out, nameOut, tp);

    //-- show image
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
