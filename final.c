#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

int solver(int main_matrix[9][9], int driver_value, int temp_value, int empty_cell_list[driver_value][2]);
bool checker(int main_matrix[9][9], int temp_row, int temp_col, int ans);
int looper(int i, int j, int answer, int main_matrix[9][9], int row, int column);

bool box(int answer, int main_matrix[9][9], int row, int column);
bool horizontal(int answer, int main_matrix[9][9], int row);
bool vertical(int answer, int main_matrix[9][9], int column);

int main(void)
{
    int column = 0, row = 0, driver_value = 0;
    int sudoku_matrix[9][9], empty_cell_list[90][2];
    char h;
    FILE *fp, *fd;
    fp = fopen("test.txt", "r");
    fd = fopen("result.txt", "w");
    if (fp == NULL)
    {
        // this part handles if the file is not available
        printf("Please create a file called test.txt with the sudoku to solve");
    }
    else
    {
        // converts file data to matrix
        while (row != 9)
        {
            h = fgetc(fp);
            if (isdigit(h))
            {
                sudoku_matrix[row][column] = atoi(&h);
                column++;
                if (column == 9)
                {
                    column = 0;
                    row++;
                }
            }
        }
        // here the driver_value keeps track of empty cells on the sudoku to solve later. It is stored in sets of two.
        // for example, when driver_value == 0, it stores two values in places (0,0), and (0, 1). this is the location of the empty cells
        driver_value = 0;
        for (int k = 0; k < 9; k++)
        {
            for (int l = 0; l < 9; l++)
            {
                if (sudoku_matrix[k][l] == 0)
                {
                    empty_cell_list[driver_value][0] = k;
                    empty_cell_list[driver_value][1] = l;
                    // printf("%i %i\n", empty_cell_list[g][0], empty_cell_list[g][1]);
                    driver_value++;
                }
            }
        }

        // this is the main part that is responsible for solving the sudoku
        int counter = driver_value;
        int temp_row, temp_col, temp_value;
        driver_value = 0;
        // initially, the program goes to the first empty cell ie driver_value==0, then adds the appropriate value in the cell after which driver_cell++
        while (driver_value < counter)
        {
            temp_row = empty_cell_list[driver_value][0];
            temp_col = empty_cell_list[driver_value][1];
            temp_value = 1;
            sudoku_matrix[temp_row][temp_col] = solver(sudoku_matrix, driver_value, temp_value, empty_cell_list);
            // but we may encounter a condition in which no values from 1 to 9 may be suitable,
            //  in this case, we backtrack which is what the below chunk does
            while (sudoku_matrix[temp_row][temp_col] == 0)
            {
                driver_value--;
                temp_row = empty_cell_list[driver_value][0];
                temp_col = empty_cell_list[driver_value][1];
                temp_value = sudoku_matrix[temp_row][temp_col];
                sudoku_matrix[temp_row][temp_col] = solver(sudoku_matrix, driver_value, temp_value, empty_cell_list);
            }
            // proceed to the next empty cell if solution is found
            driver_value++;
        }
        // print the solved sudoku into a file
        for (int k = 0; k < 9; k++)
        {
            for (int l = 0; l < 9; l++)
            {
                fprintf(fd, "%i ", sudoku_matrix[k][l]);
            }
            fprintf(fd, "\n");
        }
        fclose(fp);
        fclose(fd);
        system("kate result.txt");
    }
}

// other functions start here

int solver(int main_matrix[9][9], int driver_value, int temp_value, int empty_cell_list[driver_value][2])
{
    int temp_row = empty_cell_list[driver_value][0];
    int temp_col = empty_cell_list[driver_value][1];
    while (temp_value <= 9)
    {
        // printf("here");
        // this part checks if the value is valid, if it is, then it is added to the empty cell
        if (checker(main_matrix, temp_row, temp_col, temp_value))
        {
            return temp_value;
        }
        else
        {
            temp_value = temp_value + 1;
        }
    }
    return 0;
}
// this is a function that checks the validity of a value in the given cell
bool checker(int main_matrix[9][9], int temp_row, int temp_col, int ans)
{
    int answer = ans;
    if (box(answer, main_matrix, temp_row, temp_col) && horizontal(answer, main_matrix, temp_row)
        && vertical(answer, main_matrix, temp_col))
    {
        return true;
    }
    return false;
}
// this is a function that checks the validity of a value in the given cell according to the 3*3 box
bool box(int answer, int main_matrix[9][9], int row, int column)
{
    // these conditions are checking for the position of the values returned to figure out in which box they lie
    if (row <= 2)
    {
        // printf("0-2\n");
        if (column <= 2)
        {
            return (looper(0, 0, answer, main_matrix, 3, 3));
        }
        else if (column >= 3 && column <= 5)
        {
            return (looper(0, 3, answer, main_matrix, 3, 6));
        }
        else if (column >= 5 && column <= 8)
        {
            return (looper(0, 6, answer, main_matrix, 3, 9));
        }
    }
    else if (3 <= row && row <= 5)
    {
        // printf("3-5\n");

        if (column <= 2)
        {
            return (looper(3, 0, answer, main_matrix, 6, 3));
        }
        else if (column >= 2 && column <= 5)
        {
            return (looper(3, 3, answer, main_matrix, 6, 6));
        }
        else if (column >= 6 && column <= 8)
        {
            return (looper(3, 6, answer, main_matrix, 6, 9));
        }
    }
    else if (5 <= row && row <= 8)
    {
        // printf("6-8\n");

        if (column <= 2)
        {
            return (looper(6, 0, answer, main_matrix, 9, 3));
        }
        else if (2 <= column && column <= 5)
        {
            return (looper(6, 3, answer, main_matrix, 9, 6));
        }
        else if (5 <= column && column <= 8)
        {
            return (looper(6, 6, answer, main_matrix, 9, 9));
        }
    }
}
// this is the actual brains of the previous function. After the position gets finalized in the previous function,
// this part loops through the concerned box to validate the value
int looper(int i, int j, int answer, int qmat[9][9], int row, int column)
{
    for (int f = i; f < row; f++)
    {
        for (int driver_value = j; driver_value < column; driver_value++)
        {
            int p = qmat[i][j];
            if (qmat[f][driver_value] == answer)
            {
                return false;
            }
        }
    }
    return true;
}
// this part checks for the horizontal validity of the sudoku
bool horizontal(int answer, int main_matrix[9][9], int row)
{
    for (int i = 0; i < 9; i++)
    {
        if (main_matrix[row][i] == answer)
        {
            // printf("%i h\n", main_matrix[row][i]);
            return false;
        }
    }
    return true;
}
// this part checks for the vertical validity of the sudoku
bool vertical(int answer, int main_matrix[9][9], int column)
{
    for (int i = 0; i < 9; i++)
    {
        if (main_matrix[i][column] == answer)
        {
            return false;
        }
    }
    return true;
}