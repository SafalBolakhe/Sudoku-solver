#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

// q vaneko row w vaneko column matrix ma
int solver(int fmat[9][9], int g, int tempa, int list[g][2]);
void backtrack(int value, int fmat[9][9], int g, int list[90][2]);
bool checker(int fmat[9][9], int k, int l, int ans);
int looper(int i, int j, int a, int fmat[9][9], int q, int w);

bool box(int a, int fmat[9][9], int q, int w);
bool horizontal(int a, int fmat[9][9], int q);
bool vertical(int a, int fmat[9][9], int w);

int main(void)
{
    int count = 0, z = 0, g = 0;
    int nmat[9][9], list[90][2];
    char h;
    FILE *fp, *fd;
    fp = fopen("test.txt", "r");
    fd = fopen("result.txt", "w");
    // converts file data to matrix
    while (z != 9)
    {
        h = fgetc(fp);
        if (isdigit(h))
        {
            nmat[z][count] = atoi(&h);
            count++;
            if (count == 9)
            {
                count = 0;
                z++;
            }
        }
    }
    g = 0;
    for (int k = 0; k < 9; k++)
    {
        for (int l = 0; l < 9; l++)
        {
            if (nmat[k][l] == 0)
            {
                list[g][0] = k;
                list[g][1] = l;
                // printf("%i %i\n", list[g][0], list[g][1]);
                g++;
            }
        }
    }

    int counter = g;
    int k, l, tempa, tempval;
    g = 0;
    while (g < counter || g == 0)
    {
        k = list[g][0];
        l = list[g][1];
        tempa = 1;
        nmat[k][l] = solver(nmat, g, tempa, list);
        while (nmat[k][l] == 0)
        {
            g--;
            k = list[g][0];
            l = list[g][1];
            tempa = nmat[k][l];
            nmat[k][l] = solver(nmat, g, tempa, list);
        }
        g++;
    }
    if (g == 0)
    {
        printf("this sudoku sucks and has no solution");
        return 0;
    }

    for (int k = 0; k < 9; k++)
    {
        for (int l = 0; l < 9; l++)
        {
            fprintf(fd, "%i ", nmat[k][l]);
        }
        fprintf(fd, "\n");
    }
    fclose(fp);
    fclose(fd);
}

// other functions start here

int solver(int fmat[9][9], int g, int tempa, int list[g][2])
{
    int k = list[g][0];
    int l = list[g][1];
    while (tempa <= 9)
    {
        // printf("here");
        if (checker(fmat, k, l, tempa))
        {
            return tempa;
        }
        else
        {
            tempa = tempa + 1;
        }
    }
    return 0;
    // while (a <= 9)
    // {
    //     if (box(a, fmat, k, l) && horizontal(a, fmat, k) && vertical(a, fmat, l))
    //     {
    //         return a;
    //     }
    //     else
    //     {
    //         a++;
    //     }
    // }
}
bool checker(int fmat[9][9], int k, int l, int ans)
{
    int a = ans;
    if (box(a, fmat, k, l) && horizontal(a, fmat, k) && vertical(a, fmat, l))
    {
        return true;
    }
    return false;
}
bool box(int a, int fmat[9][9], int q, int w)
{
    if (q <= 2)
    {
        // printf("0-2\n");
        if (w <= 2)
        {
            return (looper(0, 0, a, fmat, 3, 3));
        }
        else if (w >= 3 && w <= 5)
        {
            return (looper(0, 3, a, fmat, 3, 6));
        }
        else if (w >= 5 && w <= 8)
        {
            return (looper(0, 6, a, fmat, 3, 9));
        }
    }
    else if (3 <= q && q <= 5)
    {
        // printf("3-5\n");

        if (w <= 2)
        {
            return (looper(3, 0, a, fmat, 6, 3));
        }
        else if (w >= 2 && w <= 5)
        {
            return (looper(3, 3, a, fmat, 6, 6));
        }
        else if (w >= 6 && w <= 8)
        {
            return (looper(3, 6, a, fmat, 6, 9));
        }
    }
    else if (5 <= q && q <= 8)
    {
        // printf("6-8\n");

        if (w <= 2)
        {
            return (looper(6, 0, a, fmat, 9, 3));
        }
        else if (2 <= w && w <= 5)
        {
            return (looper(6, 3, a, fmat, 9, 6));
        }
        else if (5 <= w && w <= 8)
        {
            return (looper(6, 6, a, fmat, 9, 9));
        }
    }
}
int looper(int i, int j, int a, int qmat[9][9], int q, int w)
{
    for (int f = i; f < q; f++)
    {
        for (int g = j; g < w; g++)
        {
            int p = qmat[i][j];
            if (qmat[f][g] == a)
            {
                return false;
            }
        }
    }
    return true;
}

bool horizontal(int a, int fmat[9][9], int q)
{
    for (int i = 0; i < 9; i++)
    {
        if (fmat[q][i] == a)
        {
            // printf("%i h\n", fmat[q][i]);
            return false;
        }
    }
    return true;
}
bool vertical(int a, int fmat[9][9], int w)
{
    for (int i = 0; i < 9; i++)
    {
        if (fmat[i][w] == a)
        {
            return false;
        }
    }
    return true;
}