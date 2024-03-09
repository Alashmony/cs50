#include "helpers.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            RGBTRIPLE curpxl = image[r][p];
            int mean_val = round((curpxl.rgbtRed + curpxl.rgbtGreen + curpxl.rgbtBlue) / 3.0);
            image[r][p].rgbtRed = mean_val;
            image[r][p].rgbtGreen = mean_val;
            image[r][p].rgbtBlue = mean_val;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int half = (width / 2);
    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < half; p++)
        {
            RGBTRIPLE curpxl = image[r][p];
            RGBTRIPLE opppxl = image[r][width - p - 1];
            image[r][p] = opppxl;
            image[r][width - p - 1] = curpxl;
        }
    }
    return;
}

// get average values for a pixel
RGBTRIPLE average_pxl(int n, RGBTRIPLE pixels[])
{
    int redz = 0;
    int greenz = 0;
    int bluez = 0;
    for (int p = 0; p < n; p++)
    {
        redz += pixels[p].rgbtRed;
        greenz += pixels[p].rgbtGreen;
        bluez += pixels[p].rgbtBlue;
    }
    int avg_red = round(redz * 1.0 / n);
    int avg_green = round(greenz * 1.0 / n);
    int avg_blue = round(bluez * 1.0 / n);
    RGBTRIPLE newpxl;
    newpxl.rgbtRed = avg_red;
    newpxl.rgbtGreen = avg_green;
    newpxl.rgbtBlue = avg_blue;
    return newpxl;
}

// copy image
void copy_image(int height, int width, RGBTRIPLE pixels[height][width],
                RGBTRIPLE new_pixels[height][width])
{
    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            new_pixels[r][p] = pixels[r][p];
        }
    }
}

