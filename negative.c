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

int find(int parent[], int i)
{
    while (parent[i] != i)
        i = parent[i];
    return i;
}

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

image neg_pbm(image In)
{
    image Out = img_clone(In);
    for (int i = 0; i < In->nr * In->nc; i++)
        Out->px[i] = !(In->px[i]);
    return Out;
}

image neg_pgm(image In)
{
    image Out = img_clone(In);
    for (int i = 0; i < In->nr * In->nc; i++)
        Out->px[i] = In->ml - In->px[i];
    return Out;
}

image neg_ppm(image In)
{
    image Out = img_clone(In);
    for (int i = 0; i < In->nr * In->nc; i++)
    {
        int r = In->ml - (In->px[i] >> 16) & 0xFF;
        int g = In->ml - (In->px[i] >> 8) & 0xFF;
        int b = In->ml - In->px[i] & 0xFF;
        Out->px[i] = (r << 16) + (g << 8) + b;
    }
    return Out;
}

void msg(char *s)
{
    printf("\nNegative image");
    printf("\n-------------------------------");
    printf("\nUsage:  %s  image-name[.pnm] tp\n\n", s);
    printf("    image-name[.pnm] is image file file \n");
    printf("    tp = image type (1 = BW, 2 = Gray, 3 = Color)\n\n");
    exit(1);
}

/*-------------------------------------------------------------------------
 * main function
 *-------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    int nc, nr, ml, tp;
    char *p, nameIn[100], nameOut[100], cmd[110];
    image In, Out;
    if (argc < 3)
        msg(argv[0]);
    tp = atoi(argv[2]);
    //-- define input/output file name
    img_name(argv[1], nameIn, nameOut, tp, tp);

    //-- read image
    In = img_get(nameIn, tp);
    //-- transformation
    switch (tp)
    {
    case 1:
        Out = neg_pbm(In);
        break;
    case 2:
        Out = neg_pgm(In);
        break;
    case 3:
        Out = neg_ppm(In);
    }
    //-- save image
    img_put(Out, nameOut, tp);

    //-- show image
    sprintf(cmd, "%s %s &", VIEW, nameOut);
    system(cmd);
    img_free(In);
    img_free(Out);
    return 0;
}
