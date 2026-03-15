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
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		printf("Unable to open file\n");
		return NULL;
	}
	Image *newimage = malloc(sizeof(Image));
	
	/*check for the right ppm type*/
	char *ptype = malloc(sizeof(char) * 3);
	fscanf(fp, "%2s", ptype); 
	if (strcmp(ptype, "P3") != 0) {
		printf("Error, the PPM file is not of P3 format\n");	
		free(newimage);
		fclose(fp);
		free(ptype);
		return NULL;
	}
	free(ptype);
	/* check for the dimensions of the image*/
	int dimensions[2];
	fscanf(fp, "%d %d", &dimensions[0], &dimensions[1]);
	newimage->rows = dimensions[0];
	newimage->cols = dimensions[1];	
	int scale;
	fscanf(fp, "%d", &scale);
	if (scale != 255) {
		printf("Error: scale is not equal to 256\n");
		fclose(fp);
		free(newimage);
		return NULL;
	}
	/*copy all the rgb values from the ppm file to the image */
	Color **colorlist;
	colorlist = malloc(sizeof(Color *) * newimage->rows);
	for (int i = 0; i < newimage->rows; i++) {
		colorlist[i] = malloc(sizeof(Color) * newimage->cols);
		for(int j = 0; j < newimage->cols; j++) {
			int r, g, b;
			fscanf(fp, "%d %d %d", &r, &g, &b);
			colorlist[i][j].R = r;
			colorlist[i][j].G = g;
			colorlist[i][j].B = b;
		}
	}
	newimage->image = colorlist;
	fclose(fp);
	return newimage;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	uint32_t rows, cols;
	rows = image->rows;
	cols = image->cols;
	printf("P3\n");
	printf("%d %d\n255\n", cols, rows);
	Color **colors = image->image;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (j != cols -1) {
			printf("%3d %3d %3d   ", colors[i][j].R, colors[i][j].G, colors[i][j].B);
			}
			else {
				printf("%3d %3d %3d", colors[i][j].R, colors[i][j].G, colors[i][j].B);
			}
		}
		printf("\n");
	}		 
}

//Frees an image
void freeImage(Image *image)
{
	if (image == NULL){
		return;
	}
	for (int i = 0; i < image->rows; i++) {
		free(image->image[i]);
	}
	free(image->image);
	free(image);
}
