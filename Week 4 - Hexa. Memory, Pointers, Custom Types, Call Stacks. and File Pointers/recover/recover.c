#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");

    // Create a buffer for a block of data
    uint8_t buffer[512];
    int started_image = 0;
    int filenumber = 0;
    char *filename = malloc(8 * sizeof(char));
    FILE *image = NULL;

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, 512, card) == 512)
    {
        // Create JPEGs from the data
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            sprintf(filename, "%03i.jpg", filenumber);
            if (started_image > 0)
            {
                fclose(image);
            }

            image = fopen(filename, "w");
            if (image == NULL)
            {
                printf("Cannot open image to write!");
                return 1;
            }

            filenumber++;
            started_image = 1;

            fwrite(buffer, 1, 512, image);
        }
        else
        {
            if (started_image > 0)
            {
                fwrite(buffer, 1, 512, image);
            }
        }
    }

    fclose(card);
    free(filename);
    fclose(image);
}
