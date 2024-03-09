#include "bmp.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]);

// get the average pixel
RGBTRIPLE average_pxl(int n, RGBTRIPLE pixels[n]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]);

