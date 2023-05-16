/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int ring(int m, int n)
{
	return (m + n) % n;
}
// Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
// Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
// and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
Color* result = malloc(sizeof(Color));
if (result == NULL)
{
	printf("malloc fail\n");
	exit(-1);
}
int aliveNeighbor = 0;
int isAlive = 0;
Color* cell = *(image->image + row * image->cols + col);
if (cell->R == 255 && cell->G == 255 && cell->B == 255)
{
	isAlive = 1;
}
for (int i = 0; i < 8; i++)
{
	int newRow = ring(row + dx[i], image->rows);
	int newCol = ring(col + dy[i], image->cols);
	Color* newCell = *(image->image + newRow * image->cols + newCol);
	if (newCell->R == 255 && newCell->G == 255 && newCell->B == 255)
	{
		aliveNeighbor += 1;
	}
}
int index = isAlive * 9 + aliveNeighbor;
if (rule & (1<<index)) {
	result->R = result->G = result->B = 255;
	return result;
} else 
{
	result->R = result->G = result->B = 0;
	return result;
}
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image* nextImage =  malloc(sizeof(Image));
	nextImage->rows = image->rows;
	nextImage->cols = image->cols;
	if (nextImage == NULL)
	{
		printf("malloc fail!\n");
		exit(-1);
	}
	int totalPixels = image->rows * image->cols;
	nextImage->image = malloc(sizeof(Color*)*totalPixels);
	if (nextImage->image == NULL)
	{
		printf("malloc fail!\n");
		exit(-1);
	}
	Color** p = nextImage->image;
	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols; j++)
		{
			*p = evaluateOneCell(image, i, j, rule);
			p++;
		}
	}
	return nextImage;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	Image* image = readData(argv[1]);
	uint32_t rule = (uint32_t) strtol(argv[2], NULL, 16);
	Image* nextImage = life(image, rule);
	writeData(nextImage);
	freeImage(image);
	freeImage(nextImage);
	return 0;

}