// copy image bordered
void copy_image_borderd(int height, int width, RGBTRIPLE pixels[height][width],
                        RGBTRIPLE new_pixels[height + 2][width + 2])
{
    // Declare an empty pixel
    RGBTRIPLE em;
    em.rgbtRed = 0;
    em.rgbtGreen = 0;
    em.rgbtBlue = 0;
    for (int r = 0; r < height + 2; r++)
    {
        for (int p = 0; p < width + 2; p++)
        {
            if (r == 0 || r == height + 1)
            {
                new_pixels[r][p] = em;
            }
            else if (p == 0 || p == width + 1)
            {
                new_pixels[r][p] = em;
            }
            else
            {
                new_pixels[r][p] = pixels[r - 1][p - 1];
            }
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Declare an empty pixel
    RGBTRIPLE em;
    em.rgbtRed = 0;
    em.rgbtGreen = 0;
    em.rgbtBlue = 0;
    RGBTRIPLE image_cpy[height][width];
    copy_image(height, width, image, image_cpy);
    const int corpxls = 4;
    const int edgpxls = 6;
    const int othrpxls = 9;

    for (int r = 0; r < height; r++)
    {
        bool toppxl = (r == 0);
        bool butpxl = (r == height - 1);
        for (int p = 0; p < width; p++)
        {
            bool leftpxl = (p == 0);
            bool rigtpxl = (p == width - 1);
            RGBTRIPLE cur_pxl = image[r][p];
            RGBTRIPLE pxls[9];
            // top pixels
            if (toppxl)
            {
                // top left pixel
                if (leftpxl)
                {
                    pxls[0] = cur_pxl;
                    pxls[1] = image_cpy[r + 1][p];
                    pxls[2] = image_cpy[r][p + 1];
                    pxls[3] = image_cpy[r + 1][p + 1];
                    pxls[4] = em;
                    pxls[5] = em;
                    pxls[6] = em;
                    pxls[7] = em;
                    pxls[8] = em;
                    // calculate the new pxl
                    RGBTRIPLE new_pxl = average_pxl(corpxls, pxls);
                    image[r][p] = new_pxl;
                }
                // top right corner
                else if (rigtpxl)
                {
                    pxls[0] = cur_pxl;
                    pxls[1] = image_cpy[r + 1][p];
                    pxls[2] = image_cpy[r][p - 1];
                    pxls[3] = image_cpy[r + 1][p - 1];
                    pxls[4] = em;
                    pxls[5] = em;
                    pxls[6] = em;
                    pxls[7] = em;
                    pxls[8] = em;
                    // calculate the new pxl
                    RGBTRIPLE new_pxl = average_pxl(corpxls, pxls);
                    image[r][p] = new_pxl;
                }
                // other top pixels
                else
                {
                    pxls[0] = image_cpy[r][p - 1];
                    pxls[1] = cur_pxl;
                    pxls[2] = image_cpy[r][p + 1];
                    pxls[3] = image_cpy[r + 1][p - 1];
                    pxls[4] = image_cpy[r + 1][p];
                    pxls[5] = image_cpy[r + 1][p + 1];
                    pxls[6] = em;
                    pxls[7] = em;
                    pxls[8] = em;
                    // calculate the new pxl
                    RGBTRIPLE new_pxl = average_pxl(edgpxls, pxls);
                    image[r][p] = new_pxl;
                }
            }
            // buttom pixels
            else if (butpxl)
            {
                // buttom left pixel
                if (leftpxl)
                {
                    pxls[0] = cur_pxl;
                    pxls[1] = image_cpy[r - 1][p];
                    pxls[2] = image_cpy[r][p + 1];
                    pxls[3] = image_cpy[r - 1][p + 1];
                    pxls[4] = em;
                    pxls[5] = em;
                    pxls[6] = em;
                    pxls[7] = em;
                    pxls[8] = em;
                    // calculate the new pxl
                    RGBTRIPLE new_pxl = average_pxl(corpxls, pxls);
                    image[r][p] = new_pxl;
                }
                // buttom right corner
                else if (rigtpxl)
                {
                    pxls[0] = cur_pxl;
                    pxls[1] = image_cpy[r - 1][p];
                    pxls[2] = image_cpy[r][p - 1];
                    pxls[3] = image_cpy[r - 1][p - 1];
                    pxls[4] = em;
                    pxls[5] = em;
                    pxls[6] = em;
                    pxls[7] = em;
                    pxls[8] = em;
                    // calculate the new pxl
                    RGBTRIPLE new_pxl = average_pxl(corpxls, pxls);
                    image[r][p] = new_pxl;
                }
                // other buttom pixels
                else
                {
                    pxls[0] = image_cpy[r][p - 1];
                    pxls[1] = cur_pxl;
                    pxls[2] = image_cpy[r][p + 1];
                    pxls[3] = image_cpy[r - 1][p - 1];
                    pxls[4] = image_cpy[r - 1][p];
                    pxls[5] = image_cpy[r - 1][p + 1];
                    pxls[6] = em;
                    pxls[7] = em;
                    pxls[8] = em;
                    // calculate the new pxl
                    RGBTRIPLE new_pxl = average_pxl(edgpxls, pxls);
                    image[r][p] = new_pxl;
                }
            }
            // other pixels
            else
            {
                // if left edge
                if (leftpxl)
                {
                    pxls[0] = image_cpy[r - 1][p];
                    pxls[1] = image_cpy[r - 1][p + 1];
                    pxls[2] = cur_pxl;
                    pxls[3] = image_cpy[r][p + 1];
                    pxls[4] = image_cpy[r + 1][p];
                    pxls[5] = image_cpy[r + 1][p + 1];
                    pxls[6] = em;
                    pxls[7] = em;
                    pxls[8] = em;
                    // calculate the new pxl
                    RGBTRIPLE new_pxl = average_pxl(edgpxls, pxls);
                    image[r][p] = new_pxl;
                }
                // if right edge
                else if (rigtpxl)
                {
                    pxls[0] = image_cpy[r - 1][p - 1];
                    pxls[1] = image_cpy[r - 1][p];
                    pxls[2] = image_cpy[r][p - 1];
                    pxls[3] = cur_pxl;
                    pxls[4] = image_cpy[r + 1][p - 1];
                    pxls[5] = image_cpy[r + 1][p];
                    pxls[6] = em;
                    pxls[7] = em;
                    pxls[8] = em;
                    // calculate the new pxl
                    RGBTRIPLE new_pxl = average_pxl(edgpxls, pxls);
                    image[r][p] = new_pxl;
                }
                else
                {
                    pxls[0] = image_cpy[r - 1][p - 1];
                    pxls[1] = image_cpy[r - 1][p];
                    pxls[2] = image_cpy[r - 1][p + 1];
                    pxls[3] = image_cpy[r][p - 1];
                    pxls[4] = cur_pxl;
                    pxls[5] = image_cpy[r][p + 1];
                    pxls[6] = image_cpy[r + 1][p - 1];
                    pxls[7] = image_cpy[r + 1][p];
                    pxls[8] = image_cpy[r + 1][p + 1];
                    // calculate the new pxl
                    RGBTRIPLE new_pxl = average_pxl(othrpxls, pxls);
                    image[r][p] = new_pxl;
                }
            }
        }
    }
    return;
}

// Detect changes
RGBTRIPLE detect_edges(RGBTRIPLE matrix[9])
{
    int max = 255;
    int gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    // placeholder to sum changes in x
    int redx_sum = 0;
    int greenx_sum = 0;
    int bluex_sum = 0;
    // placeholder to sum changes in y
    int redy_sum = 0;
    int greeny_sum = 0;
    int bluey_sum = 0;
    for (int p = 0; p < 9; p++)
    {
        redx_sum += matrix[p].rgbtRed * gx[p];
        greenx_sum += matrix[p].rgbtGreen * gx[p];
        bluex_sum += matrix[p].rgbtBlue * gx[p];

        redy_sum += matrix[p].rgbtRed * gy[p];
        greeny_sum += matrix[p].rgbtGreen * gy[p];
        bluey_sum += matrix[p].rgbtBlue * gy[p];
    }

    // values of the square root of the squares
    int redsq = (int) round(sqrt((redx_sum * redx_sum) + (redy_sum * redy_sum)));
    int greensq = (int) round(sqrt((greenx_sum * greenx_sum) + (greeny_sum * greeny_sum)));
    int bluesq = (int) round(sqrt((bluex_sum * bluex_sum) + (bluey_sum * bluey_sum)));

    // get the final value of the pixel as the max of the root vs 255
    RGBTRIPLE new_pixel;
    new_pixel.rgbtRed = (redsq < max) ? redsq : max;
    new_pixel.rgbtGreen = (greensq < max) ? greensq : max;
    new_pixel.rgbtBlue = (bluesq < max) ? bluesq : max;

    return new_pixel;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE cur_matrix[9];
    RGBTRIPLE image_cpy[height + 2][width + 2];
    copy_image_borderd(height, width, image, image_cpy);
    for (int r = 0; r < height; r++)
    {
        for (int p = 0; p < width; p++)
        {
            cur_matrix[0] = image_cpy[r][p];
            cur_matrix[1] = image_cpy[r][p + 1];
            cur_matrix[2] = image_cpy[r][p + 2];
            cur_matrix[3] = image_cpy[r + 1][p];
            cur_matrix[4] = image_cpy[r + 1][p + 1];
            cur_matrix[5] = image_cpy[r + 1][p + 2];
            cur_matrix[6] = image_cpy[r + 2][p];
            cur_matrix[7] = image_cpy[r + 2][p + 1];
            cur_matrix[8] = image_cpy[r + 2][p + 2];
            image[r][p] = detect_edges(cur_matrix);
        }
    }
    return;
}
