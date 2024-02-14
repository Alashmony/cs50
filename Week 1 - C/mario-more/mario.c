#include <cs50.h>
#include <stdio.h>

void build_row(int blocks, int max);

int main(void)
{
    // Prompt the user for the pyramid's height
    int height = 0;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1);

    // Print a pyramid of that height
    for (int row_num = 1; row_num <= height; row_num++)
    {
        build_row(row_num, height);
    }
}

void build_row(int blocks, int max)
{
    // Print spaces with lenght of height (max) - row_num (blocks)
    int spaces = max - blocks;
    for (int space = 0; space < spaces; space++)
    {
        printf(" ");
    }

    // Print blocks
    for (int i = 0; i < blocks; i++)
    {

        printf("#");
    }

    // Print two spaces
    printf("  ");

    // Print the second block
    for (int i = 0; i < blocks; i++)
    {

        printf("#");
    }

    printf("\n");
}
