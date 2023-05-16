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

// Parse rule into live and dead array
void parseRule(uint32_t rule, int** live, int** dead) {
	int binaryNum[32];
  
    // counter for binary array
    int i = 0;
    while (rule > 0) {
        // storing remainder in binary array
        binaryNum[i] = rule % 2;
        n = n / 2;
        i++;
    }
	for (int i = 31; i > 0; i--)
	{
		
	}

}
// Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
// Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
// and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
// change rule into binary form, extract live and dead into 2 array
int live[9], dead[9];
parseRule(rule, &live, &dead);
// if live cell's live neighbors in live's array, be alive else dead
// if dead cell's live neighbors in dead's array, be alive else dead

}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image* nextImage =  malloc(sizeof(Image));
	if (nextImage == NULL)
	{
		printf("malloc fail!\n");
		exit(-1);
	}
	int totalPixels = image->rows * image->cols;
	nextImage->image = malloc(totalPixels);
	if (nextImage->image == NULL)
	{
		printf("malloc fail!\n");
		exit(-1);
	}
	Color** p = im->image;
	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols; j++)
		{
			*(p) = evaluateOneCell(image, i, j, rule);
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
		printf("Wrong argument numbers!\n");
		exit(-1);
	}
	Image* image = readData(argv[1]);
	uint32_t rule = (uint32_t) strtol(argv[2], 0, 0);
	Image* nextImage = life(image, rule);
	writeData(nextImage);
	freeImage(image);
	freeImage(nextImage);
	return 0;

}
