/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color* cell = *(image->image + row * image->cols + col);
	Color* result = (Color*) malloc(sizeof(Color));
	if (result == NULL)
	{
		printf("Malloc fail\n");
		exit(-1);
	}
	if ((cell->B&1) == 1)
	{
		result->R = 255;
		result->G = 255;
		result->B = 255;
	} else
	{
		result->R = 0;
		result->G = 0;
		result->B = 0;
	}
	return result;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image* im = (Image*) malloc(sizeof(Image));
	if (im == NULL)
	{
		printf("Malloc fail\n");
		exit(-1);
	}
	im->rows = image->rows;
	im->cols = image->cols;
	int total_pixels = im->rows * im->cols;
	im->image = (Color**) malloc(sizeof(Color*)*total_pixels);
	if (im->image == NULL)
	{
		printf("Malloc fail\n");
		exit(-1);
	}
	Color** p = im->image;
	for (int i = 0; i < im->rows; i++)
	{
		for (int j = 0; j < im->cols; j++)
		{
			*p = evaluateOnePixel(image, i, j);
			p++;
		}
	}
	return im;
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	if (argc != 2) 
	{
		printf("Wrong usage!\n");
		exit(-1);
	}
	Image* image = readData(argv[1]);
	Image* steImage = steganography(image);
	writeData(steImage);
	freeImage(image);
	freeImage(steImage);
	return 0;
}


