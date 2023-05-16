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
	char format[3];
	int col, row, max;
	FILE* fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("Fail to open %s\n", filename);
		exit(-1);
	}
	Image *image = (Image*) malloc(sizeof(Image));
	fscanf(fp, "%s\n", format);
	if (format[0] != 'P' || format[1] != '3') {
		printf("Wrong format!\n");
		exit(-1);
	}
	fscanf(fp, "%u", &col);
	fscanf(fp, "%u", &row);
	fscanf(fp, "%u", &max);
	image->image = (Color**) malloc(sizeof(Color*)*row*col);
	for (int i=0; i<row*col; i++) 
	{
		*(image->image+i) = (Color*) malloc(sizeof(Color));
		Color* pixel = *(image->image+i);
		fscanf(fp, "%hhu %hhu %hhu", &pixel->R, &pixel->G, &pixel->B);	
	}
	fclose(fp);
	image->rows = row;
	image->cols = col;
	return image;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n");
	printf("%d %d\n", image->cols, image->rows);
	printf("255\n");
	Color** p = image->image;
	for (int i = 0; i < image->rows; i++)
	{
		for (int j = 0; j < image->cols-1; j++)
		{
			printf("%3hhu %3hhu %3hhu   ", (*p)->R, (*p)->G, (*p)->B);
			p++;
		}
		printf("%3hhu %3hhu %3hhu\n", (*p)->R, (*p)->G, (*p)->B);
		p++;
	}
}

//Frees an image
void freeImage(Image *image)
{
	for (int i = 0; i < image->rows*image->cols; i++)
	{
		free(*(image->image+i));
	}
	free(image->image);
	free(image);
}