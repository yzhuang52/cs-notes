/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	char format[10];
	int col, row, max;
	FILE* fp;
	fp = fopen(filename, "r");
	fscanf(fp, "%s\n", format);
	fscanf(fp, "%d %d\n", &col, &row);
	fscanf(fp, "%d\n", &max);
	Color colors[row][col];
	for (int i=0; i<row; i++) {
		Color color;
		for (int j=0; j<col; j++) {
			fscanf(fp, "%d %d %d", &color.R, &color.G, &color.B);
			colors[i][j] = color;
		}
	}
	fclose(fp);
	Image image;
	image.rows = row;
	image.cols = col;
	image.image = colors;
	Image* image_ptr = &image;
	return image_ptr;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
}